#include"pch.h"
#include<stdio.h>
#include <Windows.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<graphics.h>

#define SU 49
#define SD 50
#define B  32
#define U  72
#define D  80
#define L  75
#define R  77                         //功能键设置

#define S 20                          //元素的边长

#define X2 600                        //地图的宽
#define Y 600                         //界面以及地图的高
#define X1 1000                       //界面的宽


void move();                         //蛇的移动
void gameface();                     //游戏进入界面
void game_interface();               //游戏选择界面
int N=0;                             //用来记录关卡难度
int M = 0;                           //用来记录选择的地图  
void choose_game();                  //难度选择界面
void choose_map();                   //地图选择界面
void set_file();                     //建立用户档案
void ranking_list();                 //打开排行榜
void list_change();                  //用来将用户的分数录入旁行榜
void MAP_print();                    //地图打印函数
void MAP_print2(int n);              //打印第二关的地图
void print3();                       //继续游戏的打印
void game_two();                     //进入第二关
void store_game();                   //存储游戏
void snake_body_print();             //打印蛇的身体
void snake_begin_body();             //初始化蛇的身体
int check(int x, int y);             //检查在地图二维数组上的值
int check_snake_body(int x, int y);  //检验生成物是否与蛇身重合
void creat_food();                   //生成食物
void creat_poison();                 //生成毒草
void clear_poison();                 //清除毒草
int poi_x[6], poi_y[6];              //记录毒草的坐标
void creat_bomb();                   //生成炸弹
void clear_bomb();                   //清除炸弹
int bom_x[3], bom_y[3];              //记录炸弹的坐标
void snake_elongate();               //吃了食物之后，蛇身加长
void snake_shorten();                //吃了毒草之后，蛇身变短
void snake_halve();                  //吃到炸弹后，体长减半
int eat_self(int x, int y);          //判断蛇是否咬到自己
void game_shows();                   //游戏说明
void game_over();                    //游戏结束
int MAP[30][30];                     //地图中数字的含义。0：空。 1：墙。 2：食物。 3：毒草。 4:地雷。 6：蛇身 7：穿越石/
struct Snake * head, *tail, *p;      //蛇的结构体
struct Users_score;                  //用户名及其分数，准备录入旁行榜
int score = 0;                       //分数
int speed = 0;                       //用来控制蛇的移动速度
int twinkle = 1;                     //用来控制毒草闪烁               
int joint = 3;                       //用来记录蛇的身体节数
char joint1[20];                     //转化成字符输入
int food_flag = 4, flag1;            //用来检测食物是否完全被吃完
int poison_flag, flag2;              //用来检测毒草是否全被吃完
int bomb_flag, flag3;                //用来检测炸弹是否被吃
char name[20];                       //用来记录用户的名字
int level;                           //用来记录闯关的关卡数
int key;                             //键盘响应
FILE * fp1, *fp2, *fp3;              //用来指向该用户的文件
IMAGE body, bomb, poison, food, chuan, qiang;


void picture() {
	loadimage(&qiang, _T("picture\\墙.jpg"), S, S);
	loadimage(&chuan, _T("picture\\穿越石.jpg"), S, S);
	loadimage(&body, _T("picture\\蛇.jpg"), S, S);
	loadimage(&poison, _T("picture\\毒草.jpg"), S, S);
	loadimage(&bomb, _T("picture\\炸弹.jpg"), S, S);
	loadimage(&food, _T("picture\\食物.jpg"), S, S);
}

struct Snake
{
	int x;
	int y;
	struct Snake * previous;
	struct Snake * next;
};

struct store
{
	int headx;
	int heady;
	int tailx;
	int taily;
	int score;
	int joint;
	int speed;
	int n;
	int k;
	int a[30][30];
}store;

struct Users_score {
	char name[20];
	char score[20];
}users[6], users1[6], user, a1;

void store_game()
{
	int i, j;
	store.k = key;
	store.headx = head->x;
	store.heady = head->y;
	store.tailx = tail->x;
	store.taily = tail->y;
	store.score = score;
	store.joint = joint;
	store.speed = speed;
	store.n = N;
	for (i = 0; i <= 29; i++)
		for (j = 0; j <= 29; j++)
			store.a[i][j] = MAP[i][j];
	FILE *fp;
	fopen_s(&fp, "store game", "w");
	fwrite(&store, sizeof(struct store), 1, fp);
	fclose(fp);
}

void set_file()
{
	IMAGE ft1;
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\初始3.jpg"), X1, Y);
	RECT r = { 200,50,700,70 };
	settextstyle(20, 0, _T("宋体"));
	settextcolor(LIGHTBLUE);
	drawtext(_T("请输入您的昵称(20个\"英文\"字符以内, 不能包含空格):"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	scanf_s("%s", user.name, 20);
	fopen_s(&fp1, user.name, "a");
	fputs("\n", fp1);
	fclose(fp1);
}

void ranking_list()
{
	cleardevice();
	IMAGE ft1;
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\通用.jpg"), X1, Y);
	int i, j;
	fopen_s(&fp2, "Ranking list", "r");
	settextstyle(30, 0, _T("宋体"));
	settextcolor(RED);
	for (i = 0; i <=5; i++)
	{
		fscanf_s(fp2, "%s\n", users[i].name, 20);
		outtextxy(100, 100+30*i, users[i].name);
		fscanf_s(fp2, "%s\n", users[i].score, 20);
		outtextxy(200, 100+30*i, users[i].score);
	}
	fclose(fp2);
}

void list_change()
{
	int i, flag1 = 0;
	fopen_s(&fp3, "Ranking list", "r+");
	for (i = 5; i >=0; i--)
	{
		fscanf_s(fp3, "%s\n", users[i].name, 20);
		fscanf_s(fp3, "%s\n", users[i].score, 20);
		users1[i] = users[i];
		if (atoi(user.score) <= atoi(users[i].score) && i == 0) flag1 = 1;
	}
	fclose(fp3);
	if (flag1 == 0)
	{
		users1[0] = user;
		for (i = 0; i<=4 ; i++)
		{
			if (atoi(users1[i].score) > atoi(users1[i + 1].score))
			{
				a1 = users1[i];
				users1[i] = users1[i + 1];
				users1[i + 1] = a1;
			}
			else break;
		}
		for (i = 0; i <= 5; i++)
		{
			users[i] = users1[i];
		}
		fopen_s(&fp3, "Ranking list", "w");
		for (i = 5; i >=0; i--)
		{
			fprintf(fp3, "%s\n", users1[i].name);
			fprintf(fp3, "%s\n", users1[i].score);
		}
		fclose(fp3);
		return;
	}
	else if (flag1 == 1) return;
}

void snake_begin_body()
{
	int j, i;
	struct Snake *p1, *p2;
	p1 = p2 = (struct Snake *)malloc(sizeof(struct Snake));
	tail = NULL;
	head = NULL;
	for (j = 1; p2->next != NULL; j++)
	{
		p1->x = (280 + S * j); p1->y = 300;
		if (j == 1) head = p1;
		else if (j == 3) { tail = p1; p1->next = NULL; p2->next = p1; }
		else p2->next = p1;

		if (j == 1) p1->previous = NULL;
		else p1->previous = p2;

		p2 = p1;
		p1 = (struct Snake *) malloc(sizeof(struct Snake));
	}
	for (p1 = head->next, i = 1; p1 != NULL; p1 = p1->next, i++)
	{
		MAP[p1->x / S][p1->y/ S] = 6;
	}
}

void snake_body_print()
{
	for (p = head;p != NULL; p = p->next)
	{
		putimage(p->x, p->y,&body);
	}
}

void MAP_print()
{
	initgraph(X1, Y);
	loadimage(NULL, _T("picture\\地图背景.jpg"), X1, Y);
	clearrectangle(0, 0, X2, Y);
	int i, j;
	for (i = 0; i <=29 ; i++)
		for (j = 0; j <= 29; j++)
			MAP[i][j] = 0;
	for (i = 0; i <= 29; i++)
	{
		MAP[i][0] = 1; MAP[i][29] = 1;
	}
	for (j = 0; j <= 29; j++)
	{
		MAP[0][j] = 1; MAP[29][j] = 1;
	}
	switch (M) {
	case 1:
		break;
	case 2:
		for (i = 1; i <= 3; i++)
			for (j = 12; j <= 16; j++)
			{
				MAP[i][j] = 1; MAP[j][i] = 1; MAP[29 - i][j] = 1; MAP[j][29 - i] = 1;
			}
		break;
	case 3:
		for (i = 1; i <= 3; i++)
			for (j = 12; j <= 16; j++)
			{
				MAP[i][j] = 1; MAP[j][i] = 1; MAP[29 - i][j] = 1; MAP[j][29 - i] = 1;
			}
		MAP[6][6] = 1; MAP[6][24] = 1; MAP[24][24] = 1; MAP[24][6] = 1;
		break;
	default:
		break;
	}
	for (i = 0; i <= 29; i++)
	{
		for (j = 0; j <= 29; j++)
		{
			if (MAP[i][j] == 1)
			{
				putimage(S * i, S * j, &qiang);
			}
		}
	}
}

void MAP_print2(int n)
{
	int i, j;
	initgraph(X1, Y);
	loadimage(NULL, _T("picture\\地图背景.jpg"), X1, Y);
	clearrectangle(0, 0, X2, Y);
	for (i = 0; i <= 29; i++)
		for (j = 0; j <= 29; j++)
			MAP[i][j] = 0;
	switch(n)
	{
	case 4:
		for (i = 0; i <= 29; i++)
		{
			MAP[0][i] = 1; MAP[i][0] = 1; MAP[29][i] = 1; MAP[i][29] = 1;
		}
		for (i = 1; i <= 3; i++)
			for (j = 12; j <= 16; j++)
			{
				MAP[i][j] = 1; MAP[j][i] = 1; MAP[29 - i][j] = 1; MAP[j][29 - i] = 1;
			}
		MAP[6][6] = 7; MAP[24][24] = 7;
		break;
	case 5:
		for (i = 0; i <= 29; i++)
		{
			MAP[i][0] = 1; MAP[i][29] = 1;
		}

		for (i = 0; i <= 3; i++)
			for (j = 12; j <= 16; j++)
			{
				MAP[i][j] = 1; MAP[j][i] = 1; MAP[29 - i][j] = 1; MAP[j][29 - i] = 1;

			}
		MAP[6][6] = 1; MAP[6][24] = 1; MAP[24][24] = 1; MAP[24][6] = 1;
		break;
	}
	for (i = 0; i <= 29; i++)
	{
		for (j = 0; j <= 29; j++)
		{
			if (MAP[i][j] == 1)putimage(S*i, S*j, &qiang);
			else if (MAP[i][j] == 7)putimage(S*i, S*j, &chuan);
		}
	}
}

void print3() {
	initgraph(X1, Y);
	loadimage(NULL, _T("picture\\地图背景.jpg"), X1, Y);
	clearrectangle(0, 0, X2, Y);
	int i, j;
	for(i=0;i<=29;i++)
		for (j = 0; j <= 29; j++)
		{
			if (MAP[i][j] == 1)putimage(S*i, S*j, &qiang);
			else if (MAP[i][j] == 2)putimage(S*i, S*j, &food);
			else if (MAP[i][j] == 3)putimage(S*i, S*j, &poison);
			else if (MAP[i][j] == 4)putimage(S*i, S*j, &bomb);
			else if (MAP[i][j] == 6)putimage(S*i, S*j, &body);
			else if (MAP[i][j] == 7)putimage(S*i, S*j, &chuan);
		}
	putimage(head->x, head->y, &body);
	_itoa_s(score, user.score, 10);
	_itoa_s(joint, joint1, 10);
	RECT h = { X2 + 20,50,X2 + 80,70 };
	settextstyle(20, 0, _T("宋体"));
	settextcolor(RED);
	drawtext(_T("分数："), &h, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	outtextxy(X2 + 100, 50, user.score);
	RECT g = { X2 + 20,80,X2 + 80,100 };
	drawtext(_T("节数："), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	outtextxy(X2 + 100, 80, joint1);
	RECT l = { X2 + 20,110,X2 + 80,130 };
	drawtext(_T("关卡："),&l, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT m = { X2 + 80,110,X2 + 120,130 };
	if(N==1||N==2||N==3)	drawtext(_T("1"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else 	drawtext(_T("2"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
}

void gameface()
{
	IMAGE ft1;
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\初始2.1.jpg"),X1,Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.2.jpg"), X1,Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.3.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.4.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.5.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.6.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.7.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.8.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.9.jpg"), X1, Y);	
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.10.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.11.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.12.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.13.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.14.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.15.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始2.16.jpg"), X1, Y);
	Sleep(100);
	loadimage(NULL, _T("picture\\初始3.jpg"), X1, Y);
	RECT r = { 350,50,650,90 };
	settextstyle(40, 0, _T("宋体"));
	settextcolor(LIGHTBLUE);
	drawtext(_T("贪  吃  蛇"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT b = { 410,100,590,140 };
	settextstyle(40, 0, _T("宋体"));
	drawtext(_T("大作战"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT a = { 300,450,700,500 };
	settextstyle(20, 0, _T("楷体"));
	while (1)
	{

		settextcolor(RED);
		drawtext(_T("按任意键继续"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Sleep(100);
		if (_kbhit()) break;
		settextcolor(BLUE);
		drawtext(_T("按任意键继续"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Sleep(100);
		if (_kbhit()) break;
		settextcolor(CYAN);
		drawtext(_T("按任意键继续"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Sleep(100);
		if (_kbhit()) break;
	}
	_getch();
}

void choose_game()
{
	cleardevice();
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\难度选择1.jpg"),X1,Y);
	RECT a = { 400,180,600,205 };
	settextcolor(RGB(253, 153, 179));
	settextstyle(25, 0, _T("宋体"));
	drawtext(_T("简单"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT b = { 400,225,600,250 };
	settextcolor(BLUE);
	drawtext(_T("中等"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT c = { 400,270,600,295 };
	settextcolor(GREEN);
	drawtext(_T("难"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	MOUSEMSG m;
	RECT i = { 800,500,870,550 };
	while (true)
	{
		m = GetMouseMsg();
		settextcolor(BLACK);
		drawtext(_T("返回"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			if (m.x >= 800 && m.x <= 870 && m.y >= 500 && m.y <= 550)
			{
				settextcolor(RED);
				drawtext(_T("返回"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RGB(253, 153, 179));
				drawtext(_T("简单"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(BLUE);
				drawtext(_T("中等"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(GREEN);
				drawtext(_T("难"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 180 && m.y <= 205)
			{
				settextcolor(RED);
				drawtext(_T("简单"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(BLUE);
				drawtext(_T("中等"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(GREEN);
				drawtext(_T("难"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 225 && m.y <= 250)
			{
				settextcolor(RGB(253, 153, 179));
				drawtext(_T("简单"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RED);
				drawtext(_T("中等"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(GREEN);
				drawtext(_T("难"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 270 && m.y <= 295)
			{
				settextcolor(RGB(253, 153, 179));
				drawtext(_T("简单"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(BLUE);
				drawtext(_T("中等"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RED);
				drawtext(_T("难"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else 
			{
				settextcolor(RGB(253, 153, 179));
				drawtext(_T("简单"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(BLUE);
				drawtext(_T("中等"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(GREEN);
				drawtext(_T("难"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			break;
		case WM_LBUTTONDOWN:
			if (m.x >= 400 && m.x <= 600 && m.y >= 180 && m.y <= 205)
			{
				N = 1;
				speed = 150;
				poison_flag = 3;
				bomb_flag = 0;
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 225 && m.y <= 250)
			{
				N = 2;
				speed = 100;
				poison_flag = 4;
				bomb_flag = 2;
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 270 && m.y <= 295)
			{
				N = 3;
				speed = 75;
				poison_flag = 6;
				bomb_flag = 3;
			}
			if (m.x >= 800 && m.x <= 870 && m.y >= 500 && m.y <= 550)
			{
				game_interface();
			}
			break;
		}
		if (speed != 0) break;
	}
}

void choose_map()
{
	M = 0;
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\通用.jpg"), X1, Y);
	IMAGE a, b, c;
	loadimage(&a, _T("picture\\地图1.png"), 300, 300);
	loadimage(&b, _T("picture\\地图2.png"), 300, 300);
	loadimage(&c, _T("picture\\地图3.png"), 300, 300);
	putimage(0, 0, &a);
	putimage(350, 0, &b);
	putimage(700, 0, &c);
	RECT d, e, f;
	d = { 100,400,200,450 };
	e = { 450,400,550,450 };
	f = { 800,400,900,450 };
	settextcolor(BLACK);
	drawtext(_T("地图一"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("地图二"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("地图三"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			if (m.x >= 100 && m.x <= 200 && m.y >= 400 && m.y <= 450)
			{
				settextcolor(RED);
				drawtext(_T("地图一"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(BLACK);
				drawtext(_T("地图二"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				drawtext(_T("地图三"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 450 && m.x <= 550 && m.y >= 400 && m.y <= 450)
			{
				settextcolor(BLACK);
				drawtext(_T("地图一"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RED);
				drawtext(_T("地图二"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(BLACK);
				drawtext(_T("地图三"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 800 && m.x <= 900 && m.y >= 400 && m.y <= 450)
			{
				settextcolor(BLACK);
				drawtext(_T("地图一"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				drawtext(_T("地图二"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RED);
				drawtext(_T("地图三"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			break;
		case WM_LBUTTONDOWN:
			if (m.x >= 100 && m.x <= 200 && m.y >= 400 && m.y <= 450)
			{
				M = 1;
			}
			else if (m.x >= 450 && m.x <= 550 && m.y >= 400 && m.y <= 450)
			{
				M = 2;
			}
			else if (m.x >= 800 && m.x <= 900 && m.y >= 400 && m.y <= 450)
			{
				M = 3;
			}
			break;
		}
		if (M!= 0) break;
	}
}

void game_shows()
{
	IMAGE a, b, c, d, e;
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\游戏说明3.jpg"), X1, Y);
}

void game_interface()
{
	score = 0;
	speed = 0;
	N = 0;
	joint = 3;
	poison_flag = 0;
	bomb_flag = 0;
	int flag = 0;
	initgraph(X1,Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\初始3.jpg"),X1,Y);
	RECT r = { 350,50,650,90 };
	settextstyle(40, 0, _T("宋体"));
	settextcolor(LIGHTBLUE);
	drawtext(_T("贪  吃  蛇"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT b = { 410,100,590,140 };
	settextstyle(40, 0, _T("宋体"));
	drawtext(_T("大作战"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT c = { 400,350,600,370 };
	settextstyle(20, 0, _T("楷体"));
	settextcolor(RED);
	drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT d = { 400,380,600,400 };
	drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT e = { 400,410,600,430 };
	drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT f = { 400,440,600,460 };
	drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT g = { 400,470,600,490 };
	drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT q = { 400,500,600,520 };
	drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	MOUSEMSG m;
	while (true)
	{
		 m= GetMouseMsg();
		 switch (m.uMsg)
		 {
		 case WM_MOUSEMOVE:
			 if (m.x >= 400 && m.x <= 600 && m.y >= 350 && m.y <= 370)
			 {
				 settextcolor(BLUE);
				 drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 settextcolor(RED);
				 drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 380 && m.y <= 400) {
				 settextcolor(RED);
				 drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 settextcolor(BLUE);
				 drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 410 && m.y <= 430) {
				 settextcolor(RED);
				 drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 settextcolor(BLUE);
				 drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 440 && m.y <= 460) {
				 settextcolor(RED);
				 drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 settextcolor(BLUE);
				 drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 470 && m.y <= 490) {
				 settextcolor(RED);
				 drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 settextcolor(BLUE);
				 drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 500 && m.y <= 520) {
				 settextcolor(RED);
				 drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("二、游戏说明"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("三、地图选择"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("四、排行榜"), &f, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 drawtext(_T("五、继续游戏"), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				 settextcolor(BLUE);
				 drawtext(_T("六，退出游戏"), &q, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			 }
			 break;
		 case WM_LBUTTONDOWN:
			 if (m.x >= 400 && m.x <= 600 && m.y >= 350 && m.y <= 370) 
			 {
				 choose_game();
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 380 && m.y <= 400) 
			 {
				 cleardevice();
				 initgraph(1005, 555);
				 setbkmode(TRANSPARENT);
				 loadimage(NULL, _T("picture\\通用.jpg"), 1005, 555);
				 settextstyle(20, 0, _T("楷体"));
				 game_shows();
				 RECT i = { 800,500,870,550 };
				 while (flag == 0)
				 {
					 m = GetMouseMsg();
					 settextcolor(BLACK);
					 drawtext(_T("返回"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					 switch (m.uMsg)
					 {
					 case WM_MOUSEMOVE:
						 if (m.x >= 800 && m.x <= 870 && m.y >= 500 && m.y <= 550)
							 settextcolor(RED);
						 drawtext(_T("返回"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						 break;
					 case WM_LBUTTONDOWN:
						 if (m.x >= 800 && m.x <= 870 && m.y >= 500 && m.y <= 550)
							 flag = 1;
						 break;
					 }
				 }
				 flag = 0;
				 game_interface();
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 410 && m.y <= 430) 
			 {
				 cleardevice();
				 choose_map();
				 game_interface();
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 440 && m.y <= 460) 
			 {
				 ranking_list();
				 RECT i = { 800,500,870,550 };
				 while (flag == 0)
				 {
					 m = GetMouseMsg();
					 settextcolor(BLACK);
					 drawtext(_T("返回"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					 switch (m.uMsg)
					 {
					 case WM_MOUSEMOVE:
						 if (m.x >= 800 && m.x <= 870 && m.y >= 500 && m.y <= 550)
							 settextcolor(RED);
						 drawtext(_T("返回"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						 break;
					 case WM_LBUTTONDOWN:
						 if (m.x >= 800 && m.x <= 870 && m.y >= 500 && m.y <= 550)
							 flag = 1;
						 break;
					 }
				 }
				 flag = 0;
				 game_interface();
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 470 && m.y <= 490) 
			 {
				 cleardevice();
				 int  o,p;
				 FILE *fp;
				 fopen_s(&fp, "store game", "r");
				 fread(&store, sizeof(struct store), 1, fp);
				 N = store.n;
				 head->x = store.headx;
				 head->y = store.heady;
				 tail->x = store.tailx;
				 tail->y = store.taily;
				 score = store.score;
				 joint = store.joint;
				 speed = store.speed;
				 for (o = 0; o <= 29; o++)
					 for (p = 0; p <= 29; p++)
						 MAP[o][p] = store.a[o][p];
				 fclose(fp);
				 print3();
				 system("pause");
				 game_interface();
			 }
			 else if (m.x >= 400 && m.x <= 600 && m.y >= 500 && m.y <= 520) {
				 exit(0);
			 }
			 break;
		 }
		 if (speed != 0) break;
	}
}

void creat_food()
{
	flag1 = food_flag;
	int x[4], y[4], i;
	for (i = 0; i <= 3; i++)
	{
		x[i] = (rand() % 30) * S;
		y[i] = (rand() % 30) * S;
	}
	for (i = 0; i <= 3; i++)
	{
		if (MAP[x[i] / S][y[i]/S] == 0)
		{
			if (check_snake_body(x[i], y[i]))
			{
				MAP[x[i] / S][y[i] / S] = 2;
				IMAGE pi2;
				putimage(x[i], y[i], &food);
			}
		}
		if (MAP[x[i] / S][y[i] / S] != 2)
		{
			x[i] = (rand() % 30) * S;
			y[i] = (rand() % 30) * S;
			i--;
		}
	}
}

void creat_poison()
{
	flag2 = poison_flag;
	int i;
	for (i = 0; i <= poison_flag - 1; i++)
	{
		poi_x[i] = (rand() % 30) * S;
		poi_y[i] = (rand() % 30) * S;
	}
	for (i = 0; i <= poison_flag - 1; i++)
	{
		if (MAP[poi_x[i] / S][poi_y[i] / S] == 0)
		{
				MAP[poi_x[i] / S][poi_y[i] / S] = 3;
				IMAGE pi2;
				loadimage(&pi2, _T("picture\\毒草.jpg"), S, S);
				putimage(poi_x[i], poi_y[i], &pi2);
		}
		else if (MAP[poi_x[i] / S][poi_y[i]/ S] != 3)
		{
			poi_x[i] = (rand() % 30) * S;
			poi_y[i] = (rand() % 30) * S;
			i--;
		}
	}
}

void clear_poison()
{
	int i ;
	for (i = 0; i <= poison_flag - 1; i++)
	{
		if (MAP[poi_x[i] / S][poi_y[i]/S] == 3)
		{
			HRGN rgn = CreateRectRgn(poi_x[i],poi_y[i],poi_x[i]+S,poi_y[i]+S);
			setcliprgn(rgn);
			clearcliprgn();
			DeleteObject(rgn);
			setcliprgn(NULL);
			MAP[poi_x[i] / S][poi_y[i]/S] = 0;
		}
	}
}

void creat_bomb()
{
	flag3 = bomb_flag;
	int i;
	for (i = 0; i <= bomb_flag - 1; i++)
	{
		bom_x[i] = (rand() % 30) * S;
		bom_y[i] = (rand() % 30) * S;
	}
	for (i = 0; i <= bomb_flag - 1; i++)
	{
		if (MAP[bom_x[i] / S][bom_y[i] / S] == 0)
		{
			MAP[bom_x[i] / S][bom_y[i] / S] = 4;
			IMAGE pi2;
			loadimage(&pi2, _T("picture\\炸弹.jpg"), S, S);
			putimage(bom_x[i], bom_y[i], &pi2);
		}
		if (MAP[bom_x[i] / S][bom_y[i]/S] != 4)
		{
			bom_x[i] = (rand() % 30) * S;
			bom_y[i] = (rand() % 30) * S;
			i--;
		}
	}
}

void clear_bomb()
{
	int i, j;
	for (i = 0; i <= bomb_flag - 1; i++)
	{
		if (MAP[bom_x[i] / S][bom_y[i]/S] == 4)
		{
			clearrectangle(bom_x[i], bom_y[i], bom_x[i] + S, bom_y[i] + S);
			MAP[bom_x[i] / S][bom_y[i]/S] = 0;
		}
	}
}

int check(int x, int y)
{
	if (MAP[x / S][y / S] == 0) return 0;
	else if (MAP[x / S][y / S] == 1) return 1;
	else if (MAP[x / S][y / S] == 2) return 2;
	else if (MAP[x / S][y / S] == 3) return 3;
	else if (MAP[x / S][y / S] == 4) return 4;
	else if (MAP[x / S][y / S] == 7) return 7;
}

int check_snake_body(int x, int y)
{
	if (MAP[x / S][y / S] == 6) return 0;                    //重合//
	else if (x == head->x &&y == head->y) return 0;          //重合//
	else return 1;                                           //不重合// 
}

int eat_self(int x, int y)
{
	if (MAP[x / S][y / S] == 6) return 0;
	else return 1;
}

void game_two() {
	int i, j;
	void move();
	score = 0;
	joint = 3;
	if (N == 2) N = 4;
	else if (N == 3) N = 5;
	cleardevice();
	for (i = 0; i <= 29; i++)
		for (j = 0; j <= 29; j++)
			MAP[i][j] = 0;
	MAP_print2(N);
	snake_begin_body();
	snake_body_print();
	creat_food();
	creat_poison();
	creat_bomb();
	move();
}

void game_over()
{
	_itoa_s(score, user.score, 10);
	cleardevice();
	IMAGE ft1;
	initgraph(X1, Y);
	setbkmode(TRANSPARENT);
	loadimage(NULL, _T("picture\\初始3.jpg"), X1, Y);
	RECT r = { 350,50,650,90 };
	settextstyle(40, 0, _T("宋体"));
	settextcolor(LIGHTBLUE);
	drawtext(_T("GAME..."), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT b = { 350,100,650,140 };
	drawtext(_T("OVER!!"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT a = { 350,200,550,220 };
	settextstyle(20, 0, _T("宋体"));
	drawtext(_T("您的分数："), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	outtextxy(555, 200, user.score);
	list_change();
	joint = 3;
	score = 0;
	system("pause");
	cleardevice();
}

void move()
{
	key = 1;
	int flag2 = 0;                    //控制游戏存储
	int flag1 = 0;                    //控制光标闪烁
	MOUSEMSG m;
	int k;                            //用来控制及毒草闪烁
	int flag = 0;                     //用flag来表示check的结果//
	int n1 = 0;                       //采用n来记录蛇的运动方向。1：正在向右 2：正在向左 3：正在向下 4：正在向上。//
	RECT i = { X2 + 20,0,X2 + 300,20 };
	settextstyle(20, 0, _T("宋体"));
	settextcolor(RED);
	drawtext(_T("按ESC键退出游戏"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	while (flag2==0)
	{
		_itoa_s(score, user.score, 10);
		_itoa_s(joint, joint1, 10);
		RECT h = { X2 + 20,50,X2 + 80,70 };
		settextstyle(20, 0, _T("宋体"));
		settextcolor(RED);
		drawtext(_T("分数："), &h, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		outtextxy(X2 + 100, 50, user.score);
		RECT g = { X2 + 20,80,X2 + 80,100 };
		drawtext(_T("节数："), &g, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		outtextxy(X2 + 100, 80, joint1);
		if (joint <= 2 || score < 0 || check(head->x, head->y) == 1)
		{
			game_over();
			RECT r = { 350,50,650,90 };
			settextstyle(40, 0, _T("宋体"));
			settextcolor(LIGHTBLUE);
			drawtext(_T("贪  吃  蛇"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		else if (eat_self(head->x, head->y) == 0)
		{
			game_over();
			RECT b = { 410,100,590,140 };
			settextstyle(40, 0, _T("宋体"));
			drawtext(_T("大作战"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		else if (check(head->x, head->y) == 4 && joint <= 4) {
			game_over();
			RECT c = { 400,350,600,370 };
			settextstyle(20, 0, _T("楷体"));
			settextcolor(RED);
			drawtext(_T("一、开始游戏"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		else if (score >= 300 || joint >= 20)
		{
			cleardevice();
			_itoa_s(score, user.score, 10);
			initgraph(X1, Y);
			setbkmode(TRANSPARENT);
			loadimage(NULL, _T("picture\\通用.jpg"), X1, Y);
			RECT r = { 350,50,650,90 };
			settextstyle(40, 0, _T("宋体"));
			settextcolor(LIGHTBLUE);
			drawtext(_T("THE GAME"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT b = { 350,100,650,140 };
			drawtext(_T("VICTORY！！"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT a = { 350,200,550,220 };
			settextstyle(20, 0, _T("宋体"));
			drawtext(_T("分数："), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			outtextxy(555, 200, user.score);
			list_change();
			if (N == 2 || N == 3)
			{
				level++;
				RECT c = { 350,300,750,320 };
				settextstyle(20, 0, _T("宋体"));
				drawtext(_T("恭喜您，进入第二关了！！："), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextstyle(40, 0, _T("宋体"));
				RECT i = { 450,500,550,540 };
				while (flag1 == 0)
				{
					m = GetMouseMsg();
					settextcolor(BLACK);
					drawtext(_T("进入"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					switch (m.uMsg)
					{
					case WM_MOUSEMOVE:
						if (m.x >= 450 && m.x <= 550 && m.y >= 500 && m.y <= 540)
							settextcolor(RED);
						drawtext(_T("进入"), &i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						break;
					case WM_LBUTTONDOWN:
						if (m.x >= 450 && m.x <= 550 && m.y >= 500 && m.y <= 540)
							flag1 = 1;
						break;
					}
				}
				game_two();
				break;
			}
			else if (N == 4 || N == 5)
			{
				cleardevice();
				_itoa_s(score, user.score, 10);
				initgraph(X1, Y);
				setbkmode(TRANSPARENT);
				loadimage(NULL, _T("picture\\通用.jpg"), X1, Y);
				settextstyle(40, 0, _T("宋体"));
				settextcolor(LIGHTBLUE);
				drawtext(_T("THE GAME"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				drawtext(_T("VICTORY!!"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				RECT z = { 350,400,650,450 };
				settextstyle(20, 0, _T("宋体"));
				drawtext(_T("按任意键返回主界面"), &z, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				system("pause");
			}
			break;
		}
		else if (check(head->x, head->y) == 0)
		{
			if (_kbhit())
			{
				key = _getch();
			}
			switch (key)
			{
			case U:
				if (n1 != 3)
				{
					MAP[head->x / S][head->y /S] = 6;
					MAP[tail->x / S][tail->y/S] = 0;
					clearrectangle(tail->x, tail->y, tail->x + S, tail->y + S);
					putimage(head->x, head->y - S, &body);
					p = tail->previous;
					tail->x = head->x;
					tail->y = head->y - S;
					tail->previous = NULL;
					tail->next = head;
					head->previous = tail;
					head = tail;
					tail = p;
					tail->next = NULL;
					n1 = 4;
					Sleep(speed);
					break;
				}
				else
				{
					key = D; break;
				}
			case D:
				if (n1 != 4)
				{
					MAP[head->x / S][head->y/S] = 6;
					MAP[tail->x / S][tail->y/S] = 0;
					clearrectangle(tail->x, tail->y, tail->x + S, tail->y + S);
					putimage(head->x, head->y + S, &body);
					p = tail->previous;
					tail->x = head->x;
					tail->y = head->y + S;
					tail->previous = NULL;
					tail->next = head;
					head->previous = tail;
					head = tail;
					tail = p;
					tail->next = NULL;
					n1 = 3;
					Sleep(speed);
					break;
				}
				else
				{
					key = U; break;
				}
			case L:
				if (n1 != 1)
				{
					MAP[head->x / S][head->y/S] = 6;
					MAP[tail->x / S][tail->y/S] = 0;
					clearrectangle(tail->x, tail->y, tail->x + S, tail->y + S);
					putimage(head->x-S, head->y, &body);
					p = tail->previous;
					tail->x = head->x - S;
					tail->y = head->y;
					tail->previous = NULL;
					tail->next = head;
					head->previous = tail;
					head = tail;
					tail = p;
					tail->next = NULL;
					n1 = 2;
					Sleep(speed);
					break;
				}
				else
				{
					key = R; break;
				}
			case R:
				if (n1 != 2)
				{
					MAP[head->x / S][head->y/S] = 6;
					MAP[tail->x / S][tail->y/S] = 0;
					clearrectangle(tail->x, tail->y, tail->x + S, tail->y + S);
					putimage(head->x+S, head->y, &body);
					p = tail->previous;
					tail->x = head->x + S;
					tail->y = head->y;
					tail->previous = NULL;
					tail->next = head;
					head->previous = tail;
					head = tail;
					tail = p;
					tail->next = NULL;
					n1 = 1;
					Sleep(speed);
					break;
				}
				else
				{
					key = L; break;
				}
			case 32:
				while (_kbhit()==0) {
					key = _getch();
					if (_kbhit() == 1) break;
				}
				break;
			case 27:
			{
				store_game();
				flag2 = 1;
				break;
			}
			default:
				break;
			}
		}
		else if (check(head->x, head->y) == 2)
		{
			snake_elongate();
		}
		else if (check(head->x, head->y) == 3)
		{
			snake_shorten();
		}
		else if (check(head->x, head->y) == 4)
		{
			snake_halve();
		}
		if (N == 4 && check(head->x, head->y) == 7)
		{
			if (head->x == 120 && n1 == 1) { putimage(head->x, head->y, &chuan); head->x = 500; head->y = 480; putimage(head->x, head->y, &body); }
			else if (head->x == 120 && n1 == 2) { putimage(head->x, head->y, &chuan); head->x = 460; head->y = 480;  putimage(head->x, head->y, &body);}
			else if (head->x == 120 && n1 == 3) { putimage(head->x, head->y, &chuan); head->x = 480; head->y = 500; putimage(head->x, head->y, &body);}
			else if (head->x == 120 && n1 == 4) { putimage(head->x, head->y, &chuan); head->x = 480; head->y = 460; putimage(head->x, head->y, &body);}
			else if (head->x == 480 && n1 == 1) { putimage(head->x, head->y, &chuan); head->x = 140; head->y = 120;  putimage(head->x, head->y, &body);}
			else if (head->x == 480 && n1 == 2) { putimage(head->x, head->y, &chuan); head->x = 100; head->y = 120;  putimage(head->x, head->y, &body);}
			else if (head->x == 480 && n1 == 3) { putimage(head->x, head->y, &chuan); head->x = 120; head->y = 140;  putimage(head->x, head->y, &body);}
			else if (head->x == 480 && n1 == 4) { putimage(head->x, head->y, &chuan); head->x = 120; head->y = 100;  putimage(head->x, head->y, &body);}
		}
		if (N == 5 && head->x == 0 && (n1==1||n1==2))
		{
			clearrectangle(head->x, head->y, head->x + S, head->y + S);
			head->x = 580;
		}
		else if (N == 5 && head->x == 580 && (n1==1||n1==2))
		{
			clearrectangle(head->x, head->y, head->x + S, head->y + S);
			head->x = 0;
		}
		for (k = 0; k <= poison_flag - 1; k++)
		{
			if (MAP[poi_x[k] / S][poi_y[k]/S] == 3 && twinkle == 1)
			{
				clearrectangle(poi_x[k], poi_y[k], poi_x[k] + S, poi_y[k] + S);
			}
			else if (MAP[poi_x[k] / S][poi_y[k]/S] == 3 && twinkle == 0)
			{
				putimage(poi_x[k], poi_y[k], &poison);
			}
		}
		if (twinkle == 1) twinkle = 0;
		else if (twinkle == 0) twinkle = 1;
	}
}

void snake_elongate()
{
	joint++;
	score += 10;
	struct Snake *p1;
	p1 = (struct Snake*) malloc(sizeof(struct Snake));
	MAP[head->x / S][head->y / S] = 0;
	if (MAP[tail->x / S][tail->y/S + 1] == 0 && check_snake_body(tail->x, tail->y+S) == 1)
	{
		p1->x = tail->x;
		p1->y = tail->y + S;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		putimage(tail->x, tail->y, &body);
		MAP[tail->x / S][tail->y / S] = 6;
	}
	else if (MAP[tail->x / S][tail->y/S - 1] == 0 && check_snake_body(tail->x, tail->y - S) == 1)
	{
		p1->x = tail->x;
		p1->y = tail->y - S;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		putimage(tail->x, tail->y, &body);
		MAP[tail->x / S][tail->y / S] = 6;
	}
	else if (MAP[tail->x / S + 1][tail->y] == 0 && check_snake_body(tail->x + S, tail->y) == 1)
	{
		p1->x = tail->x + S;
		p1->y = tail->y;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		putimage(tail->x, tail->y, &body);
		MAP[tail->x / S][tail->y / S] = 6;
	}
	else if (MAP[tail->x / S - 1][tail->y] == 0 && check_snake_body(tail->x - S, tail->y) == 1)
	{
		p1->x = tail->x - S;
		p1->y = tail->y;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		putimage(tail->x, tail->y, &body);
		MAP[tail->x / S][tail->y / S] = 6;
	}
	flag1--;
	if (flag1 == 0)
	{
		clear_poison();
		clear_bomb();
		creat_food();
		creat_poison();
		creat_bomb();
	}
}

void snake_shorten()
{
	MAP[tail->x / S][tail->y/S] = 0;
	joint--;
	score = score - 5;
	struct Snake * p1;
	clearrectangle(tail->x, tail ->y, tail->x + S, tail->y + S);
	p1 = tail->previous;
	p1->next = NULL;
	free(tail);
	tail = p1;
	MAP[head->x / S][head->y/S] = 0;
	putimage(head->x, head->y, &body);
	flag2--;
	if (flag2 == 0) creat_poison();
}

void snake_halve()
{
	score -= 35;
	MAP[head->x / S][head->y / S] = 0;
	int i;
	struct Snake * p1, *p2;
	if (joint % 2 == 0)
	{
		joint = joint / 2;
		for (i = 0, p1 = tail; i <= joint-1 ; i++)
		{
			MAP[p1->x / S][p1->y / S] = 0;
			p2 = p1->previous;
			clearrectangle(p1->x, p1->y, p1->x + S, p1->y + S);
			free(p1);
			p1 = p2;
		}
		tail = p1;
		tail->next = NULL;
	}
	else
	{
		joint = (joint + 1) / 2;
		for (i = 0, p1 = tail; i <= joint - 1; i++)
		{
			MAP[p1->x / S][p1->y/S] = 0;
			p2 = p1->previous;
			clearrectangle(p1->x, p1->y, p1->x + S, p1->y + S);
			free(p1);
			p1 = p2;
		}
		tail = p1;
		tail->next = NULL;

	}
	flag3--;
	if (flag3 == 0) creat_bomb();
}

void main()
{
	gameface();
	picture();
	set_file();
	while (1) {
		level = 1;
		joint = 3;
		score = 0;
		int i, j;
		for (i = 0; i <= 29; i++)
			for (j = 0; j <= 29; j++)
				MAP[i][j] = 0;
		game_interface();
		MAP_print();
		snake_begin_body();
		creat_food();
		creat_poison();
		creat_bomb();
		snake_body_print();
		move();
		cleardevice();
	}
}