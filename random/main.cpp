#include <graphics.h>
#include <sys_edit.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
using namespace std;

map<int, bool> mp, ban;
int Left, Right, tot;
bool flag = false;

int getRandNum() {
    int n, a, b;
    do {
        n, a = rand() + 1, b = rand() + 1;
        n = b / a + rand();
        n *= a + b;
        n /= rand() > a ? 10 : 1;
        n = n % (Right - Left + 1) + Left;
    } while (mp[n] || ban[n]);
    mp[n] = true;
    return n;
}

void startRandNum() {
    mp.clear();
    FILE* fp = freopen("rand.txt", "w", stdout);
    int tmp;
    setfont(20, 0, "宋体");
    for (int i = 0; i < tot; ++i) {
        tmp = getRandNum();
        cout << tmp << " ";
        xyprintf((i % 8) * 100 + 15, i / 8 * 30 + 80, "%d", tmp);
    }
    fclose(fp);
}

void drawBan() {
    setfont(15, 0, "宋体");
    xyprintf(10, 550, "请输入需排除的数据范围:            -");
    xyprintf(432, 550, "添加黑名单");
    setlinewidth(3);
    rectangle(420, 540, 520, 575);
}

void start() {
    cleardevice();
    setcolor(BLACK);
    setfont(25, 0, "黑体");
    xyprintf(710, 15, "EXIT");
    setfont(15, 0, "宋体");
    xyprintf(10, 10, "请输入随机数生成范围:             -");
    xyprintf(10, 40, "请输入所需生成的个数:");
    xyprintf(410, 13, "确认");
    setlinewidth(3);
    rectangle(400, 5, 450, 35);
    rectangle(700, 5, 770, 50);
    if (flag)
        drawBan();
}

int stringToNum(char s[]) {
    int ret = 0;
    for (int i = 0; i < strlen(s); ++i) {
        if (isdigit(s[i])) {
            ret = ret * 10 + s[i] - 48;
        } else {
            ret = -1;
            break;
        }
    }
    if (strlen(s) == 0)
        ret = -1;
    return ret;
}

void wrongInput() {
    MessageBox(getHWnd(), TEXT("输入有误"), TEXT("hint"), 0);
}

void Ban() {
    for (int i = Left; i <= Right; ++i)
        ban[i] = true;
}

void buttonPush(const int& x1, const int& y1, const int& x2, const int& y2) {
    setcolor(0xB2B2B2);
    rectangle(x1, y1, x2, y2);
    Sleep(100);
    setcolor(BLACK);
    rectangle(x1, y1, x2, y2);
}

void initEdit(sys_edit& tmpEdit, const int& tx, const int& ty) {
    tmpEdit.create();
    tmpEdit.move(tx, ty);
    tmpEdit.size(80, 25);
    tmpEdit.setfont(15, 0, "");
    tmpEdit.setmaxlen(7);
    tmpEdit.setcolor(BLACK);
    tmpEdit.visible(true);
}

int main() {
    FreeConsole();
    srand(time(0));
    initgraph(800, 600, 0);
    setbkcolor(0x66FFCC);
    setcaption("随机数生成器");
    movewindow(GetSystemMetrics(SM_CXFULLSCREEN) / 2 - 400, GetSystemMetrics(SM_CYFULLSCREEN) / 2 - 300, 0);
    ege_enable_aa(true);
    start();
    key_msg key;
    mouse_msg mouse;
    sys_edit low, high, times;
    sys_edit banl, banr;
    char s1[15], s2[15], s3[15];
    initEdit(low, 180, 7), initEdit(high, 290, 7), initEdit(times, 180, 37), initEdit(banl, 190, 545), initEdit(banr, 300, 545);
    banl.visible(false), banr.visible(false);

    for (; is_run; delay_fps(1000)) {
        if (kbmsg()) {
            key = getkey();
            if (key.msg == key_msg_down && (key.key == 'B' || key.key == 'b')) {
                flushkey();
                drawBan();
                banl.visible(true), banr.visible(true);
                flag = true;
            }
        } else if (mousemsg()) {
            mouse = getmouse();
            if (mouse.is_left() && mouse.is_down()) {
                int x = mouse.x, y = mouse.y;
                if (x >= 400 && x <= 450 && y >= 5 && y <= 35) {
                    buttonPush(400, 5, 450, 35);
                    flushmouse();
                    start();
                    low.gettext(15, s1), high.gettext(15, s2), times.gettext(15, s3);
                    Left = stringToNum(s1);
                    Right = stringToNum(s2);
                    tot = stringToNum(s3);
                    if (Left == -1 || Right == -1 || tot == -1 || Left > Right) {
                        wrongInput();
                        low.settext(""), high.settext(""), times.settext("");
                        continue;
                    }
                    startRandNum();
                } else if (x >= 700 && x <= 770 && y >= 5 && y <= 50) {  //点击退出
                    buttonPush(700, 5, 770, 50);
                    flushmouse();
                    goto end;
                } else if (x >= 420 && x <= 520 && y >= 540 && y <= 575 && flag) {  // 添加
                    buttonPush(420, 540, 520, 575);
                    flushmouse();
                    banl.gettext(15, s1), banr.gettext(15, s2);
                    Left = stringToNum(s1), Right = stringToNum(s2);
                    banl.settext(""), banr.settext("");
                    if ((Left == -1 && Right == -1) || (Left > Right && Right != -1)) {
                        wrongInput();
                        continue;
                    } else if (Left == -1)
                        Left = Right;
                    else if (Right == -1)
                        Right = Left;
                    Ban();
                }
            }
        }
    }
    getch();

end:
    low.destroy(), high.destroy(), times.destroy();
    exit(0);
}
