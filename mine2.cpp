#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<mmsystem.h>
#include<windows.h>
#pragma comment(lib,"WINMM.LIB")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#define EROW 9
#define ECOL 9
#define MROW 16
#define MCOL 16
#define DROW 16
#define DCOL 30
#define IMG_SIZE 25
#define EMINE_NUM 10
#define MMINE_NUM 40
#define DMINE_NUM 99

IMAGE img[12];
int minerow = 0;
int minecol = 0;
int* pr = &minerow;
int* pc = &minecol;
int MINE_NUM = 0;
int* pM = &MINE_NUM;
int mine[50][50] = { 0 };
int mx;
int my;
int mnum = 2;
int mnum1 = 2;
bool isLoser = false;
bool isWinner = false;
void gameMinterface();

//���Ƶ�ͼ
void makeMap()
{
	for (int i = 1; i <= minerow; ++i)
	{
		for (int j = 1; j <= minecol; ++j)
		{
			printf("%4d", mine[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//��Ϸ��ʼ��
void gameInit()
{
	//��ʼ������
	memset(mine, 0, sizeof(mine));

	int row, col;
	//���ײ���
	for (int i = 0; i < MINE_NUM; )
	{
		row = rand() % minerow + 1;
		col = rand() % minecol + 1;
		if (mine[row][col] == 0)
		{
			mine[row][col] = 9;
			//���׳ɹ���һ
			++i;
		}
	}

	//�Ź���������Χ������
	for (int i = 1; i <= minerow; ++i)
	{
		for (int j = 1; j <= minecol; ++j)
		{
			if (mine[i][j] == 9)
			{
				//��������Χ�ľŹ���
				for (int a = i - 1; a <= i + 1; a++)
				{
					//���׵ĸ��Ӽ�һ
					for (int b = j - 1; b <= j + 1; b++)
					{
						if (mine[a][b] != 9)
						{
							mine[a][b]++;
						}
					}
				}
			}
		}
	}

	//���ڵ�ͼ
	for (int i = 1; i <= minerow; ++i)
	{
		for (int j = 1; j <= minecol; ++j)
		{
			mine[i][j] += 20;
		}
	}

}

//����ͼƬ
void loadGame()
{
	char buf[88] = { 0 };

	//������Ϸ��Դ
	for (int i = 0; i < 12; ++i)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "./img/%d.bmp", i);

		//����ͼƬ
		loadimage(&img[i], buf, IMG_SIZE, IMG_SIZE);

	}
}

//������ϷͼƬ
void gameDraw()
{
	//��Ӧ����������y
	for (int i = 1; i <= minerow; ++i)
	{
		//��Ӧ����������x
		for (int j = 1; j <= minecol; ++j)
		{
			int x = (j ) * IMG_SIZE;
			int y = (i ) * IMG_SIZE;
			//������
			if (mine[i][j] >= 0 && mine[i][j] < 9)
			{
				putimage(x, y, &img[mine[i][j]]);
			}
			//����
			else if (mine[i][j] == 9)
			{
				putimage(x, y, &img[9]);
			}
			//������
			else if (mine[i][j] >= 20 && mine[i][j] <= 29)
			{
				putimage(x, y, &img[10]);
			}
			//������
			else if (mine[i][j] > 29)
			{
				putimage(x, y, &img[11]);
			}
		}
	}
}

//�򿪿հ�����
void openNULL(int r, int c)
{
	//�հ�����
	if (mine[r][c] == 0)
	{
		for (int i = r - 1; i <= r + 1; ++i)
		{
			for (int j = c - 1; j <= c + 1; ++j)
			{
				//(mine[i][j] == 20 || mine[i][j] != 29) && mine[i][j] > 9
				if ((i >= 1 && i <= minerow && j >= 1 && j <= minecol) && mine[i][j] >= 20 && mine[i][j] <= 28)
				{
					mine[i][j] -= 20;
					openNULL(i, j);
				}
			}
		}
	}
}

//�ж���Ϸ�Ƿ����
void gameOver(int r, int c)
{
	//����
	if (mine[r][c] == 9)
	{
		mciSendString("close ./music/chiikawa.mp3", NULL, 0, NULL);

		//��ը��Ч
		mciSendString("close ./music/boom.mp3", NULL, 0, NULL);
		mciSendString("open ./music/boom.mp3", NULL, 0, NULL);
		mciSendString("play ./music/boom.mp3", NULL, 0, NULL);


		//��������
		for (int i = 1; i <= minerow; i++)
		{
			for (int j = 1; j <= minecol; j++)
			{
				//δ��ǵ�
				if (mine[i][j] == 29)
				{
					mine[i][j] -= 20;
				}
				//�ѱ�ǵ�
				else if (mine[i][j] == 49)
				{
					mine[i][j] -= 40;
				}
			}
		}

		isLoser = true;
	}

	//ʤ���ж�
	int num = 0;
	for (int i = 1; i <= minerow; i++)
	{
		for (int j = 1; j <= minecol; j++)
		{
			if (mine[i][j] >= 0 && mine[i][j] <= 8)
			{
				num++;
			}
		}

		//ʤ��
		if (num == minerow * minecol - MINE_NUM)
		{
			isWinner = true;
		}
	}

}

//�����
void mouseClick()
{
	ExMessage em;
	if (peekmessage(&em, EX_MOUSE))
	{
		mx = em.y / IMG_SIZE ;
		my = em.x / IMG_SIZE ;
		//��������rectangle(IMG_SIZE * minecol - 1, 0, IMG_SIZE * (minecol + 2) - 10, 20);

		if (em.lbutton)
		{
			//��ͨ�����Ч
			mciSendString("close ./music/key.mp3", NULL, 0, NULL);
			mciSendString("open ./music/key.mp3", NULL, 0, NULL);
			mciSendString("play ./music/key.mp3", NULL, 0, NULL);
			
			//�㿪ͼ��
			if (mine[mx][my] >= 20 && mine[mx][my] <= 29)
			{
				mine[mx][my] -= 20;
				openNULL(mx, my);
				gameOver(mx, my);
				makeMap();
			}
			if ((em.x >= IMG_SIZE * minecol - 1 && em.x <= IMG_SIZE * (minecol + 2) - 10) && (em.y >= 0 && em.y <= 20))
			{
				//�ر�����
				if (mnum1 % 2 == 0)
				{
					mciSendString("close ./music/chiikawa.mp3", NULL, 0, NULL);
					line(IMG_SIZE * minecol - 1, 0, IMG_SIZE * (minecol + 2) - 10, 20);
					mnum1++;
					em.x = 0;
					em.y = 0;
				}
				//������
				else if (mnum1 % 2 != 0)
				{
					gameMinterface();
					mciSendString("open ./music/chiikawa.mp3", NULL, 0, NULL);
					mciSendString("play ./music/chiikawa.mp3 repeat", NULL, 0, NULL);
					em.x = 0;
					em.y = 0;
					mnum1++;
				}
			}

		}
		else if (em.rbutton)
		{
			
			//����
			if (mine[mx][my] > 9 && mine[mx][my] <= 29)
			{
				//��������
				mciSendString("close ./music/mineflag.mp3", NULL, 0, NULL);
				mciSendString("open ./music/mineflag.mp3", NULL, 0, NULL);
				mciSendString("play ./music/mineflag.mp3", NULL, 0, NULL);
				mine[mx][my] += 20;
			}
			//����
			else
			{
				//��������
				mciSendString("close ./music/minepull.mp3", NULL, 0, NULL);
				mciSendString("open ./music/minepull.mp3", NULL, 0, NULL);
				mciSendString("play ./music/minepull.mp3", NULL, 0, NULL);
				mine[mx][my] -= 20;
			}
		}

	}
}

//�ж���Ϸ���
void gameResult()
{
	if (isLoser || isWinner)
	{
		char text[50] = { 0 };
		//ʧ��
		if (isLoser)
		{

			strcpy(text, "ѽ�������X�nX��������,�Ƿ�����һ��?");
		}
		//�ɹ�
		else
		{
			mciSendString("close ./music/chiikawa.mp3", NULL, 0, NULL);
			mciSendString("open ./music/win.mp3", NULL, 0, NULL);
			mciSendString("play ./music/win.mp3 ", NULL, 0, NULL);
			
			strcpy(text, "��������ϲ�㣡�ɹ���������(^v^),�Ƿ�����һ��?");
		}
		int ret = MessageBox(GetHWnd(), text, "��ʾ", MB_OKCANCEL);
		//����һ��
		if (ret == IDOK)
		{
			//���³�ʼ��
			gameInit();
			if (mnum1 % 2 == 0)
			{
				mciSendString("open ./music/chiikawa.mp3", NULL, 0, NULL);
				mciSendString("play ./music/chiikawa.mp3 ", NULL, 0, NULL);
			}
			isWinner = false;
			isLoser = false;
		}
		//ֱ�ӽ���
		else
		{
			//�˳�����
			exit(0);  
		}

	}

}

//������
void menuBlackboard()
{
	

	//������ɫ
	setbkcolor(RGB(255, 182, 193));
	cleardevice();
	IMAGE blk[2];
	loadimage(&blk[1], "./img/537.png");
	putimage(0, 0, &blk[1]);
	

	//��������
	settextcolor(BLACK);
	settextstyle(80, 0, "����");
	outtextxy(90, 80, "ɨ");
	outtextxy(180, 80, "��");

	//�ؿ�����

	//����
	settextstyle(40, 0, "����");
	outtextxy(90, 240, "��     ");
	outtextxy(225, 240, "��");
	outtextxy(90, 320, "��     ");
	outtextxy(225, 320, "��");
	outtextxy(90, 400, "��     ");
	outtextxy(225, 400, "��");

	//���ְ�ť
	setlinecolor(BLACK);
	settextstyle(20, 0, "����");
	rectangle(314, 0, 355, 20);
	outtextxy(315, 0, "����");
	
	//�ؿ����
	rectangle(90, 240, 265, 280);
	rectangle(90, 320, 265, 360);
	rectangle(90, 400, 265, 440);

}

//ģʽѡ��
void gameSelect() 
{
	ExMessage msc;
	while (true)
	{
		peekmessage(&msc, EX_MOUSE);

		if (msc.lbutton)
		{
			//��ģʽ
			if ((msc.x >= 90 && msc.x <= 265) && (msc.y >= 240 && msc.y <= 280))
			{
				*pr = EROW;
				*pc = ECOL;
				*pM = EMINE_NUM;
				break;
			}
			//�е�ģʽ
			else if ((msc.x >= 90 && msc.x <= 265) && (msc.y >= 320 && msc.y <= 360))
			{
				*pr = MROW;
				*pc = MCOL;
				*pM = MMINE_NUM;
				break;
			}
			//����ģʽ
			else if ((msc.x >= 90 && msc.x <= 265) && (msc.y >= 400 && msc.y <= 440))
			{
				*pr = DROW;
				*pc = DCOL;
				*pM = DMINE_NUM;
				break;
			}
			//��������
			else if ((msc.x >= 315 && msc.x <= 355) && (msc.y >= 0 && msc.y <= 20))
			{
				//�ر�����
				if (mnum % 2 == 0)
				{
					mciSendString("close ./music/sleep.mp3", NULL, 0, NULL);
					line(314, 0, 355, 20);
					mnum++;
					msc.x = 0;
					msc.y = 0;
				}
				//������
				else if(mnum % 2 != 0)
				{
					menuBlackboard();
					mciSendString("open ./music/sleep.mp3", NULL, 0, NULL);
					mciSendString("play ./music/sleep.mp3 repeat", NULL, 0, NULL);
					msc.x = 0;
					msc.y = 0;
					mnum++;
				}
			}
		}
	}
}

void gameMinterface()
{
	//������ɫ
	settextcolor(BLACK);
	setbkcolor(RGB(255, 182, 193));
	cleardevice();
	IMAGE blk[2];
	loadimage(&blk[1], "./img/5371.jpg");
	putimage(0, 0, &blk[1]);
	//���ְ�ť
	setlinecolor(BLACK);
	settextstyle(20, 0, "����");
	rectangle(IMG_SIZE * minecol - 1, 0, IMG_SIZE * (minecol + 2) - 10, 20);
	outtextxy(IMG_SIZE * minecol, 0, "����");

	gameDraw();
}

int main(void)
{

	//��������
	initgraph(360, 520);

	//��Ϸ���˵�
	menuBlackboard();

	//������ҳ������
	mciSendString("open ./music/sleep.mp3", NULL, 0, NULL);
	mciSendString("play ./music/sleep.mp3 repeat", NULL, 0, NULL);

	//ѡ��ģʽ
	gameSelect();

	//�ر���ҳ������
	mciSendString("close ./music/sleep.mp3", NULL, 0, NULL);

	//���ؾ�������
	mciSendString("open ./music/chiikawa.mp3", NULL, 0, NULL);
	mciSendString("play ./music/chiikawa.mp3 repeat", NULL, 0, NULL);

	//������Ϸ����
	initgraph(IMG_SIZE * (minecol + 2), IMG_SIZE * (minerow + 2));

	//��Ϸ������
	gameMinterface();

	//���������
	srand((unsigned int)time(NULL));

	//��Ϸ��ʼ��
	gameInit();

	//����ͼƬ
	loadGame();

	//�����ڲ���ͼ
	makeMap();

	//��Ϸ��ѭ��
	while (true)
	{
		
		//�û��������
		mouseClick();
		//��Ϸ����
		gameDraw();
		//�ж���Ϸ�Ƿ����
		gameResult();
	}

	getchar();
	closegraph();

	return 0;
}
