#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/regulator/consumer.h>

static const char * const pctl_names[] = {
	"vivo_gpio_active",
	"vivo_gpio_sleep",
};

struct pinctrl       *vivo_gpio_pinctrl;
struct pinctrl_state *vivo_gpio_pinctrl_state[ARRAY_SIZE(pctl_names)];

static int select_pin_ctl(struct device *dev, const char *name)
{
	size_t i;
	int rc;

	for (i = 0; i < ARRAY_SIZE(vivo_gpio_pinctrl_state); i++) {
		const char *temp_name = pctl_names[i];
		if (!strncmp(temp_name, name, strlen(temp_name))) {
			rc = pinctrl_select_state(vivo_gpio_pinctrl, vivo_gpio_pinctrl_state[i]);
			if (rc)
				dev_err(dev, "cannot select '%s'\n", name);
			else
				dev_dbg(dev, "Selected '%s'\n", name);
			goto exit;
		}
	}
	rc = -EINVAL;
	dev_err(dev, "%s:'%s' not found\n", __func__, name);
exit:
	return rc;
}

static int vivo_gpio_probe(struct platform_device *pdev)
{
	int ret;
	int i;

	vivo_gpio_pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(vivo_gpio_pinctrl)) {
		if (PTR_ERR(vivo_gpio_pinctrl) == -EPROBE_DEFER) {
			printk("%s: pinctrl not ready!\n", __func__);
			return -1;
		}
		printk("%s: Target does not use pinctrl\n", __func__);
		vivo_gpio_pinctrl = NULL;
		return -1;
	}

	for (i = 0; i < ARRAY_SIZE(vivo_gpio_pinctrl_state); i++) {
		const char *name = pctl_names[i];
		struct pinctrl_state *state = pinctrl_lookup_state(vivo_gpio_pinctrl, name);
		if (IS_ERR(state)) {
			printk("%s: cannot find '%s'\n", __func__, name);
			return -1;
		}
		printk("%s: found pin control %s\n", __func__, name);
		vivo_gpio_pinctrl_state[i] = state;
	}

	//set to default state
	ret = select_pin_ctl(&pdev->dev, "vivo_gpio_sleep");
	if (ret)
		return -1;

	return 0;
}

static int vivo_gpio_remove(struct platform_device *pdev)
{
	return 0;
}

static int vivo_gpio_suspend(struct platform_device *pdev, pm_message_t state)
{
	//set to default state
	//select_pin_ctl(&pdev->dev, "vivo_gpio_sleep");
	return 0;
}

static int vivo_gpio_resume(struct platform_device *pdev)
{
	return 0;
}

static struct of_device_id vivo_gpio_match_table[] = {
	{ .compatible = "vivo_gpio", },
	{},
};

static struct platform_driver vivo_gpio_driver = {
	.probe      = vivo_gpio_probe,
	.remove     = vivo_gpio_remove,
	.resume     = vivo_gpio_resume,
	.suspend    = vivo_gpio_suspend,
	.driver = {
		.name   = "vivo_gpio",
		.owner  = THIS_MODULE,
		.of_match_table = vivo_gpio_match_table,
	},
};

static int __init vivo_gpio_init(void)
{
	return platform_driver_register(&vivo_gpio_driver);
}
module_init(vivo_gpio_init);

static void __exit vivo_gpio_exit(void)
{
	platform_driver_unregister(&vivo_gpio_driver);
}
module_exit(vivo_gpio_exit);

MODULE_AUTHOR("xuling vivo Corporation");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
