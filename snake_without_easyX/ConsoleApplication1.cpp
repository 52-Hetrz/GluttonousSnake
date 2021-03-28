#include"pch.h"
#include<stdio.h>
#include <Windows.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

#define SU 49
#define SD 50
#define B  32
#define U  72
#define D  80
#define L  75
#define R  77                        //功能键设置//

#define FILE_BUFFER_LENGTH 30000

void set_file();                     //建立用户档案//
void ranking_list();                 //打开排行榜//
void list_change();                  //用来将用户的分数录入旁行榜//
void MAP_print(int n);               //地图打印函数//
void MAP_print2(int n);              //打印第二关的地图//
void game_two();                     //进入第二关//
void set_cursor(int x, int y);       //放置指针函数。
void snake_body_print();             //打印蛇的身体//
void snake_begin_body();             //初始化蛇的身体//
void game_interface();               //游戏开始界面//
void color_change(int x);            //改变颜色//
int check(int x, int y);             //检查在地图二维数组上的值//
int check_snake_body(int x, int y);  //检验生成物是否与蛇身重合//
void creat_food();                   //生成食物//
void creat_poison();                 //生成毒草//
void clear_poison();                 //清除毒草//
int poi_x[6], poi_y[6];              //记录毒草的坐标//
void creat_bomb();                   //生成炸弹//
void clear_bomb();                   //清除炸弹//
int bom_x[3], bom_y[3];              //记录炸弹的坐标//
void snake_elongate();               //吃了食物之后，蛇身加长//
void snake_shorten();                //吃了毒草之后，蛇身变短//
void snake_halve();                  //吃到炸弹后，体长减半//
int eat_self(int x, int y);          //判断蛇是否咬到自己//
void game_shows();                   //游戏说明//
void game_over();                    //游戏结束//
int MAP[30][30];                     //地图中数字的含义。0：空。 1：墙。 2：食物。 3：毒草。 4:地雷。 5：蛇身 6：穿越石// 
int MAP0[30][30] = { 0 };            //游戏初始化界面边框//
struct Snake * head, *tail, *p;      //蛇的结构体//
struct Users_score;                  //用户名及其分数，准备录入旁行榜//
int score = 0, score_fin = 0;        //记录分数//
int speed;                           //用来控制蛇的移动速度//
int twinkle = 1;                     //用来控制毒草闪烁//                 
int joint = 3;                       //用来记录蛇的身体节数//
int food_flag = 4, flag1;            //用来检测食物是否完全被吃完//
int poison_flag, flag2;              //用来检测毒草是否全被吃完//
int bomb_flag, flag3;                //用来检测炸弹是否被吃//
int N;                               //用来记录选择的关卡难度//
char name[20];                       //用来记录用户的名字//
int level;                           //用来记录闯关的关卡数//
FILE * fp1, *fp2, *fp3, *fp;          //用来指向该用户的文件//
errno_t a, b, c, d, e;

struct Users_score {
	char name[20];
	int score;
}users[6], users1[6], user, a1;

void set_file()
{
	MAP_print(3);
	set_cursor(10, 14);
	color_change(12);
	printf("请输入您的昵称：");
	set_cursor(10, 15);
	printf("(20个字符以内, 不能包含空格)");
	color_change(15);
	scanf_s("%s", user.name, 20);
	d = fopen_s(&fp1, user.name, "a");
	fputs("\n", fp1);
	fclose(fp1);
}

void ranking_list()
{
	int i;
	a = fopen_s(&fp2, "Ranking list", "r");
	for (i = 0; i <= 5; i++)
	{
		fread(&users[i], sizeof(struct Users_score), 1, fp2);
		printf("%s    %d", users[i].name, users[i].score);
		printf("\n");
	}
	fclose(fp2);
}

void list_change()
{
	int a[6];
	int i, flag1 = 0;
	b = fopen_s(&fp3, "Ranking list", "r+");
	for (i = 0; i <= 5; i++)
	{
		fread(&users[i], sizeof(struct Users_score), 1, fp3);
		users1[i] = users[i];
		if (score_fin <= users[i].score && i == 5) flag1 = 1;
	}
	fclose(fp3);
	if (flag1 == 0)
	{
		users1[5] = user;
		for (i = 5; i > 0; i--)
		{
			if (users1[i].score > users1[i - 1].score)
			{
				a1 = users1[i];
				users1[i] = users1[i - 1];
				users1[i - 1] = a1;
			}
			else break;
		}
		for (i = 0; i <= 5; i++)
		{
			users[i] = users1[i];
		}
		c = fopen_s(&fp3, "Ranking list", "w");
		for (i = 0; i <= 5; i++)
			fwrite(&users[i], sizeof(struct Users_score), 1, fp3);
		fclose(fp3);
		return;
	}
	else if (flag1 == 1) return;
}

void set_cursor(int x, int y)        //指针放置//
{
	COORD place = { x,y };
	HANDLE HOoutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(HOoutput, place);
}

void color_change(int x)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

void game_interface()
{
	level = 1;
	system("cls");
	MAP_print(1);
	game_shows();
	set_cursor(20, 5);
	color_change(10);
	printf("聪  明  的  小  蛇");
	set_cursor(10, 15);
	printf("一、难度选择(1-简单，2-中等，3-难):\n");
	set_cursor(10, 16);
	printf("二、打开排行榜(4)");
	set_cursor(10, 17);
	printf("三、离开游戏(5)");
	color_change(7);
	set_cursor(10, 20);
	printf("请进行下一步操作:");
	scanf_s("%d", &N);
	switch (N)
	{
	case 1:
		system("cls");
		MAP_print(1);
		game_shows();
		snake_body_print();
		speed = 50;
		poison_flag = 3;
		bomb_flag = 0;
		break;
	case 2:
		system("cls");
		MAP_print(2);
		game_shows();
		snake_body_print();
		speed = 30;
		poison_flag = 4;
		bomb_flag = 2;
		break;
	case 3:
		system("cls");
		MAP_print(3);
		game_shows();
		snake_body_print();
		speed = 20;
		poison_flag = 6;
		bomb_flag = 3;
		break;
	case 4:
		system("cls");
		ranking_list();
		system("pause");
		system("cls");
		game_interface();
		break;
	case 5:
		system("cls");
		break;
	default:
		set_cursor(11, 20);
		color_change(12);
		printf("请输入一个1-4的数字。");
		color_change(7);
		system("pause");
		system("cls");
		game_interface();
	}
}

void game_shows()
{
	set_cursor(62, 10);
	color_change(14);
	printf("游戏说明：");
	set_cursor(62, 11);
	printf("进入游戏界面时，请输入1-4的数字，来执行下一步操作。");
	set_cursor(62, 12);
	printf("关于小蛇操控:用上下左右来控制移动,空格暂停,开局不能向右走");
	set_cursor(62, 13);
	printf("关于游戏中图标的声明：");
	set_cursor(62, 14);
	printf("墙：■  撞到小蛇会死");
	set_cursor(62, 15);
	printf("美味的食物：+ 吃到小蛇会增加一个节点，同时分数会加10分");
	set_cursor(62, 16);
	printf("毒草：- 吃到小蛇会减少一个节点，同时分数会减少5分");
	set_cursor(62, 17);
	printf("炸弹：* 吃到小蛇会体长减少一半，同时分数减少35分(当选择");
	set_cursor(62, 18);
	printf("难度为“中等”或“简单”时，若体长为奇数， 则食用炸弹后");
	set_cursor(62, 19);
	printf(" 的体长计算方法为“体长加一再除以二”。但当选择“难”，");
	set_cursor(62, 20);
	printf("计算方法为“体长减一再处以二。");
	set_cursor(62, 21);
	printf("穿越石★ 从另一个时空断点穿出，会变成蛇身形状，永久使用");
	set_cursor(62, 22);
	printf("关于游戏胜负的规定：");
	set_cursor(62, 23);
	printf("胜：分数达到300分，或者长度达到30节。");
	set_cursor(62, 24);
	printf("负：蛇的节数小于等于2节，或撞到墙，或撞到自己的身体");
	set_cursor(62, 25);
	printf("最终得分的计算方法：分数+15*节数");
	set_cursor(62, 26);
	printf("其他注明：中等难度的第二关有穿越石，难度为难的第二关可穿越");
	set_cursor(62, 27);
	printf("左右两侧墙壁。");
	color_change(12);
	set_cursor(62, 28);
	printf("祝您，游戏愉快！");
	color_change(15);
}

void game_two()
{
	void move();
	score = 0;
	score_fin = 0;
	joint = 3;
	if (N == 2) N = 6;
	else if (N == 3) N = 7;
	system("cls");
	if (N == 6)
	{
		MAP_print2(N);
		game_shows();
		snake_begin_body();
		snake_body_print();
		move();
	}
	else if (N = 7)
	{
		MAP_print2(N);
		game_shows();
		snake_begin_body();
		snake_body_print();
		move();
	}
	system("cls");

}

void MAP_print2(int n)
{
	int i, j;
	for (i = 0; i <= 29; i++)
		for (j = 0; j <= 29; j++)
			MAP[i][j] = 0;
	switch (n)
	{
	case 6:
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
	case 7:
		for (i = 0; i <= 29; i++)
		{
			MAP[i][0] = 1; MAP[i][29] = 1;
		}

		for (i = 1; i <= 3; i++)
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
			if (MAP[i][j] == 1) printf("■");
			else if (MAP[i][j] == 7) printf("★");
			else printf("  ");
		}
		printf("\n");
	}
}

void MAP_print(int n)  //地图打印//
{
	int i, j;
	for (i = 0; i <= 29; i++)
		for (j = 0; j <= 29; j++)
			MAP[i][j] = 0;
	for (i = 0; i <= 29; i++)
	{
		MAP[0][i] = 1; MAP[i][0] = 1; MAP[29][i] = 1; MAP[i][29] = 1;
	}
	switch (n) {
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
			if (MAP[i][j] == 1) printf("■");
			else printf("  ");
		}
		printf("\n");
	}
}

struct Snake {
	int x;
	int y;
	struct Snake * next;
	struct Snake * previous;
};

void snake_begin_body()
{
	int j, i;
	struct Snake *p1, *p2;
	p1 = p2 = (struct Snake *)malloc(sizeof(struct Snake));
	tail = NULL;
	head = NULL;
	for (j = 1; p2->next != NULL; j++)
	{
		p1->x = (12 + 2 * j); p1->y = 13;
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
		MAP[p1->x / 2][p1->y] = 6;
	}
}

void snake_body_print() {
	for (p = head; p != NULL; p = p->next)
	{
		set_cursor(p->x, p->y);
		printf("●");
	}
}

void creat_food()
{
	flag1 = food_flag;
	int x[4], y[4], i;
	for (i = 0; i <= 3; i++)
	{
		x[i] = (rand() % 30) * 2;
		y[i] = rand() % 29;
	}
	for (i = 0; i <= 3; i++)
	{
		if (MAP[x[i] / 2][y[i]] == 0)
		{
			if (check_snake_body(x[i], y[i]))
			{
				MAP[x[i] / 2][y[i]] = 2;
				set_cursor(x[i], y[i]);
				printf("+");
			}
		}
		if (MAP[x[i] / 2][y[i]] != 2)
		{
			x[i] = (rand() % 30) * 2;
			y[i] = rand() % 29;
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
		poi_x[i] = (rand() % 30) * 2;
		poi_y[i] = rand() % 29;
	}
	for (i = 0; i <= poison_flag - 1; i++)
	{
		if (MAP[poi_x[i] / 2][poi_y[i]] == 0)
		{
			if (check_snake_body(poi_x[i], poi_y[i]))
			{
				MAP[poi_x[i] / 2][poi_y[i]] = 3;
				set_cursor(poi_x[i], poi_y[i]);
				printf("-");
			}
		}
		if (MAP[poi_x[i] / 2][poi_y[i]] != 3)
		{
			poi_x[i] = (rand() % 30) * 2;
			poi_y[i] = rand() % 29;
			i--;
		}
	}
}

void clear_poison()
{
	int i, j;
	for (i = 0; i <= poison_flag - 1; i++)
	{
		if (MAP[poi_x[i] / 2][poi_y[i]] == 3)
		{
			set_cursor(poi_x[i], poi_y[i]);
			printf("  ");
			MAP[poi_x[i] / 2][poi_y[i]] = 0;
		}
	}
}

void creat_bomb()
{
	flag3 = bomb_flag;
	int i;
	for (i = 0; i <= bomb_flag - 1; i++)
	{
		bom_x[i] = (rand() % 30) * 2;
		bom_y[i] = rand() % 29;
	}
	for (i = 0; i <= bomb_flag - 1; i++)
	{
		if (MAP[bom_x[i] / 2][bom_y[i]] == 0)
		{
			if (check_snake_body(bom_x[i], bom_y[i]))
			{
				MAP[bom_x[i] / 2][bom_y[i]] = 4;
				set_cursor(bom_x[i], bom_y[i]);
				printf("*");
			}
		}
		if (MAP[bom_x[i] / 2][bom_y[i]] != 4)
		{
			bom_x[i] = (rand() % 30) * 2;
			bom_y[i] = rand() % 29;
			i--;
		}
	}
}

void clear_bomb()
{
	int i, j;
	for (i = 0; i <= bomb_flag - 1; i++)
	{
		if (MAP[bom_x[i] / 2][bom_y[i]] == 4)
		{
			set_cursor(bom_x[i], bom_y[i]);
			printf("  ");
			MAP[bom_x[i] / 2][bom_y[i]] = 0;
		}
	}
}

int check(int x, int y)
{
	if (MAP[x / 2][y] == 0) return 0;
	else if (MAP[x / 2][y] == 1) return 1;
	else if (MAP[x / 2][y] == 2) return 2;
	else if (MAP[x / 2][y] == 3) return 3;
	else if (MAP[x / 2][y] == 4) return 4;
	else if (MAP[x / 2][y] == 7) return 7;
}

int check_snake_body(int x, int y)
{
	if (MAP[x / 2][y] == 6) return 0;                        //重合//
	else if (x == head->x &&y == head->y) return 0;          //重合//
	else return 1;                                           //不重合// 
}

int eat_self(int x, int y)
{
	if (MAP[x / 2][y] == 6) return 0;
	else return 1;
}

void game_over()
{
	system("cls");
	MAP_print(1);
	set_cursor(18, 14);
	color_change(15);
	printf("Game    ");
	color_change(12);
	printf("Over....\n");
	color_change(15);
	set_cursor(18, 15);
	score_fin = score + 15 * joint;
	user.score = score_fin;
	printf("您的最终成绩：%d分", score_fin);
	set_cursor(18, 16);
	printf("您的最终长度：%d节", joint);
	set_cursor(18, 17);
	d = fopen_s(&fp1, user.name, "a");
	fprintf(fp1, "分数：%d   难度：%d  关卡：%d ", score_fin, N, level);
	fputs("\n", fp1);
	fclose(fp1);
	list_change();
	system("pause");
	system("cls");
}

void move()
{
	int i, j, k;
	int key = 1;
	int flag = 0;                     //用flag来表示check的结果//
	int n1 = 0;                       //采用n来记录蛇的运动方向。1：正在向右 2：正在向左 3：正在向下 4：正在向上。//
	creat_food();
	creat_poison();
	creat_bomb();
	set_cursor(62, 7);
	printf("分数：%d", score);
	set_cursor(62, 8);
	printf("节数：%d", joint);
	while (1)
	{
		if (joint <= 2 || score < 0 || check(head->x, head->y) == 1)
		{
			game_over();
			break;
		}
		else if (eat_self(head->x, head->y) == 0)
		{
			game_over();
			break;
		}
		else if (score >= 300 || joint >= 30)
		{
			system("cls");
			MAP_print(1);
			set_cursor(24, 10);
			color_change(12);
			printf("Congratulations！！");
			set_cursor(18, 14);
			color_change(15);
			printf("The    Game    ");
			color_change(12);
			printf("Victory!\n");
			color_change(15);
			score_fin = score + 15 * joint;
			user.score = score_fin;
			set_cursor(18, 15);
			printf("您的最终成绩：%d分", score_fin);
			set_cursor(18, 16);
			printf("您的最终长度：%d节", joint);
			d = fopen_s(&fp1, user.name, "a");
			fputs("\n", fp1);
			fprintf(fp1, "分数：%d 难度：%d 关卡：%d ", score_fin, N, level);
			fclose(fp1);
			list_change();
			if (N == 2 || N == 3)
			{
				level++;
				set_cursor(18, 17);
				printf("恭喜你，可以进入第二关了！！");
				set_cursor(18, 18);
				system("pause");
				game_two();
				break;
			}
			set_cursor(18, 17);
			system("pause");
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
					MAP[head->x / 2][head->y] = 6;
					MAP[tail->x / 2][tail->y] = 0;
					set_cursor(tail->x, tail->y);
					printf("  ");
					set_cursor(head->x, head->y - 1);
					printf("●");
					p = tail->previous;
					tail->x = head->x;
					tail->y = head->y - 1;
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
					MAP[head->x / 2][head->y] = 6;
					MAP[tail->x / 2][tail->y] = 0;
					set_cursor(tail->x, tail->y);
					printf("  ");
					set_cursor(head->x, head->y + 1);
					printf("●");
					p = tail->previous;
					tail->x = head->x;
					tail->y = head->y + 1;
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
					MAP[head->x / 2][head->y] = 6;
					MAP[tail->x / 2][tail->y] = 0;
					set_cursor(tail->x, tail->y);
					printf("  ");
					set_cursor(head->x - 2, head->y);
					printf("●");
					p = tail->previous;
					tail->x = head->x - 2;
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
					MAP[head->x / 2][head->y] = 6;
					MAP[tail->x / 2][tail->y] = 0;
					set_cursor(tail->x, tail->y);
					printf("  ");
					set_cursor(head->x + 2, head->y);
					printf("●");
					p = tail->previous;
					tail->x = head->x + 2;
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
			case B:
				if (_kbhit()) {
					key = _getch();
					break;
				}
			case SD:
				speed += 10;
				if (_kbhit()) {
					key = _getch();
					break;
				}
			case SU:
				speed -= 10;
				if (_kbhit()) {
					key = _getch();
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
		else if (N == 6 && check(head->x, head->y) == 7)
		{
			if (head->x == 12 && n1 == 1) { head->x = 50; head->y = 24; set_cursor(12, 6); printf("★"); }
			else if (head->x == 12 && n1 == 2) { head->x = 46; head->y = 24; set_cursor(12, 6); printf("★"); }
			else if (head->x == 12 && n1 == 3) { head->x = 48; head->y = 25; set_cursor(12, 6); printf("★"); }
			else if (head->x == 12 && n1 == 4) { head->x = 48; head->y = 23; set_cursor(12, 6); printf("★"); }
			else if (head->x == 48 && n1 == 1) { head->x = 14; head->y = 6; set_cursor(48, 24); printf("★"); }
			else if (head->x == 48 && n1 == 2) { head->x = 10; head->y = 6; set_cursor(48, 24); printf("★"); }
			else if (head->x == 48 && n1 == 3) { head->x = 12; head->y = 7; set_cursor(48, 24); printf("★"); }
			else if (head->x == 48 && n1 == 4) { head->x = 12; head->y = 5; set_cursor(48, 24); printf("★"); }
		}
		if (N == 7 && head->x == 0)
		{
			head->x = 56;
		}
		else if (N == 7 && head->x == 58)
		{
			head->x = 0;
		}
		for (k = 0; k <= poison_flag - 1; k++)
		{
			if (MAP[poi_x[k] / 2][poi_y[k]] == 3 && twinkle == 1)
			{
				set_cursor(poi_x[k], poi_y[k]);
				printf("-");
			}
			else if (MAP[poi_x[k] / 2][poi_y[k]] == 3 && twinkle == 0)
			{
				set_cursor(poi_x[k], poi_y[k]);
				printf("  ");
			}
		}
		if (twinkle == 1) twinkle = 0;
		else if (twinkle == 0) twinkle = 1;
	}
}

void snake_elongate()
{
	joint++;
	score = score + 10;
	set_cursor(68, 7);
	printf("%d", score);
	set_cursor(68, 8);
	printf("%d", joint);
	struct Snake *p1;
	p1 = (struct Snake*) malloc(sizeof(struct Snake));
	MAP[head->x / 2][head->y] = 0;
	if (MAP[tail->x / 2][tail->y + 1] == 0 && check_snake_body(tail->x, tail->y + 1) == 1)
	{
		p1->x = tail->x;
		p1->y = tail->y + 1;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		set_cursor(tail->x, tail->y);
		printf("●");
		MAP[tail->x / 2][tail->y] = 6;
	}
	else if (MAP[tail->x / 2][tail->y - 1] == 0 && check_snake_body(tail->x, tail->y - 1) == 1)
	{
		p1->x = tail->x;
		p1->y = tail->y - 1;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		set_cursor(tail->x, tail->y);
		printf("●");
		MAP[tail->x / 2][tail->y] = 6;
	}
	else if (MAP[tail->x / 2+1][tail->y] == 0 && check_snake_body(tail->x+1, tail->y) == 1)
	{
		p1->x = tail->x+2;
		p1->y = tail->y;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		set_cursor(tail->x, tail->y);
		printf("●");
		MAP[tail->x / 2][tail->y] = 6;
	}
	else if (MAP[tail->x / 2-1][tail->y ] == 0 && check_snake_body(tail->x-2, tail->y) == 1)
	{
		p1->x = tail->x-2;
		p1->y = tail->y;
		p1->previous = tail;
		p1->next = NULL;
		tail->next = p1;
		tail = p1;
		set_cursor(tail->x-2, tail->y);
		printf("●");
		MAP[tail->x / 2][tail->y] = 6;
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
	MAP[tail->x / 2][tail->y] = 0;
	joint--;
	score = score - 5;
	set_cursor(68, 7);
	printf("%d", score);
	set_cursor(68, 8);
	printf("%d", joint);
	struct Snake * p1;
	set_cursor(tail->x, tail->y);
	printf("  ");
	p1 = tail->previous;
	p1->next = NULL;
	free(tail);
	tail = p1;
	MAP[head->x / 2][head->y] = 0;
	set_cursor(head->x, head->y);
	printf("●");
	flag2--;
	if (flag2 == 0) creat_poison();
}

void snake_halve()
{
	score -= 35;
	MAP[head->x / 2][head->y] = 0;
	int i;
	struct Snake * p1, *p2;
	if (joint % 2 == 0)
	{
		for (i = 0, p1 = tail; i <= joint / 2; i++)
		{
			MAP[p1->x / 2][p1->y] = 0;
			joint--;
			p2 = p1->previous;
			set_cursor(p1->x, p1->y);
			printf("  ");
			free(p1);
			p1 = p2;
		}
		tail = p1;
		tail->next = NULL;
	}
	else
	{
		if (N == 2)
		{
			for (i = 0, p1 = tail; i <= (joint - 1) / 2; i++)
			{
				MAP[p1->x / 2][p1->y] = 0;
				joint--;
				p2 = p1->previous;
				set_cursor(p1->x, p1->y);
				printf("  ");
				free(p1);
				p1 = p2;
			}
			tail = p1;
			tail->next = NULL;
		}
		else if (N == 3)
		{
			for (i = 0, p1 = tail; i <= (joint + 1) / 2; i++)
			{
				MAP[p1->x / 2][p1->y] = 0;
				joint--;
				p2 = p1->previous;
				set_cursor(p1->x, p1->y);
				printf("  ");
				free(p1);
				p1 = p2;
			}
			tail = p1;
			tail->next = NULL;
		}
	}
	set_cursor(68, 7);
	printf("%d", score);
	set_cursor(68, 8);
	printf("%d", joint);
	flag3--;
	if (flag3 == 0) creat_bomb();
}

void main()
{
	set_file();
	while (1)
	{
		level = 1;
		int i, j;
		for (i = 0; i <= 29; i++)
			for (j = 0; j <= 29; j++)
				MAP[i][j] = 0;
		joint = 3;
		score = 0;
		snake_begin_body();
		game_interface();
		if (N == 5) break;
		snake_body_print();
		move();
		system("cls");
	}
	system("pause");

}
