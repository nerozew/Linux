# 关闭分页，防止输出暂停
set pagination off
# 在 range.c 的 printf 语句行打断点（根据上面代码，大概是第44行）
# 这里的行号 '44' 非常重要，如果你的代码行号不同，请调整为 printf 那一行
break 44 if current % 5 == 0

# 定义断点触发时的行为
command 1
    printf "@@@ Start=%d, End=%d, Step=%d, Current=%d\n", start, end, step, current
    continue
end

# 运行程序，重定向标准输出到 /dev/null 以避免干扰
run 1 12 > /dev/null
quit
