#!/bin/sh
# POSIX / dash-compatible ASCII-art random painter with colored frame
# Supports UTF-8 (Russian, etc.), colored output, and centered display

set -eu

DELAY="${1:-0}"
export LC_ALL=C
TMP_INPUT=""
TMP_COORDS=""

# --- 颜色定义 ---
BG_COLOR=4      # 蓝色背景 (ANSI 4)
TEXT_COLOR=7    # 白色文字 (ANSI 7)
FRAME_COLOR=6   # 青色边框 (ANSI 6)

cleanup() {
    [ -n "$TMP_INPUT" ] && rm -f "$TMP_INPUT" 2>/dev/null
    [ -n "$TMP_COORDS" ] && rm -f "$TMP_COORDS" 2>/dev/null
    # 恢复默认颜色并定位光标到底部
    tput sgr0 2>/dev/null || true
    if terminal_info="$(stty size </dev/tty 2>/dev/null)"; then
        set -- $terminal_info
        rows="$1"
        if [ "$rows" -gt 0 ] 2>/dev/null; then
            tput cup "$((rows - 1))" 0 2>/dev/null || true
        fi
    fi
}

trap cleanup EXIT INT TERM HUP

# --- 参数校验 ---
case "$DELAY" in
    ''|*[!0-9.]*|.*.*.*)
        if [ "$DELAY" != "0" ]; then
            echo "Error: Invalid delay '$DELAY'" >&2
            exit 1
        fi
        ;;
esac

# --- 获取终端尺寸 ---
if ! terminal_info="$(stty size </dev/tty 2>/dev/null)"; then
    echo "Error: Cannot determine terminal size" >&2
    exit 1
fi
set -- $terminal_info
TERM_ROWS="$1"
TERM_COLS="$2"

# --- 读取输入并保存到临时文件（UTF-8）---
TMP_INPUT="$(mktemp)"
cat > "$TMP_INPUT"

# --- 计算内容尺寸（行数和最大宽度）---
CONTENT_LINES=$(wc -l < "$TMP_INPUT")
MAX_WIDTH=0
while IFS= read -r line; do
    width=$(printf '%s' "$line" | wc -m)  # 字符数（UTF-8 安全）
    [ "$width" -gt "$MAX_WIDTH" ] && MAX_WIDTH="$width"
done < "$TMP_INPUT"

# --- 计算居中位置 ---
FRAME_HEIGHT=$((CONTENT_LINES + 2))
FRAME_WIDTH=$((MAX_WIDTH + 2))

if [ "$FRAME_HEIGHT" -gt "$TERM_ROWS" ] || [ "$FRAME_WIDTH" -gt "$TERM_COLS" ]; then
    echo "Warning: Content too large for terminal" >&2
    START_Y=0
    START_X=0
else
    START_Y=$(( (TERM_ROWS - FRAME_HEIGHT) / 2 ))
    START_X=$(( (TERM_COLS - FRAME_WIDTH) / 2 ))
fi

# --- 清屏并绘制边框 + 背景 ---
tput clear 2>/dev/null || true

# 绘制顶部边框
tput cup "$START_Y" "$START_X" 2>/dev/null || true
tput setaf "$FRAME_COLOR" 2>/dev/null || true
printf '+'
i=0
while [ "$i" -lt "$MAX_WIDTH" ]; do
    printf '-'
    i=$((i + 1))
done
printf '+\n'

# 绘制中间行（带背景）
y=1
while [ "$y" -le "$CONTENT_LINES" ]; do
    tput cup "$((START_Y + y))" "$START_X" 2>/dev/null || true
    tput setaf "$FRAME_COLOR" 2>/dev/null || true
    printf '|'
    tput setab "$BG_COLOR" 2>/dev/null || true
    tput setaf "$TEXT_COLOR" 2>/dev/null || true
    i=0
    while [ "$i" -lt "$MAX_WIDTH" ]; do
        printf ' '
        i=$((i + 1))
    done
    tput setab default 2>/dev/null || true
    tput setaf "$FRAME_COLOR" 2>/dev/null || true
    printf '|'
    printf '\n'
    y=$((y + 1))
done

# 绘制底部边框
tput cup "$((START_Y + FRAME_HEIGHT - 1))" "$START_X" 2>/dev/null || true
tput setaf "$FRAME_COLOR" 2>/dev/null || true
printf '+'
i=0
while [ "$i" -lt "$MAX_WIDTH" ]; do
    printf '-'
    i=$((i + 1))
done
printf '+\n'

tput sgr0 2>/dev/null || true  # 重置颜色

# --- 准备坐标文件（UTF-8 → UCS-2BE → 坐标）---
TMP_COORDS="$(mktemp)"

{
    y=0
    x=0
    iconv -f UTF-8 -t UCS-2BE < "$TMP_INPUT" | od -An -t u1 -v -w2 | while IFS= read -r line; do
        set -- $line
        [ $# -ne 2 ] && continue
        b1="$1"; b2="$2"
        # 检测换行符 (0x00 0x0A)
        if [ "$b1" -eq 0 ] && [ "$b2" -eq 10 ]; then
            y=$((y + 1))
            x=0
            continue
        fi
        # 输出：屏幕Y 屏幕X b1 b2
        printf '%d %d %d %d\n' "$((START_Y + 1 + y))" "$((START_X + 1 + x))" "$b1" "$b2"
        x=$((x + 1))
    done
} > "$TMP_COORDS"

if [ ! -s "$TMP_COORDS" ]; then
    exit 0
fi

# --- 随机输出字符（带颜色）---
shuf "$TMP_COORDS" | while IFS=' ' read -r Y X B1 B2; do
    tput cup "$Y" "$X" 2>/dev/null || continue
    tput setab "$BG_COLOR" 2>/dev/null || true
    tput setaf "$TEXT_COLOR" 2>/dev/null || true
    printf "\\$(printf '%03o' "$B1")\\$(printf '%03o' "$B2")" | iconv -f UCS-2BE -t UTF-8
    tput sgr0 2>/dev/null || true
    if [ "$DELAY" != "0" ]; then
        sleep "$DELAY"
    fi
done

printf '\n'
