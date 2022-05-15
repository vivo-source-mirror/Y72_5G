// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2012-2019, The Linux Foundation. All rights reserved.
 *
 * author:wangweiguo, reason: add bsptest record info.
 * date:2021-02-20
 */

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/string.h>

/*struct bsptest item {	//size 96 byte
	char bsptest_type[BSPTEST_ITEM_LEN];	//测试类型，老化、压力、其他
	char bsptest_item[BSPTEST_ITEM_LEN];	//测试项
	char bsptest_time[BSPTEST_ITEM_LEN];	//开始测试时间
	char bsptest_model[BSPTEST_ITEM_LEN];	//异常报错模块
}*/
/*for example:custom,CpuTest,159535299912,cpu*/
#define BSPTEST_ITEMS_MAX	(128)
#define BSPTEST_ITEM_LEN   (32)
#define BSPTEST_NUM_MAX		(BSPTEST_ITEMS_MAX/BSPTEST_ITEM_LEN)
/*store bsptest info*/
static char bsptest_info[BSPTEST_ITEMS_MAX] = {0};

static int bsptest_info_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", bsptest_info);
	return 0;
}

static int bsptest_info_open(struct inode *inode, struct file *file)
{
	return single_open(file, bsptest_info_show, NULL);
}

static ssize_t bsptest_info_write(struct file *file, const char __user *buf,
				   size_t count, loff_t *ppos)
{
	if (count >= BSPTEST_ITEMS_MAX) {
		printk(KERN_ERR "BSPTest: write string too long(%d)\n", count);
		return -ENOSPC;
	}

	/*clear string, last index must 0*/
	memset(bsptest_info, 0, BSPTEST_ITEMS_MAX);

	/*Notice: count-1, mean remove enter char*/
	if(copy_from_user(bsptest_info, buf, count - 1)) {
		printk(KERN_ERR "BSPTest: write(%d) failed!!!\n", count);
		return -EFAULT;
	}
	printk(KERN_INFO "BSPTest:%s\n", bsptest_info);
	return count;
}

static const struct file_operations proc_bsptest_info_operations = {
	.open 		= bsptest_info_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= bsptest_info_write,
};


static int __init bsptest_setup(char *str)
{
	int i = 0;
	int len = strlen(str);
	int sum = 0;
	char *temp_str = NULL;

	if (BSPTEST_ITEMS_MAX <= len){
		printk(KERN_ERR "BSPTest: backup sum string too long(%d)\n", len);
		return -ENOSPC;
	}

	/*clear string, last index must 0*/
	memset(bsptest_info, 0, BSPTEST_ITEMS_MAX);

	for (i = 0; i < BSPTEST_NUM_MAX; i++) {
		/*split the string, find the item value.*/
		temp_str = strsep(&str, ",");
		len = strlen(temp_str);

		if (BSPTEST_ITEM_LEN <= len) {
			printk(KERN_ERR "BSPTest: backup item string too long(%d)\n", len);
			return -ENOSPC;
		}
		/*get backup partition bsptest info*/
		sum += snprintf(bsptest_info + sum, len, "%s", temp_str);
	}

	printk(KERN_INFO "BSPTest:%s\n", bsptest_info);
	return 0;
}
__setup("bsptest=", bsptest_setup);

static int __init proc_bsptest_info_init(void)
{
	if(!proc_create("bsptest_info", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, NULL,
			&proc_bsptest_info_operations)) {
		printk(KERN_ERR "BSPTest: Failed to register proc interface\n");
		return -EFAULT;
	}
	return 0;
}

fs_initcall(proc_bsptest_info_init);
