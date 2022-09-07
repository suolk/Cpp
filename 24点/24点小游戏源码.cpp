#include <graphics.h>
#include <mmsystem.h>
#include <sys_edit.h>
#include <windows.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <stack>
#pragma comment(lib, "winmm.lib")
using namespace std;

class Player {
   private:
    char player_name[100] = {0}, password[100] = {0};
    int Record = 0, New_score = 0;

   public:
    bool Login();                 //�û���¼
    void Create_pw(char pw_t[]);  //���봴��
    bool Check_pw(char pw_t[]);   //������
    void Record_up();
    void Final_save();
} player;

bool mark[15][15][15][15] = {0}, music_play = true, new_record = false;
int final_record;
stack<double> p;

void mouse_click();                                    //��������
void Start_interface();                                //��ʼ����
void click_draw(int a, int b, int c, int d);           //�������
bool num_check(char s[], int a, int b, int c, int d);  //�����������ֺ�ʵ���ϵ��Ƿ���ͬ
int check(char ch);                                    //������׺���ʽ�ķ������ȼ�
void cal(char ch);                                     //�Ӽ��˳�����
double cal(char s[]);                                  //��׺ת��׺����
void Game_interface();                                 //��Ϸ����
void end_game();                                       //��Ϸ����

int main() {
    srand(time(NULL));
    setinitmode(0, 600, 200);                                                         //���ô���λ��
    initgraph(850, 478, 0);                                                           //�������ڲ����ô�С
    setcaption("24��С��Ϸ");                                                         //���ô��ڱ���
    setbkcolor(EGERGB(153, 204, 255));                                                //���ñ���ɫ
    if (mciSendString((TEXT("open Music_Start.mp3 alias STARTBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
    {                                                                                 //���ɹ��򵯴���ʾ����������
        MessageBox(getHWnd(), TEXT("can't find Music_Start.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    mciSendString(TEXT("setaudio STARTBGM volume to 80"), NULL, 0, NULL);  //��������
    mciSendString(TEXT("play STARTBGM repeat"), NULL, 0, NULL);            //��ʼ����

    Start_interface();                                     //��Ϸ��ʼ����
    mciSendString(TEXT("close STARTBGM"), NULL, 0, NULL);  //��������
    if (!player.Login()) {
        return 0;  //��½ʧ����ֱ�ӽ�������
    }
    MessageBox(getHWnd(), TEXT("��½�ɹ�"), TEXT("hint"), MB_OK);  //��½�ɹ��򵯴���ʾ

    Game_interface();
    end_game();
}

bool Player::Login() {
    bool password_input = false;             //����Ƿ�������û������벿��׼����ʼ��������
    bool new_player = false;                 //����Ƿ��������
    key_msg login_key;                       //�������̰�������
    cleardevice();                           //����
    setbkmode(TRANSPARENT);                  //�������屳����ɫ͸��
    setfont(20, 0, "����");                  //��������
    setcolor(BLACK);                         //����������ɫ
    xyprintf(220, 165, "�������û���:");     //�ڽ����ϴ�ӡ����
    setcolor(DARKGRAY);                      //����������ɫ
    xyprintf(200, 190, "�����Ȳ�����10��");  //��ӡ����
    sys_edit player_input;                   //������������
    player_input.create(false);              //�������������
    player_input.move(350, 160);             //����λ��
    player_input.size(300, 30);              //����������С
    player_input.setmaxlen(10);              //���������ɼ�����ַ����ֵ
    player_input.setbgcolor(WHITE);          //����������ڱ���ɫ
    player_input.setfont(20, 0, "����");     //���������������
    player_input.visible(true);              //���������ɼ�
    player_input.setfocus();                 //��������ڹ����˸
    delay_ms(0);                             //ˢ�´���

    for (; is_run(); delay_fps(60)) {
        flushkey();                      //��մ�ǰ�İ�����Ϣ
        for (; is_run(); delay_fps(60))  //��ѭ�����ڼ���Ƿ���enter����ѭ��Ƶ��Ϊÿ��60��
        {
            login_key = getkey();
            if (login_key.msg == key_msg_down && login_key.key == key_enter) {
                break;
            }
        }
        player_input.gettext(10, player_name);                //��ȡ���������
        strcat(player_name, ".txt");                          //���ַ������ν��ļ���׺��
        if (freopen(player.player_name, "r", stdin) == NULL)  //�����ʧ��˵������Ҳ�����
        {
            int temp = MessageBox(getHWnd(), TEXT("�Ƿ��Դ��û��������µ��˻���"), TEXT("hint"), MB_OKCANCEL);  //ѯ���Ƿ񴴽����˻�
            if (temp == 1)                                                                                      //�������˻�
            {
                password_input = new_player = true;
                if (!freopen(player_name, "w", stdout))  //����ʧ�ܱ�����ʾ
                {
                    MessageBox(getHWnd(), TEXT("failed open user txt"), TEXT("hint"), MB_OK | MB_ICONEXCLAMATION);
                }
            } else if (temp == 2)  //���������˻�
            {
                MessageBox(getHWnd(), TEXT("�û������ڣ������������û���!"), TEXT("hint"), MB_OK | MB_ICONEXCLAMATION);
                player_input.settext("");  //��������
                player_input.setfocus();   //���ù��
                continue;
            }
        } else  //����Ѵ���
        {
            cin >> password >> Record;  //��ȡ�������߹�����
            fclose(stdin);              //�ر��ļ�
            password_input = true;
        }
        if (password_input)  //�����������벿��
            break;
    }
    player_input.settext("");                //��������
    player_input.setfocus();                 //���ù��
    cleardevice();                           //����
    setcolor(BLACK);                         //����������ɫ
    xyprintf(240, 165, "����������:");       //��ӡ����
    setcolor(DARKGRAY);                      //����������ɫ
    xyprintf(200, 190, "�����Ȳ�����10��");  //��ӡ����

    for (int cnt = 0; is_run(); delay_fps(60))  // cntΪ����������
    {
        flushkey();                      //��մ�ǰ�İ�����Ϣ
        for (; is_run(); delay_fps(60))  //���enter
        {
            login_key = getkey();
            if (login_key.msg == key_msg_down && login_key.key == key_enter) {
                break;
            }
        }
        char pw_temp[100];                  //����һ���м����
        player_input.gettext(10, pw_temp);  //��ȡ���������
        if (new_player)                     //�����û���������봴������
        {
            Create_pw(pw_temp);      //���봴��
            fclose(stdout);          //�ر��ļ�
            player_input.destroy();  //���������
            return true;             //����true�����½�ɹ�
        } else                       //�������û�����������麯��
        {
            if (!Check_pw(pw_temp))  //��������½ʧ��
            {
                ++cnt;         //�����������1
                if (cnt == 1)  //���ݲ�ͬ�Ĵ������������ͬ�Ĵ�����ʾ
                    MessageBox(getHWnd(), TEXT("�㻹��2�λ��ᣡ"), TEXT("hint"), MB_OK);
                else if (cnt == 2)
                    MessageBox(getHWnd(), TEXT("�㻹��1�λ��ᣡ"), TEXT("hint"), MB_OK);
                else if (cnt == 3)
                    MessageBox(getHWnd(), TEXT("����������������Ѻľ���"), TEXT("hint"), MB_OK | MB_ICONEXCLAMATION);
                player_input.settext("");  //��������
                player_input.setfocus();   //���ù��
            } else                         //������ȷ��½�ɹ�
            {
                player_input.destroy();  //���������
                return true;             //����true�����½�ɹ�
            }
            if (cnt > 3)  //�����������ﵽ����
            {
                player_input.destroy();  //���������
                return false;            //����false�����½ʧ��
            }
        }
    }
}

void Player::Create_pw(char pw_t[]) {
    sprintf(password, pw_t);  //�������������ṹ����
    Record = 0;               //��ʼͨ����0
    cout << pw_t << endl;     //������������ļ���
    cout << 0 << endl;        //��ͨ�ؼ�¼������ļ���
}

bool Player::Check_pw(char pw_t[]) {
    if (strcmp(pw_t, password) == 0)  //�Ա������������ļ��д��������
    {
        return true;  //��ͬ��������ȷ����true
    } else {
        return false;  //��ͬ��������󷵻�false
    }
}

void Player::Record_up() {
    ++New_score;  //������Ϸ�ļ�¼����
}

void Player::Final_save() {
    if (freopen(player_name, "w", stdout) == NULL)  //���û��ļ�
    {
        MessageBox(getHWnd(), TEXT("failed to save the data"), TEXT("hint"), MB_OK);
    }
    cout << password << endl;  //��������
    if (New_score > Record)    //����Ƿ��Ƽ�¼
    {
        Record = New_score;
        new_record = true;
    }
    final_record = Record;
    cout << Record << endl;  //�����¼�¼
    fclose(stdout);          //�ر��ļ�
}

void mouse_click()  //���ڼ�������
{
    flushmouse();
    int color = 0;
    mouse_msg mouse_click_mouse;
    for (; is_run(); delay_fps(800), ++color) {
        if (mousemsg())  //����Ƿ��������Ϣ
        {
            mouse_click_mouse = getmouse();  //���������Ϣ�����Ƿ�Ϊ���
            if (mouse_click_mouse.is_down()) {
                return;
            }
        }
        if (color < 200)  //������ͼ��Ч��
        {
            setcolor(DARKGRAY);
            xyprintf(190, 300, "лл���棡");
        } else if (color < 400) {
            setcolor(BLACK);
            xyprintf(190, 300, "лл���棡");
        } else {
            color = 0;
        }
    }
}

void Start_interface() {
    PIMAGE P_Start = newimage();         //����ͼƬ����
    getimage(P_Start, "Pic_Start.jpg");  //��ȡͼƬ�ļ�
    putimage(0, 0, P_Start);             //�ڴ����ϸ���ͼƬ
    delimage(P_Start);                   //����ͼƬ��������ֹ�ڴ�й¶

    setfontbkcolor(WHITE);             //�������屳����ɫ
    setfont(100, 0, "����");           //������������
    setcolor(DARKGRAY);                //����������ɫ
    xyprintf(160, 170, "24��С��Ϸ");  //�ڴ����ϴ�ӡ����
    setfillcolor(WHITE);               //���������ɫ
    bar(270, 340, 530, 420);           //��ʵ�ľ��θ��ǵ�������ֹ�����尴ť
    setlinewidth(3.0);                 //���û�ͼ�߿�
    setcolor(LIGHTGRAY);               //���û�ͼ��ɫ
    rectangle(275, 345, 525, 415);     //���ư�ť�߿�
    setcolor(BLACK);                   //����������ɫ
    setfont(60, 0, "����");            //���������С
    xyprintf(280, 350, "��ʼ��Ϸ");    //�ڴ����ϴ�ӡ����

    flushmouse();
    mouse_msg Start_click_mouse;
    bool push = false;
    for (; is_run(); delay_fps(800)) {
        if (mousemsg())  //��������Ϣ
        {
            Start_click_mouse = getmouse();                                                                                                                                                            //��ȡ�����Ϣ
            if (Start_click_mouse.is_down() && Start_click_mouse.is_left() && Start_click_mouse.x >= 275 && Start_click_mouse.x <= 525 && Start_click_mouse.y >= 345 && Start_click_mouse.y <= 415) {  //�����������Ұ���λ���ڰ�ť������Ƴ������¡���Ч��
                setlinewidth(2.0);
                setcolor(DARKGRAY);
                rectangle(275, 345, 525, 415);
                push = true;                               //��ʱ״̬Ϊ��갴���˰�ť
            } else if (push && Start_click_mouse.is_up())  //������ɿ��˰����Ҵ�ǰ������ڰ�ť�����µ�
            {
                if (Start_click_mouse.x >= 275 && Start_click_mouse.x <= 525 && Start_click_mouse.y >= 345 && Start_click_mouse.y <= 415) {
                    return;  //����ɿ�ʱ������ڰ�ť�Ϸ���ʼ��Ϸ���뿪��ʼ���棬����������
                }
                setlinewidth(2.0);  //���򽫰�ť�ָ�Ϊδ���µ�״̬
                setcolor(LIGHTGRAY);
                rectangle(275, 345, 525, 415);
                push = false;
            }
        }
    }
}

void click_draw(int a, int b, int c, int d) {
    setlinewidth(4.0);      //�����߿�
    setcolor(DARKGRAY);     //������ɫ
    rectangle(a, b, c, d);  //��һ������
}

bool num_check(char s[], int a, int b, int c, int d) {
    int len = strlen(s), num = 0;  //��ȡ�ַ�������
    int a1[15] = {0};              //��¼���ɵ��ĸ�����
    int a2[15] = {0};              //������������
    ++a1[a], ++a1[b], ++a1[c], ++a1[d];
    for (int i = 0; i < len; ++i) {
        if (s[i] >= '0' && s[i] <= '9')  //��ȡ���������
        {
            num = num * 10 + s[i] - 48;
            if (num > 13)  //�������13����13һ���Ǵ�������
            {
                return false;
            }
        } else {
            if (num != 0) {
                ++a2[num];
                num = 0;
            }
        }
    }
    if (num != 0) {
        ++a2[num];
    }
    for (int i = 1; i <= 13; ++i)  //�Ա�
    {
        if (a1[i] != a2[i]) {
            return false;
        }
    }
    return true;
}

int check(char ch) {
    if (ch == '(')  //���ظ�������Ŷ�Ӧ�����ȼ�
    {
        return 1;
    } else if (ch == '+' || ch == '-') {
        return 2;
    } else if (ch == '*' || ch == '/') {
        return 3;
    } else if (isdigit(ch)) {
        return 4;
    } else if (ch == ')') {
        return 5;
    } else {
        return -1;
    }
}

void cal(char ch) {
    double a, b;  //��׺���ʽ����
    b = p.top();
    p.pop();
    a = p.top();
    p.pop();
    if (ch == '+') {
        p.push(a + b);
    } else if (ch == '-') {
        p.push(a - b);
    } else if (ch == '*') {
        p.push(a * b);
    } else if (ch == '/') {
        p.push(a / b);
    }
}

double cal(char s[]) {
    stack<char> a;  //��׺���ʽת��Ϊ��׺���ʽ
    int flag = 0, f = 1;
    double num = 0;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        int level = check(s[i]);
        if (level == -1) {
            return 1e9;
        } else if (level == 4)  //���ն�Ӧ�����ȼ����ж�Ӧ�Ĳ���
        {
            num = num * 10 + s[i] - 48, flag = 1;
        } else {
            if (flag != 0) {
                p.push(num);
                flag = num = 0;
            }
            if (level == 1) {
                a.push(s[i]);
            } else if (level == 3) {
                while ((!a.empty()) && check(a.top()) == 3) {
                    cal(a.top());
                    a.pop();
                }
                a.push(s[i]);
            } else if (level == 2) {
                if (i != 0 && check(s[i - 1]) >= 4) {
                    while ((!a.empty()) && check(a.top()) >= 2) {
                        cal(a.top());
                        a.pop();
                    }
                    a.push(s[i]);
                } else if (s[i] == '-') {
                    f *= -1;
                }
            } else if (level == 5) {
                while (a.top() != '(') {
                    cal(a.top());
                    a.pop();
                }
                a.pop();
            }
        }
    }
    if (flag != 0) {
        num *= f;
        p.push(num);
        flag = num = 0;
    }
    while (!a.empty()) {
        cal(a.top());
        a.pop();
    }
    return p.top();
}

void Game_interface() {
    cleardevice();
    setlinewidth(4.0);
    setcolor(DARKGRAY);
    rectangle(450, 20, 550, 60);  //���Ʒ���
    rectangle(580, 20, 680, 60);
    rectangle(710, 20, 840, 60);
    rectangle(710, 70, 840, 110);
    rectangle(630, 295, 700, 335);
    setfont(30, 0, "����");
    setcolor(BLACK);
    setbkmode(TRANSPARENT);
    xyprintf(455, 25, "��һ��");  //�ڷ����ڴ�ӡ���֣��γɰ�ť
    xyprintf(585, 25, "��һ��");
    xyprintf(715, 25, "ֹͣ����");
    xyprintf(715, 75, "������Ϸ");
    xyprintf(635, 300, "�ύ");
    setfont(20, 0, "����");
    xyprintf(180, 335, "���޷��ó�������������������0����������");
    xyprintf(180, 370, "���ܵó����������������������ʽ");

    int cnt = 1;  //�������
    bool music_click = false, push = false;
    mouse_msg game_interface_msg;
    if (mciSendString((TEXT("open Music_bgm_1.mp3 alias GAMEBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
    {                                                                                //���ɹ��򵯴���ʾ����������
        MessageBox(getHWnd(), TEXT("can't find Music_Start.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
    }
    mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //��������
    mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //��ʼ����

    sys_edit num_1, num_2, num_3, num_4, answer;
    num_1.create(false), num_2.create(false), num_3.create(false), num_4.create(false);                                      //�������������
    num_1.move(100, 160), num_2.move(300, 160), num_3.move(500, 160), num_4.move(700, 160);                                  //����λ��
    num_1.size(40, 40), num_2.size(40, 40), num_3.size(40, 40), num_4.size(40, 40);                                          //����������С
    num_1.setmaxlen(2), num_2.setmaxlen(2), num_3.setmaxlen(2), num_4.setmaxlen(2);                                          //���������ɼ�����ַ����ֵ
    num_1.setbgcolor(WHITE), num_2.setbgcolor(WHITE), num_3.setbgcolor(WHITE), num_4.setbgcolor(WHITE);                      //����������ڱ���ɫ
    num_1.setfont(30, 0, "����"), num_2.setfont(30, 0, "����"), num_3.setfont(30, 0, "����"), num_4.setfont(30, 0, "����");  //���������������
    num_1.setreadonly(true), num_2.setreadonly(true), num_3.setreadonly(true), num_4.setreadonly(true);                      //��������򲻿����룬��ֻ��״̬
    num_1.visible(true), num_2.visible(true), num_3.visible(true), num_4.visible(true);                                      //���������ɼ�

    answer.create(false);
    answer.move(180, 300);
    answer.size(420, 30);
    answer.setmaxlen(20);
    answer.setbgcolor(WHITE);
    answer.setfont(20, 0, "����");
    answer.visible(true);

    char temp_num[5][5] = {0};
    int a = rand() % 13 + 1;  //��ȡ�ĸ������
    int b = rand() % 13 + 1;
    int c = rand() % 13 + 1;
    int d = rand() % 13 + 1;
    int x1, x2, y1, y2;

game:
    while (mark[a][b][c][d])  //�����ĸ��������ϳ��ֹ��������»�ȡ�����
    {
        a = rand() % 13 + 1;
        b = rand() % 13 + 1;
        c = rand() % 13 + 1;
        d = rand() % 13 + 1;
    }
    mark[a][b][c][d] = mark[a][b][d][c] = true;  //��Щ��ϳ��ֹ��ˣ����
    mark[a][c][b][d] = mark[a][c][d][b] = true;
    mark[a][d][b][c] = mark[a][d][c][b] = true;
    mark[b][c][d][a] = mark[b][c][a][d] = true;
    mark[b][d][c][a] = mark[b][d][a][c] = true;
    mark[b][a][c][d] = mark[b][a][d][c] = true;
    mark[c][d][a][b] = mark[c][d][b][a] = true;
    mark[c][a][d][b] = mark[c][a][b][d] = true;
    mark[c][b][a][d] = mark[c][b][d][a] = true;
    mark[d][a][b][c] = mark[d][a][c][b] = true;
    mark[d][b][a][c] = mark[d][b][c][a] = true;
    mark[d][c][a][b] = mark[d][c][b][a] = true;
    sprintf(temp_num[1], "%d", a);
    sprintf(temp_num[2], "%d", b);
    sprintf(temp_num[3], "%d", c);
    sprintf(temp_num[4], "%d", d);
    num_1.settext(temp_num[1]);  //�����ַ��������
    num_2.settext(temp_num[2]);
    num_3.settext(temp_num[3]);
    num_4.settext(temp_num[4]);
    answer.settext("");  //��մ������
    answer.setfocus();
    for (; is_run(); delay_fps(1000)) {
        if (mousemsg()) {
            game_interface_msg = getmouse();
            if (game_interface_msg.is_down() && game_interface_msg.is_left())  //�����갴��
            {
                if (game_interface_msg.x >= 450 && game_interface_msg.x <= 550 && game_interface_msg.y >= 20 && game_interface_msg.y <= 60)  //��һ��
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(450, 20, 550, 60);  //���Ƴ������°�ť����Ч��
                    x1 = 450, y1 = 20, x2 = 550, y2 = 60, push = true;
                    if (--cnt < 1)  //������ű仯
                        cnt += 5;
                    mciSendString(TEXT("close GAMEBGM"), NULL, 0, NULL);
                    music_click = true;                                                                                                             //˵�����µ����л������İ�ť
                } else if (game_interface_msg.x >= 580 && game_interface_msg.x <= 680 && game_interface_msg.y >= 20 && game_interface_msg.y <= 60)  //��һ��
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(580, 20, 680, 60);  //���Ƴ������°�ť����Ч��
                    x1 = 580, y1 = 20, x2 = 680, y2 = 60, push = true;
                    if (++cnt > 5)  //������ű仯
                    {
                        cnt -= 5;
                    }
                    mciSendString(TEXT("close GAMEBGM"), NULL, 0, NULL);
                    music_click = true;                                                                                                             //˵�����µ����л������İ�ť
                } else if (game_interface_msg.x >= 710 && game_interface_msg.x <= 840 && game_interface_msg.y >= 20 && game_interface_msg.y <= 60)  //ֹͣ/��ʼ����
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(710, 20, 840, 60);  //���Ƴ������°�ť����Ч��
                    x1 = 710, y1 = 20, x2 = 840, y2 = 60, push = true;
                    if (music_play)  //˵����ʱ�������ڲ���
                    {
                        setfillcolor(EGEARGB(255, 153, 204, 255));
                        ege_fillrect(715, 25, 60, 30);
                        setfont(30, 0, "����");
                        xyprintf(715, 25, "��ʼ");                            //����ť��Ϊ��ʼ���ţ��Եȴ���һ�ε��
                        music_play = false;                                   //������ֲ���״̬
                        mciSendString(TEXT("close GAMEBGM"), NULL, 0, NULL);  //ֹͣ���Ÿ���
                    } else {
                        setfillcolor(EGEARGB(255, 153, 204, 255));
                        ege_fillrect(715, 25, 60, 30);
                        setfont(30, 0, "����");
                        xyprintf(715, 25, "ֹͣ");  //����ť��Ϊֹͣ���ţ��Եȴ���һ�ε��
                        music_click = true;         //���״̬
                    }
                } else if (game_interface_msg.x >= 710 && game_interface_msg.x <= 840 && game_interface_msg.y >= 70 && game_interface_msg.y <= 110)  //������Ϸ
                {
                    if (MessageBox(getHWnd(), TEXT("�Ƿ��˳���Ϸ��"), TEXT("hint"), MB_OKCANCEL) == 1) {
                        return;
                    }
                } else if (game_interface_msg.x <= 700 && game_interface_msg.x >= 30 && game_interface_msg.y <= 335 && game_interface_msg.y >= 295)  //�ύ
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(630, 295, 700, 335);
                    x1 = 630, y1 = 295, x2 = 700, y2 = 335, push = true;

                    char temp_player_ans[50] = {0};  //��ȡ�𰸣���׺���ʽ��
                    bool wrong_ans = false, wrong_num = false, wrong_ch = false;
                    answer.gettext(20, temp_player_ans);
                    if (strcmp(temp_player_ans, "0") == 0)  //���ѡ������
                    {
                        goto game;
                    } else if (strcmp(temp_player_ans, "") == 0)  //����ύ�˿մ�
                    {
                        wrong_ans = true;
                    } else  //�����������׺���ʽ
                    {
                        if (!num_check(temp_player_ans, a, b, c, d))  //������������Ƿ���ȷ
                        {
                            wrong_num = true;
                        } else  //��ȷ��������
                        {
                            double cal_ans = cal(temp_player_ans);
                            if (cal_ans == 1e9) {
                                wrong_ch = true;
                            } else if (cal_ans > 24.00001 || cal_ans < 23.999999)  //��ֹ�������ĵ��������
                            {
                                wrong_ans = true;
                            }
                        }
                    }
                    if (wrong_ans)  //�𰸴���
                    {
                        MessageBox(getHWnd(), TEXT("�𰸴���"), TEXT("hint"), MB_OK);
                        wrong_ans = false;
                    } else if (wrong_num)  //��������ֲ���ȷ
                    {
                        MessageBox(getHWnd(), TEXT("������������"), TEXT("hint"), MB_OK);
                        wrong_num = false;
                    } else if (wrong_ch)  //����ķ��Ų���ȷ
                    {
                        MessageBox(getHWnd(), TEXT("ֻ�����롮+������-������*������/������(������)���⼸�ַ��ţ�"), TEXT("hint"), MB_OK);
                        wrong_ch = false;
                    } else  //����ȷ
                    {
                        MessageBox(getHWnd(), TEXT("����ȷ��"), TEXT("hint"), MB_OK);
                        player.Record_up();
                        goto game;  //�ص���Ϸ��ʼ�µ�һ��
                    }
                }
                if (music_click) {
                    music_click = false, music_play = true;
                    switch (cnt) {
                        case 1:
                            if (mciSendString((TEXT("open Music_bgm_1.mp3 alias GAMEBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
                            {                                                                                //���ɹ��򵯴���ʾ����������
                                MessageBox(getHWnd(), TEXT("can't findMusic_bgm_1.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //��������
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //��ʼ����
                            break;
                        case 2:
                            if (mciSendString((TEXT("open Music_bgm_2.mp3 alias GAMEBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
                            {                                                                                //���ɹ��򵯴���ʾ����������
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_2.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //��������
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //��ʼ����
                            break;
                        case 3:
                            if (mciSendString((TEXT("open Music_bgm_3.mp3 alias GAMEBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
                            {                                                                                //���ɹ��򵯴���ʾ����������
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_3.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //��������
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //��ʼ����
                            break;
                        case 4:
                            if (mciSendString((TEXT("open Music_bgm_4.mp3 alias GAMEBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
                            {                                                                                //���ɹ��򵯴���ʾ����������
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_4.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //��������
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //��ʼ����
                            break;
                        case 5:
                            if (mciSendString((TEXT("open Music_bgm_5.mp3 alias GAMEBGM")), NULL, 0, NULL))  //�ж��Ƿ�ɹ���������
                            {                                                                                //���ɹ��򵯴���ʾ����������
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_5.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //��������
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //��ʼ����
                            break;
                    }
                }
            } else if (push && game_interface_msg.is_up())  //���Ƴ����°�ť��Ч��
            {
                push = false;
                click_draw(x1, y1, x2, y2);
            }
        }
    }
}

void end_game() {
    cleardevice();
    setfont(30, 0, "����");
    setcolor(BLACK);
    setfont(30, 0, "����");
    setcolor(BLACK);
    setbkmode(TRANSPARENT);
    if (true)
        xyprintf(120, 150, "��ϲ����������֮ǰ�ĵ�ͨ�ؼ�¼��");  //������Ҫ��ӡ����
    xyprintf(120, 200, "������߼�¼��%d�أ�", 1);
    setfont(20, 0, "����");
    setcolor(DARKGRAY);
    xyprintf(10, 420, "���������");
    setfont(100, 0, "����");
    setcolor(DARKGRAY);
    xyprintf(190, 300, "лл���棡");
    mouse_click();
}