#include "myheader.h"
int arr[15][15], sudoCnt;
bool row[15][15], col[15][15], grid[15][15], flag;
bool r[15][15], c[15][15], g[15][15];  // 用于挖洞时记录挖过的状态
bool sudo_check(int num, int x, int y, int z) {
    if (row[x][num] || col[y][num] || grid[z][num]) return false;
    return true;
}

int sudo_cal(int x, int y) {
    if (x < 3) {
        if (y < 3)
            return 1;
        else if (y < 6)
            return 2;
        else
            return 3;
    } else if (x < 6) {
        if (y < 3)
            return 4;
        else if (y < 6)
            return 5;
        else
            return 6;
    } else {
        if (y < 3)
            return 7;
        else if (y < 6)
            return 8;
        else
            return 9;
    }
}

void init_sudo() {
    flag = false, sudoCnt = 0;
    for (int i = 0; i <= 9; ++i) {
        for (int j = 0; j <= 9; ++j) {
            arr[i][j] = digSudo[i][j] = sudo[i][j] = 0;
            row[i][j] = col[i][j] = grid[i][j] = false;
            r[i][j] = c[i][j] = g[i][j] = false;
        }
    }
}

bool create_sudo(int a) {
    if (flag) return false;
    int num = rand() % 9 + 1, x, y, z;
    x = a / 9, y = a % 9, z = sudo_cal(x, y);
    if (x > 8 || y > 8) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                sudo[i][j] = arr[i][j];
        flag = true;
        return false;
    }
    for (int i = 0; i < 9; ++i) {
        num = (num + i) % 9 + 1;
        if (sudo_check(num, x, y, z)) {
            row[x][num] = col[y][num] = grid[z][num] = true;
            arr[x][y] = num;
            if (create_sudo(a + 1)) return true;
            row[x][num] = col[y][num] = grid[z][num] = false;
            arr[x][y] = 0;
        }
    }
    return false;
}

bool sudo_full() {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (!arr[i][j])
                return false;
    return true;
}

void solve_sudo(int a) {
    int x, y, z;
    x = a / 9, y = a % 9, z = sudo_cal(x, y);
    if (a >= 81 || sudo_full()) {
        ++sudoCnt;
        return;
    }
    if (arr[x][y])
        solve_sudo(a + 1);
    else
        for (int k = 1; k <= 9; ++k) {
            if (sudo_check(k, x, y, z)) {
                row[x][k] = col[y][k] = grid[z][k] = true;
                arr[x][y] = k;
                solve_sudo(a + 1);
                row[x][k] = col[y][k] = grid[z][k] = false;
                arr[x][y] = 0;
            }
        }
}

void dig_sudo(int level) {
    int x, y, num, z;
    level = 120 / level;
    for (int i = 0; i < 81;) {
        x = i / 9, y = i % 9, z = sudo_cal(x, y);
        num = arr[x][y], z = sudo_cal(x, y);
        r[x][num] = c[y][num] = g[z][num] = false;
        digSudo[x][y] = sudoCnt = 0;
        for (int i = 0; i <= 9; ++i)
            for (int j = 0; j <= 9; ++j)
                arr[i][j] = digSudo[i][j], row[i][j] = r[i][j], col[i][j] = c[i][j], grid[i][j] = g[i][j];
        solve_sudo(0);
        if (sudoCnt > 1)
            digSudo[x][y] = num, r[x][num] = c[y][num] = g[z][num] = true;
        i += rand() % level + 1;
    }
}

void sudo_game() {
    init_sudo();
    create_sudo(0);
    for (int i = 0; i <= 9; ++i)
        for (int j = 0; j <= 9; ++j)
            arr[i][j] = digSudo[i][j] = sudo[i][j], r[i][j] = c[i][j] = g[i][j] = true;
    dig_sudo(33);
    draw_sudo();
}
