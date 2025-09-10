#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#define target_fd 3

// 原始write函数指针
static ssize_t (*original_write)(int, const void *, size_t) = NULL;

// 我们的过滤函数
ssize_t write(int fd, const void *buf, size_t count) {
    // 初始化原始函数指针
    if (!original_write) {
        original_write = dlsym(RTLD_NEXT, "write");
    }
    
    // 只对特定文件描述符进行过滤
    if (fd == target_fd) { // 你需要定义如何识别目标文件描述符
        printf("Filtered write: fd=%d, buf=%s, count=%zu\n", fd, (char*)buf, count);
        return count;
    }
    
    // 其他情况直接调用原始函数
    return original_write(fd, buf, count);
}