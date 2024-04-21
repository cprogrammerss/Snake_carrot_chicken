#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <vector>

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
	string username;
	time_t startTime;
	int duration;
	int score;
}; 

//全局变量//
int score = 0, add = 10;//总得分与每次吃食物得分。
int status, sleeptime = 200;//每次运行的时间间隔
snake* head, * food;//蛇头指针，食物指针
snake* q;//遍历蛇的时候用到的指针
int endgamestatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。

int id; // 用户id
time_t timep; //当前时间 time(&timep); printf("%s",citme(&timep)); 
clock_t start,end;
int start_time,end_time; // 游戏开始时间与游戏结束时间 
int duration; // 游戏持续时间 
vector<GameLog> allLogs;
GameLog user1;

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
//void ShowLog();
void ShowUserLog();

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

//void createfood()//随机出现食物
//{
//    snake* food_1;
//    srand((unsigned)time(NULL));
//    food_1 = (snake*)malloc(sizeof(snake));
//    while ((food_1->x % 2) != 0)    //保证其为偶数，使得食物能与蛇头对其
//    {
//        food_1->x = rand() % 52 + 2;
//    }
//    food_1->y = rand() % 24 + 1;
//    q = head;
//    while (q->next == NULL)
//    {
//        if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
//        {
//            free(food_1);
//            createfood();
//        }
//        q = q->next;
//    }
//    Pos(food_1->x, food_1->y);
//    food = food_1;
//    printf("■");
//}

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
void showUserLogs(vector<GameLog>& logs){
//	GameLog currentLog;
//    currentLog.userID = 1; // 假设用户ID为1
//    currentLog.username = "Alice"; // 用户名设置为"Alice"
//    currentLog.startTime = time(nullptr); // 
//   	currentLog.duration = end_time - start_time;
//    currentLog.score = score;
//    allLogs.push_back(currentLog);
    
	int row = 10;
	system("cls");
	Pos(24,3);
	cout << "游戏用户日志" << endl;
//	Pos(24,10);
//	cout << "用户id: " << log.userID << endl;
//	Pos(24,11);
//	cout << "用户名 " << log.username << endl;
//	Pos(24,12);
//	cout << "游戏开始时间" << ctime(&log.startTime) << endl;
//	Pos(24,13);
//	cout << "游戏持续时间" << log.duration << "秒" << endl;
//	Pos(24,14);
//	cout << "得分:" << log.score << endl;
//	Pos(40, 25);
//	system("pause");
//	system("cls");
//	creatMap();
	for(const auto& log : logs){
		Pos(24, row);
		cout << "用户id: " << log.userID << endl;
		Pos(24, ++row);
		cout << "用户名 " << log.username << endl;	
		Pos(24, ++row);
		cout << "游戏开始时间" << ctime(&log.startTime) << endl;
		Pos(24, ++row);
		cout << "游戏持续时间" << log.duration << "秒" << endl;
		Pos(24, ++row);
		cout << "得分:" << log.score << endl;
		row += 3; 
	}
	Pos(40, 25);
	system("pause");
	system("cls");
	creatMap();
	createfood();
}
void gamecircle()//控制游戏        
{
//	Pos(64, 5);
//	printf("按F5显示游戏用户日志");
//	
//
//    
//	Pos(64, 15);
//    printf("不能穿墙，不能咬到自己\n");
//    Pos(64, 16);
//    printf("用↑.↓.←.→分别控制蛇的移动.");
//    Pos(64, 17);
//    printf("F1 为加速，F2 为减速\n");
//    Pos(64, 18);
//    printf("ESC ：退出游戏.space：暂停游戏.");

	
	
            
    Pos(64, 20);
    status = R;
    while (1)
    {
    	Pos(64, 5);
		printf("按F5显示游戏用户日志");
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

			int currentUserID = 1; // 假设当前用户ID为1
            int userIndex = findUserIndex(allLogs, currentUserID);
            if (userIndex != -1) {
                // 更新现有日志而不是添加新日志
                allLogs[userIndex].startTime = time(nullptr); // 记录游戏开始时间
                // 这里添加贪吃蛇游戏持续时长和得分的更新
                allLogs[userIndex].duration = time(nullptr) - start_time;
                allLogs[userIndex].score = score;
            } else {
                GameLog currentLog;
                currentLog.userID = currentUserID;
                currentLog.username = "Alice"; // 用户名设置为"Alice"
                currentLog.startTime = time(nullptr); // 记录游戏开始时间
                // 这里添加贪吃蛇游戏持续时长和得分的记录
                currentLog.duration = time(nullptr) - start_time;
                currentLog.score = score;
                allLogs.push_back(currentLog);
            }
            showUserLogs(allLogs);
		}
        Sleep(sleeptime);
        snakemove();
    }
    
}

void welcometogame()//开始界面
{
	//游戏开始时间 
//	Pos(40,16);
	time(&timep);
//	printf("游戏开始时间为%s",ctime(&timep));

//	Pos(40,18);
	start_time = timep;
//	cout << start_time;
//	
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
    
    //游戏结束时间
//    Pos(24, 15);
//	time(&timep);
//	printf("%s",ctime(&timep));
//	end_time = timep;
//	    Pos(24, 16);
//	cout << end_time << endl;
//    
//	Pos(24, 18);
//	cout << "游戏总时间为" << end_time-start_time << "秒" << endl;
    
    exit(0);
}

void gamestart()//游戏初始化
{
    system("mode con cols=100 lines=30");
    welcometogame();
    creatMap();
    initsnake();
    createfood();
}

//void ShowLog() // 按下F5显示游戏用户日志
//{
//	system("cls");
//	Pos(24,6);
//	cout << "游戏用户日志" << endl;
//	Pos(24,10);
//	cout << "开始时间" << start_time << endl; 
//	Pos(40, 25);
//	system("pause");
//	system("cls");
//	creatMap();
// } 
 


int main()
{
    gamestart();
    gamecircle();
    endgame();
    
//	start = clock();
//    int ans = 0;
//    for(int i = 1; i < 1e8; i++)
//    	ans++;
//    end = clock();
//    double endtime = (double)(end-start)/CLOCKS_PER_SEC;
//    cout << "Total time:" << endtime << endl;
    
    
    
    return 0;
}
