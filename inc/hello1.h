// SPDX-License-Identifier: Dual BSD/GPL
#ifndef GREETING_H
#define GREETING_H

#include <linux/ktime.h>
#include <linux/list.h>

struct greeting_entry {
  struct list_head list_node;
  ktime_t timestamp;
};

int display_greeting(uint repeat_count);
void clear_greeting_list(void);

#endif // GREETING_H
