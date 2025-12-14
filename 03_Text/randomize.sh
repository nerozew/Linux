#!/bin/sh

# 默认延迟时间为 0
DELAY=${1:-0}

# 捕获信号，退出时清理临时文件并恢复光标
trap 'rm -f "$TMP_IN" "$TMP_UCS" "$TMP_LIST" "$TMP_SHUF"; tput cnorm; exit' INT TERM EXIT

# 创建临时文件
TMP_IN=$(mktemp)
TMP_UCS=$(mktemp)
TMP_LIST=$(mktemp)
TMP_SHUF=$(mktemp)

# 1. 读取标准输入到临时文件
cat > "$TMP_IN"

# 2. 获取屏幕尺寸 (stty size 在非终端输入时需要重定向 /dev/tty)
# output: "rows cols"
# 某些系统 stty 输出格式不同，这里增加容错
TERM_SIZE=$(stty size < /dev/tty 2>/dev/null || echo "24 80")
TERM_LINES=${TERM_SIZE% *}
TERM_COLS=${TERM_SIZE#* }

# 3. 计算输入文本的尺寸 (行数和最大宽度)
# 使用 awk 计算最大行宽
TEXT_DIM=$(awk '{ if (length($0) > max) max = length($0) } END { print NR, max }' "$TMP_IN")
TEXT_LINES=${TEXT_DIM% *}
TEXT_COLS=${TEXT_DIM#* }

# 计算居中偏移量
# ((TERM_LINES - TEXT_LINES) / 2)
OFFSET_Y=$(( (TERM_LINES - TEXT_LINES) / 2 ))
# ((TERM_COLS - TEXT_COLS) / 2)
OFFSET_X=$(( (TERM_COLS - TEXT_COLS) / 2 ))

# 防止负偏移
if [ "$OFFSET_Y" -lt 0 ]; then OFFSET_Y=0; fi
if [ "$OFFSET_X" -lt 0 ]; then OFFSET_X=0; fi

# 4. 将输入转换为 UCS-2BE (定长 2 字节)
# 注意：ASCII 字符 'A' 会变成 00 41
if ! iconv -f UTF-8 -t UCS-2BE "$TMP_IN" > "$TMP_UCS" 2>/dev/null; then
    echo "Error: iconv failed. Ensure input is UTF-8." >&2
    exit 1
fi

# 5. 解析 UCS-2 流并生成坐标列表
# od -An (无地址) -v (不省略重复) -t x1 (单字节十六进制) -w1 (每行一个字节)
# 列表格式: Y X HexByte1 HexByte2

# 初始化坐标
CUR_Y=0
CUR_X=0

# 这里使用文件描述符重定向，确保变量在主 shell 中可见
od -An -v -t x1 -w1 "$TMP_UCS" | while read b1; do
    read b2
    
    # 检查是否读完
    if [ -z "$b2" ]; then break; fi
    
    # 组合两个字节的 Hex 码 (去除可能的空格)
    # od 输出可能有前导空格，用 tr 删除
    b1=$(echo "$b1" | tr -d ' ')
    b2=$(echo "$b2" | tr -d ' ')
    
    hex="$b1$b2"
    
    # 检测换行符 (UCS-2BE 中换行符是 00 0a)
    if [ "$hex" = "000a" ]; then
        CUR_Y=$((CUR_Y + 1))
        CUR_X=0
    else
        # 将坐标和字符的 hex 码写入列表
        # 格式: row col hex_byte1 hex_byte2
        echo "$((CUR_Y + OFFSET_Y)) $((CUR_X + OFFSET_X)) $b1 $b2" >> "$TMP_LIST"
        CUR_X=$((CUR_X + 1))
    fi
done

# 6. 随机打乱列表
# 检查是否有内容 (空文件处理)
if [ -s "$TMP_LIST" ]; then
    shuf "$TMP_LIST" > "$TMP_SHUF"
else
    touch "$TMP_SHUF"
fi

# 7. 开始绘制
clear
tput civis # 隐藏光标

# 显式使用外部 printf 以获得更好的一致性
PRINTF_CMD=$(which printf)
[ -z "$PRINTF_CMD" ] && PRINTF_CMD="printf" # fallback

while read y x b1 b2; do
    # 移动光标
    tput cup "$y" "$x"
    
    # 输出字符
    # 使用 \xHH\xHH 格式构造二进制数据，然后 iconv 转回 UTF-8
    "$PRINTF_CMD" "\\x$b1\\x$b2" | iconv -f UCS-2BE -t UTF-8
    
    # 延迟
    if [ "$DELAY" != "0" ]; then
        sleep "$DELAY"
    fi
done < "$TMP_SHUF"

# 移动光标到最后
tput cup "$((TERM_LINES - 1))" 0
tput cnorm # 恢复光标
