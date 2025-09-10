#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: %s <消息>\n", argv[0]);
        return 1;
    }
    
    // 确保FIFO存在 (不需要创建，因为读取端已经创建了)
    if (access(FIFO_NAME, F_OK) == -1) {
        perror("FIFO不存在");
        return 1;
    }
    
    printf("尝试打开FIFO进行写入...\n");
    
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("打开FIFO失败");
        return 1;
    }
    
    printf("读取进程已连接，开始写入数据...\n");
    
    // 写入消息
    ssize_t written = write(fd, argv[1], strlen(argv[1]));
    if (written == -1) {
        perror("写入失败");
        close(fd);
        return 1;
    }
    
    // 添加换行符
    write(fd, "\n", 1);
    
    close(fd);
    return 0;
}