#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/slab.h>

#define COPY_FROM_USER(out_struct, in_struct, size) if (copy_from_user(out_struct, in_struct, size)) return -EFAULT;

#define COPY_TO_USER(out_struct, in_struct) if (copy_to_user(&out_struct, &in_struct, sizeof(out_struct))) return -EFAULT;

struct dentry_info{
	char* name;
	char* parent_name;
	unsigned int inode_uid;
	unsigned int inode_gid;
	unsigned int inode_flags;
};

SYSCALL_DEFINE3(dentry_read, char*, file_path, size_t, file_path_length, struct dentry_info*, user_dentry_info){
	struct path* my_path = kvmalloc(sizeof(struct path), GFP_ATOMIC);
	struct dentry_info* kern_dentry_info = kvmalloc(sizeof(struct dentry_info), GFP_ATOMIC);
	char* file_path_from_user = kvmalloc(sizeof(char) * file_path_length, GFP_ATOMIC);

	COPY_FROM_USER(file_path_from_user, file_path, sizeof(char)*file_path_length);
	printk(KERN_INFO "Syscall dentry_read started.");
	printk(KERN_INFO "Path: %s\n", file_path_from_user);
	int error = kern_path(file_path_from_user, LOOKUP_FOLLOW, my_path);
	if (error){
		printk(KERN_INFO "Syscall ended with error.\n");
		return error;
	}


	kern_dentry_info->name = my_path->dentry->d_name.name;
	kern_dentry_info->parent_name = my_path->dentry->d_parent->d_name.name;
	kern_dentry_info->inode_uid = my_path->dentry->d_inode->i_uid.val;
	kern_dentry_info->inode_gid = my_path->dentry->d_inode->i_gid.val;
	kern_dentry_info->inode_flags = my_path->dentry->d_inode->i_flags;

	COPY_TO_USER(user_dentry_info->name, kern_dentry_info->name);
	COPY_TO_USER(user_dentry_info->parent_name, kern_dentry_info->parent_name);
	COPY_TO_USER(user_dentry_info->inode_uid, kern_dentry_info->inode_uid);
	COPY_TO_USER(user_dentry_info->inode_gid, kern_dentry_info->inode_gid);
	COPY_TO_USER(user_dentry_info->inode_flags, kern_dentry_info->inode_flags);

	kvfree(file_path_from_user);
	kvfree(my_path);
	kvfree(kern_dentry_info);
	return 0;
}
