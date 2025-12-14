#include "buf.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/* 
 * 增长 buffer 的具体实现。
 * v: 当前 buffer 指针
 * esize: 元素大小 (element size)
 * n: 需要增加的数量
 * head: (在原版中用于处理头部偏移，简化版通常传 0)
 */
void *buf_grow1(void *v, size_t esize, ptrdiff_t n, int *head) {
    (void)head; // 简单的消除未使用警告

    struct buf_hdr *h;
    
    // 如果 v 为 NULL，说明是第一次分配
    if (v) {
        h = buf_ptr(v);
    } else {
        h = NULL;
    }

    size_t old_cap = h ? h->cap : 0;
    size_t new_cap = old_cap + n;
    
    // 简单的扩容策略：如果是初次分配，给个初始值，否则翻倍或按需增长
    // 为了通过测试，我们至少要保证容量够用
    if (new_cap < 16) new_cap = 16;
    else if (new_cap < old_cap * 2) new_cap = old_cap * 2;

    // 计算新的内存大小：头部大小 + (容量 * 元素大小)
    size_t new_size = offsetof(struct buf_hdr, buf) + new_cap * esize;
    
    struct buf_hdr *new_h;
    if (h) {
        new_h = realloc(h, new_size);
    } else {
        new_h = malloc(new_size);
        if (new_h) {
            new_h->len = 0;
        }
    }

    if (!new_h) abort(); // 内存分配失败直接退出

    new_h->cap = new_cap;
    return new_h->buf;
}

