#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x28950ef1, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x3fa89e8f, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x41ec4c1a, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x98ab5c8d, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x5f675a65, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xdacd8618, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x71de9b3f, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xfa66f77c, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x5c8b5ce8, __VMLINUX_SYMBOL_STR(prepare_to_wait) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xc8b57c27, __VMLINUX_SYMBOL_STR(autoremove_wake_function) },
	{ 0xb8c7ff88, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xe84cb310, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0xcf21d241, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x77e2f33, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x2ea2c95c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rax) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1A85BA41F9B51E451618419");
MODULE_INFO(rhelversion, "7.8");
#ifdef RETPOLINE
	MODULE_INFO(retpoline, "Y");
#endif
#ifdef CONFIG_MPROFILE_KERNEL
	MODULE_INFO(mprofile, "Y");
#endif
