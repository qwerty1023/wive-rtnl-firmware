/* NETMAP - static NAT mapping of IP network addresses (1:1).
 * The mapping can be applied to source (POSTROUTING),
 * destination (PREROUTING), or both (with separate rules).
 */

/* (C) 2000-2001 Svenning Soerensen <svenning@post5.tele.dk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ip.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter/x_tables.h>
#include <net/netfilter/nf_nat_rule.h>

#define MODULENAME "NETMAP"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Svenning Soerensen <svenning@post5.tele.dk>");
MODULE_DESCRIPTION("Xtables: 1:1 NAT mapping of IPv4 subnets");

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

static int
check(const char *tablename,
      const void *e,
      const struct xt_target *target,
      void *targinfo,
      unsigned int hook_mask)
{
	const struct nf_nat_multi_range_compat *mr = targinfo;

	if (!(mr->range[0].flags & IP_NAT_RANGE_MAP_IPS)) {
		DEBUGP(MODULENAME":check: bad MAP_IPS.\n");
		return 0;
	}
	if (mr->rangesize != 1) {
		DEBUGP(MODULENAME":check: bad rangesize %u.\n", mr->rangesize);
		return 0;
	}
	return 1;
}

static unsigned int
target(struct sk_buff **pskb,
       const struct net_device *in,
       const struct net_device *out,
       unsigned int hooknum,
       const struct xt_target *target,
       const void *targinfo)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	__be32 new_ip, netmask;
	const struct nf_nat_multi_range_compat *mr = targinfo;
	struct nf_nat_range newrange;

	NF_CT_ASSERT(hooknum == NF_IP_PRE_ROUTING
		     || hooknum == NF_IP_POST_ROUTING
		     || hooknum == NF_IP_LOCAL_OUT);
	ct = nf_ct_get(*pskb, &ctinfo);

	netmask = ~(mr->range[0].min_ip ^ mr->range[0].max_ip);

	if (hooknum == NF_IP_PRE_ROUTING || hooknum == NF_IP_LOCAL_OUT)
		new_ip = (*pskb)->nh.iph->daddr & ~netmask;
	else
		new_ip = (*pskb)->nh.iph->saddr & ~netmask;
	new_ip |= mr->range[0].min_ip & netmask;

	newrange = ((struct nf_nat_range)
		{ mr->range[0].flags | IP_NAT_RANGE_MAP_IPS,
		  new_ip, new_ip,
		  mr->range[0].min, mr->range[0].max });

	/* Hand modified range to generic setup. */
	return nf_nat_setup_info(ct, &newrange, hooknum);
}

static struct xt_target target_module __read_mostly = {
	.name 		= MODULENAME,
	.family		= AF_INET,
	.target 	= target,
	.targetsize	= sizeof(struct nf_nat_multi_range_compat),
	.table		= "nat",
	.hooks		= (1 << NF_IP_PRE_ROUTING) | (1 << NF_IP_POST_ROUTING) |
			  (1 << NF_IP_LOCAL_OUT),
	.checkentry 	= check,
	.me 		= THIS_MODULE
};

static int __init ipt_netmap_init(void)
{
	return xt_register_target(&target_module);
}

static void __exit ipt_netmap_fini(void)
{
	xt_unregister_target(&target_module);
}

module_init(ipt_netmap_init);
module_exit(ipt_netmap_fini);
