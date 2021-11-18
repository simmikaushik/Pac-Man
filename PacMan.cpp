#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
char tmp_map[18][32];

char map[18][32] = {
	"._____________________________.",
	"|    #          ##            |",
	"|          #          #       |",
	"|###  ##  ####  ####  ##  ##  |",
	"|                             |",
	"|  ###  ####   #    #     #   |",
	"|          #          #####   |",
	"|##  #  #  #  ##      #       |",
	"|    #  #      #  ##  #  #  ##|",
	"|  ###  ##        #   #       |",
	"|            ##   #  #   #    |",
	"|  ###  ##        #  #  #   ##|",
	"|           ##                |",                             
	"|##  ##     ###       ###  #  |",
	"|      #         #      #     |",
	"|#  #  ###  ##   #   #  #  #  |",
	"|      #         ##           |",
	"'''''''''''''''''''''''''''''''",
};

#define esc 27;

void record(int,char);

void ShowMap()
{
	for(int i=0;i<18;i++)
		printf("%s\n",map[i]);
}

void gotoxy(short x,short y)
{
	HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE) ;
	COORD position={x,y} ;
	SetConsoleCursorPosition(hStdout,position) ;
}

class entity {
public:
	entity(int x,int y)
	{
		this->x=x;
		this->y=y;
	}

	void move_x(int p)
	{
		if(map[y][x+p]==' ')
			x+=p;
	}

	void move_y(int p){
		if(map[y+p][x]==' ')
			y+=p;
	}

	void move(int p,int q)
	{
		x+=p;
		y+=q;
	}

	int get_x()
	{
		return x;
	}

	int get_y()
	{
		return y;
	}

	void draw(char p)
	{
		map[x][y]=p;
		gotoxy(x,y);
		printf("%c",p);
	}

private:
	int x;
	int y;
};

struct walk
{
	short walk_count;
	short x;
	short y;
	short back;
};

struct target
{
	short x;
	short y;
};

vector<target>walk_queue;
vector<walk>BFSArray;

void AddArray(int x,int y,int wc,int back)
{
	if(tmp_map[y][x]==' ' || tmp_map[y][x]=='.')
	{
		tmp_map[y][x]='#';
		walk tmp;
		tmp.x=x;
		tmp.y=y;
		tmp.walk_count=wc;
		tmp.back=back;
		BFSArray.push_back(tmp);
	}
}

void FindPath(int sx,int sy,int x,int y)
{
	memcpy(tmp_map,map,sizeof(map));
	BFSArray.clear();
	walk tmp;
	tmp.x=sx;
	tmp.y=sy;
	tmp.walk_count=0;
	tmp.back=-1;
	BFSArray.push_back(tmp);
	int i=0;
	while(i<BFSArray.size())
	{
		if(BFSArray[i].x==x && BFSArray[i].y==y)
		{
			walk_queue.clear();
			target tmp2;
			while(BFSArray[i].walk_count!=0)
			{
				tmp2.x=BFSArray[i].x;
				tmp2.y=BFSArray[i].y;
				walk_queue.push_back(tmp2);
				i=BFSArray[i].back;
			}
			break;
		}
		AddArray(BFSArray[i].x+1,BFSArray[i].y,BFSArray[i].walk_count+1,i);
		AddArray(BFSArray[i].x-1,BFSArray[i].y,BFSArray[i].walk_count+1,i);
		AddArray(BFSArray[i].x,BFSArray[i].y+1,BFSArray[i].walk_count+1,i);
		AddArray(BFSArray[i].x,BFSArray[i].y-1,BFSArray[i].walk_count+1,i);	
		i++;
	}
	BFSArray.clear();
}

int main()
{
	do
	{
		system("color f5");
		//Color attributes are specified by TWO hex digits:
		//the first corresponds to the background; the second the foreground.
		//Each digit can be any of the following values
		//    0 = Black       8 = Gray
		//    1 = Blue        9 = Light Blue
		//    2 = Green       A = Light Green
		//    3 = Aqua        B = Light Aqua
		//    4 = Red         C = Light Red
		//    5 = Purple      D = Light Purple
		//    6 = Yellow      E = Light Yellow
		//    7 = White       F = Bright White
		bool running=true;
		int x=15;
		int y=16;
		int old_x;
		int old_y;
		int ex=1;
		int ey=1;
		int pts=0;
		printf("H -> Hard\nN -> Normal\nE -> Easy\nI -> Instructions\nS -> Scores\nEsc -> Exit\nInput : ");
		char diffi;
		int speedmod;
		diffi=getch();
		if(diffi=='E' || diffi=='e')
			speedmod=3;
		else if(diffi=='N' || diffi=='n')
			speedmod=2;
		else if(diffi=='H' || diffi=='h')
			speedmod=1;
		else if(diffi=='S' || diffi=='s')
		{
			char plname[20],nplname[20],cha,c;
			int a,i,j,px;
			FILE *info;
			system("cls");
			{
				info=fopen("record.txt","r");
				do
				{
					putchar(c=getc(info));
				}
				while(c!=EOF);
			}
			printf("Press any key to continue");
			getch();
			system("cls");
			main();
		}
		else if(diffi==27)
		{
			exit(0);
		}
		else if(diffi=='I' || diffi=='i')
		{
			system("cls");
			printf("Instruction:\n1. Arrow Keys to move pacman\n2. Eat the dots produced by the Eater to gain points\n3. Don't get caught by the Eater\n\n");
			printf("Press any key to continue");
			getch();
			system("cls");
			main();
		}
		else
		{
			printf("Enter a valid choice\n\n");
			printf("Press any key to continue:");
			getch();
			system("cls");
			main();
		}
		system("cls");
		ShowMap();
		gotoxy(x,y);
		printf("O");
		int frame=0;
		FindPath(ex,ey,x,y);
		while(running)
		{
			gotoxy(x,y);
			printf(" ");
			old_x=x;
			old_y=y;
			if(GetAsyncKeyState(VK_UP))
			{
				if(map[y-1][x]=='.')
				{
					y--;
					pts++;
				}
				else if(map[y-1][x]==' ')
					y--;
			}
			if(GetAsyncKeyState(VK_DOWN))
			{
				if(map[y+1][x]=='.')
				{
					y++;
					pts++;
				}
				else if(map[y+1][x]==' ')
					y++;
			}
			if(GetAsyncKeyState(VK_LEFT))
			{
				if(map[y][x-1]=='.')
				{
					x--;
					pts++;
				}
				else if(map[y][x-1]==' ')
					x--;
			}
			if(GetAsyncKeyState(VK_RIGHT))
			{
				if(map[y][x+1]=='.')
				{
					x++;
					pts++;
				}
				else if(map[y][x+1]==' ')
					x++;
			}
			if(old_x!=x || old_y!=y)
			{
				FindPath(ex,ey,x,y);
			}
			gotoxy(x,y);
			printf("O");
			map[ey][ex]='.';
			gotoxy(ex,ey);
			printf(".");
			if(frame%speedmod==0 && walk_queue.size()!=0)
			{
				ex=walk_queue.back().x;
				ey=walk_queue.back().y;
				walk_queue.pop_back();
			}
			gotoxy(ex,ey);
			printf("G");
			if(ex==x && ey==y)
			{
				break;
			}
			gotoxy(32,18);
			gotoxy(32,1);
			cout<<pts;
			Sleep(100);
			frame++;
		}
		system("cls");
		printf("\aGame Over and your score is : %d",pts);
		cin.get();
		cin.get();
		record(pts,diffi);
	}
	while(1);
}

void record(int pts,char diffi)
{
	char plname[20],nplname[20],cha,c;
	int a,i,j,px;
	FILE *info;
	info=fopen("record.txt","a+");
	printf("Enter your name: ");
	scanf("%s",&plname);
	for(j=0; plname[j]!='\0'; j++)
	{
		nplname[0]=toupper(plname[0]);
		if(plname[j-1]==' ')
		{
			nplname[j]=toupper(plname[j]);
			nplname[j-1]=plname[j-1];
		}
		else nplname[j]=plname[j];
	}
	nplname[j]='\0';
	fprintf(info,"Player Name :%s\n",nplname);
	fprintf(info,"Score:%d (",pts);
	fprintf(info,"%c)\n",diffi);
	for(i=0; i<=25; i++)
		fprintf(info,"%c",'_');
	fprintf(info,"\n");
	fclose(info);
	printf("Wanna see past records press 'y'\n");
	cha=getch();
	system("cls");
	if(cha=='y')
	{
		info=fopen("record.txt","r");
		do
			putchar(c=getc(info));
		while(c!=EOF);
	}
	printf("Press any Key to continue!");
	getch();
	system("cls");
	fclose(info);
}
