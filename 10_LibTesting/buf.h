#ifndef BUF_H
#define BUF_H

#include <stddef.h>
#include <stdlib.h>

/* buf_grow1 的声明，它在 buf.c 中实现 */
void *buf_grow1(void *v, size_t esize, ptrdiff_t n, int *head);

/* 核心宏定义 - 直接来自 growable-buf */

// 获取头部信息结构体的指针
#define buf_ptr(v) \
    ((struct buf_hdr *)((char *)(v) - offsetof(struct buf_hdr, buf)))

// 获取 buffer 的各个属性
#define buf_free(v) \
    ((v) ? (free(buf_ptr(v)), (v) = 0) : 0)

#define buf_push(v, e) \
    ((v) == buf_cap(v) ? (v) = buf_grow1(v, sizeof(*(v)), 1, 0) : 0, \
     (v)[buf_ptr(v)->len++] = (e))

#define buf_size(v) \
    ((v) ? buf_ptr(v)->len : 0)

#define buf_capacity(v) \
    ((v) ? buf_ptr(v)->cap : 0)

#define buf_cap(v) \
    ((v) ? (v) + buf_ptr(v)->cap : 0)

#define buf_grow(v, n) \
    ((v) = buf_grow1((v), sizeof(*(v)), (n), 0))

// 内部头部结构
struct buf_hdr {
    size_t len;
    size_t cap;
    char buf[];
};

#endif

