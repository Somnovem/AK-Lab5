/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-35 Zhmura Artem");
MODULE_DESCRIPTION("Hello1 module: time measurements and list");
MODULE_VERSION("0.2");

struct hello_item {
  struct list_head list;
  ktime_t start;
  ktime_t end;
};

static LIST_HEAD(hello_head);
static unsigned int call_id;

void print_hello(void)
{
  struct hello_item *item;

  item = kmalloc(sizeof(*item), GFP_KERNEL);
  if (!item) {
    pr_err("hello1: failed to allocate memory for hello_item\n");
    return;
  }

  item->start = ktime_get();
  call_id++;
  pr_info("hello1: Hello, world! call #%u\n", call_id);
  item->end = ktime_get();

  list_add_tail(&item->list, &hello_head);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
  pr_info("hello1: module loaded\n");
  return 0;
}

static void __exit hello1_exit(void)
{
  struct hello_item *item, *tmp;
  unsigned int index = 0;

  list_for_each_entry_safe(item, tmp, &hello_head, list) {
    s64 delta_ns = ktime_to_ns(ktime_sub(item->end, item->start));

   index++;
    pr_info("hello1: call #%u took %lld ns\n",
      index, (long long)delta_ns);
    list_del(&item->list);
    kfree(item);
  }

  pr_info("hello1: module unloaded, printed %u messages\n", call_id);
}

module_init(hello1_init);
module_exit(hello1_exit);