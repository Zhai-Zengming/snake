/*
 * @brief: 贪吃蛇小游戏 Dev-C++ 
 *
 * @date : 2022/3/31
 *
 */
#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>

#define WIDTH   52
#define HEIGHT  26
#define OFFSET  20
#define INIT_SNAKELEN  3
#define MAX_SNAKELEN   100

char dir = '0';
int food_flag = 1;
int score = 0;

/* 系统辅助函数，设置光标 */
void gotoxy(int x, int y)
{
	COORD pos = {x, y};
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout,pos);
}

struct FOOD
{
	int x;
	int y;
}food;

struct Snake
{
	int x[MAX_SNAKELEN];
	int y[MAX_SNAKELEN];
	int lenth;
	int speed;
}snake;

void creat_map(void)
{
	int i = 0;
	for(i = OFFSET; i < WIDTH + OFFSET; i++)
	{
		gotoxy(i, 0);                  //上边界 
		printf("*");
		gotoxy(i, HEIGHT - 1);         //下边界 
		printf("*");
	}
	for(i = 0; i < HEIGHT; i++)
	{
		gotoxy(OFFSET, i);             //左边界 
		printf("*");
		gotoxy(WIDTH + OFFSET, i);     //右边界 
		printf("*");
	}
	gotoxy(80, 0);                     //光标移走，否则影响美观 
}

void creat_food(void)
{
	int i; 
	int food_creat = 1;
	
	if(food_flag == 1)                                  //食物被吃后才创建随机种子，否则食物坐标会一直改变 
	{
		srand(time(NULL));                              //随机数种子
		food.x = rand() % (WIDTH - 1) + OFFSET + 1;     //要防止食物出现在边界上 
		food.y = rand() % (HEIGHT - 2) + 1;
		
		for(i = 0; i < snake.lenth; i++)                //防止食物出现在蛇身上 
		{
			if((food.x == snake.x[i]) && (food.y == snake.y[i]))
				food_creat = 0;
		}
		
		if(food.x % 2 == 0)                             //蛇的x坐标是偶数，食物的x坐标也是偶数蛇才能吃到 
		{
			if(food_creat == 1)
			{
				food_flag = 0;
				gotoxy(food.x, food.y);
				printf("#");
			}
		}
	}
	
	gotoxy(80, 0);
}

void init_snake(void)
{
	int i;
	
	snake.x[0] = OFFSET + WIDTH / 2; 
	snake.y[0] = HEIGHT / 2;
	snake.lenth = INIT_SNAKELEN;
	snake.speed = 0;
	gotoxy(snake.x[0], snake.y[0]);
	printf("*");    //蛇头 
	
	for(i = 1; i < INIT_SNAKELEN; i++)
	{
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i-1];
		gotoxy(snake.x[i], snake.y[i]);
		printf("#"); 
	}
	gotoxy(80, 0);
}


/* 
 * 注意要先擦除蛇尾，再移动蛇身，最后移动蛇头
 * 因为移动蛇头把x[0], y[0]坐标改变了，而移动
 * 蛇身又需要用到未改变前的坐标
 * 
 */
void move_snake(void)
{
	int i = 0; 
	
	if(_kbhit())                              //监听键盘输入 
	{
		fflush(stdin);                        //清除缓冲区 
		dir = _getch();
	}
	
	if(dir == 'a' || dir == 'A' || dir == 'W' || dir == 'w'||
	   dir == 's' || dir == 'S' || dir == 'd' || dir == 'D')
	{
		gotoxy(snake.x[snake.lenth - 1], snake.y[snake.lenth - 1]);
		printf(" ");                          //擦除蛇尾 
		
		for(i = snake.lenth - 1; i > 0; i--)  //移动蛇身 
		{
			snake.x[i] = snake.x[i - 1];
			snake.y[i] = snake.y[i - 1];
			gotoxy(snake.x[i], snake.y[i]);
			printf("#");
		}
	}
	
	switch(dir)
	{
		case 'A':
		case 'a':
			snake.x[0] -= 2;
			break;
			
		case 'D':
		case 'd':
			snake.x[0] += 2;
			break;
			
		case 'W':
		case 'w':
			snake.y[0] -= 1;
			break;
			
		case 'S':
		case 's':
			snake.y[0] += 1;
			break;
	}
	gotoxy(snake.x[0], snake.y[0]);     //移动蛇头 
	printf("*");
	                                    //吃到食物 
	if(food.y == snake.y[0] && food.x == snake.x[0])
	{
		snake.lenth += 1;
		food_flag = 1;
		score += 2 * (snake.speed + 1);
		if(score % 10 == 0)
		{
			snake.speed += 1;
		}
	}
	
	gotoxy(80, 0);
}

int gameover(void)
{
	int i;
	for(i = 1; i < snake.lenth; i++)      //判断是否撞到自己的身体 
	{
		if(snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
		{
			gotoxy(OFFSET - 5 + WIDTH / 2, HEIGHT / 2);
			printf("Game Over!!!");
			return 1;
		}
	}
	if(snake.x[0] == OFFSET || snake.x[0] == OFFSET + WIDTH || snake.y[0] == 0 || snake.y[0] == HEIGHT - 1)   //判断是否撞墙 
	{
		gotoxy(OFFSET - 5 + WIDTH / 2, HEIGHT / 2);
		printf("Game Over!!!");
		return 1;
	}
	return 0;
}

void print_msg()                  //打印信息
{
	gotoxy(80, 0);
	printf("蛇头：(%d, %d)", snake.x[0], snake.y[0]);
	
	gotoxy(80, 2);
	printf("您的成绩：%d", score);
	
	gotoxy(80, 10);
	printf("玩法：");
	gotoxy(80, 11);
	printf("      W(上)");
	gotoxy(80, 12);
	printf("A(左) S(下) D(右)");
	gotoxy(80, 13);
	printf(" 其余任意键暂停");
	
	gotoxy(80, 24);
	printf("制作人：翟增明");
	gotoxy(80, 25);
	printf("日期：2022/3/31");
	
}

int main(void)
{
	creat_map();
	init_snake();
	while(1)
	{
		creat_food();
		move_snake();
		print_msg();
		if(gameover())
		{
			gotoxy(0, 25);
			exit(0);
		} 
		Sleep(300 - snake.speed * 80);             //Sleep和sleep不一样 
	}
}


