#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen");
        exit(1);
    }

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // 创建带边框的窗口
    int h = LINES - 2, w = COLS - 2;
    WINDOW *frame = newwin(h, w, 1, 1);
    box(frame, 0, 0);
    mvwaddstr(frame, 0, (w - strlen(argv[1])) / 2, argv[1]);
    wrefresh(frame);

    // 内容窗口（最后一列留空）
    WINDOW *win = newwin(h - 2, w - 2, 2, 2);
    scrollok(win, TRUE);
    keypad(win, TRUE);

    char line[MAX_LINE];
    int ch;

    // 显示第一页（自动滚动）
    while (fgets(line, sizeof(line), fp) && !feof(fp)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        // 截断超长行（确保最后一列为空）
        if ((int)strlen(line) >= w - 2)
            line[w - 3] = '\0';

        waddstr(win, line);
        waddch(win, '\n');
    }

    // 空格：滚动一行；ESC：退出
    while ((ch = wgetch(win)) != 27) {
        if (ch == ' ') {
            if (fgets(line, sizeof(line), fp)) {
                size_t len = strlen(line);
                if (len > 0 && line[len - 1] == '\n')
                    line[len - 1] = '\0';
                if ((int)strlen(line) >= w - 2)
                    line[w - 3] = '\0';
                waddstr(win, line);
                waddch(win, '\n');
                wrefresh(win);
            }
        }
    }

    fclose(fp);
    delwin(win);
    delwin(frame);
    endwin();
    return 0;
}
