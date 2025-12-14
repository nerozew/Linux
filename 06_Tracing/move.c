#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE 4096

void print_error(const char *msg) {
    fprintf(stderr, "Error: %s: %s\n", msg, strerror(errno));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
        return 1;
    }

    const char *infile = argv[1];
    const char *outfile = argv[2];

    int fd_in = -1, fd_out = -1;
    ssize_t bytes_read, bytes_written;
    char buffer[BUF_SIZE];
    struct stat st;

    // 1. 打开源文件
    fd_in = open(infile, O_RDONLY);
    if (fd_in < 0) {
        print_error("Cannot open input file");
        return 2;
    }

    // 获取源文件权限，以便赋给目标文件
    if (fstat(fd_in, &st) < 0) {
        print_error("Cannot stat input file");
        close(fd_in);
        return 3;
    }

    // 2. 打开/创建目标文件
    // O_TRUNC: 清空内容; O_CREAT: 不存在则创建; O_WRONLY: 只写
    fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode & 0777);
    if (fd_out < 0) {
        print_error("Cannot open output file");
        close(fd_in);
        return 4;
    }

    // 3. 复制数据
    while ((bytes_read = read(fd_in, buffer, BUF_SIZE)) > 0) {
        char *ptr = buffer;
        while (bytes_read > 0) {
            bytes_written = write(fd_out, ptr, bytes_read);
            if (bytes_written < 0) {
                print_error("Write error");
                // 写入失败，清理：关闭文件，删除可能已损坏的目标文件
                close(fd_in);
                close(fd_out);
                unlink(outfile); 
                return 5;
            }
            bytes_read -= bytes_written;
            ptr += bytes_written;
        }
    }

    if (bytes_read < 0) {
        print_error("Read error");
        close(fd_in);
        close(fd_out);
        unlink(outfile); // 读取失败，目标文件可能不完整，删除它
        return 6;
    }

    // 4. 安全关闭
    if (close(fd_in) < 0) {
        print_error("Close input error");
        // 虽然关闭输入失败很罕见，但我们并未破坏数据，只是可能没释放资源
        // 这里的策略可以是警告但不删除目标
    }

    if (close(fd_out) < 0) {
        print_error("Close output error");
        // 关闭输出失败可能意味着数据没刷盘
        unlink(outfile);
        return 7;
    }

    // 5. 只有在完全成功后，才删除源文件
    if (unlink(infile) < 0) {
        print_error("Cannot delete input file");
        // 此时数据已复制，但源文件删不掉。这也是一种错误，但至少数据还在。
        return 8;
    }

    return 0;
}
