/*************************************************************************
  > File Name: mem_channel.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Fri 26 Jun 2020 12:59:42 PM CST
 ************************************************************************/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/poll.h>

// first device id in kernel
#ifndef MEM_CHANNEL_MAJOR
#define MEM_CHANNEL_MAJOR 110
#endif

//second device id in kernel
#ifndef MEM_CHANNEL_MINOR
#define MEM_CHANNEL_MINOR 2
#endif

#ifndef MEM_CHANNEL_DATA_LENGTH
#define MEM_CHANNEL_DATA_LENGTH 4096
#endif

#define ENABLE_POLL 1
#if ENABLE_POLL
uint8_t is_have_data = 0;
#endif
const char *mem_channel_device = "memchan";
struct cdev mem_channel_dev;
struct mem_channel
{
  char *data;
  size_t size;
#if ENABLE_POLL
  wait_queue_head_t queue;
#endif
};
struct mem_channel *chan;
int channel_major = MEM_CHANNEL_MAJOR;
module_param(channel_major, int, S_IRUGO);
int mem_channel_open(struct inode *node, struct file *pfile)
{
  struct mem_channel *mem = NULL;
  int num = node->i_rdev;
  if (num > MEM_CHANNEL_MINOR || num == 0)
  {
    return -ENODEV;
  }
  mem = &chan[num];
  pfile->private_data = mem;
  printk(KERN_INFO "mem_channel_open %d device success\n", num);
  return 0;
}
int mem_channel_release(struct inode *node, struct file *pfile)
{
  return 0;
}
ssize_t mem_channel_read(struct file *pfile, char __user *buffer, size_t size, loff_t *ppos)
{
  int ret;
  unsigned long p = *ppos;
  unsigned int count = size;
  struct mem_channel *mem = pfile->private_data;
  if (p > MEM_CHANNEL_DATA_LENGTH)
  {
    return 0;
  }
  if (count > (MEM_CHANNEL_DATA_LENGTH - p))
  {
    count = MEM_CHANNEL_DATA_LENGTH - p;
  }
#if ENABLE_POLL
  while (!is_have_data)
  {
    if (pfile->f_flags & O_NONBLOCK)
    {
      return -EAGAIN;
    }
    wait_event_interruptible(mem->queue, is_have_data);
  }
#endif
  if (copy_to_user(buffer, (void *)(mem->data + p), count))
  {
    ret = -EFAULT;
  }
  else
  {
    ret = strlen(buffer);
    mem->size -= ret;
  }
  printk(KERN_INFO "read %d bytes from %ld\n", ret, p);
  return ret;
}
ssize_t mem_channel_write(struct file *pfile, const char __user *buffer, size_t size, loff_t *ppos)
{
  int ret;
  unsigned long p = *ppos;
  unsigned int count = size;
  struct mem_channel *mem = (struct mem_channel *)pfile->private_data;
  if (p > MEM_CHANNEL_DATA_LENGTH)
  {
    return 0;
  }
  if (count > (MEM_CHANNEL_DATA_LENGTH - p))
  {
    count = MEM_CHANNEL_DATA_LENGTH - p;
  }
  if (copy_from_user((void *)(mem->data + p), buffer, count))
  {
    ret = -EFAULT;
  }
  else
  {
    *ppos += count;
    ret = count;
    mem->size += count;
    *(mem->data + p + count) = '\0';
    printk(KERN_INFO "write %d bytes from %ld\n", count, p);
  }
#if ENABLE_POLL
  is_have_data = 1;
  wake_up(&mem->queue);
#endif
  return ret;
}

#if ENABLE_POLL
unsigned int mem_channel_poll(struct file *pfile, struct poll_table_struct *wait)
{

  struct mem_channel *mem = pfile->private_data;
  unsigned int mask = 0;

  poll_wait(pfile, &mem->queue, wait);

  if (is_have_data)
  {
    mask |= (POLLIN | POLLRDNORM);
  }

  return mask;
}
#endif
int mem_channel_mmap(struct file *pfile, struct vm_area_struct *vma)
{
  struct mem_channel *mem = pfile->private_data;

  vma->vm_flags |= VM_IO;
  vma->vm_flags |= (VM_DONTEXPAND | VM_DONTDUMP);

  if (remap_pfn_range(vma, vma->vm_start, virt_to_phys(mem->data) >> PAGE_SHIFT,
                      vma->vm_end - vma->vm_start, vma->vm_page_prot))
  {
    return -EAGAIN;
  }
  return 0;
}
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = mem_channel_open,
    .release = mem_channel_release,
    .read = mem_channel_read,
    .write = mem_channel_write,
    .poll = mem_channel_poll,
    .mmap = mem_channel_mmap,
};
//when execute insmod xx.ko,mem_channel_init will be called
static int mem_channel_init(void)
{
  int result;
  int i = 0;
  //init first device no
  dev_t devno = MKDEV(channel_major, 0);
  //init minor device array for first device

  if (channel_major)
  {
    result = register_chrdev_region(devno, MEM_CHANNEL_MINOR, mem_channel_device);
  }
  else
  {
    result = alloc_chrdev_region(&devno, 0, MEM_CHANNEL_MINOR, mem_channel_device);
    channel_major = MAJOR(devno);
  }

  //result = register_chrdev_region(devno, MEM_CHANNEL_MINOR, mem_channel_device);
  if (result != 0)
  {
    return result;
  }
  cdev_init(&mem_channel_dev, &fops);
  mem_channel_dev.owner = THIS_MODULE;
  mem_channel_dev.ops = &fops;
  //add device to kernel device list,that save data
  cdev_add(&mem_channel_dev, MKDEV(channel_major, 0), MEM_CHANNEL_MINOR);
  chan = kmalloc(MEM_CHANNEL_MINOR * sizeof(struct mem_channel), GFP_KERNEL);
  if (chan == NULL)
  {
    result = -ENOMEM;
    goto failed;
  }
  memset(chan, 0, sizeof(struct mem_channel) * MEM_CHANNEL_MINOR);
  for (; i < MEM_CHANNEL_MINOR; i++)
  {
    chan[i].size = MEM_CHANNEL_DATA_LENGTH;
    chan[i].data = kmalloc(MEM_CHANNEL_DATA_LENGTH * sizeof(char), GFP_KERNEL);
    if (chan[i].data == NULL)
    {
      goto failed;
    }
    memset(chan[i].data, 0, MEM_CHANNEL_DATA_LENGTH);
  }
  printk(KERN_INFO "mem_channel init success\n");
  return 0;
  failed:
      unregister_chrdev_region(devno, 1);
  for (i = 0; i < MEM_CHANNEL_MINOR; i++)
  {
    if (chan[i].data != NULL)
    {
      kfree(chan[i].data);
      chan[i].data = NULL;
    }
    chan[i].size = 0;
  }
  if (chan != NULL)
  {
    kfree(chan);
    chan = NULL;
  }
  return result;
}
//when rmmod xx.ko,mem_channel_exit will be called
static void mem_channel_exit(void)
{
  int i;
  cdev_del(&mem_channel_dev);
  for (i = 0; i < MEM_CHANNEL_MINOR; i++)
  {

    kfree(chan[i].data);
    chan[i].data = NULL;
    chan[i].size = 0;
  }
  if (chan != NULL)
  {
    kfree(chan);
    chan = NULL;
  }
  unregister_chrdev_region(MKDEV(channel_major, 0), MEM_CHANNEL_MINOR);
  printk(KERN_INFO "mem_channel_exit succes\n");
}

MODULE_AUTHOR("perrynzhou@gmail.com");
MODULE_LICENSE("GPL");

module_init(mem_channel_init);
module_exit(mem_channel_exit);
