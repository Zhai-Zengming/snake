/*
 * @brief: ̰����С��Ϸ Dev-C++ 
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

/* ϵͳ�������������ù�� */
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
		gotoxy(i, 0);                  //�ϱ߽� 
		printf("*");
		gotoxy(i, HEIGHT - 1);         //�±߽� 
		printf("*");
	}
	for(i = 0; i < HEIGHT; i++)
	{
		gotoxy(OFFSET, i);             //��߽� 
		printf("*");
		gotoxy(WIDTH + OFFSET, i);     //�ұ߽� 
		printf("*");
	}
	gotoxy(80, 0);                     //������ߣ�����Ӱ������ 
}

void creat_food(void)
{
	int i; 
	int food_creat = 1;
	
	if(food_flag == 1)                                  //ʳ�ﱻ�Ժ�Ŵ���������ӣ�����ʳ�������һֱ�ı� 
	{
		srand(time(NULL));                              //���������
		food.x = rand() % (WIDTH - 1) + OFFSET + 1;     //Ҫ��ֹʳ������ڱ߽��� 
		food.y = rand() % (HEIGHT - 2) + 1;
		
		for(i = 0; i < snake.lenth; i++)                //��ֹʳ������������� 
		{
			if((food.x == snake.x[i]) && (food.y == snake.y[i]))
				food_creat = 0;
		}
		
		if(food.x % 2 == 0)                             //�ߵ�x������ż����ʳ���x����Ҳ��ż���߲��ܳԵ� 
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
	printf("*");    //��ͷ 
	
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
 * ע��Ҫ�Ȳ�����β�����ƶ���������ƶ���ͷ
 * ��Ϊ�ƶ���ͷ��x[0], y[0]����ı��ˣ����ƶ�
 * ��������Ҫ�õ�δ�ı�ǰ������
 * 
 */
void move_snake(void)
{
	int i = 0; 
	
	if(_kbhit())                              //������������ 
	{
		fflush(stdin);                        //��������� 
		dir = _getch();
	}
	
	if(dir == 'a' || dir == 'A' || dir == 'W' || dir == 'w'||
	   dir == 's' || dir == 'S' || dir == 'd' || dir == 'D')
	{
		gotoxy(snake.x[snake.lenth - 1], snake.y[snake.lenth - 1]);
		printf(" ");                          //������β 
		
		for(i = snake.lenth - 1; i > 0; i--)  //�ƶ����� 
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
	gotoxy(snake.x[0], snake.y[0]);     //�ƶ���ͷ 
	printf("*");
	                                    //�Ե�ʳ�� 
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
	for(i = 1; i < snake.lenth; i++)      //�ж��Ƿ�ײ���Լ������� 
	{
		if(snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
		{
			gotoxy(OFFSET - 5 + WIDTH / 2, HEIGHT / 2);
			printf("Game Over!!!");
			return 1;
		}
	}
	if(snake.x[0] == OFFSET || snake.x[0] == OFFSET + WIDTH || snake.y[0] == 0 || snake.y[0] == HEIGHT - 1)   //�ж��Ƿ�ײǽ 
	{
		gotoxy(OFFSET - 5 + WIDTH / 2, HEIGHT / 2);
		printf("Game Over!!!");
		return 1;
	}
	return 0;
}

void print_msg()                  //��ӡ��Ϣ
{
	gotoxy(80, 0);
	printf("��ͷ��(%d, %d)", snake.x[0], snake.y[0]);
	
	gotoxy(80, 2);
	printf("���ĳɼ���%d", score);
	
	gotoxy(80, 10);
	printf("�淨��");
	gotoxy(80, 11);
	printf("      W(��)");
	gotoxy(80, 12);
	printf("A(��) S(��) D(��)");
	gotoxy(80, 13);
	printf(" �����������ͣ");
	
	gotoxy(80, 24);
	printf("�����ˣ�������");
	gotoxy(80, 25);
	printf("���ڣ�2022/3/31");
	
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
		Sleep(300 - snake.speed * 80);             //Sleep��sleep��һ�� 
	}
}


