#include "myheader.h"

void play_music(const char musicName[], bool repeat) {
    if (mciSendString(musicName, NULL, 0, NULL)) {  // 判断是否成功打开音乐文件
        MessageBox(0, TEXT("OPEN BGM FAILED"), TEXT("hint"), 0);
        exit(0);
    }
    if (repeat)
        mciSendString(TEXT("play BGM repeat"), NULL, 0, NULL);  // 循环播放音乐
    else
        mciSendString(TEXT("play BGM"), NULL, 0, NULL);  // 播放音乐
}
void close_music() { mciSendString(TEXT("close BGM"), NULL, 0, NULL); }

void init_input(sys_edit& tmpInput, int posX, int posY, int sizew, int bgColor, int color) {
    tmpInput.create(false);           // 创建单行输入框
    tmpInput.move(posX, posY);        // 设置位置
    tmpInput.size(sizew, 30);         // 设置大小
    tmpInput.setmaxlen(10);           // 设置最大输入长度
    tmpInput.setbgcolor(bgColor);     // 设置输入框颜色
    tmpInput.setcolor(color);         // 设置键入文字颜色
    tmpInput.setfont(20, 0, "宋体");  // 设置字体大小
    tmpInput.visible(true);           // 设置输入框可见
    tmpInput.setfocus();              // 设置光标
}

void set_bg_pic(PIMAGE& tempPic, const char s[], int picW, int picH) {
    PIMAGE oriPic = newimage();
    getimage(oriPic, s);             // 获取原图
    tempPic = newimage(picW, picH);  // 定义新的图片指针
    ege_enable_aa(true, tempPic);    // 抗锯齿
    putimage(tempPic, 0, 0, picW, picH,
             oriPic, 0, 0, getwidth(oriPic), getheight(oriPic));  // 缩放图片适应窗口
    putimage(0, 0, tempPic);                                      // 放置背景
    delimage(tempPic), delimage(oriPic);                          // 销毁图片释放内存
    tempPic = NULL;
}

void draw_button() {
    rectangle(600, 10, 720, 60);  // 画一个空心的长方形
    rectangle(10, 10, 190, 60);
    setfont(40, 0, "宋体");        // 设置字体大小
    setbkmode(TRANSPARENT);        // 将文字背景设置为透明
    xyprintf(20, 15, "退出游戏");  // 键入文字
    xyprintf(610, 15, "CHEAT");
    setfont(20, 0, "宋体");  // 恢复成常用的字体大小
}

void draw_maze() {
    setbkcolor(EGERGB(30, 30, 30));  // 设置背景颜色
    draw_button();                   // 调用绘制按钮函数
    setlinewidth(2.0);               // 设置线宽为2.0
    setcolor(WHITE);                 // 设置线为白色

    for (int i = 0; i < MazeSize; i += 2)
        for (int j = 1; j < MazeSize; j += 2)
            if (maze[i][j] == Wall)
                line(160 + (j - 1) * 14, 85 + i * 14, 160 + 14 * (j + 1), 85 + 14 * i);  // 画横线
    for (int i = 1; i < MazeSize; i += 2) {
        for (int j = 0; j < MazeSize; j += 2) {
            if ((i == 1 && j == 0) || (i == 33 && j == 34))  // 限定出入口在左上角和右下角因此这两个位置不画线
                continue;
            else if (maze[i][j] == Wall)
                line(160 + 14 * j, 85 + 14 * (i - 1), 160 + 14 * j, 85 + 14 * (i + 1));  // 画竖线
        }
    }
}

void story_start() {
    PIMAGE p = newimage(203, 197);                                // 定义图片类型的指针
    getimage(p, "src/img/player.png");                            // 获取玩家形象图片
    setfont(20, 0, "宋体");                                       // 设置字体和大小
    setbkmode(TRANSPARENT);                                       // 将文字背景设置为透明
    setcolor(WHITE);                                              // 设置文字颜色
    xyprintf(100, 205, "点击鼠标来进行剧情,按w、a、s、d来移动");  // 在背景上打印文字
    xyprintf(100, 230, "移动的时候如果没反应，就按shift切换成英文输入法");
    mouse_click();  // 检测鼠标点击
    cleardevice();  // 清屏
    xyprintf(100, 205, "这一天，当你醒过来时，你照了照镜子想看看自己帅气的脸……");
    mouse_click();        // 检测鼠标点击
    putimage(175, 0, p);  // 放置玩家形象图片
    delimage(p);
    p = NULL;
    mouse_click();  // 检测鼠标点击
    xyprintf(100, 230, "?!你变成了一只小鸡！还是Q版小黄鸡!");
    mouse_click();  // 检测鼠标点击
    xyprintf(100, 255, "原来你误入了一个Q版世界，这里所有东西都是Q版的");
    mouse_click();  // 检测鼠标点击
    xyprintf(100, 280, "而想要回去必须通过一个迷宫——");
    mouse_click();  // 检测鼠标点击
    cleardevice();
}

void erase(int x, int y) {
    setfillcolor(BLACK);        // 设置颜色黑色
    bar(x, y, x + 14, y + 14);  // 画一个实心方块
}

void path_draw() {
    setfillcolor(EGERGB(112, 48, 60));  // 路线的颜色是浅蓝色（实际上更类似于紫色）
    for (int i = 1; i < MazeSize; i++) {
        for (int j = 1; j < MazeSize; j++) {
            if (i == player.x && j == player.y)
                continue;
            else if (maze[i][j] == Path)  // 检测到路线则画出一个小方块覆盖当前格
                bar(j * 14 + 150, i * 14 + 80, (j + 1) * 14 + 150, (i + 1) * 14 + 80);
            else if (maze[i][j] == Boss) {  // 检测到boss则更换颜色画一个小方块覆盖当前格
                setfillcolor(RED);          // boss是红色小方块
                bar(j * 14 + 150, i * 14 + 80, (j + 1) * 14 + 150, (i + 1) * 14 + 80);
                setfillcolor(EGERGB(112, 48, 60));  // 恢复颜色为路线颜色
            }
        }
    }
}

void end_game() {
    if (player.stay) {  // 玩家达成特殊结局
        play_music("open src/music/ding.mp3 alias BGM");
        xyprintf(210, 200, "达成特殊结局！");
        xyprintf(210, 235, "“它们实在太可爱了”");
        close_music();
    } else {  // 达成普通结局
        xyprintf(210, 200, "你回到了现实");
        xyprintf(210, 235, "再也没有遇到什么特别的事");
        xyprintf(210, 270, "有时你会想");
        xyprintf(210, 305, "如果当时留下来，是不是会更好呢");
        mouse_click();
        xyprintf(210, 340, "达成结局：回到日常");
    }
    mouse_click();
}

void draw_sudo() {
    cleardevice();  // 清屏
    PIMAGE pic;
    set_bg_pic(pic, "src/img/sdBG.png", 800, 600);  // 放置背景
    sys_edit sudoInput[9][9];                       // 定义输入框数组变量
    mouse_msg mouseMsg;                             // 定义鼠标变量
    bool Wrong_ans = false;                         // 定义用于标记错误答案的开关，初始状态代表没有错误答案
    pic = newimage(364, 364);
    getimage(pic, "src/img/sdGrid.png");  // 获得数独格子图片
    ege_enable_aa(true, pic);
    setcolor(BLACK);          // 设置文字颜色黑色
    putimage(202, 106, pic);  // 放置数独格子图片
    delimage(pic);            // 销毁图片释放内存
    pic = NULL;

    for (int i = 0; i < 9; i++) {  // 对数独矩阵进行设置
        for (int j = 0; j < 9; j++) {
            if (digSudo[i][j])                                           // 说明这个位置应该由程序填入
                xyprintf(218 + 40 * j, 120 + 40 * i, "%d", sudo[i][j]);  // 打印文字
            else                                                         // 说明这位置应该由玩家输入
                init_input(sudoInput[i][j], 210 + 40 * j, 112 + 40 * i, 30, WHITE, BLACK);
        }
    }

    // 绘制几个数独所需的按钮
    rectangle(10, 100, 190, 150);  // 画一个空心的矩形
    rectangle(585, 272, 685, 322);
    setbkmode(TRANSPARENT);         // 设置文字背景透明
    setfont(40, 0, "宋体");         // 设置文字字体
    setcolor(BLACK);                // 设置颜色
    xyprintf(20, 105, "清空输入");  // 打印文字
    xyprintf(600, 277, "提交");
    setfont(20, 0, "宋体");  // 恢复为常用字体
    draw_button();           // 绘制两个通用按钮
    flushmouse();            // 清空鼠标信息

    for (; is_run; delay_fps(180)) {                                             // 检查点击
        while (mousemsg()) {                                                     // 当检测到鼠标信息
            mouseMsg = getmouse();                                               // 获取鼠标信息
            if (mouseMsg.is_down() && mouseMsg.is_left()) {                      // 当检测到鼠标左键按下
                int msgX, msgY;                                                  // 用于储存鼠标点击的坐标
                char num[3] = {0};                                               // 用于储存输入的字符，由于输入的数字是以字符串形式读取，因此建立一个用于中转的数组
                mousepos(&msgX, &msgY);                                          // 获取鼠标点击坐标
                if (msgX >= 585 && msgX <= 685 && msgY >= 272 && msgY <= 322) {  // 点击位置为提交按钮所在范围
                    for (int i = 0; i < 9; i++) {
                        for (int j = 0; j < 9; j++) {
                            if (!digSudo[i][j]) {                 // 该位置应为玩家输入
                                sudoInput[i][j].gettext(2, num);  // 获取玩家输入
                                if (num[0] - 48 != sudo[i][j]) {  // 输入不正确
                                    Wrong_ans = true;             // 修改开关状态为由错误答案
                                    break;                        // 跳出循环
                                }
                            }
                        }
                        if (Wrong_ans)  // 有错误答案则不再检测下一行跳出循环
                            break;
                    }
                    if (Wrong_ans) {
                        MessageBox(0, TEXT("有错误答案！"), TEXT("hint"), 0);  // 弹出提示
                        Wrong_ans = false;                                     // 恢复开关状态以进行下一次检测
                        continue;                                              // 跳过接下来的内容等待下一次提交
                    } else {
                        MessageBox(0, TEXT("恭喜你完成了！"), TEXT("hint"), 0);  // 答案正确
                        return;                                                  // 离开函数
                    }
                } else if (msgX >= 600 && msgX <= 720 && msgY >= 10 && msgY <= 60) {  // 选择作弊，管理员选项
                    if (cheat_judge()) {
                        for (int i = 0; i < 9; i++) {  // 自动填入答案
                            for (int j = 0; j < 9; j++) {
                                if (!digSudo[i][j]) {
                                    char t[1];
                                    t[0] = sudo[i][j] + 48;
                                    sudoInput[i][j].settext(t);  // 直接定义char t在这里会强制转换出错
                                }
                            }
                        }
                    }
                } else if (msgX >= 10 && msgX <= 190 && msgY >= 10 && msgY <= 60) {  // 选择退出
                    if (exit_game()) return;
                } else if (msgX >= 10 && msgX <= 190 && msgY >= 100 && msgY <= 150 && MessageBox(0, TEXT("确定清空输入？"), TEXT("hint"), 1) != 1) {  // 选择清空输入
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