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

char FORWARD_DIRECTION;   //定义当前前进的方向
snake* head, * p, * nexthead;//蛇头指针、创建贪吃蛇的间接指针、下一步生成的头指针、尾指针
snake* food;//随机生成的食物
int grade = 0;   //当前得分
int addGrade = 5;  //加的分数随速度而增加
int sleepTime = 250; //贪吃蛇停顿的时间，可通过修改停顿时间改变贪吃蛇前进的速度

void hideCursor();   //定义隐藏光标函数
void gotoxy(int x, int y);  //移动光标函数
void color(int c); //设置接下来输出字体的颜色
void createMap(); //创建地图函数
void initSnake();  //初始化贪吃蛇函数
void speedUp();  //加速函数
void speedDown(); //减速函数
void createFood();  //随机生成食物函数
void collideWall();  //检测撞墙函数
void collideSelf();  //检测是否撞到自己
void moveSnake();  //贪吃蛇移动函数
void gradeBoard();  //显示得分板函数
void endGame();   //结束游戏函数
void keyboardControl();   //检测键盘按键函数
void welcometoGame();   //初始化函数


void hideCursor()   //定义隐藏光标函数
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}


void gotoxy(int x, int y) 
{       //移动光标到相应位置
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


void color(int c) 
{               //设置接下来输出字符的颜色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void createMap() 
{    //创建地图
	system("mode con cols=150 lines=30");
	system("cls");
	color(3 | FOREGROUND_INTENSITY);
	for (int i = 1; i <= 40; i++) 
	{  //上边框
		printf("■");
	}
	for (int i = 1; i <= 28; i++) 
	{   //左右边框
		gotoxy(0, i);
		printf("■");
		gotoxy(78, i);
		printf("■");
	}
	gotoxy(0, 29);
	for (int i = 1; i <= 40; i++) 
	{  //上边框
		printf("■");
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
	p = head;                  //记录下头指针的位置
	gotoxy(head->x, head->y);
	while (head != NULL) 
	{
		printf("■");
		head = head->next;
	}
	gotoxy(149, 29);
}

void speedUp()    //通过减小sleepTime加快速度，但sleepTime不能小于100
{
	if (sleepTime > 50)
	{
		sleepTime = sleepTime - 50; addGrade++;
	}
}

void speedDown()  //通过增大sleepTime减慢速度，但sleepTime不能大于500
{
	if (sleepTime < 450)
	{
		sleepTime = sleepTime + 50; addGrade--;
	}
}

void createFood() 
{   //随机生成食物
	byte i;   
	head = p;
	food = (snake*)malloc(sizeof(snake));
	food->next = NULL;
	while (1)
	{
		i = 0;                             //每次循环时重置i为0。若此条语句放在外部while语句外部，当随机生成的食物与蛇身重叠时，i被置1
										   //此时i就一直为1，将陷入死循环
		food->x = rand() % 75 + 2;         //注意在范围内生成的数要为偶数，要与蛇头对齐
		food->y = rand() % 25 + 2;
		while (head != NULL) 
		{
			if (food->x == head->x && food->y == head->y)
				i = 1;
			head = head->next;
		}                            //重置head的值，当随机生成的食物与蛇身重叠时，又从外部while的第一条语句开始执行
		head = p;					 //，此时若没有重置head的值，内嵌的while将直接跳过，没有二次判断
		if (food->x % 2 == 0 && food->y % 2 == 0 && i == 0)
			break;
		
	}
	gotoxy(food->x, food->y);
	color(12 | FOREGROUND_INTENSITY);
	printf("■");
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
	head = p;                //初始化头指针的位置
	if (FORWARD_DIRECTION == L) 
	{
		//gotoxy(100, 3);
		nexthead = (snake*)malloc(sizeof(snake));
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;             //头指针指向新开创的空间
		p = head;                    //记录下头指针的值
		collideWall();
		collideSelf();
		gotoxy(head->x, head->y);
		printf("■");
		if (head->x == food->x && head->y == food->y)    //若吃到了食物，则尾部不清除
		{
			free(food);
			grade = grade + addGrade;
			createFood();
		}
		else {                                           //若没有吃到食物，则清除尾部，保持原有长度
			while (head->next->next != NULL)
			{      //此处注意
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //通过next释放尾部是因为直接释放尾部会导致next指向混乱
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
		printf("■");
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
			{      //此处注意
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //通过next释放尾部是因为直接释放尾部会导致next指向混乱
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
		printf("■");
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
			{      //此处注意
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //通过next释放尾部是因为直接释放尾部会导致next指向混乱
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
		printf("■");
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
			{      //此处注意
				head = head->next;
			}
			gotoxy(head->next->x, head->next->y);    //通过next释放尾部是因为直接释放尾部会导致next指向混乱
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
	printf("得分：%d  ", grade);

	gotoxy(100, 14);
	printf("每个食物得分： %d分", addGrade);
	gotoxy(100, 16);
	printf("碰墙或碰到自己即Game Over.");
	gotoxy(100, 18);
	printf("↑ ↓ ← →控制蛇的移动");
	gotoxy(100, 20);
	printf("F1 加速，F2 减速");
	gotoxy(100, 22);
	printf("空格space：暂停游戏");
	gotoxy(100, 24);
	printf("ESC ：退出游戏");
}

void endGame() {
	system("cls");
	gotoxy(66, 10);
	color(12 | FOREGROUND_INTENSITY);
	printf("游戏结束");
	gotoxy(64, 12);
	printf("您的得分为：%d", grade);
	gotoxy(61, 14);
	printf("再按一次ESC退出游戏");
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
		else if (GetAsyncKeyState(VK_F1))          //按F1加快速度
		{
			speedUp();
		}
		else if (GetAsyncKeyState(VK_F2))            //按F2减慢速度
		{
			speedDown();
		}
		else if (GetAsyncKeyState(VK_SPACE))     //空格键暂停
		{
			while (1)
			{
				if (GetAsyncKeyState(VK_ESCAPE))    //在暂停时也能响应ESC键退出游戏
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
		Sleep(sleepTime);     //通过改变sleepTime大小更改贪吃蛇速度
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
	printf("按1开始游戏");
	gotoxy(65, 27);
	printf("按任意除1按键退出游戏");
	color(3 | FOREGROUND_INTENSITY);
	gotoxy(40, 14);
	printf("每个食物得分： %d分", addGrade);
	gotoxy(38, 16);
	printf("碰墙或咬到自己即Game Over.");
	gotoxy(38, 18);
	printf("↑ ↓ ← →控制蛇的移动");
	gotoxy(41, 20);
	printf("F1 加速，F2 减速");
	gotoxy(40, 22);
	printf("空格space：暂停游戏");
	gotoxy(43, 24);
	printf("ESC ：退出游戏");
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