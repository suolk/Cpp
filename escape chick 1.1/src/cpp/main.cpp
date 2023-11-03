#include <string.h>
#include "myheader.h"

using namespace std;
const int sX = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽度
const int sY = GetSystemMetrics(SM_CYSCREEN);  // 屏幕高
int windowX;
int windowY;
int startTime;
int sudo[15][15];     // 完整数独数组
int digSudo[15][15];  // 挖洞后的数独数组
int maze[36][36];     // 迷宫状态储存矩阵
Player player;        // 玩家
bool out = false;
bool sudoing = false;
PIMAGE backgroundPic;
sys_edit input;  // 定义输入框类

int main() {
    FreeConsole();      // 关闭控制台窗口
    srand(time(NULL));  // 获取随机数种子

    // 开局询问弹窗
    play_music("open src/music/Regrets.mp3 alias BGM");
    if (MessageBox(0, TEXT("Start Game Now?"), TEXT("hint"), 1) != 1)
        exit(0);
    close_music();  // 在这里停止播放否则输入时会出现音乐卡顿

    // 登录界面
    windowX = 400, windowY = 300;
    initgraph(windowX, windowY, 0);                      // 创建窗口
    setbkcolor(WHITE);                                   // 设置背景颜色
    setinitmode(INIT_NOBORDER, 0, 0);                    // 设置出口类型为无边框
    setcaption("小黄鸡出逃");                            // 设置窗口标题
    movewindow((sX - windowX) / 2, (sY - windowY) / 2);  // 窗口居中
    set_bg_pic(backgroundPic, "src/img/login.png", windowX, windowY);
    init_input(input, 100, 100);
    setcolor(BLACK);                            // 设置字体颜色
    setfont(20, 0, "宋体");                     // 设置字体大小
    xyprintf(130, 50, "名字长度不超过%d", 10);  // 打印文字

    login(input);
    cleardevice();
    input.destroy();

    // click to start
    windowX = 800, windowY = 600;
    play_music("open src/music/CTS.mp3 alias BGM");
    initgraph(windowX, windowY);                                       // 重置窗口大小
    setcaption("小黄鸡出逃！");                                        // 设置标题
    set_bg_pic(backgroundPic, "src/img/start.png", windowX, windowY);  // 设置背景图片
    delay_ms(1);                                                       // 刷新一下屏幕
    mouse_click();                                                     // 检测鼠标点击
    close_music();
    cleardevice();  // 清屏

    // 开始游戏
    play_music("open src/music/bgm.mp3 alias BGM", true);
    setbkcolor(EGERGB(30, 30, 30));
    create_maze();
    find_path(1, 1);
    boss_pos();
    story_start();
    draw_maze();
    // HANDLE h1;
    // h1 = CreateThread(NULL, 0, MouseControl, NULL, 0, NULL);  // 创建线程
    // CloseHandle(h1);                                          // 关闭线程句柄
    player.x = player.y = 1;
    game_maze();
    int endTime = fclock();  // 游戏结束时间
    close_music();
    if (out)  // 检测玩家是否选择退出游戏
        return 0;

    setbkmode(TRANSPARENT);  // 设置字体背景透明
    set_bg_pic(backgroundPic, "src/img/sdBG.png", windowX, windowY);
    end_game();
    player.highest_record = endTime - startTime;  // 计算通关时间
    char tmpStr[50];
    sprintf(tmpStr, "全程用时：%02d:%02d:%02d", player.highest_record / 3600, (player.highest_record % 3600) / 60, player.highest_record % 60);
    if (player.stay)                 // 不同结局位置也不同
        xyprintf(210, 270, tmpStr);  // 按xx:xx:xx的形式打印通关时间
    else
        xyprintf(210, 375, tmpStr);
    mouse_click();
    getch();
}

DWORD WINAPI MouseControl(LPVOID lpParameter) {
    mouse_msg mouseMsg;                                  // 定义鼠标类
    while (mousemsg()) {                                 // 检测到鼠标信息产生
        mouseMsg = getmouse();                           // 获取鼠标信息
        if (mouseMsg.is_down() && mouseMsg.is_left()) {  // 为鼠标点击
            int x, y;                                    // 用于记录鼠标坐标
            char num[2] = {0};
            mousepos(&x, &y);  // 获取坐标
            // setcolor(WHITE);
            // xyprintf(100, 100, "x : %d  y: %d ", x, y);
            if (x >= 600 && x <= 720 && y >= 10 && y <= 60) {  // 选择作弊，管理员选项
                if (cheat_judge())
                    path_draw();                                   // 画出路线图
            } else if (x >= 10 && x <= 190 && y >= 10 && y <= 60)  // 选择退出
                if (exit_game()) return 0L;
        }
    }
}