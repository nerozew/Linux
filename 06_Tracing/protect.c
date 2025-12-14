#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>

// 定义函数指针类型
typedef int (*unlink_type)(const char *pathname);

int unlink(const char *pathname) {
    // 检查文件名是否包含 "PROTECT"
    if (strstr(pathname, "PROTECT") != NULL) {
        // 如果包含，禁止删除
        // 我们可以打印一条消息（可选），并返回错误
        // 注意：不要用 printf，因为它可能会干扰被注入程序的标准输出流
        // 最好写入 stderr
        // fprintf(stderr, "[PROTECT] Prevented deletion of %s\n", pathname);
        errno = EACCES; // 设置错误码为 Permission denied
        return -1;
    }

    // 如果不包含，调用原始的 unlink
    unlink_type original_unlink;
    original_unlink = (unlink_type)dlsym(RTLD_NEXT, "unlink");

    if (!original_unlink) {
        fprintf(stderr, "Error finding original unlink: %s\n", dlerror());
        errno = EFAULT;
        return -1;
    }

    return original_unlink(pathname);
}
