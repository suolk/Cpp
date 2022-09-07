#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;

int main() {
    FreeConsole();
    setcaption("È¸»ê¼ÇÅÆÆ÷");
    initgraph(1000, 750);
    setbkcolor(0x333333);
    setcolor(0xdddddd);
start:
    int arr[5][10];
    PIMAGE img = newimage();
    string url, back;
    mouse_msg msg;
    setfont(30, 0, "ËÎÌå");
    setbkmode(TRANSPARENT);
    setfillcolor(0x333333);
    cleardevice();
    for (int j = 0; j < 4; ++j) {
        if (j == 0)
            back = "m.png";
        else if (j == 1)
            back = "p.png";
        else if (j == 2)
            back = "s.png";
        else
            back = "z.png";
        for (int i = 1; i <= 9; ++i) {
            if (j == 3 && i > 7)
                break;
            arr[j][i] = 4;
            url = "pic/";
            url += i + 48;
            url += back;
            if (getimage(img, url.c_str()) == 0) {
                putimage(i * 110 - 90, j * 180 + 30, img);
                rectangle(i * 110 - 80, j * 180 + 160, i * 110 - 20, j * 180 + 190);
                xyprintf(i * 110 - 65, j * 180 + 161, "%d", arr[j][i]);
            }
        }
    }
    delimage(img);
    setlinewidth(2.0);
    setcolor(0xdddddd);
    xyprintf(857, 613, "reset");
    xyprintf(865, 683, "exit");
    rectangle(850, 610, 940, 650);
    rectangle(850, 680, 940, 720);
    for (; is_run(); delay_fps(1800)) {
        if (mousemsg()) {
            msg = {0};
            msg = getmouse();
            if (msg.is_down()) {
                int x = msg.x, y = msg.y, flag = 0;
                cout << x << " " << y << endl;
                if (x <= 940 && x >= 850 && y >= 610 && y <= 650) {
                    goto start;
                } else if (x <= 940 && x >= 850 && y >= 680 && y <= 720) {
                    goto end;
                } else {
                    for (int i = 0; i < 4; ++i) {
                        for (int j = 1; j <= 9; ++j) {
                            if (i == 3 && j > 7)
                                break;
                            if (x >= j * 110 - 80 && x <= j * 110 - 20 && y >= i * 180 + 160 && y <= i * 180 + 190) {
                                flag = 1;
                                bar(j * 110 - 70, i * 180 + 161, j * 110 - 30, i * 180 + 188);
                                --arr[i][j];
                                setcolor(0xdddddd);
                                xyprintf(j * 110 - 65, i * 180 + 161, "%d", arr[i][j]);
                                break;
                            }
                        }
                        if (flag)
                            break;
                    }
                }
            }
        }
    }
end:
    exit(0);
}