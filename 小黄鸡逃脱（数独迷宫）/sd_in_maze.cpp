#include <time.h>
#include <random>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <graphics.h>
#include <sys_edit.h>
#define Chosen_Len 15       //�����������
#define Maze_Size 17        //�Թ�һ�еĿո���
#define Wall_Num 544        //�Թ�������ж�����ǽ
#define Maze_Max_Num 35     //һ��ǽ�Ϳո������
#pragma comment (lib, "winmm.lib")

bool S_login=false,Exit=false,Path_View=false;                  //��˳��ֱ�Ϊ������Ա��¼����Ƿ�ѡ���˳���Ϸ���Ƿ���ʾ�Թ���·
int password_count=0,X_Player=1,Y_Player=1;                     //��˳��ֱ�Ϊ����Ա�����������������Թ������x��y����
int maze[Maze_Max_Num+1][Maze_Max_Num+1];                       //�Թ�״̬�������
int dire[4][2]={{-1,0},{1,0},{0,1},{0,-1}};                     //����·��ʱ�ķ�������
int Path_Length=-2;                                             //�Թ���·�ĳ���,��ʼ��Ϊ-2ȷ�����ɵ�bossλ�ò������յ�
double Start_Time,End_Time;                                     //��ʱ��
enum Maze_Mark{Blank=0,Wall,Path,Boss};                         //ö�٣������Թ�״̬�洢

class Merge_Find                //���鼯��
{
    private:
        int *set;               //����
        int *value;             //���ϵ�Ȩ��
        int size;               //���ϵĴ�С���������ɼ���
        int cnt;                //��������
    public:
        Merge_Find(int Size)    //��ʼ��
        {
            this->size=Size;    //��ü�������
            set=new int[Size];  //�����ڴ�
            value=new int[Size];
            init();             //���ݳ�ʼ��
        }
        ~Merge_Find()           //��������
        {
            delete[] set;       //�ͷ��ڴ�
            delete[] value;
        }
        
        void init()             //���ݳ�ʼ��
        {
            cnt=size;
            for(int i=0;i<size;i++)
                set[i]=i,value[i]=1;
        }
        
        int Find_Root(int Index)        //���Ҹ��ڵ�
        {
            while(set[Index]!=Index)
                Index=set[Index];
            return Index;
        }

        int Path_Find(int Index)        //·��ѹ��
        {
            int Root=Find_Root(Index);
            while(set[Index]!=Root)     //���ӽڵ�ֱ��ָ����ڵ�
            {
                int id=Index;
                Index=set[Index];
                set[id]=Root;
            }
            return Root;
        }

        bool Set_Union(int set1,int set2)               //�ϲ����ϣ�����ֵ�����Ƿ�ϲ�����
        {
            int i=Path_Find(set1),j=Path_Find(set2);
            if(i==j)                                    //˵����ͬһ����������ϲ�
                return false;
            cnt--;                                      //���ϲ��򼯺�������1
            if(value[i]<value[j])                       //��Ȩ��С�ļ��Ϻϲ���Ȩ�ش�ļ�����
            {
                value[j]+=value[i];
                set[i]=j;
            }
            else
            {
                value[i]+=value[j];
                set[j]=i;
            }
            return true;
        }

        int Set_Quantity()//����ʣ�༯����
        {
            return cnt;
        }
};

struct Player
{
    char Name[20]={0};
    int highest_record=86400;       //���ͨ��ʱ��
    bool stay=false;                //����Ƿ�������ɾ�
    bool chosen[Chosen_Len]={0};    //������������ù�û��
}player;

void oneselection(char s[100])      //����ֻ��һ��ȷ��ѡ��ĵ���
{
    MessageBox(0,s,"hint",0);
    return ;
}

bool twoselection(char s[100])      //������ȷ����ȡ������ѡ��ĵ���
{
    int a=MessageBox(0,s,"hint",1); //��������ȡ���ڴ��������ַ���
    if(a==1)                        //ѡ��ȷ���򷵻���
        return true;
    else                            //ѡ��ȡ�����ؼ�
        return false;
}

void Mouse_Click()                                  //��������
{
    mouse_msg MOUSEMSG;                             //������������
    for(;is_run();delay_fps(180))                   //����ѭ����ѭ��Ƶ��Ϊÿ��180��
    {
        MOUSEMSG=getmouse();                        //��ȡ�����Ϣ
        if(MOUSEMSG.is_down()&&MOUSEMSG.is_left())  //��⵽���������
            break;                                  //���Ŀ���˳�ѭ��
    }
}

bool Cheat_Judge()                                                          //����Ա�����ж�
{
    if(S_login)                                                             //�ѵ�¼����Ա
        return true;
    else if(password_count<3&&!S_login&&strcmp(player.Name,"Suolk")==0)     //��¼�û���Ϊ����Ա�û��������������������
    {
        if(twoselection((char*)"��⵽����δ��¼����Ա��ݣ��Ƿ����ڵ�¼"))     //ѡ���¼����Ա������������������
        {
            sys_edit PassWord;                  //����һ���������
            key_msg S_password_msg;             //����һ��������
            char Password[20]={0};              //����һ���ַ������鴢�����������
            bool password_right=false;          //����һ�������ء������ж����������Ƿ���ȷ����ʼ��Ϊ����ȷ
            PassWord.create(false);             //���������������������������ʱ�������Ȼ�����ڣ���Ҫ���д���������
            PassWord.move(440,10);              //������������Ͻ�����
            PassWord.size(150,30);              //���������Ĵ�С
            PassWord.setmaxlen(10);             //����������볤��
            PassWord.setbgcolor(LIGHTBLUE);     //�����������ɫ
            PassWord.setcolor(WHITE);           //���������������ɫ
            PassWord.setfont(20,0,"����");      //������������ִ�С������
            PassWord.settext("����������");      //�����ɾ��������˵����������Ŀ��
            PassWord.visible(true);             //���������ɼ�
            PassWord.setfocus();                //���������Ĺ��
            flushkey();                         //��մ�ǰ�ļ�����Ϣ
            for (; is_run(); delay_fps(60))     //����ѭ����ѭ��Ƶ����ÿ��60��
            {
                char hint[50]={0};              //�����ַ�����������д�����ݲ�ȷ���Ĵ�����ʾ
                S_password_msg=getkey();        //��ȡ������Ϣ
                if(S_password_msg.msg==key_msg_down&&S_password_msg.key==key_enter) //��⵽����enter��
                {
                    password_count++;                       //�����������+1
                    PassWord.gettext(10,Password);          //��ȡ������������ڵ��ı�
                    if(strcmp(Password,"tgb951")==0)        //����Ƿ�������ȷ
                    {
                        oneselection((char*)"������ȷ��");   //������ȷ�򵯳�����ʾ��
                        password_right=true;                //������״̬���Ϊ��ȷ
                        S_login=true;                       //�������Ƿ��¼��ȫ�ֱ������Ϊ��
                    }
                    else if(password_count<3)               //����������󣬵������������������ʣ
                    {
                        sprintf(hint,"��������㻹��%d�λ���",3-password_count); //��������ʾд��hint����
                        PassWord.settext("");               //������������
                        PassWord.setfocus();                //���ù��
                        oneselection(hint);                 //������ʾ��
                    }
                    flushkey();                             //��ռ�����Ϣ��ֹ�´μ���⵽���õļ�����Ϣ
                    memset(Password,0,sizeof(Password));    //��մ�������������ַ������飬��ֹ�´μ��ʱ��������ַ����¼�����
                }
                else if(password_right)     //�������������ȷ����Ż���
                {
                    return true;
                }
                if(password_count>=3)       //���������������ﵽ������û����ȷ�������ؼ�
                {
                    return false;
                }
            }
        }
        else                                                //ѡ�񲻵�¼����Ա���
        {
            oneselection((char*)"���׹���ֻ�ܵ�¼��ʹ�ã�");   //����������ʾ
            return false;
        }
    }
    else if(password_count==3)  //�ڴ�֮ǰ�����������Ѿ��ﵽ������
    {
        oneselection((char*)"���������������Ѵ����ޣ��޷���¼��");
        return false;
    }
    else //�ǹ���Ա
    {
        oneselection((char*)"���׹���ֻ�ܹ���Աʹ�ã�");
        return false;
    }          
}

bool Exit_Game() //����ѯ���Ƿ��˳���Ϸ
{
    if(twoselection((char*)"ȷ���˳���Ϸ��"))
    {
        Exit=true; //������״̬�޸�Ϊ�˳�
        return true;
    }
    else 
        return false;
}

void Create_Maze()                              //ԭ��������С������������ͨ��·�������õ��ǲ��鼯+kruskal
{
    for(int i=0;i<=Maze_Max_Num;i++)            //�Ƚ��Թ�ȫ����ʼ��Ϊǽ
        for(int j=0;j<=Maze_Max_Num;j++)
            maze[i][j]=Wall;
    for(int i=1;i<Maze_Max_Num;i+=2)            //�ٽ�����Ŀո�����
        for(int j=1;j<Maze_Max_Num;j+=2)
            maze[i][j]=Blank;
    std::vector<int> Random_Sequence;
    for(int i=0;i<Wall_Num;i++)                 //����һ��vector�������������������
        Random_Sequence.push_back(i);
    shuffle(Random_Sequence.begin(),Random_Sequence.end(),std::default_random_engine(time(NULL))); //����һ������������������ǽ�����Թ�
    Merge_Find bcj(Maze_Size*Maze_Size);        //���ò�����һ�����鼯��
    for(int i=0;i<Wall_Num;i++)
    {
        if(bcj.Set_Quantity()<=1)               //ֻʣ���ڸ�����˵����·��ȫ����ͨ�����Թ������
            break;
        
        int num,Horizon=0,Vertical=0,Rand_Wall_Num,x_Wall,y_Wall;
        if(Random_Sequence[i]<Wall_Num/2)       //�����б��Ϊ0~Wall_Num/2-1��Ϊ����ǽ
        {
            Rand_Wall_Num=Random_Sequence[i];
            Horizon=1;
            num=Maze_Size-1;
        }
        else                                    //�����б��ΪWall_Num/2~Wall_Num��Ϊ����ǽ
        {
            Rand_Wall_Num=Random_Sequence[i]-Wall_Num/2;
            Vertical=1;
            num=Maze_Size;
        }
        x_Wall=Rand_Wall_Num%num;
        y_Wall=Rand_Wall_Num/num;

        int x1=x_Wall,y1=y_Wall,x2=x_Wall+Horizon,y2=y_Wall+Vertical;
		if (bcj.Set_Union(y1*Maze_Size+x1,y2*Maze_Size+x2))
        { //�ϲ��˼���˵����Ҫ��ǽ
			maze[y_Wall+y2+1][x_Wall+x2+1]=Blank;
		}
    }
}

void Button_Draw() //��������ȫ��ͨ�ð�ť
{
    rectangle(600,10,720,60);   //��һ�����ĵĳ�����
    rectangle(10,10,190,60);
    setfont(40,0,"����");       //���������С
    setbkmode(TRANSPARENT);     //�����ֱ�������Ϊ͸��
    xyprintf(20,15,"�˳���Ϸ");  //��������
    xyprintf(610,15,"CHEAT");
    setfont(20,0,"����");       //�ָ��ɳ��õ������С
}

void Maze_Draw() //�����Թ�
{
    PIMAGE BG_Pic=newimage(768,575);    //����һ��ͼƬ���͵�ָ��
    ege_enable_aa(true,BG_Pic);         //�����
    getimage(BG_Pic,"p_Maze.png");      //��ȡͼƬ
    cleardevice();                      //����
    putimage(0,0,BG_Pic);               //����ͼƬ
    delimage(BG_Pic);                   //����ͼƬ�ͷ��ڴ�
    BG_Pic=NULL;                        //ͼƬ������ʱָ����Ȼ���ڣ�����ָ��ӦΪ�գ�Ϊ��ֹ��������ٵ��ø�ָ��ʱ�����ֶ�����ָ��NULL
    Button_Draw();                      //���û��ư�ť����
    setlinewidth(2.0);                  //�����߿�Ϊ2.0
    setcolor(WHITE);                    //������Ϊ��ɫ

    for(int i=0;i<Maze_Max_Num;i+=2)
        for(int j=1;j<Maze_Max_Num;j+=2)
            if(maze[i][j]==Wall)
                line(160+(j-1)*14,85+i*14,160+14*(j+1),85+14*i);      //������
    for(int i=1;i<Maze_Max_Num;i+=2)
    {
        for(int j=0;j<Maze_Max_Num;j+=2)
        {
            if((i==1&&j==0)||(i==33&&j==34))                            //�޶�����������ϽǺ����½����������λ�ò�����
                continue;
            else if(maze[i][j]==Wall)
                line(160+14*j,85+14*(i-1),160+14*j,85+14*(i+1));      //������
        }
    }
}

bool Find_Path(int x,int y)                             //��dfs���ݷ��������뿪�Թ������·��
{
    if (maze[x][y]==Blank)                              //��⵽�ո�
    {
		maze[x][y] =Path;                               //�ȱ��Ϊ·��
		if (x==Maze_Max_Num-2&&y==Maze_Max_Num-2)       //������ڣ�˵��·����ȷ��������
			return true;
		for (int i=0;i<4;i++)                           //dire����ֱ�������ĸ�����
        {
			if(Find_Path(x+dire[i][0],y+dire[i][1]))    //�ݹ����
                return true;                            //·����ȷ��������
		}
		maze[x][y]=Blank;                               //���ĸ�����û����ȷ·����˵����һ���ǳ��Թ��ıؾ�֮·�����±��Ϊ�ո�
	}
    return false;                                       //���е���һ��˵��������·�߲���ȷ�����ؼ�
}

void Boss_pos(int pos1,int pos2)                //�ڱؾ�֮·�������������boss��pos1��pos2�������
{
    int cnt=0;                                  //pos1��pos2����Boss��·�ߵĵڼ���
    for(int i=0;i<Maze_Max_Num;i++)
    {
        for(int j=0;j<Maze_Max_Num;j++)
        {
            if(maze[i][j]==Path)
            {
                cnt++;                          //���㵱ǰ����·���ϵĵڼ���
                if(cnt==pos1)                   //����boss1
                    maze[i][j]=Boss;
                else if(cnt==pos2)
                    maze[i][j]=Boss;            //����boss2
                else if(cnt==Path_Length+2)
                {
                    maze[i][j]=Boss;            //�������һ��boss
                    return ;
                }
            }
        }
    }
}

void Path_Draw()                                                    //������Ա���Թ�ģ��ѡ�����׵�ʱ����Ҫ���뿪�Թ���·�߻����������boss��λ��
{
    setfillcolor(LIGHTBLUE);                                        //·�ߵ���ɫ��ǳ��ɫ��ʵ���ϸ���������ɫ��
    for(int i=1;i<Maze_Max_Num;i++)
    {
        for(int j=1;j<Maze_Max_Num;j++)
        {
            if(i==X_Player&&j==Y_Player)
                continue;
            else if(maze[i][j]==Path)                                    //��⵽·���򻭳�һ��С���鸲�ǵ�ǰ��
                bar(j*14+150,i*14+80,(j+1)*14+150,(i+1)*14+80);
            else if(maze[i][j]==Boss)                               //��⵽boss�������ɫ��һ��С���鸲�ǵ�ǰ��
            {
                setfillcolor(RED);                                  //boss�Ǻ�ɫС����
                bar(j*14+150,i*14+80,(j+1)*14+150,(i+1)*14+80);
                setfillcolor(LIGHTBLUE);                            //�ָ���ɫΪ·����ɫ
            }
        }
    }
}

void Story_Start() //��Ϸ��ʼʱ�Ĺ��±�������
{
    PIMAGE BG=newimage(768,575),p=newimage(203,197);             //����ͼƬ���͵�ָ�벢��ȡ����ͼƬ���������ͼƬ
    getimage(BG,"p_Maze.png");
    getimage(p,"p_player.png");
    putimage(0,0,BG);                                            //���ñ���ͼƬ
    setfont(20,0,"����");                                        //��������ʹ�С
    setbkmode(TRANSPARENT);                                     //�����ֱ�������Ϊ͸��
    setcolor(WHITE);                                            //����������ɫ
    xyprintf(100,205,"�����������о���,��w��a��s��d���ƶ�");     //�ڱ����ϴ�ӡ����
    xyprintf(100,230,"�ƶ���ʱ�����û��Ӧ�����л���Ӣ�����뷨");
    Mouse_Click();                                              //���������������Զ��庯����
    cleardevice();                                              //����
    putimage(0,0,BG);                                           //�ٴ����ϱ���ͼƬ
    delimage(BG);                                               //����ͼƬ�������õ��ˣ��������ͼƬ�ͷ��ڴ�
    BG=NULL;                                                    //ͼƬ������ʱָ����Ȼ���ڣ�����ָ��ӦΪ�գ�Ϊ��ֹ��������ٵ��ø�ָ��ʱ�����ֶ�����ָ��NULL
    xyprintf(100,205,"��һ�죬�����ѹ���ʱ���������վ����뿴���Լ�˧����������");
    Mouse_Click();
    putimage(175,0,p);                                          //�����������ͼƬ
    delimage(p);
    p=NULL;
    Mouse_Click();
    xyprintf(100,230,"?!������һֻ��������Q��С�Ƽ�!");
    Mouse_Click();
    xyprintf(100,255,"ԭ����������һ��Q�����磬�������ж�������Q���");
    Mouse_Click();
    xyprintf(100,280,"����Ҫ��ȥ����ͨ��һ���Թ�������");
    Mouse_Click();
}

void Erase(int x,int y)     //��ɫ���Թ��ƶ�ʱ����ԭ��ɫλ�ã�����ԭ��ɫλ�û���һ����ɫС���飬���ڱ���߹���·
{
    setfillcolor(BLACK);    //������ɫ��ɫ
    bar(x,y,x+14,y+14);     //��һ��ʵ�ķ���
}

void sd(int randnum) //��������С��Ϸ����������������ɣ���randnum���������
{
    char sd[20]={0};                    //�����ַ��������ȡ�ļ����������ļ�����ȷ�������Ҫͨ���ַ���������ת
    sprintf(sd,"sd%02d.bat",randnum);   //д���ַ�������
    FILE* fp;                           //����һ���ļ����͵�ָ��
    fp=fopen(sd,"r");                   //��ȡ��������洢�ļ�
    if(fp==NULL)                        //������ʾ��˵��ȱ�ٶ�Ӧ�������ļ�
    {
        char hint[14]={0};              //ͬ����Ϊ��Ӧ�Բ�ȷ�����ַ�������Ϊ��ת
        sprintf(hint,"Can't Find %s",sd);
        oneselection(hint);             //����
        return ;
    }
    int sd_square[9][9],sd_mark[9][9];  //��������ֱ����� �����������󣬴����λ�������Ƿ�Ҫ�ɳ����ӡ����ҪΪ1������ҪΪ0��
    for(int i=0;i<9;i++)                //���Ƿ������״̬��ѹ���mark��
    {                                   //���ڽ�״̬�����ļ�ʱ�õ��Ƕ�����ת��Ϊʮ���ƵĴ��뷽ʽ����˶�����Ҫ��ѹ
        int a;
        fscanf(fp,"%d",&a);
        for(int j=8;j>=0;j--)           //һ�����ִ���һ�е�״̬
        {                               //����λ������������ͨ���㣬��˲���λ��������ѹ
            sd_mark[i][j]=(a&1);        //λ���㣬�൱��a%2
            a>>=1;                      //λ���㣬�൱��a/2
        }
    }
    for(int i=0;i<9;i++) //���ļ��е���������ת�浽square��
    {
        int a;
        for(int j=0;j<9;j++)
        {
            fscanf(fp,"%d",&a);
            sd_square[i][j]=a;
        }
    }
    //������������������Ϸ����
    cleardevice();                      //����
    PIMAGE BG_Pic=newimage(768,575);    //����ͼƬָ��
    ege_enable_aa(true,BG_Pic);         //���ÿ����
    getimage(BG_Pic,"p_sdBG.png");      //��ȡ����ͼƬ
    putimage(0,0,BG_Pic);               //���ñ���ͼƬ
    delimage(BG_Pic);                   //ͼƬ�����ظ�ʹ�ã����ֱ������ͼƬ�ͷ��ڴ�
    BG_Pic=NULL;                        //ͼƬ������ʱָ����Ȼ���ڣ�����ָ��ӦΪ�գ�Ϊ��ֹ��������ٵ��ø�ָ��ʱ�����ֶ�����ָ��NULL
    PIMAGE Square_Pic=newimage(364,364);
    sys_edit Input[9][9];               //����������������
    mouse_msg MOUSEMSG;                 //����������
    bool Wrong_ans=false;               //�������ڱ�Ǵ���𰸵Ŀ��أ���ʼ״̬����û�д����
    getimage(Square_Pic,"p_����.png");  //�����������ͼƬ
    ege_enable_aa(true,Square_Pic);
    setcolor(BLACK);                    //����������ɫ��ɫ
    putimage(202,106,Square_Pic);       //������������ͼƬ
    delimage(Square_Pic);               //����ͼƬ�ͷ��ڴ�
    Square_Pic=NULL;
    for(int i=0;i<9;i++)                //��������������
    {
        for(int j=0;j<9;j++)
        {
            if(sd_mark[i][j])           //˵�����λ��Ӧ���ɳ�������
            {
                xyprintf(218+40*j,120+40*i,"%d",sd_square[i][j]); //��ӡ����
            }
            else                        //˵����λ��Ӧ�����������
            {
                Input[i][j].create(false);              //���������������������������ʱ�������Ȼ�����ڣ���Ҫ���д���������
                Input[i][j].move(210+40*j,112+40*i);    //���������λ��
                Input[i][j].size(30,30);                //����������С
                Input[i][j].setbgcolor(WHITE);          //�����������ɫ
                Input[i][j].setcolor(BLACK);            //���������������ɫ
                Input[i][j].setmaxlen(2);               //���������������볤��
                Input[i][j].settext("");                //�������������Ϊ��
                Input[i][j].setfont(20,0,"����");       //������������������С
                Input[i][j].visible(true);              //���������ɼ�
            }
        }
    }
    //���Ƽ�����������İ�ť
    setfont(40,0,"����");       //������������
    setcolor(BLACK);            //������ɫ
    rectangle(10,100,190,150);  //��һ�����ĵľ���
    rectangle(585,272,685,322);
    setbkmode(TRANSPARENT);     //�������ֱ���͸��
    xyprintf(20,105,"�������"); //��ӡ����
    xyprintf(600,277,"�ύ");
    setfont(20,0,"����");       //�ָ�Ϊ��������
    Button_Draw();              //��������ͨ�ð�ť
    flushmouse();               //��������Ϣ
    
    for(;is_run;delay_fps(180))//�����
    {
        while(mousemsg())                               //����⵽�����Ϣ
        {
            MOUSEMSG=getmouse();                        //��ȡ�����Ϣ
            if(MOUSEMSG.is_down()&&MOUSEMSG.is_left())  //����⵽����������
            {
                int x,y;                                //���ڴ��������������
                char num[3]={0};                        //���ڴ���������ַ���������������������ַ�����ʽ��ȡ����˽���һ��������ת������
                mousepos(&x,&y);                        //��ȡ���������
                if(x>=585&&x<=685&&y>=272&&y<=322)      //���λ��Ϊ�ύ��ť���ڷ�Χ
                {
                    for(int i=0;i<9;i++)
                    {
                        for(int j=0;j<9;j++)
                        {
                            if(!sd_mark[i][j])                  //��λ��ӦΪ�������
                            {
                                Input[i][j].gettext(2,num);     //��ȡ�������
                                if(num[0]-48!=sd_square[i][j])  //���벻��ȷ
                                {
                                    Wrong_ans=true;             //�޸Ŀ���״̬Ϊ�ɴ����
                                    break;                      //����ѭ��
                                }
                            }
                        }
                        if(Wrong_ans)                           //�д�������ټ����һ������ѭ��
                            break;
                    }
                    if(Wrong_ans)
                    {
                        oneselection((char*)"�д���𰸣�");    //������ʾ
                        Wrong_ans=false;                       //�ָ�����״̬�Խ�����һ�μ��
                        continue;                              //���������������ݵȴ���һ���ύ
                    }
                    else
                    {
                        oneselection((char*)"��ϲ������ˣ�");  //����ȷ
                        player.chosen[randnum-1]=true;         //����������ѱ���ɣ�����Ϊ�ù�
                        return ;                               //�뿪����
                    }
                }
                else if(x>=600&&x<=720&&y>=10&&y<=60)//ѡ�����ף�����Աѡ��
                {
                    if(Cheat_Judge())
                    {
                        for(int i=0;i<9;i++)    //�Զ������
                        {
                            for(int j=0;j<9;j++)
                            {
                                if(!sd_mark[i][j])
                                {
                                    char t[1];
                                    t[0]=sd_square[i][j]+48;
                                    Input[i][j].settext(t); //ֱ�Ӷ���char t;�������ǿ��ת������
                                }
                            }
                        }
                    }
                }
                else if(x>=10&&x<=190&&y>=10&&y<=60)//ѡ���˳�
                {
                    if(Exit_Game())
                    {
                        return ;
                    }
                }
                else if(x>=10&&x<=190&&y>=100&&y<=150&&twoselection((char*)"ȷ��������룿"))//ѡ���������
                {
                    for(int i=0;i<9;i++)
                    {
                        for(int j=0;j<9;j++)
                        {
                            if(!sd_mark[i][j])
                                Input[i][j].settext("");
                        }
                    }
                }
            }
        }
    }
    cleardevice();
}

DWORD WINAPI MouseControl(LPVOID lpParameter);//�̺߳�������

void Maze_Movement()
{
    HANDLE h1;
    PIMAGE bp=newimage(203,197),sp=newimage(14,14),boss;        //����ͼ��ָ�룬�ֱ�Ϊ������󣨴󣩣��������С����boss����
    PIMAGE temp1,talk=newimage(696,218),BG=newimage(768,575);   //��ת��ͼƬ���Ի��򣬱���
    key_msg KEYMSG;     //���������
    int cnt=0;          //���ڼ�¼�ڼ���boss
    getimage(bp,"p_player.png");            //��ȡ�������
    putimage(sp,0,0,14,14,bp,0,0,203,197);  //��С�������
    putimage(164,94,sp);                   //���Թ���ڷ���С���
    getimage(talk,"p_talk.png");            //��ȡ�Ի���ͼƬ�ļ�
    getimage(BG,"p_Maze.png");              //��ȡ�Թ�����ͼƬ�ļ�
    Start_Time=fclock();                    //��ʼ��ʱ
    flushkey();                             //��ռ�����Ϣ
    flushmouse();                           //��������Ϣ
    for(;is_run;delay_fps(180))             //ѭ��Ƶ��1��180��
    {
        h1=CreateThread(NULL,0,MouseControl,NULL,0,NULL); //�����߳�
        CloseHandle(h1);                                  //�ر��߳̾��
        while(kbmsg())          //��⵽������Ϣ����
        {
            KEYMSG=getkey();    //��ȡ������Ϣ
            if(KEYMSG.msg==key_msg_down)                                    //��⵽���̰���
            {
                if(KEYMSG.key=='A'&&maze[X_Player][Y_Player-1]!=Wall)       //����a������߲���ǽ
                {
                    Erase(Y_Player*14+150,X_Player*14+80);                 //����ԭλ�����ͼƬ
                    Y_Player--;                                             //����������
                }
                else if(KEYMSG.key=='D'&&maze[X_Player][Y_Player+1]!=Wall)  //����d�����ұ߲���ǽ
                {
                    Erase(Y_Player*14+150,X_Player*14+80);
                    Y_Player++;
                }
                else if(KEYMSG.key=='W'&&maze[X_Player-1][Y_Player]!=Wall)  //����w�����Ϸ�����ǽ
                {
                    Erase(Y_Player*14+150,X_Player*14+80);
                    X_Player--;
                }
                else if(KEYMSG.key=='S'&&maze[X_Player+1][Y_Player]!=Wall)  //����s�����·�����ǽ
                {
                    Erase(Y_Player*14+150,X_Player*14+80);
                    X_Player++;
                }
                else                                                        //��֪�������Ϲ������ʲô��ֵĶ���
                    continue;
                putimage(150+Y_Player*14,80+X_Player*14,sp);               //�ڸ����˵����λ���ϻ����������
                if(maze[X_Player][Y_Player]==Boss)                          //������boss
                {
                    cnt++,maze[X_Player][Y_Player]=Blank;                   //��boss��Ϊ�ո񣬷�ֹ�����е�û�¸ɵ��˻���
                    temp1=newimage(768,575);                                //����ͼƬ��С
                    getimage(temp1,0,0,768,575);                            //��ȡ��ǰ��Ļ���ݣ���������ͼƬ���temp1�
                    if(cnt==1)                                              //������һ��boss
                    {
                        boss=newimage(208,182);                             //����ͼƬ��С
                        getimage(boss,"p_boss1.png");                       //��ȡһ��boss����
                        putimage(0,0,BG);                                   //���ϱ���
                        setcolor(WHITE);                                    //����������ɫ��ɫ
                        setfont(20,0,"����");                               //���������С
                        setbkmode(TRANSPARENT);                             //�������屳��͸��
                        putimage(450,100,bp);                               //����������󣨴�
                        putimage(10,355,talk);                              //���öԻ���
                        xyprintf(60,375,player.Name);                       //��ӡ�������
                        xyprintf(60,410,"���Թ���úڣ�����ײ����ʲô��");      //��ӡ�Ի�����
                        Mouse_Click();                                      //��������
                        putimage(100,100,boss);                             //����boss����
                        putimage(10,355,talk);                              //�ٴη��öԻ����Ը��ǵ�ǰ���ӡ������
                        Mouse_Click();                                      //��ͬ
                        putimage(10,355,talk);
                        xyprintf(100,375,"���׵��ͻ�"); 
                        xyprintf(60,410,"ʲô�˸ҵ��ҵ�·���Ѿ����ñ��ҳԵ��˵�׼������");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(60,375,player.Name);
                        xyprintf(60,410,"�ף��⣬����Сè���ÿɰ�");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(100,375,"���׵��ͻ�"); 
                        xyprintf(60,410,"���ں�˵ʲô����������ͷ�ϵ�������");
                        xyprintf(60,435,"�ҿ����ͻ����ˣ�");
                        xyprintf(60,460,"��Ȼ��˵����è������������ֻ�ɶ��С�Ƽ���");
                        Mouse_Click();
                        putimage(0,0,BG);
                        xyprintf(100,205,"������������ܶ��ְ�");
                        Mouse_Click();
                        cleardevice();                              //����
                        srand(time(NULL));                          //����ʱ����Ϊ���������
                        while(true)                                 //���ڷ�ֹͬһ����������������ε�ѭ��
                        {
                            int rand_num=rand()%Chosen_Len+1;       //���������
                            if(!player.chosen[rand_num])            //���������δ���ֹ�
                            {
                                sd(rand_num);                       //���ɸ��������󲢽���������Ϸ
                                break;
                            }
                        }
                        if(Exit)        //�����������ѡ�����˳���Ϸ
                            return ;    //�뿪����
                        putimage(0,0,BG);                           //���ñ���ͼƬ
                        putimage(450,100,bp);                       //����������󣨴�
                        putimage(100,100,boss);                     //����boss
                        putimage(10,355,talk);                      //���öԻ���
                        xyprintf(100,375,"���׵��ͻ�");              //��ӡ�Ի�������
                        xyprintf(60,410,"�ⲻ���ܡ�");
                        xyprintf(60,435,"�޵е��ͻ����˾�Ȼ�����һֻ����");
                        xyprintf(60,460,"���۰�������������");
                        Mouse_Click();                              //��������
                        putimage(0,0,BG);                           //���·��ñ������Ի�����������Ӫ���boss�ܵ��˵�Ч��
                        putimage(450,100,bp);
                        putimage(10,355,talk);
                        xyprintf(60,375,player.Name);
                        xyprintf(60,410,"�����ܵ��ˡ�");
                        xyprintf(60,435,"����ruaһ��β�͵ġ�");
                        Mouse_Click();                              //��⵽���
                        delimage(boss);                             //����ͼƬ�ͷ��ڴ�
                        putimage(0,0,temp1);                        //���ô�ǰ�����Ļ��棬�����ص��Թ���Ϸ�����Ч��
                        delimage(temp1);
                        temp1=boss=NULL;                            //��ֹ�ٵ��ø�ָ��ָ������ͼƬʱ�����ֶ�����ָ��NULL
                    }
                    else if(cnt==2) //�ڶ���boss
                    {
                        boss=newimage(186,161);         //����ͼƬ��С
                        getimage(boss,"p_boss2.png");   //��ȡ�ڶ���boss���������������ͬ��
                        putimage(0,0,BG);
                        setcolor(WHITE);
                        setfont(20,0,"����");
                        setbkmode(TRANSPARENT);
                        putimage(100,100,boss);
                        putimage(450,100,bp);
                        putimage(10,355,talk);
                        xyprintf(60,375,player.Name);
                        xyprintf(60,410,"���֣�����ԭ�������ڵ���������");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(100,375,"�������"); 
                        xyprintf(60,410,"����ү���߰������������ߣ�������");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(60,375,player.Name);
                        xyprintf(60,410,"���⣬���ǡ���");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(100,375,"������"); 
                        xyprintf(60,410,"�������ɱ���ү�𣡣�������");
                        xyprintf(60,435,"��Ȼ�ѱ�������ˮ�������죡������");
                        xyprintf(60,460,"����ү�������ж��ﰡ�������ͷ�죡������");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(60,375,player.Name);
                        xyprintf(60,410,"�������Ǽ�����");
                        Mouse_Click();
                        srand(time(NULL));
                        while(true)
                        {
                            int rand_num=rand()%Chosen_Len+1;
                            if(!player.chosen[rand_num])
                            {
                                player.chosen[rand_num]=true;
                                sd(rand_num);
                                break;
                            }
                        }
                        if(Exit)
                            return ;
                        putimage(0,0,BG);
                        putimage(450,100,bp);
                        putimage(100,100,boss);
                        putimage(10,355,talk);
                        xyprintf(100,375,"һ���ܵ��������"); 
                        xyprintf(60,410,"����ү������ү��Ȼ��");
                        xyprintf(60,435,"�ܸ���һֻѼ�ӣ�������һֻˮ�����������");
                        xyprintf(60,460,"�������ж���ĳ��裡������");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(60,375,player.Name);
                        xyprintf(60,410,"��Ȼ���ǡ��ҿ����������ô��ˮ��������");
                        Mouse_Click();
                        delimage(boss);
                        putimage(0,0,temp1);
                        delimage(temp1);
                        boss=temp1=NULL;
                    }
                    else if(cnt==3) //���һ��boss
                    {
                        boss=newimage(225,200);         //����ͼƬ��С
                        getimage(boss,"p_boss3.png");   //��ȡ����bossͼƬ��С�����������ͬ��
                        putimage(0,0,BG);
                        setcolor(WHITE);
                        setfont(20,0,"����");
                        setbkmode(TRANSPARENT);
                        putimage(100,100,boss);
                        putimage(450,100,bp);
                        putimage(10,355,talk);
                        xyprintf(60,375,"һֻ�޹���С����"); 
                        xyprintf(60,410,"Ϊʲôһ��Ҫ��ȥ�أ�����������粻���𡭡�");
                        xyprintf(60,435,"�ѵ���Ҳ����������");
                        Mouse_Click();
                        if(twoselection((char*)"Ҫ��������"))       //������һ��С�ʵ�
                        {
                            player.stay=true;                       //���ѡ������������ɾͿ��ر��Ϊ���
                            putimage(10,355,talk);                  //���öԻ���ʹ�ӡ����
                            xyprintf(60,375,"һֻ���ֵ�С����"); 
                            xyprintf(60,410,"��С����û��˵����Ϊ���ܿ��֣�");
                            delimage(boss);
                            delimage(temp1);
                            delimage(BG);
                            boss=temp1=BG=NULL;
                            Mouse_Click();
                            return ;    //�뿪�Թ���Ϸ
                        }
                        putimage(10,355,talk);                      //����������Ϸ����
                        xyprintf(60,375,"һֻ��̫���С����"); 
                        xyprintf(60,410,"��Ȼ���඼����");
                        Mouse_Click();
                        xyprintf(60,435,"��Ȼ�㲻Ը������������ֻ��������������");
                        Mouse_Click();
                        putimage(10,355,talk);
                        xyprintf(100,375,player.Name); 
                        xyprintf(60,410,"ʲ��������ô֪������");
                        Mouse_Click();
                        srand(time(NULL));
                        while(true)
                        {
                            int rand_num=rand()%Chosen_Len+1;
                            if(!player.chosen[rand_num])
                            {
                                player.chosen[rand_num]=true;
                                sd(rand_num);
                                break;
                            }
                        }
                        if(Exit)
                            return ;
                        putimage(0,0,BG);
                        putimage(100,100,boss);
                        putimage(450,100,bp);
                        putimage(10,355,talk);
                        xyprintf(60,375,"һֻ�ѹ���С����"); 
                        xyprintf(60,410,"�������û����Ը�������������ǡ���");
                        Mouse_Click();
                        delimage(boss);
                        delimage(temp1);
                        delimage(BG);
                        boss=temp1=BG=NULL;
                        return ;    //��Ϸ����
                    }
                }
            }
        }
        if(Exit)
            return ;
    }
}

void calculate_PL()     //�����Թ�·�߳��ȣ���������boss��λ��
{
    for(int i=0;i<Maze_Max_Num;i++)
        for(int j=0;j<Maze_Max_Num;j++)
            if(maze[i][j]==Path)
                Path_Length++;
}

int main()
{
    FreeConsole();      //�رտ���̨����
    srand(time(NULL));  //��ȡ���������
    int Name_Length=0,randnum=0;    //�����������
    char Password[20]={0};          //���ڴ�������
    key_msg KEYMSG,S_password_msg;  //���������
    bool Key_press_flag=false,Mouse_press_flag=false,password_right=false;
    
    if(mciSendString(("open m_Regrets.mp3 alias BGM"), NULL, 0, NULL)) //�������ļ�
    {
        oneselection((char*)"OPEN BGM FAILED");
        return 0;
    }
    mciSendString("play BGM", NULL, 0, NULL);       //��������
    if(MessageBox(0,"Start Game Now?","hint",1)==2)
        return 0;
    mciSendString("close BGM", NULL, 0, NULL);      //������ֹͣ���ŷ���������ʱ��������ֿ���

    initgraph(400,300,0);           //��������
    setbkcolor(WHITE);              //���ñ�����ɫ
    setinitmode(INIT_NOBORDER,0,0); //���ó�������Ϊ�ޱ߿�
    setcaption("С�Ƽ����ӣ�");      //���ô��ڱ���
    PIMAGE BG_Pic=newimage();       //����ͼƬָ��
    ege_enable_aa(true,BG_Pic);     //�����
    getimage(BG_Pic,"p_login.png"); //��ȡ��¼����ͼƬ
    putimage(0,0,BG_Pic);           //���ñ���
    delimage(BG_Pic);               //����ͼƬ�ͷ��ڴ�
    BG_Pic=NULL;                    //ͼƬ������ʱָ����Ȼ���ڣ�����ָ��ӦΪ�գ�Ϊ��ֹ��������ٵ��ø�ָ��ʱ�����ֶ�����ָ��NULL

    sys_edit nameinput;             //�����������
    nameinput.create(false);        //�������������
    nameinput.move(100,100);        //����λ��
    nameinput.size(200,30);         //���ô�С
    nameinput.setmaxlen(10);        //����������볤��
    nameinput.setbgcolor(BLUE);     //�����������ɫ
    nameinput.setcolor(WHITE);      //���ü���������ɫ
    nameinput.setfont(20,0,"����");  //���������С
    nameinput.visible(true);        //���������ɼ�
    nameinput.setfocus();           //���ù��

    setcolor(BLACK);                        //����������ɫ
    setfont(20,0,"����");                   //���������С
    xyprintf(130, 50, "���ֳ��Ȳ�����%d",10);//��ӡ����
    
    for (; is_run(); delay_fps(60))//����Ա��¼��������ͨ��ҽ�����Ϸ
    {
        while(kbmsg())
        {
            KEYMSG=getkey();
            if (KEYMSG.msg==key_msg_down&&KEYMSG.key==key_enter)//��׽��enter����
            {
                nameinput.gettext(10,player.Name);  //��ȡ���������
                Name_Length = strlen(player.Name);  //��ȡ��������
                Key_press_flag = true;              //����Ƿ��������

                if(strcmp(player.Name,"Suolk")==0)//��֤����Ա��¼
                {
                    if(twoselection((char*)"ȷ��Ҫ�Թ���Ա��ݵ�¼��?"))
                    {
                        nameinput.settext("����������"); //�����ı�����ʾ����
                        nameinput.setfocus();           //���ù��
                        flushkey();                     //��ռ�����Ϣ
                        for (; is_run(); delay_fps(60)) //ѭ��Ƶ��Ϊÿ��60��
                        {
                            char hint[40]={0};          //��ת���ַ�������
                            S_password_msg=getkey();    //��ȡ������Ϣ
                            if (S_password_msg.msg==key_msg_down&&S_password_msg.key==key_enter) //����enter
                            {
                                password_count++;                   //�����������+1
                                nameinput.gettext(10,Password);     //��ȡ���������
                                if(strcmp(Password,"tgb951")==0)    //��������Ƿ���ȷ
                                {
                                    oneselection((char*)"������ȷ��");
                                    password_right=true;            //������ȷ
                                    S_login=true;                   //����Ա��½״̬�޸�Ϊ�ѵ�¼
                                }
                                else if(password_count<3)           //������󵫻����������
                                {
                                    sprintf(hint,"��������㻹��%d�λ���",3-password_count);
                                    nameinput.settext("");
                                    nameinput.setfocus();
                                    twoselection(hint);
                                }
                                else    //��������Ҵ�����
                                {
                                    password_right=false;           //���벻��ȷ
                                    nameinput.settext("Suolk");     //�������������������
                                }
                                flushkey();
                                memset(Password,0,sizeof(Password));//������ݷ�ֹ���������
                            }
                            if(password_right||password_count==3)   //������ȷ�������������������뿪���������ѭ��
                            {
                                break;
                            }
                        }
                    }
                }
                if(password_right)  //����Ա�ѵ�¼
                {
                    break;
                }
                if(Name_Length==0)  //��ͨ��ҽ�����Ϸ���������û��������
                {
                    oneselection((char*)"���������ƣ�");
                    nameinput.setfocus();
                    Key_press_flag=false;
                }
                else if(!twoselection((char*)"ȷ����������ƿ�ʼ��Ϸ��?"))
                {
                    Key_press_flag=false;
                }
            }
        }
        if(Key_press_flag)  //˵���Ѽ�������
        {
            break;
        }
    }

    cleardevice();                      //����
    nameinput.destroy();                //���������
    if(mciSendString("open m_CTS.mp3 alias BGM", NULL, 0, NULL))  //��ȡ�µ������ļ�
    {
        oneselection((char*)"OPEN BGM FAILED");
        return 0;
    }
    mciSendString("play BGM",NULL,0,NULL);
    initgraph(768,575);                 //���ô��ڴ�С
    setbkcolor(WHITE);                  //���ñ�����ɫ
    setcaption("С�Ƽ����ӣ�");          //���ñ���
    BG_Pic=newimage(768,575);           //���´���ͼƬ��С
    getimage(BG_Pic,"p_��ʼ��Ϸ.png");   //��ȡͼƬ�ļ�
    putimage(0,0,BG_Pic);               //���ñ���ͼƬ
    delay_ms(1);                        //ˢ��һ����Ļ
    Mouse_Click();                      //��������
    delimage(BG_Pic);                   //����ͼƬ
    BG_Pic=NULL;                        //ͼƬ������ʱָ����Ȼ���ڣ�����ָ��ӦΪ�գ�Ϊ��ֹ��������ٵ��ø�ָ��ʱ�����ֶ�����ָ��NULL
    mciSendString("close BGM", NULL, 0, NULL);
    cleardevice();                      //����

    if(mciSendString("open m_bgm.mp3 alias BGM", NULL, 0, NULL))  //��ȡ�µ������ļ�
    {
        oneselection((char*)"OPEN BGM FAILED");
        return 0;
    }
    mciSendString("play BGM repeat",NULL,0,NULL);  //ѭ�����ű�������
    Create_Maze();              //�����Թ�
    Story_Start();              //���¿�ʼ����
    Find_Path(1,1);             //dfs������̳�·
    calculate_PL();             //�����·����
    int pos_a=rand()%Path_Length+2,pos_b=rand()%Path_Length+2; //ͨ���������·�߳��ȼ���boss��·���ϵĵڼ���
    while(pos_a==pos_b) //ȷ��boss��λ�ò��غ�
        pos_b=rand()%Path_Length+2;

    Boss_pos(pos_a,pos_b);          //����boss
    Maze_Draw();                    //�����Թ�
    Maze_Movement();                //��ҽ����Թ���ʼ��Ϸ
    End_Time=fclock();              //��Ϸ����ʱ��
    mciSendString("close BGM", NULL, 0, NULL);
    if(Exit)                        //�������Ƿ�ѡ���˳���Ϸ
        return 0;

    BG_Pic=newimage(768,575);       //���»�ȡ����ͼƬ
    setcolor(BLACK);                //����������ɫ
    setfont(30,0,"����");           //���������С
    setbkmode(TRANSPARENT);         //�������屳��͸��
    getimage(BG_Pic,"p_sdBG.png");  //��ȡͼƬ�ļ�
    putimage(0,0,BG_Pic);           //���ñ���ͼƬ
    delimage(BG_Pic);               //����ͼƬ�ͷ��ڴ�
    BG_Pic=NULL;
    char hint[40]={0};              //��ת�ַ�������
    if(player.stay)                 //��Ҵ��������
    {
        if(mciSendString("open m_ding.mp3 alias BGM", NULL, 0, NULL))  //��ȡ�µ������ļ�
        {
            oneselection((char*)"OPEN BGM FAILED");
            return 0;
        }
        mciSendString("play BGM",NULL,0,NULL);     //������������Ч
        xyprintf(210,200,"��������֣�");
        xyprintf(210,235,"������ʵ��̫�ɰ��ˡ�");
        Mouse_Click();
        mciSendString("close BGM",NULL,0,NULL);
    }
    else  //�����ͨ���
    {
        xyprintf(210,200,"��ص�����ʵ");
        xyprintf(210,235,"��Ҳû������ʲô�ر����");
        xyprintf(210,270,"��ʱ�����");
        xyprintf(210,305,"�����ʱ���������ǲ��ǻ������");
        Mouse_Click();
        xyprintf(210,340,"��ɽ�֣��ص��ճ�");
    }
    player.highest_record=End_Time-Start_Time;  //����ͨ��ʱ��
    sprintf(hint,"ȫ����ʱ��%02d:%02d:%02d",player.highest_record/3600,(player.highest_record%3600)/60,player.highest_record%60);
    if(player.stay)             //��ͬ���λ��Ҳ��ͬ
        xyprintf(210,270,hint); //��xx:xx:xx����ʽ��ӡͨ��ʱ��
    else 
        xyprintf(210,375,hint);
    Mouse_Click();
    return 0;
}

DWORD WINAPI MouseControl(LPVOID lpParameter)       //���߳����ڹ����Թ��е������Ϣ
{                                                   //��ֹ���Թ�ʱ��Ϊ����ƶ����µļ�����Ӧ�ӳ�
    mouse_msg MOUSEMSG;                             //���������
    while(mousemsg())                               //��⵽�����Ϣ����
    {
        MOUSEMSG=getmouse();                        //��ȡ�����Ϣ
        if(MOUSEMSG.is_down()&&MOUSEMSG.is_left())  //Ϊ�����
        {
            int x,y;                                //���ڼ�¼�������
            char num[2]={0};                        
            mousepos(&x,&y);                        //��ȡ����
            if(x>=600&&x<=720&&y>=10&&y<=60)        //ѡ�����ף�����Աѡ��
            {
                if(Cheat_Judge())
                {
                    Path_Draw();                    //����·��ͼ
                    Path_View=true;                 //���·��״̬Ϊ�ɼ�
                }
            }
            else if(x>=10&&x<=190&&y>=10&&y<=60)    //ѡ���˳�
            {
                if(Exit_Game())
                {
                    return 0L;
                }
            }
        }
    }
}