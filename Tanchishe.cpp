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
	char username[20];
	time_t startTime;
	int duration;
	int score;
}; 

typedef struct USERS//һ����� 
{
	char username[20];
	char password[20];
	//time_t startTime;
	int duration;
	int score;
	int userID;

}users;

//ȫ�ֱ���//
int score = 0, add = 10;//�ܵ÷���ÿ�γ�ʳ��÷֡�
int status, sleeptime = 200;//ÿ�����е�ʱ����
snake* head, * food;//��ͷָ�룬ʳ��ָ��
snake* q;//�����ߵ�ʱ���õ���ָ��
int endgamestatus = 0; //��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ��

int id; // �û�id
time_t timep; //��ǰʱ�� time(&timep); printf("%s",citme(&timep)); 
int start_time,end_time; // ��Ϸ��ʼʱ������Ϸ����ʱ�� 
int duration; // ��Ϸ����ʱ�� 
vector<GameLog> allLogs;
GameLog log;
users user;
char login_name[20];

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
int menu();
int denglu();
void zhuce();
void UpdateLogs();
void showDetail();
void ShowUserLog();
void WriteUserLogToFile();
void DisplayUserLogsFromFile();

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


void showDetail(){
	    Pos(64, 5);
		printf("��F5��ʾ��Ϸ�û���־");
		Pos(64, 3);
		cout << "***" <<login_name << "������Ϸ��***";
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
}

void gamecircle()//������Ϸ        
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
//			int startIndex = 0;
//			UpdateLogs();
//            showUserLogs(allLogs, startIndex);
			DisplayUserLogsFromFile();
		}
        Sleep(sleeptime);
        snakemove();
    }
    
}

void welcometogame()//��ʼ����
{
	time(&timep);
	start_time = timep;

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
    
    int flag = menu();
    
    if(flag == 1);
    {
		creatMap();
    	initsnake();
    	createfood();
    }
}

int menu()//���˵����� 
{
	int select;
	system("cls");
	printf("\n		******************�˵���*****************\n");
	printf("\t\t*\t      1����¼                    *\n");
	printf("\t\t*\t      2��ע��                    *\n");
	printf("\t\t*\t      3���˳���Ϸ\t\t*\n");
	printf("		*****************************************\n");
	printf("\n");
	printf("\t\t������ѡ�[\t]\b\b\b");
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
			printf("��лʹ��\n");
			exit(0);
		default:
			printf("�������!");
			Sleep(1000);
			system("cls");
			menu();
	}
	return flag;
} 

int denglu()//��½���� 
{
	system("cls");
	users a,b;
	FILE* fp = fopen("Users.txt","r");
	int i,flag;//flag��Ϊ���յĽ�� 
	printf("��ӭ������½���棡\n");
	printf("�������˺ţ�\n");
	scanf("%s",&b.username);
	printf("���������룺\n");
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
				printf("�˺Ż��������\n");
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
		printf("%s����ӭ����������\n",a.username);
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
		printf("������󣡣���\n");
		fclose(fp);
		Sleep(1000);
		system("cls");
		menu();	
//		flag = 0;
//		return flag;
	}
//	printf("%d",flag);
}

void zhuce()//ע��ģ��
{
	int i;
	system("cls");
	char c[20];
	users a, b;
	FILE* fp;
	printf("\n\t\t��ӭ����ע����棡\n");
	Sleep(500);
	fp = fopen("Users.txt","r");
	fscanf(fp, "%s %s", b.username, b.password);
	printf("\n\t\t�������û���:[ \t\t ]\b\b\b\b\b\b\b\b\b\b\b\b\b");
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
			printf("���˺��Ѵ��ڣ�����");
			Sleep(1000);
			fclose(fp);
			return;
		}
	}
	int time = 2;
	do
	{
		printf("\t\t����������:[ \t\t ]\b\b\b\b\b\b\b\b\b\b\b\b\b");
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
		printf("\t\t��ȷ�����룺[ \t\t ]\b\b\b\b\b\b\b\b\b\b\b\b\b");
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
			
		    printf("\t\t������ID��");
			scanf("%d",&a.userID);
			fp = fopen("Users.txt", "a");
			fprintf(fp, "%d %s %s\n",a.userID, a.username,a.password); 
//			fwrite(&a,sizeof(users),1,fp);
			printf("%s,ע��ɹ�������", a.username);
			fclose(fp);
			Sleep(1500);
			return;
		}
		else
		{
			if (time != 0)
			{
				printf("�������\n\n�������������룡��\n\n������%d�λ��ᣡ����", time);
				time--;
				continue;
			}
			else
			{
				printf("\n\t\t���������󣬼����˳���");
				fclose(fp);
				Sleep(1500);
				return;
			}
		}
	} while (time >= 0);
	
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
	
    std::ofstream file("GameLogs.txt", std::ios_base::app); // ��׷��ģʽ���ļ�
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
    cout << "��Ϸ�û���־" << endl;


    
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

void gamestart()//��Ϸ��ʼ��
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
