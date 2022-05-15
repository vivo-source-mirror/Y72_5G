/***************************************************************************************
  wangyuanliang created @ 2013/4/17
***************************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/bbk_drivers_info.h>
#include <linux/gpio.h>


static ssize_t debug_object_show(struct kobject *k, struct attribute *attr, char *buf)
{
	struct kobj_attribute *kobj_attr;
	int ret = -EIO;

	kobj_attr = container_of(attr, struct kobj_attribute, attr);

	if (kobj_attr->show)
		ret = kobj_attr->show(k, kobj_attr, buf);

	return ret;
}

static ssize_t debug_object_store(struct kobject *k, struct attribute *attr,
			      const char *buf, size_t count)
{
	struct kobj_attribute *kobj_attr;
	int ret = -EIO;

	kobj_attr = container_of(attr, struct kobj_attribute, attr);

	if (kobj_attr->store)
		ret = kobj_attr->store(k, kobj_attr, buf, count);

	return ret;
}

static void debug_object_release(struct kobject *kobj)
{
	/* nothing to do temply */
	return;
}

static const struct sysfs_ops debug_object_sysfs_ops = {
	.show = debug_object_show,
	.store = debug_object_store,
};
static struct kobj_type debug_object_type = {
	.sysfs_ops	= &debug_object_sysfs_ops,
	.release	= debug_object_release,
};

static struct kobject kobject_debug;

static int creat_sys_files(void) 
{ 
    int ret; 
	
	ret = kobject_init_and_add(&kobject_debug, &debug_object_type,
					NULL, "devs_list");
    if (ret) { 
		printk("%s: Create kobjetct error!\n", __func__); 
		return -1; 
    } 
    return 0; 
} 

int devs_create_sys_files(const struct attribute *attr)
{
	int ret = 0;
	ret = sysfs_create_file(&kobject_debug, attr);
	if (ret) {
		printk("%s: Create %s sys files error!", __func__, attr->name);
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(devs_create_sys_files);


static int __init bbk_drivers_log_switch_init(void)
{
	int error;

	error = creat_sys_files();
	if (error) {
		printk(KERN_ERR "%s: creat sysfs files failed\n", __func__);
		return error;
	}
	return 0;
}

static void __exit bbk_drivers_log_switch_exit(void)
{
	/*nothing to do */
}

early_initcall(bbk_drivers_log_switch_init);
module_exit(bbk_drivers_log_switch_exit);
