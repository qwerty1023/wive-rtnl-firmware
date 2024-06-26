/*
 * This is a module which is used for setting the MSS option in TCP packets.
 *
 * Copyright (C) 2000 Marc Boucher <marc@mbsi.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <net/dst.h>
#include <net/flow.h>
#include <net/ipv6.h>
#include <net/route.h>
#include <net/ip6_route.h>
#include <net/tcp.h>

#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_tcpudp.h>
#include <linux/netfilter/xt_TCPMSS.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marc Boucher <marc@mbsi.ca>");
MODULE_DESCRIPTION("x_tables TCP Maximum Segment Size (MSS) adjustment");
MODULE_ALIAS("ipt_TCPMSS");
MODULE_ALIAS("ip6t_TCPMSS");

static inline unsigned int
optlen(const u_int8_t *opt, unsigned int offset)
{
	/* Beware zero-length options: make finite progress */
	if (opt[offset] <= TCPOPT_NOP || opt[offset+1] == 0)
		return 1;
	else
		return opt[offset+1];
}

static u_int32_t tcpmss_reverse_mtu(const struct sk_buff *skb,
				    unsigned int family)
{
	struct flowi fl = {};
	struct rtable *rt = NULL;
	u_int32_t mtu     = ~0U;

	if (family == PF_INET)
		fl.fl4_dst = ip_hdr(skb)->saddr;
	else
		fl.fl6_dst = skb->nh.ipv6h->saddr;

	rcu_read_lock();
	if (family == PF_INET)
		ip_route_output_key(&rt, &fl);
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	else
		rt = (struct rtable *)ip6_route_output(NULL, &fl);
#endif
	rcu_read_unlock();

	if (rt != NULL) {
		mtu = dst_mtu(&rt->u.dst);
		dst_release(&rt->u.dst);
	}
	return mtu;
}

static int
tcpmss_mangle_packet(struct sk_buff **pskb,
		     const struct xt_tcpmss_info *info,
		     unsigned int family,
		     unsigned int tcphoff,
		     unsigned int minlen)
{
	struct tcphdr *tcph;
	unsigned int tcplen, i;
	__be16 oldval;
	u16 newmss;
	u8 *opt;

	if (!skb_make_writable(*pskb, (*pskb)->len))
		return -1;

	tcplen = (*pskb)->len - tcphoff;
	tcph = (struct tcphdr *)(skb_network_header(*pskb) + tcphoff);

	/* Header cannot be larger than the packet */
	if (tcplen < tcph->doff*4)
		return -1;

	if (info->mss == XT_TCPMSS_CLAMP_PMTU) {
		unsigned int in_mtu = tcpmss_reverse_mtu(*pskb, family);

		if (dst_mtu((*pskb)->dst) <= minlen) {
			if (net_ratelimit())
				printk(KERN_ERR "xt_TCPMSS: "
				       "unknown or invalid path-MTU (%u)\n",
				       dst_mtu((*pskb)->dst));
			return -1;
		}
		if (in_mtu <= minlen) {
			if (net_ratelimit())
				printk(KERN_ERR "xt_TCPMSS: unknown or "
				       "invalid path-MTU (%u)\n", in_mtu);
			return -1;
		}
		newmss = min(dst_mtu((*pskb)->dst), in_mtu) - minlen;
	} else
		newmss = info->mss;

	opt = (u_int8_t *)tcph;
	for (i = sizeof(struct tcphdr); i < tcph->doff*4; i += optlen(opt, i)) {
		if (opt[i] == TCPOPT_MSS && tcph->doff*4 - i >= TCPOLEN_MSS &&
		    opt[i+1] == TCPOLEN_MSS) {
			u_int16_t oldmss;

			oldmss = (opt[i+2] << 8) | opt[i+3];

			/* Never increase MSS, even when setting it, as
			 * doing so results in problems for hosts that rely
			 * on MSS being set correctly.
			 */
			if (oldmss <= newmss)
				return 0;

			opt[i+2] = (newmss & 0xff00) >> 8;
			opt[i+3] = (newmss & 0x00ff);

			nf_proto_csum_replace2(&tcph->check, *pskb,
					       htons(oldmss), htons(newmss), 0);
			return 0;
		}
	}

	/* There is data after the header so the option can't be added
	   without moving it, and doing so may make the SYN packet
	   itself too large. Accept the packet unmodified instead. */
	if (tcplen > tcph->doff*4)
		return 0;

	/*
	 * MSS Option not found ?! add it..
	 */
	if (skb_tailroom((*pskb)) < TCPOLEN_MSS) {
		if (pskb_expand_head(*pskb, 0,
				    TCPOLEN_MSS - skb_tailroom(*pskb),
				    GFP_ATOMIC))
			return -1;
		tcph = (struct tcphdr *)(skb_network_header(*pskb) + tcphoff);
	}

	skb_put((*pskb), TCPOLEN_MSS);

	/*
	 * IPv4: RFC 1122 states "If an MSS option is not received at
	 * connection setup, TCP MUST assume a default send MSS of 536".
	 * IPv6: RFC 2460 states IPv6 has a minimum MTU of 1280 and a minimum
	 * length IPv6 header of 60, ergo the default MSS value is 1220
	 * Since no MSS was provided, we must use the default values
	 */
	if (family == PF_INET)
		newmss = min(newmss, (u16)536);
	else
		newmss = min(newmss, (u16)1220);

	opt = (u_int8_t *)tcph + sizeof(struct tcphdr);
	memmove(opt + TCPOLEN_MSS, opt, tcplen - sizeof(struct tcphdr));

	nf_proto_csum_replace2(&tcph->check, *pskb,
			       htons(tcplen), htons(tcplen + TCPOLEN_MSS), 1);
	opt[0] = TCPOPT_MSS;
	opt[1] = TCPOLEN_MSS;
	opt[2] = (newmss & 0xff00) >> 8;
	opt[3] = (newmss & 0x00ff);

	nf_proto_csum_replace4(&tcph->check, *pskb, 0, *((__be32 *)opt), 0);

	oldval = ((__be16 *)tcph)[6];
	tcph->doff += TCPOLEN_MSS/4;
	nf_proto_csum_replace2(&tcph->check, *pskb,
				oldval, ((__be16 *)tcph)[6], 0);
	return TCPOLEN_MSS;
}

static unsigned int
xt_tcpmss_target4(struct sk_buff **pskb,
		  const struct net_device *in,
		  const struct net_device *out,
		  unsigned int hooknum,
		  const struct xt_target *target,
		  const void *targinfo)
{
	struct iphdr *iph = (*pskb)->nh.iph;
	__be16 newlen;
	int ret;

	ret = tcpmss_mangle_packet(pskb, targinfo,
				   PF_INET,
				   iph->ihl * 4,
				   sizeof(*iph) + sizeof(struct tcphdr));
	if (ret < 0)
		return NF_DROP;
	if (ret > 0) {
		iph = (*pskb)->nh.iph;
		newlen = htons(ntohs(iph->tot_len) + ret);
		nf_csum_replace2(&iph->check, iph->tot_len, newlen);
		iph->tot_len = newlen;
	}
	return XT_CONTINUE;
}

#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
static unsigned int
xt_tcpmss_target6(struct sk_buff **pskb,
		  const struct net_device *in,
		  const struct net_device *out,
		  unsigned int hooknum,
		  const struct xt_target *target,
		  const void *targinfo)
{
	struct ipv6hdr *ipv6h = (*pskb)->nh.ipv6h;
	u8 nexthdr;
	int tcphoff;
	int ret;

	nexthdr = ipv6h->nexthdr;
	tcphoff = ipv6_skip_exthdr(*pskb, sizeof(*ipv6h), &nexthdr);
	if (tcphoff < 0)
		return NF_DROP;
	ret = tcpmss_mangle_packet(pskb, targinfo,
				   PF_INET6,
				   tcphoff,
				   sizeof(*ipv6h) + sizeof(struct tcphdr));
	if (ret < 0)
		return NF_DROP;
	if (ret > 0) {
		ipv6h = (*pskb)->nh.ipv6h;
		ipv6h->payload_len = htons(ntohs(ipv6h->payload_len) + ret);
	}
	return XT_CONTINUE;
}
#endif

/* Must specify -p tcp --syn */
static inline int find_syn_match(const struct xt_entry_match *m)
{
	const struct xt_tcp *tcpinfo = (const struct xt_tcp *)m->data;

	if (strcmp(m->u.kernel.match->name, "tcp") == 0 &&
	    tcpinfo->flg_cmp & TCPHDR_SYN &&
	    !(tcpinfo->invflags & XT_TCP_INV_FLAGS))
		return 1;

	return 0;
}

static int
xt_tcpmss_checkentry4(const char *tablename,
		      const void *entry,
		      const struct xt_target *target,
		      void *targinfo,
		      unsigned int hook_mask)
{
	const struct xt_tcpmss_info *info = targinfo;
	const struct ipt_entry *e = entry;

	if (info->mss == XT_TCPMSS_CLAMP_PMTU &&
	    (hook_mask & ~((1 << NF_IP_FORWARD) |
			   (1 << NF_IP_LOCAL_OUT) |
			   (1 << NF_IP_POST_ROUTING))) != 0) {
		printk("xt_TCPMSS: path-MTU clamping only supported in "
		       "FORWARD, OUTPUT and POSTROUTING hooks\n");
		return 0;
	}
	if (IPT_MATCH_ITERATE(e, find_syn_match))
		return 1;
	printk("xt_TCPMSS: Only works on TCP SYN packets\n");
	return 0;
}

#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
static int
xt_tcpmss_checkentry6(const char *tablename,
		      const void *entry,
		      const struct xt_target *target,
		      void *targinfo,
		      unsigned int hook_mask)
{
	const struct xt_tcpmss_info *info = targinfo;
	const struct ip6t_entry *e = entry;

	if (info->mss == XT_TCPMSS_CLAMP_PMTU &&
	    (hook_mask & ~((1 << NF_IP6_FORWARD) |
			   (1 << NF_IP6_LOCAL_OUT) |
			   (1 << NF_IP6_POST_ROUTING))) != 0) {
		printk("xt_TCPMSS: path-MTU clamping only supported in "
		       "FORWARD, OUTPUT and POSTROUTING hooks\n");
		return 0;
	}
	if (IP6T_MATCH_ITERATE(e, find_syn_match))
		return 1;
	printk("xt_TCPMSS: Only works on TCP SYN packets\n");
	return 0;
}
#endif

static struct xt_target xt_tcpmss_reg[] __read_mostly = {
	{
		.family		= AF_INET,
		.name		= "TCPMSS",
		.checkentry	= xt_tcpmss_checkentry4,
		.target		= xt_tcpmss_target4,
		.targetsize	= sizeof(struct xt_tcpmss_info),
		.proto		= IPPROTO_TCP,
		.me		= THIS_MODULE,
	},
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
	{
		.family		= AF_INET6,
		.name		= "TCPMSS",
		.checkentry	= xt_tcpmss_checkentry6,
		.target		= xt_tcpmss_target6,
		.targetsize	= sizeof(struct xt_tcpmss_info),
		.proto		= IPPROTO_TCP,
		.me		= THIS_MODULE,
	},
#endif
};

static int __init xt_tcpmss_init(void)
{
	return xt_register_targets(xt_tcpmss_reg, ARRAY_SIZE(xt_tcpmss_reg));
}

static void __exit xt_tcpmss_fini(void)
{
	xt_unregister_targets(xt_tcpmss_reg, ARRAY_SIZE(xt_tcpmss_reg));
}

module_init(xt_tcpmss_init);
module_exit(xt_tcpmss_fini);
