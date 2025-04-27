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

//绘制地图
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

//游戏初始化
void gameInit()
{
	//初始化数据
	memset(mine, 0, sizeof(mine));

	int row, col;
	//布雷操作
	for (int i = 0; i < MINE_NUM; )
	{
		row = rand() % minerow + 1;
		col = rand() % minecol + 1;
		if (mine[row][col] == 0)
		{
			mine[row][col] = 9;
			//布雷成功加一
			++i;
		}
	}

	//九宫格中雷周围的数字
	for (int i = 1; i <= minerow; ++i)
	{
		for (int j = 1; j <= minecol; ++j)
		{
			if (mine[i][j] == 9)
			{
				//遍历雷周围的九宫格
				for (int a = i - 1; a <= i + 1; a++)
				{
					//非雷的格子加一
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

	//遮掩地图
	for (int i = 1; i <= minerow; ++i)
	{
		for (int j = 1; j <= minecol; ++j)
		{
			mine[i][j] += 20;
		}
	}

}

//加载图片
void loadGame()
{
	char buf[88] = { 0 };

	//加载游戏资源
	for (int i = 0; i < 12; ++i)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "./img/%d.bmp", i);

		//加载图片
		loadimage(&img[i], buf, IMG_SIZE, IMG_SIZE);

	}
}

//绘制游戏图片
void gameDraw()
{
	//对应的是纵坐标y
	for (int i = 1; i <= minerow; ++i)
	{
		//对应的是纵坐标x
		for (int j = 1; j <= minecol; ++j)
		{
			int x = (j ) * IMG_SIZE;
			int y = (i ) * IMG_SIZE;
			//贴数字
			if (mine[i][j] >= 0 && mine[i][j] < 9)
			{
				putimage(x, y, &img[mine[i][j]]);
			}
			//贴雷
			else if (mine[i][j] == 9)
			{
				putimage(x, y, &img[9]);
			}
			//贴盖子
			else if (mine[i][j] >= 20 && mine[i][j] <= 29)
			{
				putimage(x, y, &img[10]);
			}
			//插旗子
			else if (mine[i][j] > 29)
			{
				putimage(x, y, &img[11]);
			}
		}
	}
}

//打开空白区域
void openNULL(int r, int c)
{
	//空白区域
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

//判断游戏是否结束
void gameOver(int r, int c)
{
	//踩雷
	if (mine[r][c] == 9)
	{
		mciSendString("close ./music/chiikawa.mp3", NULL, 0, NULL);

		//爆炸音效
		mciSendString("close ./music/boom.mp3", NULL, 0, NULL);
		mciSendString("open ./music/boom.mp3", NULL, 0, NULL);
		mciSendString("play ./music/boom.mp3", NULL, 0, NULL);


		//打开所有雷
		for (int i = 1; i <= minerow; i++)
		{
			for (int j = 1; j <= minecol; j++)
			{
				//未标记的
				if (mine[i][j] == 29)
				{
					mine[i][j] -= 20;
				}
				//已标记的
				else if (mine[i][j] == 49)
				{
					mine[i][j] -= 40;
				}
			}
		}

		isLoser = true;
	}

	//胜利判断
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

		//胜利
		if (num == minerow * minecol - MINE_NUM)
		{
			isWinner = true;
		}
	}

}

//鼠标点击
void mouseClick()
{
	ExMessage em;
	if (peekmessage(&em, EX_MOUSE))
	{
		mx = em.y / IMG_SIZE ;
		my = em.x / IMG_SIZE ;
		//音乐设置rectangle(IMG_SIZE * minecol - 1, 0, IMG_SIZE * (minecol + 2) - 10, 20);

		if (em.lbutton)
		{
			//普通点击音效
			mciSendString("close ./music/key.mp3", NULL, 0, NULL);
			mciSendString("open ./music/key.mp3", NULL, 0, NULL);
			mciSendString("play ./music/key.mp3", NULL, 0, NULL);
			
			//点开图标
			if (mine[mx][my] >= 20 && mine[mx][my] <= 29)
			{
				mine[mx][my] -= 20;
				openNULL(mx, my);
				gameOver(mx, my);
				makeMap();
			}
			if ((em.x >= IMG_SIZE * minecol - 1 && em.x <= IMG_SIZE * (minecol + 2) - 10) && (em.y >= 0 && em.y <= 20))
			{
				//关闭音乐
				if (mnum1 % 2 == 0)
				{
					mciSendString("close ./music/chiikawa.mp3", NULL, 0, NULL);
					line(IMG_SIZE * minecol - 1, 0, IMG_SIZE * (minecol + 2) - 10, 20);
					mnum1++;
					em.x = 0;
					em.y = 0;
				}
				//打开音乐
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
			
			//插旗
			if (mine[mx][my] > 9 && mine[mx][my] <= 29)
			{
				//插旗音乐
				mciSendString("close ./music/mineflag.mp3", NULL, 0, NULL);
				mciSendString("open ./music/mineflag.mp3", NULL, 0, NULL);
				mciSendString("play ./music/mineflag.mp3", NULL, 0, NULL);
				mine[mx][my] += 20;
			}
			//拔旗
			else
			{
				//拔旗音乐
				mciSendString("close ./music/minepull.mp3", NULL, 0, NULL);
				mciSendString("open ./music/minepull.mp3", NULL, 0, NULL);
				mciSendString("play ./music/minepull.mp3", NULL, 0, NULL);
				mine[mx][my] -= 20;
			}
		}

	}
}

//判断游戏结果
void gameResult()
{
	if (isLoser || isWinner)
	{
		char text[50] = { 0 };
		//失败
		if (isLoser)
		{

			strcpy(text, "呀哈！糟糕X﹏X！踩雷了,是否再来一把?");
		}
		//成功
		else
		{
			mciSendString("close ./music/chiikawa.mp3", NULL, 0, NULL);
			mciSendString("open ./music/win.mp3", NULL, 0, NULL);
			mciSendString("play ./music/win.mp3 ", NULL, 0, NULL);
			
			strcpy(text, "乌拉！恭喜你！成功啦！！！(^v^),是否再来一把?");
		}
		int ret = MessageBox(GetHWnd(), text, "提示", MB_OKCANCEL);
		//再来一局
		if (ret == IDOK)
		{
			//重新初始化
			gameInit();
			if (mnum1 % 2 == 0)
			{
				mciSendString("open ./music/chiikawa.mp3", NULL, 0, NULL);
				mciSendString("play ./music/chiikawa.mp3 ", NULL, 0, NULL);
			}
			isWinner = false;
			isLoser = false;
		}
		//直接结束
		else
		{
			//退出程序
			exit(0);  
		}

	}

}

//主界面
void menuBlackboard()
{
	

	//背景颜色
	setbkcolor(RGB(255, 182, 193));
	cleardevice();
	IMAGE blk[2];
	loadimage(&blk[1], "./img/537.png");
	putimage(0, 0, &blk[1]);
	

	//标题设置
	settextcolor(BLACK);
	settextstyle(80, 0, "楷体");
	outtextxy(90, 80, "扫");
	outtextxy(180, 80, "雷");

	//关卡设置

	//汉字
	settextstyle(40, 0, "楷体");
	outtextxy(90, 240, "简     ");
	outtextxy(225, 240, "单");
	outtextxy(90, 320, "中     ");
	outtextxy(225, 320, "等");
	outtextxy(90, 400, "困     ");
	outtextxy(225, 400, "难");

	//音乐按钮
	setlinecolor(BLACK);
	settextstyle(20, 0, "楷体");
	rectangle(314, 0, 355, 20);
	outtextxy(315, 0, "音乐");
	
	//关卡框框
	rectangle(90, 240, 265, 280);
	rectangle(90, 320, 265, 360);
	rectangle(90, 400, 265, 440);

}

//模式选择
void gameSelect() 
{
	ExMessage msc;
	while (true)
	{
		peekmessage(&msc, EX_MOUSE);

		if (msc.lbutton)
		{
			//简单模式
			if ((msc.x >= 90 && msc.x <= 265) && (msc.y >= 240 && msc.y <= 280))
			{
				*pr = EROW;
				*pc = ECOL;
				*pM = EMINE_NUM;
				break;
			}
			//中等模式
			else if ((msc.x >= 90 && msc.x <= 265) && (msc.y >= 320 && msc.y <= 360))
			{
				*pr = MROW;
				*pc = MCOL;
				*pM = MMINE_NUM;
				break;
			}
			//困难模式
			else if ((msc.x >= 90 && msc.x <= 265) && (msc.y >= 400 && msc.y <= 440))
			{
				*pr = DROW;
				*pc = DCOL;
				*pM = DMINE_NUM;
				break;
			}
			//音乐设置
			else if ((msc.x >= 315 && msc.x <= 355) && (msc.y >= 0 && msc.y <= 20))
			{
				//关闭音乐
				if (mnum % 2 == 0)
				{
					mciSendString("close ./music/sleep.mp3", NULL, 0, NULL);
					line(314, 0, 355, 20);
					mnum++;
					msc.x = 0;
					msc.y = 0;
				}
				//打开音乐
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
	//背景颜色
	settextcolor(BLACK);
	setbkcolor(RGB(255, 182, 193));
	cleardevice();
	IMAGE blk[2];
	loadimage(&blk[1], "./img/5371.jpg");
	putimage(0, 0, &blk[1]);
	//音乐按钮
	setlinecolor(BLACK);
	settextstyle(20, 0, "楷体");
	rectangle(IMG_SIZE * minecol - 1, 0, IMG_SIZE * (minecol + 2) - 10, 20);
	outtextxy(IMG_SIZE * minecol, 0, "音乐");

	gameDraw();
}

int main(void)
{

	//创建窗口
	initgraph(360, 520);

	//游戏主菜单
	menuBlackboard();

	//加载主页面音乐
	mciSendString("open ./music/sleep.mp3", NULL, 0, NULL);
	mciSendString("play ./music/sleep.mp3 repeat", NULL, 0, NULL);

	//选择模式
	gameSelect();

	//关闭主页面音乐
	mciSendString("close ./music/sleep.mp3", NULL, 0, NULL);

	//加载局内音乐
	mciSendString("open ./music/chiikawa.mp3", NULL, 0, NULL);
	mciSendString("play ./music/chiikawa.mp3 repeat", NULL, 0, NULL);

	//创建游戏窗口
	initgraph(IMG_SIZE * (minecol + 2), IMG_SIZE * (minerow + 2));

	//游戏主界面
	gameMinterface();

	//设置随机数
	srand((unsigned int)time(NULL));

	//游戏初始化
	gameInit();

	//加载图片
	loadGame();

	//绘制内部地图
	makeMap();

	//游戏主循环
	while (true)
	{
		
		//用户鼠标输入
		mouseClick();
		//游戏绘制
		gameDraw();
		//判断游戏是否结束
		gameResult();
	}

	getchar();
	closegraph();

	return 0;
}
