#include "myheader.h"

int dire[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
int pwCnt = 0;

void login(sys_edit &nameInput) {
    key_msg keyMsg;
    int nameLen;
    bool nameFlag = false, pwFlag = false;
    char pw[20];
    for (; is_run(); delay_fps(60)) {  // 管理员登录操作和普通玩家进入游戏
        while (kbmsg()) {
            keyMsg = getkey();
            if (keyMsg.msg == key_msg_down && keyMsg.key == key_enter) {  // 捕捉到enter输入
                nameInput.gettext(10, player.name);                       // 获取输入框内容
                nameLen = strlen(player.name);                            // 获取姓名长度
                nameFlag = true;                                          // 标记是否键入姓名
                if (strcmp(player.name, "Suolk") == 0) {
                    if (MessageBox(0, TEXT("确定要以管理员身份登录吗?"), TEXT("hint"), 1) != 1) {  // 验证管理员登录
                        nameInput.settext("请输入密码");                                           // 键入文本以显示作用
                        nameInput.setfocus();                                                      // 设置光标
                        flushkey();                                                                // 清空键盘信息
                        for (; is_run(); delay_fps(60)) {                                          // 循环频率为每秒60次
                            char tmpStr[40] = {0};                                                 // 中转用字符串数组
                            keyMsg = getkey();                                                     // 获取键盘信息
                            if (keyMsg.msg == key_msg_down && keyMsg.key == key_enter) {           // 键入enter
                                pwCnt++;                                                           // 密码输入次数+1
                                nameInput.gettext(10, pw);                                         // 获取输入框内容
                                if (strcmp(pw, "tgb951") == 0) {                                   // 检测密码是否正确
                                    MessageBox(0, TEXT("密码正确！"), TEXT("hint"), 0);
                                    pwFlag = true;        // 密码正确
                                    player.admin = true;  // 管理员登陆状态修改为已登录
                                } else if (pwCnt < 3) {   // 密码错误但还有输入机会
                                    sprintf(tmpStr, "密码错误！你还有%d次机会", 3 - pwCnt);
                                    nameInput.settext("");
                                    nameInput.setfocus();
                                    MessageBox(0, tmpStr, TEXT("hint"), 0);
                                } else {                         // 密码错误且达上限
                                    pwFlag = false;              // 密码不正确
                                    nameInput.settext("Suolk");  // 将姓名重新置入输入框
                                    break;
                                }
                                flushkey();
                                memset(pw, 0, sizeof(pw));  // 清空内容防止密码检测出错
                            }
                            if (pwFlag || pwCnt == 3) break;  // 密码正确或输入错误次数三次则离开密码的输入循环W
                        }
                    }
                }
                if (pwFlag) break;   // 管理员已登录
                if (nameLen == 0) {  // 普通玩家进入游戏，但是玩家没有输名字
                    MessageBox(0, TEXT("请输入名称！"), TEXT("hint"), 0);
                    nameInput.setfocus();
                    nameFlag = false;
                } else if (MessageBox(0, TEXT("确定以这个名称开始游戏吗"), TEXT("hint"), 1) != 1)
                    nameFlag = false;
            }
        }
        if (nameFlag) break;  // 说明已键入名称
    }
}

void create_maze() {
    for (int i = 0; i <= MazeSize; i++)  // 先将迷宫全部初始化为墙
        for (int j = 0; j <= MazeSize; j++)
            maze[i][j] = Wall;
    for (int i = 1; i < MazeSize; i += 2)  // 再将必需的空格填入
        for (int j = 1; j < MazeSize; j += 2)
            maze[i][j] = Blank;

    std::vector<int> randomSequence;
    for (int i = 0; i < WallNum; i++)  // 设置一个vector数组用于生成随机序列
        randomSequence.push_back(i);
    shuffle(randomSequence.begin(), randomSequence.end(), std::default_random_engine(time(NULL)));  // 生成一个随机序列用于随机拆墙生成迷宫

    merge_find bcj(MazeBlank * MazeBlank);  // 调用一个并查集类
    for (int i = 0; i < WallNum; i++) {
        if (bcj.get_cnt() <= 1)  // 不多于1个集合说明道路已全部联通，即迷宫已完成
            break;

        int num, Horizon = 0, Vertical = 0, randWallNum, xWall, yWall;
        if (randomSequence[i] < WallNum / 2) {  // 序列中编号为0~Wall_Num/2-1的为横向墙
            randWallNum = randomSequence[i];
            Horizon = 1;
            num = MazeBlank - 1;
        } else {  // 序列中编号为Wall_Num/2~Wall_Num的为纵向墙
            randWallNum = randomSequence[i] - WallNum / 2;
            Vertical = 1;
            num = MazeBlank;
        }
        xWall = randWallNum % num;
        yWall = randWallNum / num;

        int x1 = xWall, y1 = yWall, x2 = xWall + Horizon, y2 = yWall + Vertical;
        if (bcj.merge(y1 * MazeBlank + x1, y2 * MazeBlank + x2)) {  // 合并了集合说明需要拆墙
            maze[yWall + y2 + 1][xWall + x2 + 1] = Blank;
        }
    }
}

bool find_path(int x, int y) {
    if (maze[x][y] == Blank) {                       // 检测到空格
        maze[x][y] = Path;                           // 先标记为路线
        if (x == MazeSize - 2 && y == MazeSize - 2)  // 到达出口，说明路线正确，返回真
            return true;
        for (int i = 0; i < 4; i++)  // dire数组分别代表了四个方向
            if (find_path(x + dire[i][0], y + dire[i][1]))
                return true;  // 路线正确，返回真
        maze[x][y] = Blank;   // 若四个方向都没有正确路线则说明这一格不是出迷宫的必经之路，重新标记为空格
    }
    return false;  // 进行到这一步说明遇到了路线不正确，返回假
}

void boss_pos() {
    int pathLen = 0;
    // 计算离开迷宫的最短路线长度
    for (int i = 0; i < MazeSize; i++)
        for (int j = 0; j < MazeSize; j++)
            if (maze[i][j] == Path)
                pathLen++;
    // 通过随机数和路线长度计算boss在路线上的第几格
    int pos1 = rand() % pathLen + 2, pos2;
    do {
        pos2 = rand() % pathLen + 2;
    } while (pos1 == pos2);     // 确保boss的位置不重合
    int cnt = 1, x = 1, y = 1;  // pos1和pos2代表Boss在路线的第几格
    while (cnt <= pos1 || cnt <= pos2) {
        for (int i = 0; i < 4; ++i) {
            if (maze[x + dire[i][0]][y + dire[i][1]] == Path) {
                x = x + dire[i][0], y = y + dire[i][1], ++cnt;
                break;
            }
        }
        if (cnt == pos1) maze[x][y] = Boss;  // 放置boss1
        if (cnt == pos2) maze[x][y] = Boss;  // 放置boss2
        if (cnt == pathLen + 2) {
            maze[x][y] = Boss;  // 放置最后一个boss
            return;
        }
    }
}

void mouse_click() {
    mouse_msg tmpMsg;                              // 定义鼠标类变量
    for (; is_run(); delay_fps(180)) {             // 循环频率为每秒180次
        tmpMsg = getmouse();                       // 获取鼠标信息
        if (tmpMsg.is_down() && tmpMsg.is_left())  // 检测到鼠标左键点击
            break;
    }
}

void game_maze() {
    PIMAGE tmpPic, bigPlayer, miniPlayer, boss;  // 定义图像指针，分别为玩家形象（小），boss形象
    bigPlayer = newimage(), miniPlayer = newimage(14, 14);
    getimage(bigPlayer, "src/img/player.png");                      // 获取玩家形象（大）
    putimage(miniPlayer, 0, 0, 14, 14, bigPlayer, 0, 0, 203, 197);  // 缩小玩家形象

    PIMAGE talk = newimage(696, 218);    // 中转用图片，对话框，背景
    key_msg keyMsg;                      // 定义键盘类
    int cnt = 0;                         // 用于记录第几个boss
    putimage(164, 94, miniPlayer);       // 在迷宫入口放置小玩家
    getimage(talk, "src/img/talk.png");  // 获取对话框图片文件
    startTime = fclock();                // 开始计时
    flushkey();                          // 清空键盘信息
    flushmouse();                        // 清空鼠标信息
    HANDLE h1;
    h1 = CreateThread(NULL, 0, MouseControl, NULL, 0, NULL);  // 创建线程
    CloseHandle(h1);                                          // 关闭线程句柄
    for (; is_run; delay_fps(180)) {                          // 循环频率1秒180次
        while (kbmsg()) {                                     // 检测到键盘信息产生
            keyMsg = getkey();                                // 获取键盘信息
            if (keyMsg.msg == key_msg_down) {
                if (player_move(keyMsg))  // 在更新了的玩家位置上画出玩家形象
                    putimage(150 + player.y * 14, 80 + player.x * 14, miniPlayer);
                if (maze[player.x][player.y] == Boss) {                      // 遇到了boss
                    cnt++, maze[player.x][player.y] = Blank;                 // 将boss改为空格，防止有人闲的没事干倒退回来
                    tmpPic = newimage();                                     // 创建图片大小
                    getimage(tmpPic, 0, 0, windowX, windowY);                // 获取当前窗口内容
                    if (cnt == 1) {                                          // 遇到第一个boss
                        boss = newimage();                                   // 创建图片大小
                        getimage(boss, "src/img/boss1.png");                 // 获取一号boss形象
                        cleardevice();                                       // 铺上背景
                        setcolor(WHITE);                                     // 设置字体颜色白色
                        setfont(20, 0, "宋体");                              // 设置字体大小
                        setbkmode(TRANSPARENT);                              // 设置字体背景透明
                        putimage(450, 100, bigPlayer);                       // 放置玩家形象（大）
                        putimage(10, 355, talk);                             // 放置对话框
                        xyprintf(60, 375, player.name);                      // 打印玩家姓名
                        xyprintf(60, 410, "啊迷宫里好黑，好像撞到了什么…");  // 打印对话内容
                        mouse_click();                                       // 检测鼠标点击
                        putimage(100, 100, boss);                            // 放置boss形象
                        putimage(10, 355, talk);                             // 再次放置对话框以覆盖掉前面打印的文字
                        mouse_click();                                       // 下同
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "很凶的猛虎");
                        xyprintf(60, 410, "什么人敢挡我的路！已经做好被我吃掉了的准备了吗！");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "咦，这，这是小猫？好可爱");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "很凶的猛虎");
                        xyprintf(60, 410, "你在胡说什么！看不见我头上的王字吗！");
                        xyprintf(60, 435, "我可是猛虎大人！");
                        xyprintf(60, 460, "竟然敢说我是猫！决斗吧你这只可恶的小黄鸡！");
                        mouse_click();
                        cleardevice();
                        xyprintf(100, 205, "完成数独来击败对手吧");
                        mouse_click();
                        cleardevice();      // 清屏
                        srand(time(NULL));  // 引入时间作为随机数种子

                        sudo_game();  // 生成数独

                        if (out)     // 玩家在数独中选择了退出游戏
                            return;  // 离开函数
                        cleardevice();
                        putimage(450, 100, bigPlayer);     // 放置玩家形象（大）
                        putimage(100, 100, boss);          // 放置boss
                        putimage(10, 355, talk);           // 放置对话框
                        xyprintf(100, 375, "很凶的猛虎");  // 打印对话与名称
                        xyprintf(60, 410, "这不可能…");
                        xyprintf(60, 435, "无敌的猛虎大人居然输给了一只鸡…");
                        xyprintf(60, 460, "呜哇啊啊啊啊啊啊啊");
                        mouse_click();  // 检测鼠标点击
                        cleardevice();
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "啊…跑掉了…");
                        xyprintf(60, 435, "还想rua一下尾巴的…");
                        mouse_click();           // 检测到点击
                        delimage(boss);          // 销毁图片释放内存
                        putimage(0, 0, tmpPic);  // 放置此前截屏的画面，做出回到迷宫游戏界面的效果
                        delimage(tmpPic);
                        tmpPic = boss = NULL;                 // 防止再调用该指针指向其他图片时出错，手动让其指向NULL
                    } else if (cnt == 2) {                    // 第二个boss
                        boss = newimage(186, 161);            // 创建图片大小
                        getimage(boss, "src/img/boss2.png");  // 获取第二个boss的形象，其余操作均同上
                        cleardevice();
                        setcolor(WHITE);
                        setfont(20, 0, "宋体");
                        setbkmode(TRANSPARENT);
                        putimage(100, 100, boss);
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "啊咧？海豚原来可以在地上生存吗");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "大概是蛇");
                        xyprintf(60, 410, "本大爷是蛇啊！！！！是蛇！！！！");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "啊，可是……");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "是蛇呢");
                        xyprintf(60, 410, "你在质疑本大爷吗！！！！！");
                        xyprintf(60, 435, "居然把本和那种水生动物搞混！！！！");
                        xyprintf(60, 460, "本大爷可是爬行动物啊你这个呆头鹅！！！！");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "不…我也不是鹅啊…");
                        mouse_click();
                        srand(time(NULL));

                        sudo_game();  // 生成数独

                        if (out)
                            return;
                        cleardevice();
                        putimage(450, 100, bigPlayer);
                        putimage(100, 100, boss);
                        putimage(10, 355, talk);
                        xyprintf(100, 375, "一条受到打击的蛇");
                        xyprintf(60, 410, "本大爷…本大爷居然…");
                        xyprintf(60, 435, "败给了一只鸭子？！！！一只水生生物？！！！");
                        xyprintf(60, 460, "这是爬行动物的耻辱！！！！");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(60, 375, player.name);
                        xyprintf(60, 410, "虽然但是…我看起来真的那么像水上生物吗");
                        mouse_click();
                        delimage(boss);
                        putimage(0, 0, tmpPic);
                        delimage(tmpPic);
                        boss = tmpPic = NULL;
                    } else if (cnt == 3) {                    // 最后一个boss
                        boss = newimage(225, 200);            // 创建图片大小
                        getimage(boss, "src/img/boss3.png");  // 获取最终boss图片大小，其余操作均同上
                        cleardevice();
                        setcolor(WHITE);
                        setfont(20, 0, "宋体");
                        setbkmode(TRANSPARENT);
                        putimage(100, 100, boss);
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, "一只无辜的小白兔");
                        xyprintf(60, 410, "为什么一定要回去呢，留在这个世界不好吗……");
                        xyprintf(60, 435, "难道你想回到那个充满了烦恼的现实世界吗");
                        mouse_click();
                        if (MessageBox(0, TEXT("要留下来吗"), TEXT("hint"), 1) != 1) {  // 这里是一个小分支
                            player.stay = true;                                         // 如果选择留下则特殊成就开关标记为达成
                            putimage(10, 355, talk);                                    // 放置对话框和打印文字
                            xyprintf(60, 375, "一只快乐的小白兔");
                            xyprintf(60, 410, "（小白兔没有说话因为它很快乐）");
                            delimage(boss);
                            delimage(tmpPic);
                            boss = tmpPic = NULL;
                            mouse_click();
                            return;  // 离开迷宫游戏
                        }
                        putimage(10, 355, talk);  // 这是正常游戏流程
                        xyprintf(60, 375, "一只不太妙的小白兔");
                        xyprintf(60, 410, "果然人类都……");
                        mouse_click();
                        xyprintf(60, 435, "既然你不愿意留下来，那只好由我来帮你了");
                        mouse_click();
                        putimage(10, 355, talk);
                        xyprintf(100, 375, player.name);
                        xyprintf(60, 410, "什——你怎么知道我是人类？");
                        mouse_click();
                        srand(time(NULL));

                        sudo_game();  // 生成数独

                        if (out)
                            return;
                        putimage(100, 100, boss);
                        putimage(450, 100, bigPlayer);
                        putimage(10, 355, talk);
                        xyprintf(60, 375, "一只难过的小白兔");
                        xyprintf(60, 410, "结果还是没有人愿意留下来陪我们……");
                        mouse_click();
                        delimage(boss);
                        delimage(tmpPic);
                        boss = tmpPic = NULL;
                        return;  // 游戏结束
                    }
                }
            }
        }
        if (out)
            return;
    }
}

bool cheat_judge() {
    if (player.admin)  // 已登录管理员
        return true;
    else if (pwCnt < 3 && !player.admin && strcmp(player.name, "Suolk") == 0) {                       // 登录用户名为管理员用户名且密码错误少于三次
        if (MessageBox(0, TEXT("检测到您还未登录管理员身份，是否现在登录"), TEXT("hint"), 1) != 1) {  // 选择登录管理员身份则生成密码输入框
            sys_edit pwEdit;                                                                          // 定义一个输入框类
            key_msg keyMsg;                                                                           // 定义一个键盘类
            char pwStr[20] = {0};                                                                     // 定义一个字符串数组储存输入的密码
            bool pwFlag = false;                                                                      // 定义一个“开关”用于判断输入密码是否正确，初始化为不正确
            init_input(pwEdit, 440, 10, 150);
            flushkey();                                                          // 清空此前的键盘信息
            for (; is_run(); delay_fps(60)) {                                    // 设置循环，循环频率是每秒60次
                char tmpStr[50] = {0};                                           // 定义字符串数组用于写入内容不确定的错误提示
                keyMsg = getkey();                                               // 获取键盘信息
                if (keyMsg.msg == key_msg_down && keyMsg.key == key_enter) {     // 检测到键入enter键
                    pwCnt++;                                                     // 密码输入次数+1
                    pwEdit.gettext(10, pwStr);                                   // 获取输入在输入框内的文本
                    if (strcmp(pwStr, "tgb951") == 0) {                          // 检测是否输入正确
                        MessageBox(0, TEXT("密码正确！"), TEXT("hint"), 0);      // 输入正确则弹出该提示框
                        pwFlag = true;                                           // 将开关状态标记为正确
                        player.admin = true;                                     // 将代表是否登录的全局变量标记为真
                    } else if (pwCnt < 3) {                                      // 密码输入错误，但是密码输入次数还有剩
                        sprintf(tmpStr, "密码错误！你还有%d次机会", 3 - pwCnt);  // 将错误提示写入hint数组
                        pwEdit.settext("");                                      // 清空输入框内容
                        pwEdit.setfocus();                                       // 放置光标
                        MessageBox(0, tmpStr, TEXT("hint"), 0);                  // 弹出提示框
                    }
                    flushkey();                       // 清空键盘信息防止下次检测检测到无用的键盘信息
                    memset(pwStr, 0, sizeof(pwStr));  // 清空储存输入密码的字符串数组，防止下次检测时混入额外字符导致检测出错
                } else if (pwFlag)
                    return true;               // 如果密码输入正确，则放回真
                if (pwCnt >= 3) return false;  // 如果密码输入次数达到上限且没有正确过，返回假
            }
        } else {                                                         // 选择不登录管理员身份
            MessageBox(0, TEXT("平民玩家还想作弊？"), TEXT("hint"), 0);  // 弹出错误提示
            return false;
        }
    } else if (pwCnt == 3) {  // 在此之前密码错误次数已经达到了三次
        MessageBox(0, TEXT("密码输入错误次数已达上限！无法登录！"), TEXT("hint"), 0);
        return false;
    } else {  // 非管理员
        MessageBox(0, TEXT("平民玩家还想作弊？"), TEXT("hint"), 0);
        return false;
    }
}

bool exit_game() {
    if (MessageBox(0, TEXT("确定退出游戏？"), TEXT("hint"), 1) != 1) {
        out = true;  // 将开关状态修改为退出
        return true;
    }
    return false;
}
