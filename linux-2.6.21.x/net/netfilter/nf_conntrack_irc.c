/* IRC extension for IP connection tracking, Version 1.21
 * (C) 2000-2002 by Harald Welte <laforge@gnumonks.org>
 * based on RR's ip_conntrack_ftp.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/netfilter.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <linux/netfilter/nf_conntrack_irc.h>

#define MAX_PORTS 8
static unsigned short ports[MAX_PORTS];
static int ports_c;
static unsigned int max_dcc_channels = 8;
static unsigned int dcc_timeout __read_mostly = 300;
/* This is slow, but it's simple. --RR */
static char *irc_buffer;
static DEFINE_SPINLOCK(irc_buffer_lock);

unsigned int (*nf_nat_irc_hook)(struct sk_buff **pskb,
				enum ip_conntrack_info ctinfo,
				unsigned int matchoff,
				unsigned int matchlen,
				struct nf_conntrack_expect *exp) __read_mostly;
EXPORT_SYMBOL_GPL(nf_nat_irc_hook);

MODULE_AUTHOR("Harald Welte <laforge@netfilter.org>");
MODULE_DESCRIPTION("IRC (DCC) connection tracking helper");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ip_conntrack_irc");
MODULE_ALIAS_NFCT_HELPER("irc");

module_param_array(ports, ushort, &ports_c, 0400);
MODULE_PARM_DESC(ports, "port numbers of IRC servers");
module_param(max_dcc_channels, uint, 0400);
MODULE_PARM_DESC(max_dcc_channels, "max number of expected DCC channels per "
				   "IRC session");
module_param(dcc_timeout, uint, 0400);
MODULE_PARM_DESC(dcc_timeout, "timeout on for unestablished DCC channels");

static const char *dccprotos[] = {
	"SEND ", "CHAT ", "MOVE ", "TSEND ", "SCHAT "
};

#define MINMATCHLEN	5

#if 0
#define DEBUGP(format, args...) printk(KERN_DEBUG "%s:%s:" format, \
				       __FILE__, __FUNCTION__ , ## args)
#else
#define DEBUGP(format, args...)
#endif

/* tries to get the ip_addr and port out of a dcc command
 * return value: -1 on failure, 0 on success
 *	data		pointer to first byte of DCC command data
 *	data_end	pointer to last byte of dcc command data
 *	ip		returns parsed ip of dcc command
 *	port		returns parsed port of dcc command
 *	ad_beg_p	returns pointer to first byte of addr data
 *	ad_end_p	returns pointer to last byte of addr data
 */
static int parse_dcc(char *data, char *data_end, u_int32_t *ip,
		     u_int16_t *port, char **ad_beg_p, char **ad_end_p)
{
	/* at least 12: "AAAAAAAA P\1\n" */
	while (*data++ != ' ')
		if (data > data_end - 12)
			return -1;

	*ad_beg_p = data;
	*ip = simple_strtoul(data, &data, 10);

	/* skip blanks between ip and port */
	while (*data == ' ') {
		if (data >= data_end)
			return -1;
		data++;
	}

	*port = simple_strtoul(data, &data, 10);
	*ad_end_p = data;

	return 0;
}

static int help(struct sk_buff **pskb, unsigned int protoff,
		struct nf_conn *ct, enum ip_conntrack_info ctinfo)
{
	unsigned int dataoff;
	struct tcphdr _tcph, *th;
	char *data, *data_limit, *ib_ptr;
	int dir = CTINFO2DIR(ctinfo);
	struct nf_conntrack_expect *exp;
	struct nf_conntrack_tuple *tuple;
	u_int32_t dcc_ip;
	u_int16_t dcc_port;
	__be16 port;
	int i, ret = NF_ACCEPT;
	char *addr_beg_p, *addr_end_p;
	typeof(nf_nat_irc_hook) nf_nat_irc;

	/* If packet is coming from IRC server */
	if (dir == IP_CT_DIR_REPLY)
		return NF_ACCEPT;

	/* Until there's been traffic both ways, don't look in packets. */
	if (ctinfo != IP_CT_ESTABLISHED && ctinfo != IP_CT_ESTABLISHED_REPLY)
		return NF_ACCEPT;

	/* Not a full tcp header? */
	th = skb_header_pointer(*pskb, protoff, sizeof(_tcph), &_tcph);
	if (th == NULL)
		return NF_ACCEPT;

	/* No data? */
	dataoff = protoff + th->doff*4;
	if (dataoff >= (*pskb)->len)
		return NF_ACCEPT;

	spin_lock_bh(&irc_buffer_lock);
	ib_ptr = skb_header_pointer(*pskb, dataoff, (*pskb)->len - dataoff,
				    irc_buffer);
	if (!ib_ptr) {
		spin_unlock_bh(&irc_buffer_lock);
		return NF_ACCEPT;
	}

	data = ib_ptr;
	data_limit = ib_ptr + (*pskb)->len - dataoff;

	/* strlen("\1DCC SENT t AAAAAAAA P\1\n")=24
	 * 5+MINMATCHLEN+strlen("t AAAAAAAA P\1\n")=14 */
	while (data < data_limit - (19 + MINMATCHLEN)) {
		if (memcmp(data, "\1DCC ", 5)) {
			data++;
			continue;
		}
		data += 5;
		/* we have at least (19+MINMATCHLEN)-5 bytes valid data left */

		DEBUGP("DCC found in master %u.%u.%u.%u:%u %u.%u.%u.%u:%u...\n",
			NIPQUAD(iph->saddr), ntohs(th->source),
			NIPQUAD(iph->daddr), ntohs(th->dest));

		for (i = 0; i < ARRAY_SIZE(dccprotos); i++) {
			if (memcmp(data, dccprotos[i], strlen(dccprotos[i]))) {
				/* no match */
				continue;
			}
			data += strlen(dccprotos[i]);
			DEBUGP("DCC %s detected\n", dccprotos[i]);

			/* we have at least
			 * (19+MINMATCHLEN)-5-dccprotos[i].matchlen bytes valid
			 * data left (== 14/13 bytes) */
			if (parse_dcc((char *)data, data_limit, &dcc_ip,
				       &dcc_port, &addr_beg_p, &addr_end_p)) {
				DEBUGP("unable to parse dcc command\n");
				continue;
			}
			DEBUGP("DCC bound ip/port: %u.%u.%u.%u:%u\n",
				HIPQUAD(dcc_ip), dcc_port);

			/* dcc_ip can be the internal OR external (NAT'ed) IP */
			tuple = &ct->tuplehash[dir].tuple;
			if (tuple->src.u3.ip != htonl(dcc_ip) &&
			    tuple->dst.u3.ip != htonl(dcc_ip)) {
				if (net_ratelimit())
					printk(KERN_WARNING
						"Forged DCC command from "
						"%u.%u.%u.%u: %u.%u.%u.%u:%u\n",
						NIPQUAD(tuple->src.u3.ip),
						HIPQUAD(dcc_ip), dcc_port);
				continue;
			}

			exp = nf_conntrack_expect_alloc(ct);
			if (exp == NULL) {
				ret = NF_DROP;
				goto out;
			}
			tuple = &ct->tuplehash[!dir].tuple;
			port = htons(dcc_port);
			nf_conntrack_expect_init(exp, tuple->src.l3num,
						 NULL, &tuple->dst.u3,
						 IPPROTO_TCP, NULL, &port);

			nf_nat_irc = rcu_dereference(nf_nat_irc_hook);
			if (nf_nat_irc && ct->status & IPS_NAT_MASK)
				ret = nf_nat_irc(pskb, ctinfo,
						 addr_beg_p - ib_ptr,
						 addr_end_p - addr_beg_p,
						 exp);
			else if (nf_conntrack_expect_related(exp) != 0)
				ret = NF_DROP;
			nf_conntrack_expect_put(exp);
			goto out;
		}
	}
 out:
	spin_unlock_bh(&irc_buffer_lock);
	return ret;
}

static struct nf_conntrack_helper irc[MAX_PORTS] __read_mostly;
static char irc_names[MAX_PORTS][sizeof("irc-65535")] __read_mostly;

static void nf_conntrack_irc_fini(void);

static int __init nf_conntrack_irc_init(void)
{
	int i, ret;
	char *tmpname;

	if (max_dcc_channels < 1) {
		printk("nf_ct_irc: max_dcc_channels must not be zero\n");
		return -EINVAL;
	}

	irc_buffer = kmalloc(65536, GFP_KERNEL);
	if (!irc_buffer)
		return -ENOMEM;

	/* If no port given, default to standard irc port */
	if (ports_c == 0)
		ports[ports_c++] = IRC_PORT;

	for (i = 0; i < ports_c; i++) {
		irc[i].tuple.src.l3num = AF_INET;
		irc[i].tuple.src.u.tcp.port = htons(ports[i]);
		irc[i].tuple.dst.protonum = IPPROTO_TCP;
		irc[i].mask.src.l3num = 0xFFFF;
		irc[i].mask.src.u.tcp.port = htons(0xFFFF);
		irc[i].mask.dst.protonum = 0xFF;
		irc[i].max_expected = max_dcc_channels;
		irc[i].timeout = dcc_timeout;
		irc[i].me = THIS_MODULE;
		irc[i].help = help;

		tmpname = &irc_names[i][0];
		if (ports[i] == IRC_PORT)
			sprintf(tmpname, "irc");
		else
			sprintf(tmpname, "irc-%u", i);
		irc[i].name = tmpname;

		ret = nf_conntrack_helper_register(&irc[i]);
		if (ret) {
			printk("nf_ct_irc: failed to register helper "
			       "for pf: %u port: %u\n",
			       irc[i].tuple.src.l3num, ports[i]);
			nf_conntrack_irc_fini();
			return ret;
		}
	}
	return 0;
}

/* This function is intentionally _NOT_ defined as __exit, because
 * it is needed by the init function */
static void nf_conntrack_irc_fini(void)
{
	int i;

	for (i = 0; i < ports_c; i++)
		nf_conntrack_helper_unregister(&irc[i]);
	kfree(irc_buffer);
}

module_init(nf_conntrack_irc_init);
module_exit(nf_conntrack_irc_fini);
