#include <string.h>
#include "myheader.h"

using namespace std;
const int sX = GetSystemMetrics(SM_CXSCREEN);  // ��Ļ���
const int sY = GetSystemMetrics(SM_CYSCREEN);  // ��Ļ��
int windowX;
int windowY;
int startTime;
int sudo[15][15];     // ������������
int digSudo[15][15];  // �ڶ������������
int maze[36][36];     // �Թ�״̬�������
Player player;        // ���
bool out = false;
bool sudoing = false;
PIMAGE backgroundPic;
sys_edit input;  // �����������

int main() {
    FreeConsole();      // �رտ���̨����
    srand(time(NULL));  // ��ȡ���������

    // ����ѯ�ʵ���
    play_music("open src/music/Regrets.mp3 alias BGM");
    if (MessageBox(0, TEXT("Start Game Now?"), TEXT("hint"), 1) != 1)
        exit(0);
    close_music();  // ������ֹͣ���ŷ�������ʱ��������ֿ���

    // ��¼����
    windowX = 400, windowY = 300;
    initgraph(windowX, windowY, 0);                      // ��������
    setbkcolor(WHITE);                                   // ���ñ�����ɫ
    setinitmode(INIT_NOBORDER, 0, 0);                    // ���ó�������Ϊ�ޱ߿�
    setcaption("С�Ƽ�����");                            // ���ô��ڱ���
    movewindow((sX - windowX) / 2, (sY - windowY) / 2);  // ���ھ���
    set_bg_pic(backgroundPic, "src/img/login.png", windowX, windowY);
    init_input(input, 100, 100);
    setcolor(BLACK);                            // ����������ɫ
    setfont(20, 0, "����");                     // ���������С
    xyprintf(130, 50, "���ֳ��Ȳ�����%d", 10);  // ��ӡ����

    login(input);
    cleardevice();
    input.destroy();

    // click to start
    windowX = 800, windowY = 600;
    play_music("open src/music/CTS.mp3 alias BGM");
    initgraph(windowX, windowY);                                       // ���ô��ڴ�С
    setcaption("С�Ƽ����ӣ�");                                        // ���ñ���
    set_bg_pic(backgroundPic, "src/img/start.png", windowX, windowY);  // ���ñ���ͼƬ
    delay_ms(1);                                                       // ˢ��һ����Ļ
    mouse_click();                                                     // ��������
    close_music();
    cleardevice();  // ����

    // ��ʼ��Ϸ
    play_music("open src/music/bgm.mp3 alias BGM", true);
    setbkcolor(EGERGB(30, 30, 30));
    create_maze();
    find_path(1, 1);
    boss_pos();
    story_start();
    draw_maze();
    // HANDLE h1;
    // h1 = CreateThread(NULL, 0, MouseControl, NULL, 0, NULL);  // �����߳�
    // CloseHandle(h1);                                          // �ر��߳̾��
    player.x = player.y = 1;
    game_maze();
    int endTime = fclock();  // ��Ϸ����ʱ��
    close_music();
    if (out)  // �������Ƿ�ѡ���˳���Ϸ
        return 0;

    setbkmode(TRANSPARENT);  // �������屳��͸��
    set_bg_pic(backgroundPic, "src/img/sdBG.png", windowX, windowY);
    end_game();
    player.highest_record = endTime - startTime;  // ����ͨ��ʱ��
    char tmpStr[50];
    sprintf(tmpStr, "ȫ����ʱ��%02d:%02d:%02d", player.highest_record / 3600, (player.highest_record % 3600) / 60, player.highest_record % 60);
    if (player.stay)                 // ��ͬ���λ��Ҳ��ͬ
        xyprintf(210, 270, tmpStr);  // ��xx:xx:xx����ʽ��ӡͨ��ʱ��
    else
        xyprintf(210, 375, tmpStr);
    mouse_click();
    getch();
}

DWORD WINAPI MouseControl(LPVOID lpParameter) {
    mouse_msg mouseMsg;                                  // ���������
    while (mousemsg()) {                                 // ��⵽�����Ϣ����
        mouseMsg = getmouse();                           // ��ȡ�����Ϣ
        if (mouseMsg.is_down() && mouseMsg.is_left()) {  // Ϊ�����
            int x, y;                                    // ���ڼ�¼�������
            char num[2] = {0};
            mousepos(&x, &y);  // ��ȡ����
            // setcolor(WHITE);
            // xyprintf(100, 100, "x : %d  y: %d ", x, y);
            if (x >= 600 && x <= 720 && y >= 10 && y <= 60) {  // ѡ�����ף�����Աѡ��
                if (cheat_judge())
                    path_draw();                                   // ����·��ͼ
            } else if (x >= 10 && x <= 190 && y >= 10 && y <= 60)  // ѡ���˳�
                if (exit_game()) return 0L;
        }
    }
}