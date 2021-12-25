#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#define R 'R'
#define L 'L'
#define U 'U'
#define D 'D'
typedef struct Node
{
	int x;
	int y;
	struct Node* next;
}snake;

char FORWARD_DIRECTION;   //���嵱ǰǰ���ķ���
snake* head, * p, * nexthead;//��ͷָ�롢����̰���ߵļ��ָ�롢��һ�����ɵ�ͷָ�롢βָ��
snake* food;//������ɵ�ʳ��
int grade = 0;   //��ǰ�÷�
int addGrade = 5;  //�ӵķ������ٶȶ�����
int sleepTime = 250; //̰����ͣ�ٵ�ʱ�䣬��ͨ���޸�ͣ��ʱ��ı�̰����ǰ�����ٶ�

void hideCursor();   //�������ع�꺯��
void gotoxy(int x, int y);  //�ƶ���꺯��
void color(int c); //���ý���������������ɫ
void createMap(); //������ͼ����
void initSnake();  //��ʼ��̰���ߺ���
void speedUp();  //���ٺ���
void speedDown(); //���ٺ���
void createFood();  //�������ʳ�ﺯ��
void collideWall();  //���ײǽ����
void collideSelf();  //����Ƿ�ײ���Լ�
void moveSnake();  //̰�����ƶ�����
void gradeBoard();  //��ʾ�÷ְ庯��
void endGame();   //������Ϸ����
void keyboardControl();   //�����̰�������
void welcometoGame();   //��ʼ������


void hideCursor()   //�������ع�꺯��
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}


void gotoxy(int x, int y) 
{       //�ƶ���굽��Ӧλ��
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


void color(int c) 
{               //���ý���������ַ�����ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void createMap() 
{    //������ͼ
	system("mode con cols=150 lines=30");
	system("cls");
	color(3 | FOREGROUND_INTENSITY);
	for (int i = 1; i <= 40; i++) 
	{  //�ϱ߿�
		printf("��");
	}
	for (int i = 1; i <= 28; i++) 
	{   //���ұ߿�
		gotoxy(0, i);
		printf("��");
		gotoxy(78, i);
		printf("��");
	}
	gotoxy(0, 29);
	for (int i = 1; i <= 40; i++) 
	{  //�ϱ߿�
		printf("��");
	}
}

void initSnake() 
{
	snake* tail;
	color(15);
	head = (snake*)malloc(sizeof(snake));
	head->x = 36;
	head->y = 12;
	p = head;
	for (int i = 1; i <= 4; i++) {

		tail = (snake*)malloc(sizeof(snake));
		tail->x = head->x + 2 * i;
		tail->y = head->y;
		p->next = tail;
		p = tail;
	}
	p->next = NULL;
	p = head;                  //��¼��ͷָ���λ��
	gotoxy(head->x, head->y);
	while (head != NULL) 
	{
		printf("��");
		head = head->next;
	}
	gotoxy(149, 29);
}

void speedUp()    //ͨ����СsleepTime�ӿ��ٶȣ���sleepTime����С��100
{
	if (sleepTime > 50)
	{
		sleepTime = sleepTime - 50; addGrade++;
	}
}

void speedDown()  //ͨ������sleepTime�����ٶȣ���sleepTime���ܴ���500
{
	if (sleepTime < 450)
	{
		sleepTime = sleepTime + 50; addGrade--;
	}
}

void createFood() 
{   //�������ʳ��
	byte i;   
	head = p;
	food = (snake*)malloc(sizeof(snake));
	food->next = NULL;
	while (1)
	{
		i = 0;                             //ÿ��ѭ��ʱ����iΪ0���������������ⲿwhile����ⲿ����������ɵ�ʳ���������ص�ʱ��i����1
										   //��ʱi��һֱΪ1����������ѭ��
		food->x = rand() % 75 + 2;         //ע���ڷ�Χ�����ɵ���ҪΪż����Ҫ����ͷ����
		food->y = rand() % 25 + 2;
		while (head != NULL) 
		{
			if (food->x == head->x && food->y == head->y)
				i = 1;
			head = head->next;
		}                            //����head��ֵ����������ɵ�ʳ���������ص�ʱ���ִ��ⲿwhile�ĵ�һ����俪ʼִ��
		head = p;					 //����ʱ��û������head��ֵ����Ƕ��while��ֱ��������û�ж����ж�
		if (food->x % 2 == 0 && food->y % 2 == 0 && i == 0)
			break;
		
	}
	gotoxy(food->x, food->y);
	color(12 | FOREGROUND_INTENSITY);
	printf("��");
}

void collideWall() {
	if (head->x == 0)
		endGame();
	if (head->x == 78)
		endGame();
	if (head->y == 0)
		endGame();
	if (head->y == 29)
		endGame();
}

void collideSelf() {
	head = head->next;
	while (head != NULL)
	{
		if (head->x == p->x && head->y == p->y)
			endGame();
		head = head->next;
	}
	head = p;
}

void moveSnake() 
{
	color(15);
	head = p;                //��ʼ��ͷָ���λ��
	if (FORWARD_DIRECTION == L) 
	{
		//gotoxy(100, 3);
		nexthead = (snake*)malloc(sizeof(snake));
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;             //ͷָ��ָ���¿����Ŀռ�
		p = head;                    //��¼��ͷָ���ֵ
		collideWall();
		collideSelf();
		gotoxy(head->x, head->y);
		printf("��");
		if (head->x == food->x && head->y == food->y)    //���Ե���ʳ���β�������
		{
			free(food);
			grade = grade + addGrade;
			createFood();
		}
		else {                                           //��û�гԵ�ʳ������β��������ԭ�г���
			while (head->next->next != NULL)
			{      //�˴�ע��
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //ͨ��next�ͷ�β������Ϊֱ���ͷ�β���ᵼ��nextָ�����
			printf("  ");
			free(head->next);
			head->next = NULL;
		}
		head = p;
	}
	if (FORWARD_DIRECTION == U) 
	{
		//gotoxy(100, 3);
		nexthead = (snake*)malloc(sizeof(snake));
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		nexthead->next = head;
		head = nexthead;
		p = head;
		gotoxy(head->x, head->y);
		printf("��");
		collideWall();
		collideSelf();
		if (head->x == food->x && head->y == food->y)
		{
			free(food);
			grade = grade + addGrade;
			createFood();
		}
		else {
			while (head->next->next != NULL)
			{      //�˴�ע��
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //ͨ��next�ͷ�β������Ϊֱ���ͷ�β���ᵼ��nextָ�����
			printf("  ");
			free(head->next);
			head->next = NULL;
		}
		head = p;
	}
	if (FORWARD_DIRECTION == R) 
	{
		//gotoxy(100, 3);
		nexthead = (snake*)malloc(sizeof(snake));
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;
		p = head;
		gotoxy(head->x, head->y);
		printf("��");
		collideWall();
		collideSelf();
		if (head->x == food->x && head->y == food->y)
		{
			free(food);
			grade = grade + addGrade;
			createFood();
		}
		else {
			while (head->next->next != NULL)
			{      //�˴�ע��
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //ͨ��next�ͷ�β������Ϊֱ���ͷ�β���ᵼ��nextָ�����
			printf("  ");
			free(head->next);
			head->next = NULL;
		}
		head = p;
	}
	if (FORWARD_DIRECTION == D) 
	{
		//gotoxy(100, 3);
		nexthead = (snake*)malloc(sizeof(snake));
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		nexthead->next = head;
		head = nexthead;
		p = head;
		gotoxy(head->x, head->y);
		printf("��");
		collideWall();
		collideSelf();
		if (head->x == food->x && head->y == food->y)
		{
			free(food);
			grade = grade + addGrade;
			createFood();
		}
		else {
			while (head->next->next != NULL)
			{      //�˴�ע��
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //ͨ��next�ͷ�β������Ϊֱ���ͷ�β���ᵼ��nextָ�����
			printf("  ");
			free(head->next);
			head->next = NULL;
		}
		head = p;
	}
}

void gradeBoard() 
{
	gotoxy(100, 12);
	color(4);
	printf("�÷֣�%d  ", grade);

	gotoxy(100, 14);
	printf("ÿ��ʳ��÷֣� %d��", addGrade);
	gotoxy(100, 16);
	printf("��ǽ�������Լ���Game Over.");
	gotoxy(100, 18);
	printf("�� �� �� �������ߵ��ƶ�");
	gotoxy(100, 20);
	printf("F1 ���٣�F2 ����");
	gotoxy(100, 22);
	printf("�ո�space����ͣ��Ϸ");
	gotoxy(100, 24);
	printf("ESC ���˳���Ϸ");
}

void endGame() {
	system("cls");
	gotoxy(66, 10);
	color(12 | FOREGROUND_INTENSITY);
	printf("��Ϸ����");
	gotoxy(64, 12);
	printf("���ĵ÷�Ϊ��%d", grade);
	gotoxy(61, 14);
	printf("�ٰ�һ��ESC�˳���Ϸ");
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_ESCAPE))
			exit(0);
	}
}

void keyboardControl() 
{
	FORWARD_DIRECTION = L;
	while (1) 
	{
		gradeBoard();
		if (GetAsyncKeyState(VK_UP) && FORWARD_DIRECTION != D) 
		{
			FORWARD_DIRECTION = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && FORWARD_DIRECTION != U) 
		{
			FORWARD_DIRECTION = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && FORWARD_DIRECTION != R) 
		{
			FORWARD_DIRECTION = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && FORWARD_DIRECTION != L) 
		{
			FORWARD_DIRECTION = R;
		}
		else if (GetAsyncKeyState(VK_F1))          //��F1�ӿ��ٶ�
		{
			speedUp();
		}
		else if (GetAsyncKeyState(VK_F2))            //��F2�����ٶ�
		{
			speedDown();
		}
		else if (GetAsyncKeyState(VK_SPACE))     //�ո����ͣ
		{
			while (1)
			{
				if (GetAsyncKeyState(VK_ESCAPE))    //����ͣʱҲ����ӦESC���˳���Ϸ
				{
					endGame();
				}
				gotoxy(100, 8);
				color(4);
				printf("Pause.");
				Sleep(300);
				if (GetAsyncKeyState(VK_SPACE))
				{
					gotoxy(100, 8);
					printf("Start.");
					break;
				}
			}
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			endGame();
		}
		moveSnake();
		Sleep(sleepTime);     //ͨ���ı�sleepTime��С����̰�����ٶ�
	}
}

void welcometoGame()
{
	char n;
	color(15);
	gotoxy(42, 10);
	printf("Wecome to snake.");
	color(12);
	gotoxy(20, 27);
	printf("��1��ʼ��Ϸ");
	gotoxy(65, 27);
	printf("�������1�����˳���Ϸ");
	color(3 | FOREGROUND_INTENSITY);
	gotoxy(40, 14);
	printf("ÿ��ʳ��÷֣� %d��", addGrade);
	gotoxy(38, 16);
	printf("��ǽ��ҧ���Լ���Game Over.");
	gotoxy(38, 18);
	printf("�� �� �� �������ߵ��ƶ�");
	gotoxy(41, 20);
	printf("F1 ���٣�F2 ����");
	gotoxy(40, 22);
	printf("�ո�space����ͣ��Ϸ");
	gotoxy(43, 24);
	printf("ESC ���˳���Ϸ");
	gotoxy(43, 28);
	printf("please select:");
	n = getch();
	switch (n)
	{
	case '1':
	{
		createMap();
		initSnake();
		createFood();
		keyboardControl();
		break;
	}
	default:exit(0);
	}
}

int main() 
{
	system("mode con cols=100 lines=30");
	hideCursor();
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	//printf("Hello, World");
	//system("cls");
	welcometoGame();


}