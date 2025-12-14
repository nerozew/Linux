#!/bin/bash

# 全局清理
rm -f in.txt out.txt in_PROTECT.txt out_PROTECT.txt

echo "--- Test 1: Normal Move ---"
echo "Hello World" > in.txt
rm -f out.txt
./move in.txt out.txt
if [ ! -f in.txt ] && [ -f out.txt ] && grep -q "Hello World" out.txt; then
    echo "[PASS] Normal move"
else
    echo "[FAIL] Normal move"
fi

echo "--- Test 2: Strace Fault Injection (Open Output Fail) ---"
echo "Data" > in.txt
rm -f out.txt  # 关键修复：确保测试前没有 out.txt
# 注入 openat 错误阻止输出文件打开
strace -e inject=openat:error=EACCES:when=4 ./move in.txt out.txt 2> /dev/null
# 预期：程序报错退出，in.txt 保留，out.txt 依然不存在
if [ -f in.txt ] && [ ! -f out.txt ]; then
    echo "[PASS] Injection: Open output failed (Source preserved)"
else
    echo "[FAIL] Injection: Open output failed"
fi

echo "--- Test 3: Strace Fault Injection (Write Fail) ---"
echo "Data" > in.txt
rm -f out.txt
# 注入 write 错误
strace -e inject=write:error=ENOSPC ./move in.txt out.txt 2> /dev/null
# 预期：in.txt 保留，out.txt 被程序清理删除
if [ -f in.txt ] && [ ! -f out.txt ]; then
    echo "[PASS] Injection: Write failed (Cleanup done)"
else
    echo "[FAIL] Injection: Write failed"
fi

echo "--- Test 4: Strace Fault Injection (Close Output Fail) ---"
echo "Data" > in.txt
rm -f out.txt
# 注入 close 错误
strace -e inject=close:error=EIO:when=4 ./move in.txt out.txt 2> /dev/null
# 预期：close 失败视为严重错误，程序应删除 out.txt 并保留 in.txt
if [ -f in.txt ] && [ ! -f out.txt ]; then
    echo "[PASS] Injection: Close failed"
else
    echo "[FAIL] Injection: Close failed"
fi

echo "--- Test 5: LD_PRELOAD Protection ---"
echo "Important Data" > in_PROTECT.txt
rm -f out_PROTECT.txt
# 使用 LD_PRELOAD 保护源文件不被删除
LD_PRELOAD=./protect.so ./move in_PROTECT.txt out_PROTECT.txt 2> /dev/null
# 预期：in_PROTECT.txt 被保护（存在），out_PROTECT.txt 复制成功（存在）
if [ -f in_PROTECT.txt ] && [ -f out_PROTECT.txt ]; then
    echo "[PASS] Protected file not deleted"
else
    echo "[FAIL] Protected file WAS deleted or copy failed"
fi

# 最后清理
rm -f in.txt out.txt in_PROTECT.txt out_PROTECT.txt
