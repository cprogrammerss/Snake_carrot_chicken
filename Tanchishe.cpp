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
#define R 4       //�ߵ�״̬��U���� ��D���£�L:�� R����

using namespace std;

typedef struct SNAKE //�����һ���ڵ�
{
    int x;
    int y;
    struct SNAKE* next;
}snake;

//��Ϸ��־
struct GameLog {
	int userID;
	string username;
	time_t startTime;
	int duration;
	int score;
}; 

//ȫ�ֱ���//
int score = 0, add = 10;//�ܵ÷���ÿ�γ�ʳ��÷֡�
int status, sleeptime = 200;//ÿ�����е�ʱ����
snake* head, * food;//��ͷָ�룬ʳ��ָ��
snake* q;//�����ߵ�ʱ���õ���ָ��
int endgamestatus = 0; //��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ��

int id; // �û�id
time_t timep; //��ǰʱ�� time(&timep); printf("%s",citme(&timep)); 
clock_t start,end;
int start_time,end_time; // ��Ϸ��ʼʱ������Ϸ����ʱ�� 
int duration; // ��Ϸ����ʱ�� 
vector<GameLog> allLogs;
GameLog user1;

//����ȫ������//
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

void Pos(int x, int y)//���ù��λ��
{
    COORD pos;
    HANDLE hOutput;
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);
}

void creatMap()//������ͼ
{
    int i;
    for (i = 0; i < 58; i += 2)//��ӡ���±߿�
    {
        Pos(i, 0);
        printf("��");
        Pos(i, 26);
        printf("��");
    }
    for (i = 1; i < 26; i++)//��ӡ���ұ߿�
    {
        Pos(0, i);
        printf("��");
        Pos(56, i);
        printf("��");
    }
}

void initsnake()//��ʼ������
{
    snake* tail;
    int i;
    tail = (snake*)malloc(sizeof(snake));//����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��//
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
    while (tail != NULL)//��ͷ��Ϊ���������
    {
        Pos(tail->x, tail->y);
        printf("��");
        tail = tail->next;
    }
}

int biteself()//�ж��Ƿ�ҧ�����Լ�
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

//void createfood()//�������ʳ��
//{
//    snake* food_1;
//    srand((unsigned)time(NULL));
//    food_1 = (snake*)malloc(sizeof(snake));
//    while ((food_1->x % 2) != 0)    //��֤��Ϊż����ʹ��ʳ��������ͷ����
//    {
//        food_1->x = rand() % 52 + 2;
//    }
//    food_1->y = rand() % 24 + 1;
//    q = head;
//    while (q->next == NULL)
//    {
//        if (q->x == food_1->x && q->y == food_1->y) //�ж������Ƿ���ʳ���غ�
//        {
//            free(food_1);
//            createfood();
//        }
//        q = q->next;
//    }
//    Pos(food_1->x, food_1->y);
//    food = food_1;
//    printf("��");
//}

void createfood()//�������ʳ��
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
        if (q->x == food_1->x && q->y == food_1->y) //�ж������Ƿ���ʳ���غ�
        {
            free(food_1);
            createfood();
        }
        q = q->next;
    }
    Pos(food_1->x, food_1->y);
    food = food_1;
    printf("��");
}


void cantcrosswall()//���ܴ�ǽ
{
    if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
    {
        endgamestatus = 1;
        endgame();
    }
}

void snakemove()//��ǰ��,��U,��D,��L,��R
{
    snake* nexthead;
    cantcrosswall();

    nexthead = (snake*)malloc(sizeof(snake));
    if (status == U)
    {
        nexthead->x = head->x;
        nexthead->y = head->y - 1;
        if (nexthead->x == food->x && nexthead->y == food->y)//�����һ����ʳ��//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                               //���û��ʳ��//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
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
        if (nexthead->x == food->x && nexthead->y == food->y)  //��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                               //û��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
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
        if (nexthead->x == food->x && nexthead->y == food->y)//��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                //û��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
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
        if (nexthead->x == food->x && nexthead->y == food->y)//��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                         //û��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (biteself() == 1)       //�ж��Ƿ��ҧ���Լ�
    {
        endgamestatus = 2;
        endgame();
    }
}

void pause()//��ͣ
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
//    currentLog.userID = 1; // �����û�IDΪ1
//    currentLog.username = "Alice"; // �û�������Ϊ"Alice"
//    currentLog.startTime = time(nullptr); // 
//   	currentLog.duration = end_time - start_time;
//    currentLog.score = score;
//    allLogs.push_back(currentLog);
    
	int row = 10;
	system("cls");
	Pos(24,3);
	cout << "��Ϸ�û���־" << endl;
//	Pos(24,10);
//	cout << "�û�id: " << log.userID << endl;
//	Pos(24,11);
//	cout << "�û��� " << log.username << endl;
//	Pos(24,12);
//	cout << "��Ϸ��ʼʱ��" << ctime(&log.startTime) << endl;
//	Pos(24,13);
//	cout << "��Ϸ����ʱ��" << log.duration << "��" << endl;
//	Pos(24,14);
//	cout << "�÷�:" << log.score << endl;
//	Pos(40, 25);
//	system("pause");
//	system("cls");
//	creatMap();
	for(const auto& log : logs){
		Pos(24, row);
		cout << "�û�id: " << log.userID << endl;
		Pos(24, ++row);
		cout << "�û��� " << log.username << endl;	
		Pos(24, ++row);
		cout << "��Ϸ��ʼʱ��" << ctime(&log.startTime) << endl;
		Pos(24, ++row);
		cout << "��Ϸ����ʱ��" << log.duration << "��" << endl;
		Pos(24, ++row);
		cout << "�÷�:" << log.score << endl;
		row += 3; 
	}
	Pos(40, 25);
	system("pause");
	system("cls");
	creatMap();
	createfood();
}
void gamecircle()//������Ϸ        
{
//	Pos(64, 5);
//	printf("��F5��ʾ��Ϸ�û���־");
//	
//
//    
//	Pos(64, 15);
//    printf("���ܴ�ǽ������ҧ���Լ�\n");
//    Pos(64, 16);
//    printf("�á�.��.��.���ֱ�����ߵ��ƶ�.");
//    Pos(64, 17);
//    printf("F1 Ϊ���٣�F2 Ϊ����\n");
//    Pos(64, 18);
//    printf("ESC ���˳���Ϸ.space����ͣ��Ϸ.");

	
	
            
    Pos(64, 20);
    status = R;
    while (1)
    {
    	Pos(64, 5);
		printf("��F5��ʾ��Ϸ�û���־");
        Pos(64, 10);
        printf("�÷֣�%d  ", score);
        Pos(64, 11);
        printf("ÿ��ʳ��÷֣�%d��", add);
        Pos(64, 15);
	    printf("���ܴ�ǽ������ҧ���Լ�\n");
	    Pos(64, 16);
	    printf("�á�.��.��.���ֱ�����ߵ��ƶ�.");
	    Pos(64, 17);
	    printf("F1 Ϊ���٣�F2 Ϊ����\n");
	    Pos(64, 18);
	    printf("ESC ���˳���Ϸ.space����ͣ��Ϸ.");
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
                    add = 2;//��ֹ����1֮���ټӻ����д�
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
                    add = 1;  //��֤��ͷ�Ϊ1
                }
            }
        }
        else if(GetAsyncKeyState(VK_F5))
        {

			int currentUserID = 1; // ���赱ǰ�û�IDΪ1
            int userIndex = findUserIndex(allLogs, currentUserID);
            if (userIndex != -1) {
                // ����������־�������������־
                allLogs[userIndex].startTime = time(nullptr); // ��¼��Ϸ��ʼʱ��
                // �������̰������Ϸ����ʱ���͵÷ֵĸ���
                allLogs[userIndex].duration = time(nullptr) - start_time;
                allLogs[userIndex].score = score;
            } else {
                GameLog currentLog;
                currentLog.userID = currentUserID;
                currentLog.username = "Alice"; // �û�������Ϊ"Alice"
                currentLog.startTime = time(nullptr); // ��¼��Ϸ��ʼʱ��
                // �������̰������Ϸ����ʱ���͵÷ֵļ�¼
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

void welcometogame()//��ʼ����
{
	//��Ϸ��ʼʱ�� 
//	Pos(40,16);
	time(&timep);
//	printf("��Ϸ��ʼʱ��Ϊ%s",ctime(&timep));

//	Pos(40,18);
	start_time = timep;
//	cout << start_time;
//	
    Pos(40, 12);
    printf("��ӭ����̰ʳ����Ϸ��");
    Pos(40, 25);
    system("pause");
    system("cls");
    Pos(25, 12);
    printf("�á�.��.��.���ֱ�����ߵ��ƶ��� F1 Ϊ���٣�F2 Ϊ����\n");
    Pos(25, 13);
    printf("���ٽ��ܵõ����ߵķ�����\n");
    Pos(40, 25);
    system("pause");
    system("cls");
}



void endgame()//������Ϸ
{

    system("cls");
    Pos(24, 12);
    if (endgamestatus == 1)
    {
        printf("�Բ�����ײ��ǽ�ˡ���Ϸ����!");
    }
    else if (endgamestatus == 2)
    {
        printf("�Բ�����ҧ���Լ��ˡ���Ϸ����!");
    }
    else if (endgamestatus == 3)
    {
        printf("���Ѿ���������Ϸ��");
    }
    Pos(24, 13);
    printf("���ĵ÷���%d\n", score);
    
    //��Ϸ����ʱ��
//    Pos(24, 15);
//	time(&timep);
//	printf("%s",ctime(&timep));
//	end_time = timep;
//	    Pos(24, 16);
//	cout << end_time << endl;
//    
//	Pos(24, 18);
//	cout << "��Ϸ��ʱ��Ϊ" << end_time-start_time << "��" << endl;
    
    exit(0);
}

void gamestart()//��Ϸ��ʼ��
{
    system("mode con cols=100 lines=30");
    welcometogame();
    creatMap();
    initsnake();
    createfood();
}

//void ShowLog() // ����F5��ʾ��Ϸ�û���־
//{
//	system("cls");
//	Pos(24,6);
//	cout << "��Ϸ�û���־" << endl;
//	Pos(24,10);
//	cout << "��ʼʱ��" << start_time << endl; 
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
