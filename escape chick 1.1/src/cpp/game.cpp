#include "myheader.h"

int dire[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
int pwCnt = 0;

void login(sys_edit &nameInput) {
    key_msg keyMsg;
    int nameLen;
    bool nameFlag = false, pwFlag = false;
    char pw[20];
    for (; is_run(); delay_fps(60)) {  // ����Ա��¼��������ͨ��ҽ�����Ϸ
        while (kbmsg()) {
            keyMsg = getkey();
            if (keyMsg.msg == key_msg_down && keyMsg.key == key_enter) {  // ��׽��enter����
                nameInput.gettext(10, player.name);                       // ��ȡ���������
                nameLen = strlen(player.name);                            // ��ȡ��������
                nameFlag = true;                                          // ����Ƿ��������
                if (strcmp(player.name, "Suolk") == 0) {
                    if (MessageBox(0, TEXT("ȷ��Ҫ�Թ���Ա��ݵ�¼��?"), TEXT("hint"), 1) != 1) {  // ��֤����Ա��¼
                        nameInput.settext("����������");                                           // �����ı�����ʾ����
                        nameInput.setfocus();                                                      // ���ù��
                        flushkey();                                                                // ��ռ�����Ϣ
                        for (; is_run(); delay_fps(60)) {                                          // ѭ��Ƶ��Ϊÿ��60��
                            char tmpStr[40] = {0};                                                 // ��ת���ַ�������
                            keyMsg = getkey();                                                     // ��ȡ������Ϣ
                            if (keyMsg.msg == key_msg_down && keyMsg.key == key_enter) {           // ����enter
                                pwCnt++;                                                           // �����������+1
                                nameInput.gettext(10, pw);                                         // ��ȡ���������
                                if (strcmp(pw, "tgb951") == 0) {                                   // ��������Ƿ���ȷ
                                    MessageBox(0, TEXT("������ȷ��"), TEXT("hint"), 0);
                                    pwFlag = true;        // ������ȷ
                                    player.admin = true;  // ����Ա��½״̬�޸�Ϊ�ѵ�¼
                                } else if (pwCnt < 3) {   // ������󵫻����������
                                    sprintf(tmpStr, "��������㻹��%d�λ���", 3 - pwCnt);
                                    nameInput.settext("");
                                    nameInput.setfocus();
                                    MessageBox(0, tmpStr, TEXT("hint"), 0);
                                } else {                         // ��������Ҵ�����
                                    pwFlag = false;              // ���벻��ȷ
                                    nameInput.settext("Suolk");  // �������������������
                                    break;
                                }
                                flushkey();
                                memset(pw, 0, sizeof(pw));  // ������ݷ�ֹ���������
                            }
                            if (pwFlag || pwCnt == 3) break;  // ������ȷ�������������������뿪���������ѭ��W
                        }
                    }
                }
                if (pwFlag) break;   // ����Ա�ѵ�¼
                if (nameLen == 0) {  // ��ͨ��ҽ�����Ϸ���������û��������
                    MessageBox(0, TEXT("���������ƣ�"), TEXT("hint"), 0);
                    nameInput.setfocus();
                    nameFlag = false;
                } else if (MessageBox(0, TEXT("ȷ����������ƿ�ʼ��Ϸ��"), TEXT("hint"), 1) != 1)
                    nameFlag = false;
            }
        }
        if (nameFlag) break;  // ˵���Ѽ�������
    }
}

void create_maze() {
    for (int i = 0; i <= MazeSize; i++)  // �Ƚ��Թ�ȫ����ʼ��Ϊǽ
        for (int j = 0; j <= MazeSize; j++)
            maze[i][j] = Wall;
    for (int i = 1; i < MazeSize; i += 2)  // �ٽ�����Ŀո�����
        for (int j = 1; j < MazeSize; j += 2)
            maze[i][j] = Blank;

    std::vector<int> randomSequence;
    for (int i = 0; i < WallNum; i++)  // ����һ��vector�������������������
        randomSequence.push_back(i);
    shuffle(randomSequence.begin(), randomSequence.end(), std::default_random_engine(time(NULL)));  // ����һ������������������ǽ�����Թ�

    merge_find bcj(MazeBlank * MazeBlank);  // ����һ�����鼯��
    for (int i = 0; i < WallNum; i++) {
        if (bcj.get_cnt() <= 1)  // ������1������˵����·��ȫ����ͨ�����Թ������
            break;

        int num, Horizon = 0, Vertical = 0, randWallNum, xWall, yWall;
        if (randomSequence[i] < WallNum / 2) {  // �����б��Ϊ0~Wall_Num/2-1��Ϊ����ǽ
            randWallNum = randomSequence[i];
            Horizon = 1;
            num = MazeBlank - 1;
        } else {  // �����б��ΪWall_Num/2~Wall_Num��Ϊ����ǽ
            randWallNum = randomSequence[i] - WallNum / 2;
            Vertical = 1;
            num = MazeBlank;
        }
        xWall = randWallNum % num;
        yWall = randWallNum / num;

        int x1 = xWall, y1 = yWall, x2 = xWall + Horizon, y2 = yWall + Vertical;
        if (bcj.merge(y1 * MazeBlank + x1, y2 * MazeBlank + x2)) {  // �ϲ��˼���˵����Ҫ��ǽ
            maze[yWall + y2 + 1][xWall + x2 + 1] = Blank;
        }
    }
}

bool find_path(int x, int y) {
    if (maze[x][y] == Blank) {                       // ��⵽�ո�
        maze[x][y] = Path;                           // �ȱ��Ϊ·��
        if (x == MazeSize - 2 && y == MazeSize - 2)  // ������ڣ�˵��·����ȷ��������
            return true;
        for (int i = 0; i < 4; i++)  // dire����ֱ�������ĸ�����
            if (find_path(x + dire[i][0], y + dire[i][1]))
                return true;  // ·����ȷ��������
        maze[x][y] = Blank;   // ���ĸ�����û����ȷ·����˵����һ���ǳ��Թ��ıؾ�֮·�����±��Ϊ�ո�
    }
    return false;  // ���е���һ��˵��������·�߲���ȷ�����ؼ�
}

void boss_pos() {
    int pathLen = 0;
    // �����뿪�Թ������·�߳���
    for (int i = 0; i < MazeSize; i++)
        for (int j = 0; j < MazeSize; j++)
            if (maze[i][j] == Path)
                pathLen++;
    // ͨ���������·�߳��ȼ���boss��·���ϵĵڼ���
    int pos1 = rand() % pathLen + 2, pos2;
    do {
        pos2 = rand() % pathLen + 2;
    } while (pos1 == pos2);     // ȷ��boss��λ�ò��غ�
    int cnt = 1, x = 1, y = 1;  // pos1��pos2����Boss��·�ߵĵڼ���
    while (cnt <= pos1 || cnt <= pos2) {
        for (int i = 0; i < 4; ++i) {
            if (maze[x + dire[i][0]][y + dire[i][1]] == Path) {
                x = x + dire[i][0], y = y + dire[i][1], ++cnt;
                break;
            }
        }
        if (cnt == pos1) maze[x][y] = Boss;  // ����boss1
        if (cnt == pos2) maze[x][y] = Boss;  // ����boss2
        if (cnt == pathLen + 2) {
            maze[x][y] = Boss;  // �������һ��boss
            return;
        }
    }
}

void mouse_click() {
    mouse_msg tmpMsg;                              // ������������
    for (; is_run(); delay_fps(180)) {             // ѭ��Ƶ��Ϊÿ��180��
        tmpMsg = getmouse();                       // ��ȡ�����Ϣ
        if (tmpMsg.is_down() && tmpMsg.is_left())  // ��⵽���������
            break;
    }
}

void game_maze() {
    PIMAGE tmpPic, bigPlayer, miniPlayer, boss;  // ����ͼ��ָ�룬�ֱ�Ϊ�������С����boss����
    bigPlayer = newimage(), miniPlayer = newimage(14, 14);
    getimage(bigPlayer, "src/img/player.png");                      // ��ȡ������󣨴�
    putimage(miniPlayer, 0, 0, 14, 14, bigPlayer, 0, 0, 203, 197);  // ��С�������

    PIMAGE talk = newimage(696, 218);    // ��ת��ͼƬ���Ի��򣬱���
    key_msg keyMsg;                      // ���������
    int cnt = 0;                         // ���ڼ�¼�ڼ���boss
    putimage(164, 94, miniPlayer);       // ���Թ���ڷ���С���
    getimage(talk, "src/img/talk.png");  // ��ȡ�Ի���ͼƬ�ļ�
    startTime = fclock();                // ��ʼ��ʱ
    flushkey();                          // ��ռ�����Ϣ
    flushmouse();                        // ��������Ϣ
    HANDLE h1;
    h1 = CreateThread(NULL, 0, MouseControl, NULL, 0, NULL);  // �����߳�
    CloseHandle(h1);                                          // �ر��߳̾��
    for (; is_run; delay_fps(180)) {                          // ѭ��Ƶ��1��180��
        while (kbmsg()) {                                     // ��⵽������Ϣ����
            keyMsg = getkey();                                // ��ȡ������Ϣ
            if (keyMsg.msg == key_msg_down) {
                if (player_move(keyMsg))  // �ڸ����˵����λ���ϻ����������
                    putimage(150 + player.y * 14, 80 + player.x * 14, miniPlayer);
                if (maze[player.x][player.y] == Boss) {                      // ������boss
                    cnt++, maze[player.x][player.y] = Blank;                 // ��boss��Ϊ�ո񣬷�ֹ�����е�û�¸ɵ��˻���
                    tmpPic = newimage();                                     // ����ͼƬ��С
                    getimage(tmpPic, 0, 0, windowX, windowY);                // ��ȡ��ǰ��������
                    if (cnt == 1) {                                          // ������һ��boss
                        boss = newimage();                                   // ����ͼƬ��С
                        getimage(boss, "src/img/boss1.png");                 // ��ȡһ��boss����
                        cleardevice();                                       // ���ϱ���
                        setcolor(WHITE);                                     // ����������ɫ��ɫ
                        setfont(20, 0, "����");                              // ���������С
                        setbkmode(TRANSPARENT);                              // �������屳��͸��
                        putimage(450, 100, bigPlayer);                       // ����������󣨴�
                        putimage(10, 355, talk);                             // ���öԻ���
                        xyprintf(60, 375, player.name);                      // ��ӡ�������
                        xyprintf(60, 410, "���Թ���úڣ�����ײ����ʲô��");  // ��ӡ�Ի�����
                        mouse_click();                                       // ��������
                        putimage(100, 100, boss);                            // ����boss����
                        putimage(10, 355, talk);                             // �ٴη��öԻ����Ը��ǵ�ǰ���ӡ������
                        mouse_click();                                       // ��ͬ
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "���׵��ͻ�");
                        xyprintf(60, 410, "ʲô�˸ҵ��ҵ�·���Ѿ����ñ��ҳԵ��˵�׼������");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "�ף��⣬����Сè���ÿɰ�");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "���׵��ͻ�");
                        xyprintf(60, 410, "���ں�˵ʲô����������ͷ�ϵ�������");
                        xyprintf(60, 435, "�ҿ����ͻ����ˣ�");
                        xyprintf(60, 460, "��Ȼ��˵����è������������ֻ�ɶ��С�Ƽ���");
                        mouse_click();
                        cleardevice();
                        xyprintf(100, 205, "������������ܶ��ְ�");
                        mouse_click();
                        cleardevice();      // ����
                        srand(time(NULL));  // ����ʱ����Ϊ���������

                        sudo_game();  // ��������

                        if (out)     // �����������ѡ�����˳���Ϸ
                            return;  // �뿪����
                        cleardevice();
                        putimage(450, 100, bigPlayer);     // ����������󣨴�
                        putimage(100, 100, boss);          // ����boss
                        putimage(10, 355, talk);           // ���öԻ���
                        xyprintf(100, 375, "���׵��ͻ�");  // ��ӡ�Ի�������
                        xyprintf(60, 410, "�ⲻ���ܡ�");
                        xyprintf(60, 435, "�޵е��ͻ����˾�Ȼ�����һֻ����");
                        xyprintf(60, 460, "���۰�������������");
                        mouse_click();  // ��������
                        cleardevice();
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "�����ܵ��ˡ�");
                        xyprintf(60, 435, "����ruaһ��β�͵ġ�");
                        mouse_click();           // ��⵽���
                        delimage(boss);          // ����ͼƬ�ͷ��ڴ�
                        putimage(0, 0, tmpPic);  // ���ô�ǰ�����Ļ��棬�����ص��Թ���Ϸ�����Ч��
                        delimage(tmpPic);
                        tmpPic = boss = NULL;                 // ��ֹ�ٵ��ø�ָ��ָ������ͼƬʱ�����ֶ�����ָ��NULL
                    } else if (cnt == 2) {                    // �ڶ���boss
                        boss = newimage(186, 161);            // ����ͼƬ��С
                        getimage(boss, "src/img/boss2.png");  // ��ȡ�ڶ���boss���������������ͬ��
                        cleardevice();
                        setcolor(WHITE);
                        setfont(20, 0, "����");
                        setbkmode(TRANSPARENT);
                        putimage(100, 100, boss);
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "���֣�����ԭ�������ڵ���������");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "�������");
                        xyprintf(60, 410, "����ү���߰������������ߣ�������");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "�������ǡ���");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "������");
                        xyprintf(60, 410, "�������ɱ���ү�𣡣�������");
                        xyprintf(60, 435, "��Ȼ�ѱ�������ˮ�������죡������");
                        xyprintf(60, 460, "����ү�������ж��ﰡ�������ͷ�죡������");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "������Ҳ���Ƕ찡��");
                        mouse_click();
                        srand(time(NULL));

                        sudo_game();  // ��������

                        if (out)
                            return;
                        cleardevice();
                        putimage(450, 100, bigPlayer);
                        putimage(100, 100, boss);
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "һ���ܵ��������");
                        xyprintf(60, 410, "����ү������ү��Ȼ��");
                        xyprintf(60, 435, "�ܸ���һֻѼ�ӣ�������һֻˮ�����������");
                        xyprintf(60, 460, "�������ж���ĳ��裡������");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "��Ȼ���ǡ��ҿ����������ô��ˮ��������");
                        mouse_click();
                        delimage(boss);
                        putimage(0, 0, tmpPic);
                        delimage(tmpPic);
                        boss = tmpPic = NULL;
                    } else if (cnt == 3) {                    // ���һ��boss
                        boss = newimage(225, 200);            // ����ͼƬ��С
                        getimage(boss, "src/img/boss3.png");  // ��ȡ����bossͼƬ��С�����������ͬ��
                        cleardevice();
                        setcolor(WHITE);
                        setfont(20, 0, "����");
                        setbkmode(TRANSPARENT);
                        putimage(100, 100, boss);
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, "һֻ�޹���С����");
                        xyprintf(60, 410, "Ϊʲôһ��Ҫ��ȥ�أ�����������粻���𡭡�");
                        xyprintf(60, 435, "�ѵ�����ص��Ǹ������˷��յ���ʵ������");
                        mouse_click();
                        if (MessageBox(0, TEXT("Ҫ��������"), TEXT("hint"), 1) != 1) {  // ������һ��С��֧
                            player.stay = true;                                         // ���ѡ������������ɾͿ��ر��Ϊ���
                            putimage(10, 355, talk);                                    // ���öԻ���ʹ�ӡ����
                            xyprintf(60, 375, "һֻ���ֵ�С����");
                            xyprintf(60, 410, "��С����û��˵����Ϊ���ܿ��֣�");
                            delimage(boss);
                            delimage(tmpPic);
                            boss = tmpPic = NULL;
                            mouse_click();
                            return;  // �뿪�Թ���Ϸ
                        }
                        putimage(10, 355, talk);  // ����������Ϸ����
                        xyprintf(60, 375, "һֻ��̫���С����");
                        xyprintf(60, 410, "��Ȼ���඼����");
                        mouse_click();
                        xyprintf(60, 435, "��Ȼ�㲻Ը������������ֻ��������������");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, player.name);
                        xyprintf(60, 410, "ʲ��������ô֪���������ࣿ");
                        mouse_click();
                        srand(time(NULL));

                        sudo_game();  // ��������

                        if (out)
                            return;
                        putimage(100, 100, boss);
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, "һֻ�ѹ���С����");
                        xyprintf(60, 410, "�������û����Ը�������������ǡ���");
                        mouse_click();
                        delimage(boss);
                        delimage(tmpPic);
                        boss = tmpPic = NULL;
                        return;  // ��Ϸ����
                    }
                }
            }
        }
        if (out)
            return;
    }
}

bool cheat_judge() {
    if (player.admin)  // �ѵ�¼����Ա
        return true;
    else if (pwCnt < 3 && !player.admin && strcmp(player.name, "Suolk") == 0) {                       // ��¼�û���Ϊ����Ա�û��������������������
        if (MessageBox(0, TEXT("��⵽����δ��¼����Ա��ݣ��Ƿ����ڵ�¼"), TEXT("hint"), 1) != 1) {  // ѡ���¼����Ա������������������
            sys_edit pwEdit;                                                                          // ����һ���������
            key_msg keyMsg;                                                                           // ����һ��������
            char pwStr[20] = {0};                                                                     // ����һ���ַ������鴢�����������
            bool pwFlag = false;                                                                      // ����һ�������ء������ж����������Ƿ���ȷ����ʼ��Ϊ����ȷ
            init_input(pwEdit, 440, 10, 150);
            flushkey();                                                          // ��մ�ǰ�ļ�����Ϣ
            for (; is_run(); delay_fps(60)) {                                    // ����ѭ����ѭ��Ƶ����ÿ��60��
                char tmpStr[50] = {0};                                           // �����ַ�����������д�����ݲ�ȷ���Ĵ�����ʾ
                keyMsg = getkey();                                               // ��ȡ������Ϣ
                if (keyMsg.msg == key_msg_down && keyMsg.key == key_enter) {     // ��⵽����enter��
                    pwCnt++;                                                     // �����������+1
                    pwEdit.gettext(10, pwStr);                                   // ��ȡ������������ڵ��ı�
                    if (strcmp(pwStr, "tgb951") == 0) {                          // ����Ƿ�������ȷ
                        MessageBox(0, TEXT("������ȷ��"), TEXT("hint"), 0);      // ������ȷ�򵯳�����ʾ��
                        pwFlag = true;                                           // ������״̬���Ϊ��ȷ
                        player.admin = true;                                     // �������Ƿ��¼��ȫ�ֱ������Ϊ��
                    } else if (pwCnt < 3) {                                      // ����������󣬵������������������ʣ
                        sprintf(tmpStr, "��������㻹��%d�λ���", 3 - pwCnt);  // ��������ʾд��hint����
                        pwEdit.settext("");                                      // ������������
                        pwEdit.setfocus();                                       // ���ù��
                        MessageBox(0, tmpStr, TEXT("hint"), 0);                  // ������ʾ��
                    }
                    flushkey();                       // ��ռ�����Ϣ��ֹ�´μ���⵽���õļ�����Ϣ
                    memset(pwStr, 0, sizeof(pwStr));  // ��մ�������������ַ������飬��ֹ�´μ��ʱ��������ַ����¼�����
                } else if (pwFlag)
                    return true;               // �������������ȷ����Ż���
                if (pwCnt >= 3) return false;  // ���������������ﵽ������û����ȷ�������ؼ�
            }
        } else {                                                         // ѡ�񲻵�¼����Ա���
            MessageBox(0, TEXT("ƽ����һ������ף�"), TEXT("hint"), 0);  // ����������ʾ
            return false;
        }
    } else if (pwCnt == 3) {  // �ڴ�֮ǰ�����������Ѿ��ﵽ������
        MessageBox(0, TEXT("���������������Ѵ����ޣ��޷���¼��"), TEXT("hint"), 0);
        return false;
    } else {  // �ǹ���Ա
        MessageBox(0, TEXT("ƽ����һ������ף�"), TEXT("hint"), 0);
        return false;
    }
}

bool exit_game() {
    if (MessageBox(0, TEXT("ȷ���˳���Ϸ��"), TEXT("hint"), 1) != 1) {
        out = true;  // ������״̬�޸�Ϊ�˳�
        return true;
    }
    return false;
}
