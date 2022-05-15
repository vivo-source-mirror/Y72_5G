/****************************************************************************************
  wangyuanliang created @ 2013/4/17
****************************************************************************************/
#ifndef BBK_DRIVERS_LOG_SWITCH_H
#define BBK_DRIVERS_LOG_SWITCH_H


struct debug_sysfs_entry {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf);
	ssize_t (*store)(struct kobject *kobj,
					struct kobj_attribute *attr,  const char *buf, size_t count);
};


extern char *get_board_version(void);
extern int devs_create_sys_files(const struct attribute *attr);
#endif
