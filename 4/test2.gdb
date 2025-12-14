set pagination off
# 在 printf 行打断点，条件是计数器 i 在 [27, 34] 之间
# 注意：人类说的第1个通常对应计算机的索引0，所以第28个是索引27
break 44 if i >= 27 && i <= 34

command 1
    printf "@@@ Start=%d, End=%d, Step=%d, Current=%d, Index=%d\n", start, end, step, current, i
    continue
end

run -100 100 3 > /dev/null
quit
