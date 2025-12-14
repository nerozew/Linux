#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void visit(char *map, int h, int w, int r, int c, char wall, char pass) {
    int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    // 随机打乱方向
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int t0 = dirs[i][0], t1 = dirs[i][1];
        dirs[i][0] = dirs[j][0]; dirs[i][1] = dirs[j][1];
        dirs[j][0] = t0; dirs[j][1] = t1;
    }

    map[r * w + c] = pass;

    for (int i = 0; i < 4; i++) {
        int dr = dirs[i][0];
        int dc = dirs[i][1];
        int nr = r + dr * 2;
        int nc = c + dc * 2;

        if (nr >= 0 && nr < h && nc >= 0 && nc < w && map[nr * w + nc] == wall) {
            map[(r + dr) * w + (c + dc)] = pass; // 打通墙壁
            visit(map, h, w, nr, nc, wall, pass); // 递归访问
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    char pass = '.';
    char wall = '#';
    int N = 6;

    int h = N * 2 + 1;
    int w = N * 2 + 1;
    char *map = (char *)malloc(h * w * sizeof(char));

    for (int i = 0; i < h * w; i++) map[i] = wall;

    visit(map, h, w, 1, 1, wall, pass);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            putchar(map[i * w + j]);
        }
        putchar('\n');
    }

    free(map);
    return 0;
}
