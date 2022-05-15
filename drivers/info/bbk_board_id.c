/*
 *  bbk_board_id.c
 *
 * Copyright (C) 2020 Vivo, Inc.
 * Author: MRQ <rongqiang.ma@vivo.com.cn>
 *
 * Description: cali the hardware board version and show it
 *
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/iio/consumer.h>
#include <linux/iio/iio.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/iio/consumer.h>
#include <linux/iio/iio.h>
#include <linux/bbk_drivers_info.h>

struct board_auxadc_data {

	char board_id[128];
	int board_adc_num;
	int board_adc_val;

};

struct board_auxadc_data *bd_data;

static struct iio_channel *md_channel;

static ssize_t board_id_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	if(bd_data->board_adc_val >= 0 && bd_data->board_adc_val < 50)
		sprintf(bd_data->board_id, "Version1");
	else if(bd_data->board_adc_val >= 50 && bd_data->board_adc_val < 150)
		sprintf(bd_data->board_id, "Version2");
	else if(bd_data->board_adc_val >= 150 && bd_data->board_adc_val < 250)
		sprintf(bd_data->board_id, "Version3");
	else if(bd_data->board_adc_val >= 250 && bd_data->board_adc_val < 350)
		sprintf(bd_data->board_id, "Version4");
	else if(bd_data->board_adc_val >= 350 && bd_data->board_adc_val < 450)
		sprintf(bd_data->board_id, "Version5");
	else if(bd_data->board_adc_val >= 450 && bd_data->board_adc_val < 550)
		sprintf(bd_data->board_id, "Version6");
	else if(bd_data->board_adc_val >= 550 && bd_data->board_adc_val < 650)
		sprintf(bd_data->board_id, "Version7");
	else if(bd_data->board_adc_val >= 650 && bd_data->board_adc_val < 750)
		sprintf(bd_data->board_id, "Version8");
	else if(bd_data->board_adc_val >= 750 && bd_data->board_adc_val < 850)
		sprintf(bd_data->board_id, "Version9");
	else if(bd_data->board_adc_val >= 850 && bd_data->board_adc_val < 950)
		sprintf(bd_data->board_id, "Version10");
	else if(bd_data->board_adc_val >= 950 && bd_data->board_adc_val < 1050)
		sprintf(bd_data->board_id, "Version11");
	else if(bd_data->board_adc_val >= 1050 && bd_data->board_adc_val < 1150)
		sprintf(bd_data->board_id, "Version12");
	else if(bd_data->board_adc_val >= 1150 && bd_data->board_adc_val < 1250)
		sprintf(bd_data->board_id, "Version13");
	else if(bd_data->board_adc_val >= 1250 && bd_data->board_adc_val < 1350)
		sprintf(bd_data->board_id, "Version14");
	else if(bd_data->board_adc_val >= 1350 && bd_data->board_adc_val < 1450)
		sprintf(bd_data->board_id, "Version15");
	else 
		sprintf(bd_data->board_id, "Error");

	return sprintf(buf, bd_data->board_id);
}

static struct debug_sysfs_entry board_id = 
		__ATTR(board_id, S_IRUGO, board_id_show, NULL);

static int board_get_adc_info(struct device *dev)
{	
	int ret, val, adc_num, adc_val;

	md_channel = devm_kzalloc(dev, sizeof(struct iio_channel), GFP_KERNEL);
	if (!md_channel) {
		printk("allocate md channel fail");
		return -1;
	}
	md_channel = iio_channel_get(dev, "board-channel");

	ret = IS_ERR(md_channel);
	if (ret) {
		printk("fail to get iio channel (%d)", ret);
		goto Fail;
	}
	adc_num = md_channel->channel->channel;
	ret = iio_read_channel_processed(md_channel, &val);
	if (ret < 0) {
		printk("iio_read_channel_processed fail");
		goto Fail;
	}

	adc_val = ((val * 1500) / 4096);
	printk("adc_num = %d, val = %d, adc_val =%d", adc_num, val, adc_val);

	return adc_val;
Fail:
	return -1;

}

int get_board_auxadc_probe(struct platform_device *pdev)
{	
	int ret;

	struct board_auxadc_data *board_auxadc_data;

	board_auxadc_data = kzalloc(sizeof(struct board_auxadc_data), GFP_KERNEL);
	if (!board_auxadc_data)
		return -ENOMEM;

	bd_data = board_auxadc_data;

	bd_data->board_adc_val = board_get_adc_info(&pdev->dev);
	if (bd_data->board_adc_val < 0)
		printk("board get adc info fail");

	ret = devs_create_sys_files(&board_id.attr);
	if (ret < 0) {
		printk(KERN_ERR ": model value sys files create error \n");
		goto free_pdata;
	}

	return 0;

free_pdata:
	kfree(board_auxadc_data);
	
	return ret;
}

static int get_board_auxadc_remove(struct platform_device *pdev)
{
	struct board_auxadc_data *board_auxadc_data = platform_get_drvdata(pdev);

	kfree(board_auxadc_data);

	return 0;
}

              
static const struct of_device_id board_auxadc_of_ids[] = {
	{.compatible = "mediatek,board_auxadc"},
	{}
};

static struct platform_driver board_auxadc_driver = {
    .probe      = get_board_auxadc_probe,
    .remove     = get_board_auxadc_remove,
    .driver     = {
		.name   = "board_auxadc",       
		.of_match_table = board_auxadc_of_ids,
	},
	.probe = get_board_auxadc_probe,
};

static int __init board_auxadc_init(void)
{	
	int ret;
	ret = platform_driver_register(&board_auxadc_driver);
	if (ret) {
		printk("board auxadc driver init fail %d", ret);
		return ret;
	}
	return 0;
}

static void __exit board_auxadc_exit(void)
{
	platform_driver_unregister(&board_auxadc_driver);
}

module_init(board_auxadc_init);
module_exit(board_auxadc_exit);

MODULE_AUTHOR("mrq");
MODULE_DESCRIPTION("board auxadc driver");
MODULE_LICENSE("GPL");

