#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <rhash.h>
#include <ctype.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

// 辅助函数：将算法名称转为 rhash 的 ID
// 同时返回是否需要大写输出 (format: RHPR_HEX or RHPR_BASE64)
int get_hash_id(const char *name, int *out_format) {
    int id = 0;
    int is_upper = isupper(name[0]);
    *out_format = is_upper ? RHPR_HEX : RHPR_BASE64;

    if (strcasecmp(name, "md5") == 0) id = RHASH_MD5;
    else if (strcasecmp(name, "sha1") == 0) id = RHASH_SHA1;
    else if (strcasecmp(name, "tth") == 0) id = RHASH_TTH;
    // 还可以支持更多，题目只要求这三个
    else id = 0;
    
    return id;
}

void process_line(char *line) {
    if (!line || strlen(line) == 0) return;

    // 1. 解析第一个词 (算法名)
    char *algo_str = strtok(line, " \t\n");
    if (!algo_str) return;

    // 2. 解析第二个词 (文件名或字符串)
    char *data_str = strtok(NULL, " \t\n");
    if (!data_str) {
        fprintf(stderr, "Error: missing argument\n");
        return;
    }

    int format;
    int hash_id = get_hash_id(algo_str, &format);
    if (hash_id == 0) {
        fprintf(stderr, "Error: unknown algorithm '%s'\n", algo_str);
        return;
    }

    unsigned char digest[64];
    int res = 0;
    
    // 3. 判断是字符串还是文件
    if (data_str[0] == '"') {
        // 字符串模式：去除首个 "，内容直到结束（或者你可以更严谨地找下一个 "）
        char *content = data_str + 1; 
        // 简单的处理：计算 content 的长度
        size_t len = strlen(content);
        // 如果末尾有 " 则去掉 (可选优化)
        // if (len > 0 && content[len-1] == '"') content[len-1] = 0;
        
        res = rhash_msg(hash_id, content, strlen(content), digest);
    } else {
        // 文件模式
        res = rhash_file(hash_id, data_str, digest);
    }

    if (res < 0) {
        fprintf(stderr, "Error processing '%s': %s\n", data_str, strerror(errno));
        return;
    }

    // 4. 输出结果
    char output[130];
    rhash_print_bytes(output, digest, rhash_get_digest_size(hash_id), format);
    printf("%s\n", output);
}

int main() {
    rhash_library_init();

    char *line = NULL;
#ifdef HAVE_LIBREADLINE
    while ((line = readline(NULL)) != NULL) {
        if (*line) add_history(line);
        process_line(line);
        free(line);
    }
#else
    size_t len = 0;
    while (getline(&line, &len, stdin) != -1) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        process_line(line);
    }
    free(line);
#endif

    return 0;
}
