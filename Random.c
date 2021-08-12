#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>

#define DRIVER_AUTHOR "Tran Xuan Son - 19127321 - <tranxuason2013@gmail.com>"
#define DRIVER_DESC "Generate random number"
#define DRIVER_LICENSE "GPL"
#define DRIVER_SUPPORTED_DEVICE "test"

#define DEVICE_NAME "RandomDevice"
#define DEVICE_CLASS_NAME "Random"

static dev_t deviceNumber;
static struct class *cl = NULL;
static struct cdev c_dev;
static int randomNumber;
static char temp[11];
static char res[12];
static int i = 0;
static int j = 0;

static int r_open(struct inode *, struct file *);
static ssize_t r_read(struct file *, char *, size_t, loff_t *);
static int r_release(struct inode *, struct file *);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = r_open,
    .read = r_read,
    .release = r_release
};

static int __init RANDOM_init(void) {
    if (alloc_chrdev_region(&deviceNumber, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT
               "Random Driver: Failed to allocate major and minor number\n");
        return -1;
    }

    if ((cl = class_create(THIS_MODULE, DEVICE_CLASS_NAME)) == NULL) {
        unregister_chrdev_region(deviceNumber, 1);
        printk(KERN_ALERT "Random Driver: Failed to register device class\n");
        return -1;
    }

    if (device_create(cl, NULL, deviceNumber, NULL, DEVICE_NAME) == NULL) {
        class_destroy(cl);
        unregister_chrdev_region(deviceNumber, 1);
        return -1;
    }

    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, deviceNumber, 1) == -1) {
        device_destroy(cl, deviceNumber);
        class_destroy(cl);
        unregister_chrdev_region(deviceNumber, 1);
        return -1;
    }

    printk(KERN_INFO "Random Driver registered\n<Major, Minor>: <%d, %d>\n",
           MAJOR(deviceNumber), MINOR(deviceNumber));

    return 0;
}

static void __exit RANDOM_exit(void) /* Destructor */
{
    device_destroy(cl, deviceNumber);
    class_destroy(cl);
    unregister_chrdev_region(deviceNumber, 1);
    printk(KERN_INFO "Random Driver unregistered");
}

static int r_open(struct inode *ipf, struct file *f) {
    printk(KERN_INFO "Random Driver: Device opening\n");
    return 0;
}

static ssize_t r_read(struct file *ipf, char *buffer, size_t len,
                      loff_t *offset) {
    if (len < 12){
        printk(KERN_ALERT "Random Driver: Buffer length too small");
        return -EFAULT;
    }

    get_random_bytes(&randomNumber, sizeof(randomNumber));
    printk(KERN_INFO "Random Driver: Generated number = %d", randomNumber);
    i = 0;
    j = 0;
    if (randomNumber < 0) {
        randomNumber *= -1;
        res[j++] = '-';
    }
    do
    {
        temp[i++] = randomNumber % 10 + '0';
        randomNumber /= 10;
    } while (randomNumber != 0);
    
    res[i] = '\0';
    while (i >= 0){
        res[j++] = temp[--i]; 
    }
    printk(KERN_INFO "Random Driver: String = '%s'", res);
    if (copy_to_user(buffer, res, j) != 0) 
        return -EFAULT; 
    else 
        return 1;
}

static int r_release(struct inode *ipf, struct file *f) {
    printk(KERN_INFO "Random Driver: Device released\n");
    return 0;
}

module_init(RANDOM_init);
module_exit(RANDOM_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE(DRIVER_SUPPORTED_DEVICE);