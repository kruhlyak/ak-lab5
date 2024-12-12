// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Kruhliak Artem <ar.krugliak@gmail.com>");
MODULE_DESCRIPTION("Hello, world - Part 1 in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(greeting_list);

int display_greeting(uint repeat_count)
{
	struct greeting_entry *entry;
	int index;

	for (index = 0; index < repeat_count; index++) {
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry)
			return -ENOMEM;

		entry->timestamp = ktime_get();
		list_add_tail(&entry->list_node, &greeting_list);
		pr_info("Hello, world!\n");
	}
	return 0;
}
EXPORT_SYMBOL(display_greeting);

void clear_greeting_list(void)
{
	struct greeting_entry *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &greeting_list, list_node) {
		pr_info("Printed at time: %lld ns\n",
			ktime_to_ns(entry->timestamp));
		list_del(&entry->list_node);
		kfree(entry);
	}
}
EXPORT_SYMBOL(clear_greeting_list);

static int __init greeting_module_init(void)
{
	pr_info("greeting_module loaded\n");
	return 0;
}

static void __exit greeting_module_exit(void)
{
	clear_greeting_list();
	pr_info("greeting_module unloaded\n");
}

module_init(greeting_module_init);
module_exit(greeting_module_exit);
