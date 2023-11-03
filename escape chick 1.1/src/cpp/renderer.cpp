#include "myheader.h"

void play_music(const char musicName[], bool repeat) {
    if (mciSendString(musicName, NULL, 0, NULL)) {  // �ж��Ƿ�ɹ��������ļ�
        MessageBox(0, TEXT("OPEN BGM FAILED"), TEXT("hint"), 0);
        exit(0);
    }
    if (repeat)
        mciSendString(TEXT("play BGM repeat"), NULL, 0, NULL);  // ѭ����������
    else
        mciSendString(TEXT("play BGM"), NULL, 0, NULL);  // ��������
}
void close_music() { mciSendString(TEXT("close BGM"), NULL, 0, NULL); }

void init_input(sys_edit& tmpInput, int posX, int posY, int sizew, int bgColor, int color) {
    tmpInput.create(false);           // �������������
    tmpInput.move(posX, posY);        // ����λ��
    tmpInput.size(sizew, 30);         // ���ô�С
    tmpInput.setmaxlen(10);           // ����������볤��
    tmpInput.setbgcolor(bgColor);     // �����������ɫ
    tmpInput.setcolor(color);         // ���ü���������ɫ
    tmpInput.setfont(20, 0, "����");  // ���������С
    tmpInput.visible(true);           // ���������ɼ�
    tmpInput.setfocus();              // ���ù��
}

void set_bg_pic(PIMAGE& tempPic, const char s[], int picW, int picH) {
    PIMAGE oriPic = newimage();
    getimage(oriPic, s);             // ��ȡԭͼ
    tempPic = newimage(picW, picH);  // �����µ�ͼƬָ��
    ege_enable_aa(true, tempPic);    // �����
    putimage(tempPic, 0, 0, picW, picH,
             oriPic, 0, 0, getwidth(oriPic), getheight(oriPic));  // ����ͼƬ��Ӧ����
    putimage(0, 0, tempPic);                                      // ���ñ���
    delimage(tempPic), delimage(oriPic);                          // ����ͼƬ�ͷ��ڴ�
    tempPic = NULL;
}

void draw_button() {
    rectangle(600, 10, 720, 60);  // ��һ�����ĵĳ�����
    rectangle(10, 10, 190, 60);
    setfont(40, 0, "����");        // ���������С
    setbkmode(TRANSPARENT);        // �����ֱ�������Ϊ͸��
    xyprintf(20, 15, "�˳���Ϸ");  // ��������
    xyprintf(610, 15, "CHEAT");
    setfont(20, 0, "����");  // �ָ��ɳ��õ������С
}

void draw_maze() {
    setbkcolor(EGERGB(30, 30, 30));  // ���ñ�����ɫ
    draw_button();                   // ���û��ư�ť����
    setlinewidth(2.0);               // �����߿�Ϊ2.0
    setcolor(WHITE);                 // ������Ϊ��ɫ

    for (int i = 0; i < MazeSize; i += 2)
        for (int j = 1; j < MazeSize; j += 2)
            if (maze[i][j] == Wall)
                line(160 + (j - 1) * 14, 85 + i * 14, 160 + 14 * (j + 1), 85 + 14 * i);  // ������
    for (int i = 1; i < MazeSize; i += 2) {
        for (int j = 0; j < MazeSize; j += 2) {
            if ((i == 1 && j == 0) || (i == 33 && j == 34))  // �޶�����������ϽǺ����½����������λ�ò�����
                continue;
            else if (maze[i][j] == Wall)
                line(160 + 14 * j, 85 + 14 * (i - 1), 160 + 14 * j, 85 + 14 * (i + 1));  // ������
        }
    }
}

void story_start() {
    PIMAGE p = newimage(203, 197);                                // ����ͼƬ���͵�ָ��
    getimage(p, "src/img/player.png");                            // ��ȡ�������ͼƬ
    setfont(20, 0, "����");                                       // ��������ʹ�С
    setbkmode(TRANSPARENT);                                       // �����ֱ�������Ϊ͸��
    setcolor(WHITE);                                              // ����������ɫ
    xyprintf(100, 205, "�����������о���,��w��a��s��d���ƶ�");  // �ڱ����ϴ�ӡ����
    xyprintf(100, 230, "�ƶ���ʱ�����û��Ӧ���Ͱ�shift�л���Ӣ�����뷨");
    mouse_click();  // ��������
    cleardevice();  // ����
    xyprintf(100, 205, "��һ�죬�����ѹ���ʱ���������վ����뿴���Լ�˧����������");
    mouse_click();        // ��������
    putimage(175, 0, p);  // �����������ͼƬ
    delimage(p);
    p = NULL;
    mouse_click();  // ��������
    xyprintf(100, 230, "?!������һֻС��������Q��С�Ƽ�!");
    mouse_click();  // ��������
    xyprintf(100, 255, "ԭ����������һ��Q�����磬�������ж�������Q���");
    mouse_click();  // ��������
    xyprintf(100, 280, "����Ҫ��ȥ����ͨ��һ���Թ�����");
    mouse_click();  // ��������
    cleardevice();
}

void erase(int x, int y) {
    setfillcolor(BLACK);        // ������ɫ��ɫ
    bar(x, y, x + 14, y + 14);  // ��һ��ʵ�ķ���
}

void path_draw() {
    setfillcolor(EGERGB(112, 48, 60));  // ·�ߵ���ɫ��ǳ��ɫ��ʵ���ϸ���������ɫ��
    for (int i = 1; i < MazeSize; i++) {
        for (int j = 1; j < MazeSize; j++) {
            if (i == player.x && j == player.y)
                continue;
            else if (maze[i][j] == Path)  // ��⵽·���򻭳�һ��С���鸲�ǵ�ǰ��
                bar(j * 14 + 150, i * 14 + 80, (j + 1) * 14 + 150, (i + 1) * 14 + 80);
            else if (maze[i][j] == Boss) {  // ��⵽boss�������ɫ��һ��С���鸲�ǵ�ǰ��
                setfillcolor(RED);          // boss�Ǻ�ɫС����
                bar(j * 14 + 150, i * 14 + 80, (j + 1) * 14 + 150, (i + 1) * 14 + 80);
                setfillcolor(EGERGB(112, 48, 60));  // �ָ���ɫΪ·����ɫ
            }
        }
    }
}

void end_game() {
    if (player.stay) {  // ��Ҵ��������
        play_music("open src/music/ding.mp3 alias BGM");
        xyprintf(210, 200, "��������֣�");
        xyprintf(210, 235, "������ʵ��̫�ɰ��ˡ�");
        close_music();
    } else {  // �����ͨ���
        xyprintf(210, 200, "��ص�����ʵ");
        xyprintf(210, 235, "��Ҳû������ʲô�ر����");
        xyprintf(210, 270, "��ʱ�����");
        xyprintf(210, 305, "�����ʱ���������ǲ��ǻ������");
        mouse_click();
        xyprintf(210, 340, "��ɽ�֣��ص��ճ�");
    }
    mouse_click();
}

void draw_sudo() {
    cleardevice();  // ����
    PIMAGE pic;
    set_bg_pic(pic, "src/img/sdBG.png", 800, 600);  // ���ñ���
    sys_edit sudoInput[9][9];                       // ����������������
    mouse_msg mouseMsg;                             // ����������
    bool Wrong_ans = false;                         // �������ڱ�Ǵ���𰸵Ŀ��أ���ʼ״̬����û�д����
    pic = newimage(364, 364);
    getimage(pic, "src/img/sdGrid.png");  // �����������ͼƬ
    ege_enable_aa(true, pic);
    setcolor(BLACK);          // ����������ɫ��ɫ
    putimage(202, 106, pic);  // ������������ͼƬ
    delimage(pic);            // ����ͼƬ�ͷ��ڴ�
    pic = NULL;

    for (int i = 0; i < 9; i++) {  // �����������������
        for (int j = 0; j < 9; j++) {
            if (digSudo[i][j])                                           // ˵�����λ��Ӧ���ɳ�������
                xyprintf(218 + 40 * j, 120 + 40 * i, "%d", sudo[i][j]);  // ��ӡ����
            else                                                         // ˵����λ��Ӧ�����������
                init_input(sudoInput[i][j], 210 + 40 * j, 112 + 40 * i, 30, WHITE, BLACK);
        }
    }

    // ���Ƽ�����������İ�ť
    rectangle(10, 100, 190, 150);  // ��һ�����ĵľ���
    rectangle(585, 272, 685, 322);
    setbkmode(TRANSPARENT);         // �������ֱ���͸��
    setfont(40, 0, "����");         // ������������
    setcolor(BLACK);                // ������ɫ
    xyprintf(20, 105, "�������");  // ��ӡ����
    xyprintf(600, 277, "�ύ");
    setfont(20, 0, "����");  // �ָ�Ϊ��������
    draw_button();           // ��������ͨ�ð�ť
    flushmouse();            // ��������Ϣ

    for (; is_run; delay_fps(180)) {                                             // �����
        while (mousemsg()) {                                                     // ����⵽�����Ϣ
            mouseMsg = getmouse();                                               // ��ȡ�����Ϣ
            if (mouseMsg.is_down() && mouseMsg.is_left()) {                      // ����⵽����������
                int msgX, msgY;                                                  // ���ڴ��������������
                char num[3] = {0};                                               // ���ڴ���������ַ���������������������ַ�����ʽ��ȡ����˽���һ��������ת������
                mousepos(&msgX, &msgY);                                          // ��ȡ���������
                if (msgX >= 585 && msgX <= 685 && msgY >= 272 && msgY <= 322) {  // ���λ��Ϊ�ύ��ť���ڷ�Χ
                    for (int i = 0; i < 9; i++) {
                        for (int j = 0; j < 9; j++) {
                            if (!digSudo[i][j]) {                 // ��λ��ӦΪ�������
                                sudoInput[i][j].gettext(2, num);  // ��ȡ�������
                                if (num[0] - 48 != sudo[i][j]) {  // ���벻��ȷ
                                    Wrong_ans = true;             // �޸Ŀ���״̬Ϊ�ɴ����
                                    break;                        // ����ѭ��
                                }
                            }
                        }
                        if (Wrong_ans)  // �д�������ټ����һ������ѭ��
                            break;
                    }
                    if (Wrong_ans) {
                        MessageBox(0, TEXT("�д���𰸣�"), TEXT("hint"), 0);  // ������ʾ
                        Wrong_ans = false;                                     // �ָ�����״̬�Խ�����һ�μ��
                        continue;                                              // ���������������ݵȴ���һ���ύ
                    } else {
                        MessageBox(0, TEXT("��ϲ������ˣ�"), TEXT("hint"), 0);  // ����ȷ
                        return;                                                  // �뿪����
                    }
                } else if (msgX >= 600 && msgX <= 720 && msgY >= 10 && msgY <= 60) {  // ѡ�����ף�����Աѡ��
                    if (cheat_judge()) {
                        for (int i = 0; i < 9; i++) {  // �Զ������
                            for (int j = 0; j < 9; j++) {
                                if (!digSudo[i][j]) {
                                    char t[1];
                                    t[0] = sudo[i][j] + 48;
                                    sudoInput[i][j].settext(t);  // ֱ�Ӷ���char t�������ǿ��ת������
                                }
                            }
                        }
                    }
                } else if (msgX >= 10 && msgX <= 190 && msgY >= 10 && msgY <= 60) {  // ѡ���˳�
                    if (exit_game()) return;
                } else if (msgX >= 10 && msgX <= 190 && msgY >= 100 && msgY <= 150 && MessageBox(0, TEXT("ȷ��������룿"), TEXT("hint"), 1) != 1) {  // ѡ���������
                    for (int i = 0; i < 9; i++) {
                        for (int j = 0; j < 9; j++) {
                            if (!digSudo[i][j])
                                sudoInput[i][j].settext("");
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            sudoInput[i][j].destroy();
    cleardevice();
}