/* Hop Limit matching module */

/* (C) 2001-2002 Maciej Soltysiak <solt@dns.toxicfilms.tv>
 * Based on HW's ttl module
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ipv6.h>
#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv6/ip6t_hl.h>
#include <linux/netfilter/x_tables.h>

MODULE_AUTHOR("Maciej Soltysiak <solt@dns.toxicfilms.tv>");
MODULE_DESCRIPTION("Xtables: IPv6 Hop Limit field match");
MODULE_LICENSE("GPL");

static bool match(const struct sk_buff *skb,
		  const struct net_device *in, const struct net_device *out,
		  const struct xt_match *match, const void *matchinfo,
		  int offset, unsigned int protoff, bool *hotdrop)
{
	const struct ip6t_hl_info *info = matchinfo;
	const struct ipv6hdr *ip6h = skb->nh.ipv6h;

	switch (info->mode) {
		case IP6T_HL_EQ:
			return (ip6h->hop_limit == info->hop_limit);
			break;
		case IP6T_HL_NE:
			return (!(ip6h->hop_limit == info->hop_limit));
			break;
		case IP6T_HL_LT:
			return (ip6h->hop_limit < info->hop_limit);
			break;
		case IP6T_HL_GT:
			return (ip6h->hop_limit > info->hop_limit);
			break;
		default:
			printk(KERN_WARNING "ip6t_hl: unknown mode %d\n",
				info->mode);
			return false;
	}

	return false;
}

static struct xt_match hl_match __read_mostly = {
	.name		= "hl",
	.family		= AF_INET6,
	.match		= match,
	.matchsize	= sizeof(struct ip6t_hl_info),
	.me		= THIS_MODULE,
};

static int __init ip6t_hl_init(void)
{
	return xt_register_match(&hl_match);
}

static void __exit ip6t_hl_fini(void)
{
	xt_unregister_match(&hl_match);
}

module_init(ip6t_hl_init);
module_exit(ip6t_hl_fini);
