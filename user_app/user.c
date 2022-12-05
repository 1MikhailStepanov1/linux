#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <malloc.h>
#include <stdio.h>


struct page_info{
	unsigned long page_flags;
	void* page_mapping;
	long vm_address;
};


struct dentry_info{
	char* name;
	char* parent_name;
	unsigned int inode_uid;
	unsigned int inode_gid;
	unsigned int inode_flags;
};


static void write_page_info(struct page_info* input_page_info){
	printf("Page flags: %d\n", input_page_info->page_flags);
	printf("Virtual address: %x, Page address: %x\n", input_page_info->vm_address, input_page_info->page_mapping);
}

static void write_dentry_info(struct dentry_info* input_dentry_info){
	printf("Dentry name: %s\n", input_dentry_info->name);
	printf("Dentry's parent name: %s\n", input_dentry_info->parent_name);
	printf("Inode UID: %d  Inode GID: %d\n", input_dentry_info->inode_uid, input_dentry_info->inode_gid);
	printf("Inode flags: %d\n", input_dentry_info->inode_flags);
}

int main(){
	int ans = 0;
	int pid;
	char* file_path;
	struct page_info* my_page_info = (struct page_info*) malloc(sizeof(struct page_info));
	struct dentry_info* my_dentry_info = (struct dentry_info*) malloc(sizeof(struct dentry_info));

	printf("Input PID:\n");
	scanf("%d", &pid);
	ans = syscall(548, pid, my_page_info);
	if (my_page_info == NULL){
		printf("Cannot find page by PID");
	} else {
		write_page_info(my_page_info);
	}
	printf("Input file path:\n");
	scanf("%s", file_path);
	ans = syscall(549, file_path, sizeof(file_path), my_dentry_info);
	printf("%d\n", ans);
	write_dentry_info(my_dentry_info);
	free(my_page_info);
	free(my_dentry_info);
	return 0;
}
