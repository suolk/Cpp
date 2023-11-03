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

/** * @brief �������� */
void play_music(const char musicName[], bool repeat = false);
/** * @brief ֹͣ�������� */
void close_music();
/** * @brief ��ʼ������� */
void init_input(sys_edit& tmpInput, int posX = 0, int posY = 0, int sizew = 200, int bgColor = EGERGB(0, 102, 204), int color = EGERGB(234, 234, 234));
/** * @brief ���ø����������ڵı���ͼƬ */
void set_bg_pic(PIMAGE& tempPic, const char s[], int picW, int picH);
/** * @brief ��������ȫ��ͨ�ð�ť */
void draw_button();
/** * @brief �����Թ� */
void draw_maze();
/** * @brief ������������ */
void draw_sudo();
/** * @brief ���ܹ��±����Լ�������ʽ */
void story_start();
/** * @brief ��ɫ���Թ��ƶ�ʱ����ԭ��ɫλ�� */
void erase(int x, int y);
/** * @brief ����ͨ��·�� */
void path_draw();
/** * @brief ��Ϸ���� */
void end_game();

/** * @brief ��¼���� */
void login(sys_edit& nameInput);
/** * @brief �����Թ� */
void create_maze();
/** * @brief �����뿪�Թ������·�� */
bool find_path(int x, int y);
/** * @brief �ڱؾ�֮·�������������boss */
void boss_pos();
/** * @brief �������� */
void mouse_click();
/** * @brief �Թ�time */
void game_maze();
/** * @brief ����Ա���� */
bool cheat_judge();
/** * @brief �˳���Ϸ�� */
bool exit_game();

/** * @brief �̺߳��������ڹ����Թ��е������Ϣ����ֹ���Թ�ʱ��Ϊ����ƶ����µļ�����Ӧ�ӳ� */
DWORD WINAPI MouseControl(LPVOID lpParameter);
/** * @brief ����ƶ� */
bool player_move(key_msg& keyMsg);

/** * @brief ���ڴ����������� */
void sudo_game();

class Player {
   public:
    char name[20];
    int highest_record = 86400;  // ���ͨ��ʱ��
    int x, y;                    // �������
    bool stay = false;           // ����Ƿ�������ɾ�
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

const int MazeBlank = 17;  // �Թ�һ�еĿո���
const int WallNum = 544;   // �Թ�������ж�����ǽ
const int MazeSize = 35;   // һ��ǽ�Ϳո������
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