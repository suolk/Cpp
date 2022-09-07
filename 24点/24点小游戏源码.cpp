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
    bool Login();                 //用户登录
    void Create_pw(char pw_t[]);  //密码创建
    bool Check_pw(char pw_t[]);   //密码检查
    void Record_up();
    void Final_save();
} player;

bool mark[15][15][15][15] = {0}, music_play = true, new_record = false;
int final_record;
stack<double> p;

void mouse_click();                                    //鼠标点击检测
void Start_interface();                                //开始界面
void click_draw(int a, int b, int c, int d);           //点击动画
bool num_check(char s[], int a, int b, int c, int d);  //检查输入的数字和实际上的是否相同
int check(char ch);                                    //返回中缀表达式的符号优先级
void cal(char ch);                                     //加减乘除计算
double cal(char s[]);                                  //中缀转后缀主体
void Game_interface();                                 //游戏界面
void end_game();                                       //游戏结束

int main() {
    srand(time(NULL));
    setinitmode(0, 600, 200);                                                         //设置窗口位置
    initgraph(850, 478, 0);                                                           //创建窗口并设置大小
    setcaption("24点小游戏");                                                         //设置窗口标题
    setbkcolor(EGERGB(153, 204, 255));                                                //设置背景色
    if (mciSendString((TEXT("open Music_Start.mp3 alias STARTBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
    {                                                                                 //不成功则弹窗提示并结束程序
        MessageBox(getHWnd(), TEXT("can't find Music_Start.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    mciSendString(TEXT("setaudio STARTBGM volume to 80"), NULL, 0, NULL);  //调整音量
    mciSendString(TEXT("play STARTBGM repeat"), NULL, 0, NULL);            //开始播放

    Start_interface();                                     //游戏开始界面
    mciSendString(TEXT("close STARTBGM"), NULL, 0, NULL);  //结束播放
    if (!player.Login()) {
        return 0;  //登陆失败则直接结束程序
    }
    MessageBox(getHWnd(), TEXT("登陆成功"), TEXT("hint"), MB_OK);  //登陆成功则弹窗提示

    Game_interface();
    end_game();
}

bool Player::Login() {
    bool password_input = false;             //检查是否完成了用户名输入部分准备开始密码输入
    bool new_player = false;                 //检查是否是新玩家
    key_msg login_key;                       //创建键盘按键变量
    cleardevice();                           //清屏
    setbkmode(TRANSPARENT);                  //设置字体背景颜色透明
    setfont(20, 0, "宋体");                  //设置字体
    setcolor(BLACK);                         //设置文字颜色
    xyprintf(220, 165, "请输入用户名:");     //在界面上打印文字
    setcolor(DARKGRAY);                      //设置字体颜色
    xyprintf(200, 190, "（长度不超过10）");  //打印文字
    sys_edit player_input;                   //创建输入框变量
    player_input.create(false);              //创建单行输入框
    player_input.move(350, 160);             //设置位置
    player_input.size(300, 30);              //设置输入框大小
    player_input.setmaxlen(10);              //设置输入框可键入的字符最大值
    player_input.setbgcolor(WHITE);          //设置输入框内背景色
    player_input.setfont(20, 0, "宋体");     //设置输入框内文字
    player_input.visible(true);              //设置输入框可见
    player_input.setfocus();                 //令输入框内光标闪烁
    delay_ms(0);                             //刷新窗口

    for (; is_run(); delay_fps(60)) {
        flushkey();                      //清空此前的按键消息
        for (; is_run(); delay_fps(60))  //该循环用于检测是否按下enter键，循环频率为每秒60次
        {
            login_key = getkey();
            if (login_key.msg == key_msg_down && login_key.key == key_enter) {
                break;
            }
        }
        player_input.gettext(10, player_name);                //获取输入框内容
        strcat(player_name, ".txt");                          //在字符串后衔接文件后缀名
        if (freopen(player.player_name, "r", stdin) == NULL)  //如果打开失败说明该玩家不存在
        {
            int temp = MessageBox(getHWnd(), TEXT("是否以此用户名创建新的账户？"), TEXT("hint"), MB_OKCANCEL);  //询问是否创建新账户
            if (temp == 1)                                                                                      //创建新账户
            {
                password_input = new_player = true;
                if (!freopen(player_name, "w", stdout))  //创建失败报警提示
                {
                    MessageBox(getHWnd(), TEXT("failed open user txt"), TEXT("hint"), MB_OK | MB_ICONEXCLAMATION);
                }
            } else if (temp == 2)  //不创建新账户
            {
                MessageBox(getHWnd(), TEXT("用户不存在，请重新输入用户名!"), TEXT("hint"), MB_OK | MB_ICONEXCLAMATION);
                player_input.settext("");  //清空输入框
                player_input.setfocus();   //重置光标
                continue;
            }
        } else  //玩家已存在
        {
            cin >> password >> Record;  //获取密码和最高过关数
            fclose(stdin);              //关闭文件
            password_input = true;
        }
        if (password_input)  //进入密码输入部分
            break;
    }
    player_input.settext("");                //清空输入框
    player_input.setfocus();                 //重置光标
    cleardevice();                           //清屏
    setcolor(BLACK);                         //设置字体颜色
    xyprintf(240, 165, "请输入密码:");       //打印文字
    setcolor(DARKGRAY);                      //设置字体颜色
    xyprintf(200, 190, "（长度不超过10）");  //打印文字

    for (int cnt = 0; is_run(); delay_fps(60))  // cnt为密码错误次数
    {
        flushkey();                      //清空此前的按键消息
        for (; is_run(); delay_fps(60))  //检测enter
        {
            login_key = getkey();
            if (login_key.msg == key_msg_down && login_key.key == key_enter) {
                break;
            }
        }
        char pw_temp[100];                  //设置一个中间变量
        player_input.gettext(10, pw_temp);  //获取输入框内容
        if (new_player)                     //是新用户则进入密码创建函数
        {
            Create_pw(pw_temp);      //密码创建
            fclose(stdout);          //关闭文件
            player_input.destroy();  //销毁输入框
            return true;             //返回true代表登陆成功
        } else                       //不是新用户则进入密码检查函数
        {
            if (!Check_pw(pw_temp))  //密码错误登陆失败
            {
                ++cnt;         //错误次数自增1
                if (cnt == 1)  //根据不同的错误次数弹出不同的错误提示
                    MessageBox(getHWnd(), TEXT("你还有2次机会！"), TEXT("hint"), MB_OK);
                else if (cnt == 2)
                    MessageBox(getHWnd(), TEXT("你还有1次机会！"), TEXT("hint"), MB_OK);
                else if (cnt == 3)
                    MessageBox(getHWnd(), TEXT("您的密码输入次数已耗尽！"), TEXT("hint"), MB_OK | MB_ICONEXCLAMATION);
                player_input.settext("");  //清空输入框
                player_input.setfocus();   //重置光标
            } else                         //密码正确登陆成功
            {
                player_input.destroy();  //销毁输入框
                return true;             //返回true代表登陆成功
            }
            if (cnt > 3)  //密码错误次数达到三次
            {
                player_input.destroy();  //销毁输入框
                return false;            //返回false代表登陆失败
            }
        }
    }
}

void Player::Create_pw(char pw_t[]) {
    sprintf(password, pw_t);  //将输入密码存入结构体中
    Record = 0;               //初始通关数0
    cout << pw_t << endl;     //将密码输出到文件中
    cout << 0 << endl;        //将通关记录输出到文件中
}

bool Player::Check_pw(char pw_t[]) {
    if (strcmp(pw_t, password) == 0)  //对比输入的密码和文件中储存的密码
    {
        return true;  //相同，密码正确返回true
    } else {
        return false;  //不同，密码错误返回false
    }
}

void Player::Record_up() {
    ++New_score;  //本次游戏的记录自增
}

void Player::Final_save() {
    if (freopen(player_name, "w", stdout) == NULL)  //打开用户文件
    {
        MessageBox(getHWnd(), TEXT("failed to save the data"), TEXT("hint"), MB_OK);
    }
    cout << password << endl;  //输入密码
    if (New_score > Record)    //检查是否破记录
    {
        Record = New_score;
        new_record = true;
    }
    final_record = Record;
    cout << Record << endl;  //输入新纪录
    fclose(stdout);          //关闭文件
}

void mouse_click()  //用于检测鼠标点击
{
    flushmouse();
    int color = 0;
    mouse_msg mouse_click_mouse;
    for (; is_run(); delay_fps(800), ++color) {
        if (mousemsg())  //检查是否有鼠标消息
        {
            mouse_click_mouse = getmouse();  //产生鼠标消息则检查是否为点击
            if (mouse_click_mouse.is_down()) {
                return;
            }
        }
        if (color < 200)  //做出动图的效果
        {
            setcolor(DARKGRAY);
            xyprintf(190, 300, "谢谢游玩！");
        } else if (color < 400) {
            setcolor(BLACK);
            xyprintf(190, 300, "谢谢游玩！");
        } else {
            color = 0;
        }
    }
}

void Start_interface() {
    PIMAGE P_Start = newimage();         //创建图片变量
    getimage(P_Start, "Pic_Start.jpg");  //获取图片文件
    putimage(0, 0, P_Start);             //在窗口上覆盖图片
    delimage(P_Start);                   //销毁图片变量，防止内存泄露

    setfontbkcolor(WHITE);             //设置字体背景颜色
    setfont(100, 0, "楷体");           //设置字体属性
    setcolor(DARKGRAY);                //设置字体颜色
    xyprintf(160, 170, "24点小游戏");  //在窗口上打印文字
    setfillcolor(WHITE);               //设置填充颜色
    bar(270, 340, 530, 420);           //用实心矩形覆盖掉背景防止看不清按钮
    setlinewidth(3.0);                 //设置绘图线宽
    setcolor(LIGHTGRAY);               //设置绘图颜色
    rectangle(275, 345, 525, 415);     //绘制按钮边框
    setcolor(BLACK);                   //设置字体颜色
    setfont(60, 0, "楷体");            //更改字体大小
    xyprintf(280, 350, "开始游戏");    //在窗口上打印文字

    flushmouse();
    mouse_msg Start_click_mouse;
    bool push = false;
    for (; is_run(); delay_fps(800)) {
        if (mousemsg())  //检查鼠标消息
        {
            Start_click_mouse = getmouse();                                                                                                                                                            //获取鼠标消息
            if (Start_click_mouse.is_down() && Start_click_mouse.is_left() && Start_click_mouse.x >= 275 && Start_click_mouse.x <= 525 && Start_click_mouse.y >= 345 && Start_click_mouse.y <= 415) {  //若是左键点击且按键位置在按钮处则绘制出“按下”的效果
                setlinewidth(2.0);
                setcolor(DARKGRAY);
                rectangle(275, 345, 525, 415);
                push = true;                               //此时状态为鼠标按下了按钮
            } else if (push && Start_click_mouse.is_up())  //如果是松开了按键且此前鼠标是在按钮处按下的
            {
                if (Start_click_mouse.x >= 275 && Start_click_mouse.x <= 525 && Start_click_mouse.y >= 345 && Start_click_mouse.y <= 415) {
                    return;  //如果松开时鼠标仍在按钮上方则开始游戏，离开初始界面，返回主函数
                }
                setlinewidth(2.0);  //否则将按钮恢复为未按下的状态
                setcolor(LIGHTGRAY);
                rectangle(275, 345, 525, 415);
                push = false;
            }
        }
    }
}

void click_draw(int a, int b, int c, int d) {
    setlinewidth(4.0);      //设置线宽
    setcolor(DARKGRAY);     //设置颜色
    rectangle(a, b, c, d);  //画一个矩形
}

bool num_check(char s[], int a, int b, int c, int d) {
    int len = strlen(s), num = 0;  //获取字符串长度
    int a1[15] = {0};              //记录生成的四个数字
    int a2[15] = {0};              //检查输入的数字
    ++a1[a], ++a1[b], ++a1[c], ++a1[d];
    for (int i = 0; i < len; ++i) {
        if (s[i] >= '0' && s[i] <= '9')  //获取输入的数字
        {
            num = num * 10 + s[i] - 48;
            if (num > 13)  //数字最多13超过13一定是错误数字
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
    for (int i = 1; i <= 13; ++i)  //对比
    {
        if (a1[i] != a2[i]) {
            return false;
        }
    }
    return true;
}

int check(char ch) {
    if (ch == '(')  //返回各运算符号对应的优先级
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
    double a, b;  //后缀表达式计算
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
    stack<char> a;  //中缀表达式转换为后缀表达式
    int flag = 0, f = 1;
    double num = 0;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        int level = check(s[i]);
        if (level == -1) {
            return 1e9;
        } else if (level == 4)  //按照对应的优先级进行对应的操作
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
    rectangle(450, 20, 550, 60);  //绘制方框
    rectangle(580, 20, 680, 60);
    rectangle(710, 20, 840, 60);
    rectangle(710, 70, 840, 110);
    rectangle(630, 295, 700, 335);
    setfont(30, 0, "宋体");
    setcolor(BLACK);
    setbkmode(TRANSPARENT);
    xyprintf(455, 25, "上一首");  //在方框内打印文字，形成按钮
    xyprintf(585, 25, "下一首");
    xyprintf(715, 25, "停止播放");
    xyprintf(715, 75, "结束游戏");
    xyprintf(635, 300, "提交");
    setfont(20, 0, "楷体");
    xyprintf(180, 335, "若无法得出结果，在输入框中输入0来跳过本题");
    xyprintf(180, 370, "若能得出结果则在输入框中输入计算式");

    int cnt = 1;  //歌曲编号
    bool music_click = false, push = false;
    mouse_msg game_interface_msg;
    if (mciSendString((TEXT("open Music_bgm_1.mp3 alias GAMEBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
    {                                                                                //不成功则弹窗提示并结束程序
        MessageBox(getHWnd(), TEXT("can't find Music_Start.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
    }
    mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //调整音量
    mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //开始播放

    sys_edit num_1, num_2, num_3, num_4, answer;
    num_1.create(false), num_2.create(false), num_3.create(false), num_4.create(false);                                      //创建单行输入框
    num_1.move(100, 160), num_2.move(300, 160), num_3.move(500, 160), num_4.move(700, 160);                                  //设置位置
    num_1.size(40, 40), num_2.size(40, 40), num_3.size(40, 40), num_4.size(40, 40);                                          //设置输入框大小
    num_1.setmaxlen(2), num_2.setmaxlen(2), num_3.setmaxlen(2), num_4.setmaxlen(2);                                          //设置输入框可键入的字符最大值
    num_1.setbgcolor(WHITE), num_2.setbgcolor(WHITE), num_3.setbgcolor(WHITE), num_4.setbgcolor(WHITE);                      //设置输入框内背景色
    num_1.setfont(30, 0, "宋体"), num_2.setfont(30, 0, "宋体"), num_3.setfont(30, 0, "宋体"), num_4.setfont(30, 0, "宋体");  //设置输入框内文字
    num_1.setreadonly(true), num_2.setreadonly(true), num_3.setreadonly(true), num_4.setreadonly(true);                      //设置输入框不可输入，即只读状态
    num_1.visible(true), num_2.visible(true), num_3.visible(true), num_4.visible(true);                                      //设置输入框可见

    answer.create(false);
    answer.move(180, 300);
    answer.size(420, 30);
    answer.setmaxlen(20);
    answer.setbgcolor(WHITE);
    answer.setfont(20, 0, "宋体");
    answer.visible(true);

    char temp_num[5][5] = {0};
    int a = rand() % 13 + 1;  //获取四个随机数
    int b = rand() % 13 + 1;
    int c = rand() % 13 + 1;
    int d = rand() % 13 + 1;
    int x1, x2, y1, y2;

game:
    while (mark[a][b][c][d])  //若这四个随机数组合出现过，则重新获取随机数
    {
        a = rand() % 13 + 1;
        b = rand() % 13 + 1;
        c = rand() % 13 + 1;
        d = rand() % 13 + 1;
    }
    mark[a][b][c][d] = mark[a][b][d][c] = true;  //这些组合出现过了，标记
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
    num_1.settext(temp_num[1]);  //将数字放入输入框
    num_2.settext(temp_num[2]);
    num_3.settext(temp_num[3]);
    num_4.settext(temp_num[4]);
    answer.settext("");  //清空答案输入框
    answer.setfocus();
    for (; is_run(); delay_fps(1000)) {
        if (mousemsg()) {
            game_interface_msg = getmouse();
            if (game_interface_msg.is_down() && game_interface_msg.is_left())  //检测鼠标按下
            {
                if (game_interface_msg.x >= 450 && game_interface_msg.x <= 550 && game_interface_msg.y >= 20 && game_interface_msg.y <= 60)  //上一首
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(450, 20, 550, 60);  //绘制出“按下按钮”的效果
                    x1 = 450, y1 = 20, x2 = 550, y2 = 60, push = true;
                    if (--cnt < 1)  //歌曲编号变化
                        cnt += 5;
                    mciSendString(TEXT("close GAMEBGM"), NULL, 0, NULL);
                    music_click = true;                                                                                                             //说明按下的是切换歌曲的按钮
                } else if (game_interface_msg.x >= 580 && game_interface_msg.x <= 680 && game_interface_msg.y >= 20 && game_interface_msg.y <= 60)  //下一首
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(580, 20, 680, 60);  //绘制出“按下按钮”的效果
                    x1 = 580, y1 = 20, x2 = 680, y2 = 60, push = true;
                    if (++cnt > 5)  //歌曲编号变化
                    {
                        cnt -= 5;
                    }
                    mciSendString(TEXT("close GAMEBGM"), NULL, 0, NULL);
                    music_click = true;                                                                                                             //说明按下的是切换歌曲的按钮
                } else if (game_interface_msg.x >= 710 && game_interface_msg.x <= 840 && game_interface_msg.y >= 20 && game_interface_msg.y <= 60)  //停止/开始播放
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(710, 20, 840, 60);  //绘制出“按下按钮”的效果
                    x1 = 710, y1 = 20, x2 = 840, y2 = 60, push = true;
                    if (music_play)  //说明此时音乐正在播放
                    {
                        setfillcolor(EGEARGB(255, 153, 204, 255));
                        ege_fillrect(715, 25, 60, 30);
                        setfont(30, 0, "宋体");
                        xyprintf(715, 25, "开始");                            //将按钮改为开始播放，以等待下一次点击
                        music_play = false;                                   //变更音乐播放状态
                        mciSendString(TEXT("close GAMEBGM"), NULL, 0, NULL);  //停止播放歌曲
                    } else {
                        setfillcolor(EGEARGB(255, 153, 204, 255));
                        ege_fillrect(715, 25, 60, 30);
                        setfont(30, 0, "宋体");
                        xyprintf(715, 25, "停止");  //将按钮改为停止播放，以等待下一次点击
                        music_click = true;         //标记状态
                    }
                } else if (game_interface_msg.x >= 710 && game_interface_msg.x <= 840 && game_interface_msg.y >= 70 && game_interface_msg.y <= 110)  //结束游戏
                {
                    if (MessageBox(getHWnd(), TEXT("是否退出游戏？"), TEXT("hint"), MB_OKCANCEL) == 1) {
                        return;
                    }
                } else if (game_interface_msg.x <= 700 && game_interface_msg.x >= 30 && game_interface_msg.y <= 335 && game_interface_msg.y >= 295)  //提交
                {
                    setlinewidth(4.0);
                    setcolor(BLACK);
                    rectangle(630, 295, 700, 335);
                    x1 = 630, y1 = 295, x2 = 700, y2 = 335, push = true;

                    char temp_player_ans[50] = {0};  //获取答案（中缀表达式）
                    bool wrong_ans = false, wrong_num = false, wrong_ch = false;
                    answer.gettext(20, temp_player_ans);
                    if (strcmp(temp_player_ans, "0") == 0)  //玩家选择跳过
                    {
                        goto game;
                    } else if (strcmp(temp_player_ans, "") == 0)  //玩家提交了空答案
                    {
                        wrong_ans = true;
                    } else  //玩家输入了中缀表达式
                    {
                        if (!num_check(temp_player_ans, a, b, c, d))  //检测数字输入是否正确
                        {
                            wrong_num = true;
                        } else  //正确则进入计算
                        {
                            double cal_ans = cal(temp_player_ans);
                            if (cal_ans == 1e9) {
                                wrong_ch = true;
                            } else if (cal_ans > 24.00001 || cal_ans < 23.999999)  //防止浮点数的掉精度误差
                            {
                                wrong_ans = true;
                            }
                        }
                    }
                    if (wrong_ans)  //答案错误
                    {
                        MessageBox(getHWnd(), TEXT("答案错误！"), TEXT("hint"), MB_OK);
                        wrong_ans = false;
                    } else if (wrong_num)  //输入的数字不正确
                    {
                        MessageBox(getHWnd(), TEXT("数字输入有误！"), TEXT("hint"), MB_OK);
                        wrong_num = false;
                    } else if (wrong_ch)  //输入的符号不正确
                    {
                        MessageBox(getHWnd(), TEXT("只能输入‘+’、‘-’、‘*’、‘/’、‘(’、‘)’这几种符号！"), TEXT("hint"), MB_OK);
                        wrong_ch = false;
                    } else  //答案正确
                    {
                        MessageBox(getHWnd(), TEXT("答案正确！"), TEXT("hint"), MB_OK);
                        player.Record_up();
                        goto game;  //回到游戏开始新的一局
                    }
                }
                if (music_click) {
                    music_click = false, music_play = true;
                    switch (cnt) {
                        case 1:
                            if (mciSendString((TEXT("open Music_bgm_1.mp3 alias GAMEBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
                            {                                                                                //不成功则弹窗提示并结束程序
                                MessageBox(getHWnd(), TEXT("can't findMusic_bgm_1.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //调整音量
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //开始播放
                            break;
                        case 2:
                            if (mciSendString((TEXT("open Music_bgm_2.mp3 alias GAMEBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
                            {                                                                                //不成功则弹窗提示并结束程序
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_2.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //调整音量
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //开始播放
                            break;
                        case 3:
                            if (mciSendString((TEXT("open Music_bgm_3.mp3 alias GAMEBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
                            {                                                                                //不成功则弹窗提示并结束程序
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_3.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //调整音量
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //开始播放
                            break;
                        case 4:
                            if (mciSendString((TEXT("open Music_bgm_4.mp3 alias GAMEBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
                            {                                                                                //不成功则弹窗提示并结束程序
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_4.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //调整音量
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //开始播放
                            break;
                        case 5:
                            if (mciSendString((TEXT("open Music_bgm_5.mp3 alias GAMEBGM")), NULL, 0, NULL))  //判断是否成功打开了音乐
                            {                                                                                //不成功则弹窗提示并结束程序
                                MessageBox(getHWnd(), TEXT("can't find Music_bgm_5.mp3"), TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
                            }
                            mciSendString(TEXT("setaudio GAMEBGM volume to 80"), NULL, 0, NULL);  //调整音量
                            mciSendString(TEXT("play GAMEBGM repeat"), NULL, 0, NULL);            //开始播放
                            break;
                    }
                }
            } else if (push && game_interface_msg.is_up())  //绘制出按下按钮的效果
            {
                push = false;
                click_draw(x1, y1, x2, y2);
            }
        }
    }
}

void end_game() {
    cleardevice();
    setfont(30, 0, "宋体");
    setcolor(BLACK);
    setfont(30, 0, "宋体");
    setcolor(BLACK);
    setbkmode(TRANSPARENT);
    if (true)
        xyprintf(120, 150, "恭喜！您打破了之前的的通关纪录！");  //根据需要打印文字
    xyprintf(120, 200, "您的最高纪录是%d关！", 1);
    setfont(20, 0, "宋体");
    setcolor(DARKGRAY);
    xyprintf(10, 420, "鼠标点击继续");
    setfont(100, 0, "宋体");
    setcolor(DARKGRAY);
    xyprintf(190, 300, "谢谢游玩！");
    mouse_click();
}