#ifndef MYHEADER_H
#define MYHEADER_H
#include <graphics.h>
#include <sys_edit.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <vector>
#include <random>
#include <algorithm>
#pragma comment(lib, "winmm.lib")

/** * @brief 播放音乐 */
void play_music(const char musicName[], bool repeat = false);
/** * @brief 停止播放音乐 */
void close_music();
/** * @brief 初始化输入框 */
void init_input(sys_edit& tmpInput, int posX = 0, int posY = 0, int sizew = 200, int bgColor = EGERGB(0, 102, 204), int color = EGERGB(234, 234, 234));
/** * @brief 设置覆盖整个窗口的背景图片 */
void set_bg_pic(PIMAGE& tempPic, const char s[], int picW, int picH);
/** * @brief 绘制两个全程通用按钮 */
void draw_button();
/** * @brief 绘制迷宫 */
void draw_maze();
/** * @brief 绘制数独界面 */
void draw_sudo();
/** * @brief 介绍故事背景以及操作方式 */
void story_start();
/** * @brief 角色在迷宫移动时擦除原角色位置 */
void erase(int x, int y);
/** * @brief 绘制通关路线 */
void path_draw();
/** * @brief 游戏结算 */
void end_game();

/** * @brief 登录操作 */
void login(sys_edit& nameInput);
/** * @brief 创建迷宫 */
void create_maze();
/** * @brief 搜索离开迷宫的最短路线 */
bool find_path(int x, int y);
/** * @brief 在必经之路上随机放置两个boss */
void boss_pos();
/** * @brief 检测鼠标点击 */
void mouse_click();
/** * @brief 迷宫time */
void game_maze();
/** * @brief 管理员作弊 */
bool cheat_judge();
/** * @brief 退出游戏？ */
bool exit_game();

/** * @brief 线程函数，用于管理迷宫中的鼠标信息，防止走迷宫时因为鼠标移动导致的键盘响应延迟 */
DWORD WINAPI MouseControl(LPVOID lpParameter);
/** * @brief 玩家移动 */
bool player_move(key_msg& keyMsg);

/** * @brief 用于创建数独矩阵 */
void sudo_game();

class Player {
   public:
    char name[20];
    int highest_record = 86400;  // 最快通关时间
    int x, y;                    // 玩家坐标
    bool stay = false;           // 检测是否达成特殊成就
    bool admin = false;
};
class merge_find {
   private:
    int* set;
    int* value;
    int size;
    int cnt;

   public:
    merge_find(int Size);
    ~merge_find();
    void init();
    int path_find(int index);
    bool merge(int set1, int set2);
    int get_cnt();
};

const int MazeBlank = 17;  // 迷宫一行的空格数
const int WallNum = 544;   // 迷宫最多能有多少面墙
const int MazeSize = 35;   // 一行墙和空格的总数
const int Blank = 0;
const int Wall = 1;
const int Path = 2;
const int Boss = 3;
extern int windowX;
extern int windowY;
extern int startTime;
extern int maze[36][36];
extern int sudo[15][15];
extern int digSudo[15][15];
extern Player player;
extern bool out;
extern bool sudoing;

#endif