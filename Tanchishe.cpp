#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <vector>
#include<conio.h>
#include <string.h>
#include <fstream>
#include <sstream>

#define U 1
#define D 2
#define L 3 
#define R 4       //蛇的状态，U：上 ；D：下；L:左 R：右

using namespace std;

typedef struct SNAKE //蛇身的一个节点
{
    int x;
    int y;
    struct SNAKE* next;
}snake;

//游戏日志
struct GameLog {
	int userID;
	char username[20];
	time_t startTime;
	int duration;
	int score;
}; 

typedef struct USERS//一个玩家 
{
	char username[20];
	char password[20];
	//time_t startTime;
	int duration;
	int score;
	int userID;

}users;

//全局变量//
int score = 0, add = 10;//总得分与每次吃食物得分。
int status, sleeptime = 200;//每次运行的时间间隔
snake* head, * food;//蛇头指针，食物指针
snake* q;//遍历蛇的时候用到的指针
int endgamestatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。

int id; // 用户id
time_t timep; //当前时间 time(&timep); printf("%s",citme(&timep)); 
int start_time,end_time; // 游戏开始时间与游戏结束时间 
int duration; // 游戏持续时间 
vector<GameLog> allLogs;
GameLog log;
users user;
char login_name[20];

//声明全部函数//
void Pos();
void creatMap();
void initsnake();
int biteself();
void createfood();
void cantcrosswall();
void snakemove();
void pause();
void gamecircle();
void welcometogame();
void endgame();
void gamestart();
int menu();
int denglu();
void zhuce();
void UpdateLogs();
void showDetail();
void ShowUserLog();
void WriteUserLogToFile();
void DisplayUserLogsFromFile();

void Pos(int x, int y)//设置光标位置
{
    COORD pos;
    HANDLE hOutput;
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);
}

void creatMap()//创建地图
{
    int i;
    for (i = 0; i < 58; i += 2)//打印上下边框
    {
        Pos(i, 0);
        printf("■");
        Pos(i, 26);
        printf("■");
    }
    for (i = 1; i < 26; i++)//打印左右边框
    {
        Pos(0, i);
        printf("■");
        Pos(56, i);
        printf("■");
    }
}

void initsnake()//初始化蛇身
{
    snake* tail;
    int i;
    tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置//
    tail->x = 24;
    tail->y = 5;
    tail->next = NULL;
    for (i = 1; i <= 4; i++)
    {
        head = (snake*)malloc(sizeof(snake));
        head->next = tail;
        head->x = 24 + 2 * i;
        head->y = 5;
        tail = head;
    }
    while (tail != NULL)//从头到为，输出蛇身
    {
        Pos(tail->x, tail->y);
        printf("■");
        tail = tail->next;
    }
}

int biteself()//判断是否咬到了自己
{
    snake* self;
    self = head->next;
    while (self != NULL)
    {
        if (self->x == head->x && self->y == head->y)
        {
            return 1;
        }
        self = self->next;
    }
    return 0;
}

void createfood()//随机出现食物
{
    snake* food_1;
    srand((unsigned)time(NULL));
    food_1 = (snake*)malloc(sizeof(snake));
    while ((food_1->x % 2) != 0 || food_1->x < 2 || food_1->x > 53 || food_1->y < 1 || food_1->y > 25)    
    {
        food_1->x = rand() % 52 + 2;
        food_1->y = rand() % 24 + 1;
    }
    q = head;
    while (q->next != NULL)
    {
        if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
        {
            free(food_1);
            createfood();
        }
        q = q->next;
    }
    Pos(food_1->x, food_1->y);
    food = food_1;
    printf("■");
}


void cantcrosswall()//不能穿墙
{
    if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
    {
        endgamestatus = 1;
        endgame();
    }
}

void snakemove()//蛇前进,上U,下D,左L,右R
{
    snake* nexthead;
    cantcrosswall();

    nexthead = (snake*)malloc(sizeof(snake));
    if (status == U)
    {
        nexthead->x = head->x;
        nexthead->y = head->y - 1;
        if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                               //如果没有食物//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (status == D)
    {
        nexthead->x = head->x;
        nexthead->y = head->y + 1;
        if (nexthead->x == food->x && nexthead->y == food->y)  //有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                               //没有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (status == L)
    {
        nexthead->x = head->x - 2;
        nexthead->y = head->y;
        if (nexthead->x == food->x && nexthead->y == food->y)//有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                //没有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (status == R)
    {
        nexthead->x = head->x + 2;
        nexthead->y = head->y;
        if (nexthead->x == food->x && nexthead->y == food->y)//有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                         //没有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (biteself() == 1)       //判断是否会咬到自己
    {
        endgamestatus = 2;
        endgame();
    }
}

void pause()//暂停
{
    while (1)
    {
        Sleep(300);
        if (GetAsyncKeyState(VK_SPACE))
        {
            break;
        }

    }
}

int findUserIndex(const std::vector<GameLog>& logs, int userID) {
    for (size_t i = 0; i < logs.size(); ++i) {
        if (logs[i].userID == userID) {
            return i;
        }
    }
    return -1;
}


void showDetail(){
	    Pos(64, 5);
		printf("按F5显示游戏用户日志");
		Pos(64, 3);
		cout << "***" <<login_name << "正在游戏中***";
        Pos(64, 10);
        printf("得分：%d  ", score);
        Pos(64, 11);
        printf("每个食物得分：%d分", add);
        Pos(64, 15);
	    printf("不能穿墙，不能咬到自己\n");
	    Pos(64, 16);
	    printf("用↑.↓.←.→分别控制蛇的移动.");
	    Pos(64, 17);
	    printf("F1 为加速，F2 为减速\n");
	    Pos(64, 18);
	    printf("ESC ：退出游戏.space：暂停游戏.");
}

void gamecircle()//控制游戏        
{    
    Pos(64, 20);
    status = R;
    while (1)
    {
		showDetail();
	    //Pos(64, 20);
        if (GetAsyncKeyState(VK_UP) && status != D)
        {
            status = U;
        }
        else if (GetAsyncKeyState(VK_DOWN) && status != U)
        {
            status = D;
        }
        else if (GetAsyncKeyState(VK_LEFT) && status != R)
        {
            status = L;
        }
        else if (GetAsyncKeyState(VK_RIGHT) && status != L)
        {
            status = R;
        }
        else if (GetAsyncKeyState(VK_SPACE))
        {
            pause();
        }
        else if (GetAsyncKeyState(VK_ESCAPE))
        {
            endgamestatus = 3;
            break;
        }
        else if (GetAsyncKeyState(VK_F1))
        {
            if (sleeptime >= 50)
            {
                sleeptime = sleeptime - 30;
                add = add + 2;
                if (sleeptime == 320)
                {
                    add = 2;//防止减到1之后再加回来有错
                }
            }
        }
        else if (GetAsyncKeyState(VK_F2))
        {
            if (sleeptime < 350)
            {
                sleeptime = sleeptime + 30;
                add = add - 2;
                if (sleeptime == 350)
                {
                    add = 1;  //保证最低分为1
                }
            }
        }
        else if(GetAsyncKeyState(VK_F5))
        {
//			int startIndex = 0;
//			UpdateLogs();
//            showUserLogs(allLogs, startIndex);
			DisplayUserLogsFromFile();
		}
        Sleep(sleeptime);
        snakemove();
    }
    
}

void welcometogame()//开始界面
{
	time(&timep);
	start_time = timep;

    Pos(40, 12);
    printf("欢迎来到贪食蛇游戏！");
    Pos(40, 25);
    system("pause");
    system("cls");
    Pos(25, 12);
    printf("用↑.↓.←.→分别控制蛇的移动， F1 为加速，F2 为减速\n");
    Pos(25, 13);
    printf("加速将能得到更高的分数。\n");
    Pos(40, 25);
    system("pause");
    system("cls");
    
    int flag = menu();
    
    if(flag == 1);
    {
		creatMap();
    	initsnake();
    	createfood();
    }
}

int menu()//主菜单界面 
{
	int select;
	system("cls");
	printf("\n		******************菜单栏*****************\n");
	printf("\t\t*\t      1、登录                    *\n");
	printf("\t\t*\t      2、注册                    *\n");
	printf("\t\t*\t      3、退出游戏\t\t*\n");
	printf("		*****************************************\n");
	printf("\n");
	printf("\t\t请输入选项：[\t]\b\b\b");
	scanf("%d", &select);
	int flag;
	switch(select)
	{
		case 1:
			denglu();
			
			break;
		case 2:
			zhuce();
			menu();
			break;
		case 3:
			printf("感谢使用\n");
			exit(0);
		default:
			printf("输入错误!");
			Sleep(1000);
			system("cls");
			menu();
	}
	return flag;
} 

int denglu()//登陆界面 
{
	system("cls");
	users a,b;
	FILE* fp = fopen("Users.txt","r");
	int i,flag;//flag作为最终的结果 
	printf("欢迎来到登陆界面！\n");
	printf("请输入账号：\n");
	scanf("%s",&b.username);
	printf("请输入密码：\n");
	for(i = 0;i < 20;i++)
	{
		b.password[i] = _getch();
		if(b.password[i] == '\r')
			break;
		else
			printf("*");
	}
	b.password[i] = '\0';
	printf("\n");
	
	while(1)
	{
//		fread(&a, sizeof(users),1,fp);
		fscanf(fp, "%d %s %s",&a.userID, a.username, a.password);
		
		if(strcmp(b.username,a.username)==0)
			break;
		else
		{
			if(!feof(fp))
			{
				//fscanf(fp, "%d %s %s",&a.userID, a.username, a.password);
				continue;
			}
			else
			{
				printf("账号或密码错误！\n");
				fclose(fp);
				Sleep(1000);
				
				system("cls");
				menu();
//				flag=0; 
//				return flag;
			}
		}
	}
	
	if(strcmp(b.password,a.password)==0)
	{
		printf("%s，欢迎回来！！！\n",a.username);
		strcpy(login_name, a.username);
//		printf("%s",login_name);
		fclose(fp);

		Sleep(1000);
		system("cls");
		creatMap();
    	initsnake();
    	createfood();
		//flag = 1;
		//return flag;
	}
	else
	{
		printf("密码错误！！！\n");
		fclose(fp);
		Sleep(1000);
		system("cls");
		menu();	
//		flag = 0;
//		return flag;
	}
//	printf("%d",flag);
}

void zhuce()//注册模块
{
	int i;
	system("cls");
	char c[20];
	users a, b;
	FILE* fp;
	printf("\n\t\t欢迎来到注册界面！\n");
	Sleep(500);
	fp = fopen("Users.txt","r");
	fscanf(fp, "%s %s", b.username, b.password);
	printf("\n\t\t请输入用户名:[ \t\t ]\b\b\b\b\b\b\b\b\b\b\b\b\b");
	scanf("%s",&a.username);
	while(1)
	{
		if (strcmp(a.username, b.username))
		{
			if (!feof(fp))
			{
			  fscanf(fp, "%d %s %s",&b.userID, b.username, b.password);
			}
			else
			{
				break;
			}
		}
		else 
		{
			printf("该账号已存在！！！");
			Sleep(1000);
			fclose(fp);
			return;
		}
	}
	int time = 2;
	do
	{
		printf("\t\t请设置密码:[ \t\t ]\b\b\b\b\b\b\b\b\b\b\b\b\b");
		i = 0;
		for (i = 0; i < 20; i++)
		{
			a.password[i] = _getch();
			if (a.password[i] == '\r')
			{
				break;
			}
			else
			{
				printf("*");
			}
		}
		a.password[i] = '\0';
		printf("\n");
		printf("\t\t请确认密码：[ \t\t ]\b\b\b\b\b\b\b\b\b\b\b\b\b");
		i = 0;
		for (i = 0; i < 20; i++)
		{
			c[i] = _getch();
			if (c[i] == '\r')
			{
				break;
			}
			else
			{
				printf("*");
			}
		}
		c[i] = '\0';
		printf("\n");
		if (!strcmp( a.password,c) )
		{
			
		    printf("\t\t请输入ID：");
			scanf("%d",&a.userID);
			fp = fopen("Users.txt", "a");
			fprintf(fp, "%d %s %s\n",a.userID, a.username,a.password); 
//			fwrite(&a,sizeof(users),1,fp);
			printf("%s,注册成功！！！", a.username);
			fclose(fp);
			Sleep(1500);
			return;
		}
		else
		{
			if (time != 0)
			{
				printf("密码错误！\n\n请重新输入密码！！\n\n您还有%d次机会！！！", time);
				time--;
				continue;
			}
			else
			{
				printf("\n\t\t多次输入错误，即将退出！");
				fclose(fp);
				Sleep(1500);
				return;
			}
		}
	} while (time >= 0);
	
}

void endgame()//结束游戏
{

    system("cls");
    Pos(24, 12);
    if (endgamestatus == 1)
    {
        printf("对不起，您撞到墙了。游戏结束!");
    }
    else if (endgamestatus == 2)
    {
        printf("对不起，您咬到自己了。游戏结束!");
    }
    else if (endgamestatus == 3)
    {
        printf("您已经结束了游戏。");
    }
    Pos(24, 13);
    printf("您的得分是%d\n", score);
    WriteUserLogToFile();
    
    
    
    exit(0);
}

void ReadData(){
	users a;
	FILE* fp = fopen("Users.txt","r");
	while(1)
	{
		fscanf(fp, "%d %s %s",&a.userID, a.username, a.password);
		
		if(strcmp(login_name,a.username)==0)
			break;
		else
		{
			if(!feof(fp))
			{
				continue;
			}
			else
			{
				fclose(fp);
				Sleep(1000);
				
//				system("cls");
//				menu();
//				flag=0; 
//				return flag;
			}
		}
	}

	
//	fscanf(fp, "%d %s %s",&a.userID, a.username, a.password);
	log.userID = a.userID;
//	log.username = a.username;
	strcpy(log.username, a.username);
	log.startTime = start_time;
	log.duration = time(nullptr) - start_time;
	log.score = score;
}

void WriteUserLogToFile() {
	
    std::ofstream file("GameLogs.txt", std::ios_base::app); // 以追加模式打开文件
    ReadData();
    if (file.is_open()) {
        file << log.userID << " " << log.username << " " << log.startTime << " " << log.duration << " " << log.score << std::endl;
        file.close();
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

void DisplayUserLogsFromFile() {
	int row = 5;
    system("cls");
    Pos(24, 0);
    cout << "游戏用户日志" << endl;


    
    std::ifstream file("GameLogs.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
    }

    GameLog log;
    while (file >> log.userID >> log.username >> log.startTime >> log.duration >> log.score) {
        Pos(24, row);
		std::cout << "User ID: " << log.userID << std::endl;
        Pos(24, ++row);
		std::cout << "Username: " << log.username << std::endl;
        Pos(24, ++row);
		std::cout << "Start Time: " << ctime(&log.startTime);
        Pos(24, ++row);
		std::cout << "Duration: " << log.duration << " seconds" << std::endl;
        Pos(24, ++row);
		std::cout << "Score: " << log.score << std::endl;
        Pos(24, ++row);
		std::cout << "---------------------" << std::endl;
		row += 3;
    }
    
    
    Pos(40, 25);
    system("pause");
    system("cls");
    creatMap();
    createfood();

    file.close();
}

void gamestart()//游戏初始化
{
    system("mode con cols=100 lines=30");
    welcometogame();
    creatMap();
    initsnake();
    createfood();
}



int main()
{
    gamestart();
    gamecircle();
    endgame();
    
    return 0;
}
