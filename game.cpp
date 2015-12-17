#include <graphics.h>
#include <conio.h>
#include <math.h>
#include "MyTimer.h"             //Ò»¸ö±£Ö¤¾«È·ÑÓÊ±µÄÀ¡
#pragma comment (lib,"Winmm.lib")   //¸øÓÎÏ·Ìí¼ÓÒôÀÖÒªÓÃµ½Ëü

#define G 9.8                      //ÖØÁ¦¼ÓËÙ¶È
#define XSIZE 512                  //ÆÁÄ»´óÐ¡
#define YSIZE 384
#define X 64                       //Ö÷½ÇÆðÊ¼Î»ÖÃ
#define Y 192
#define W 32                       //Ö÷½ÇµÄ¿íºÍ¸ß
#define H 32
#define STEP 4                     //Ö÷½Ç×ßÒ»²½Ïà¾àµÄÏñËØ¸öÊý
#define HIGH (2*role.w+1)          //Ö÷½ÇÌøÔ¾µÄ×î´ó¸ß¶È
 
#define	CMD_LEFT 1                 //·½Ïò¼üµÄºê¶¨Òå
#define	CMD_RIGHT 2	
#define	CMD_UP 4
#define CMD_DOWN 8
#define CMD_SHOOT 16
#define CMD_ESC 32
int life;                          //È«¾Ö±äÁ¿£¬Ö÷½Ç¹²ÓÐ¶àÉÙÌõÉúÃü
int score;                         //È«¾Ö±äÁ¿£¬Ö÷½Ç»ñµÃµÄ·ÖÊý
struct ROLE
{
	int id;
    int x;//ºá×ø±ê
    int y;//×Ý×ø±ê
    int w;//Í¼Æ¬¿í¶È
    int h;//Í¼Æ¬¸ß¶È
    int xleft;//Ë®Æ½ÔË¶¯µÄ×ó½çÏÞ 
    int xright;//Ë®Æ½ÔË¶¯µÄÓÒ½çÏÞ
    int turn;//¾«ÁéµÄÔË¶¯·½Ïò
    int jump;//¾«ÁéÊÇ·ñÌøÔ¾
    int iframe;//¼ÓÔØµÚ¼¸¸±¾«ÁéÍ¼£¬ÕâÑù¾ÍÄÜÈÃ¾«Áé¿´ÉÏÈ¥¶¯ÆðÀ´ÁË
};
struct MAP        //´¢´æµØÍ¼µÄ½á¹¹Ìå
{
	int id;
	int x;
	int y;
};
struct BULLET      //×Óµ¯µÄ½á¹¹Ìå
{
	int x;
	int y;
	int turn;
	int iframe;
	int id;
};
struct COINT      //Ó²±ÒµÄ½á¹¹Ìå
{
	int x;
	int y;
	double iframe;
};
struct ENEMY      //µÐÈËµÄ½á¹¹Ìå
{
	int id;
	int x;
	int y;
	int turn;
	int iframe;
};
class game        //Õû¸öÓÎÏ·Ö»ÉèÖÃÁËÕâÒ»¸öÀà
{
private:
	ROLE role;
	MAP map[350];
	BULLET bullet[20];
	COINT coint[50];
	ENEMY enemy[20];
	IMAGE img_mapsky,img_p,img_map,img_ani,img_mapbk,img_home;
	int xmapsky;           //±³¾°Ìì¿ÕµÄÆðÊ¼ºá×ø±ê
	int xmap;              //µØÍ¼µÄÆðÊ¼×ø±ê
	double v0;             //¾«ÁéÌøÔ¾µÄ³õËÙ¶È             
	double h;              //¾«ÁéÌøÔ¾µÄ¸ß¶È
	double t;              //¾«ÁéÌøÔ¾µÄÊ±¼ä
	int ibullet;           //µÚ¼¸¿Å×Óµ¯
	int xbullet;           //×Óµ¯µÄx×ø±ê
	int ybullet;           //×Óµ¯µÄy×ø±ê
	int get_bullet;        //ÊÇ·ñ»ñµÃÎäÆ÷£¬0±íÊ¾Ã»ÓÐ»ñµÃ£¬1±íÊ¾ÒÑ»ñµÃ
	POINT icoint;          //´¢´æÓ²±ÒµÄ×ø±ê
	POINT bomb[20];        //´¢´æÄÄÐ©µØ·½±¬Õ¨ÁËµÄ×ø±ê
	POINT temp;            //ÁÙÊ±×ø±ê¡£´¢´æÄÄÐ©µØ·½±¬Õ¨ÁËµÄ×ø±ê
	double score_frame;    //ÏÂÃæ3¸ödoubleÐÍµÄ±äÁ¿ÓÃÓÚ¿ØÖÆ¸÷×ÔÍ¼Æ¬µÄÖ¡£¬ÒÔÊµÏÖ¶¯»­µÄÐ§¹û¡£Èç»­ÃæÖÐµÄÁ÷Ë®
	double bomb_frame;
	double mapbk_frame;
	int win;               //Íæ¼ÒÊÇ·ñ¹ý¹Ø
	int pause;             //Íæ¼ÒÊÇ·ñ°´Esc£¨ÔÝÍ£¼ü£©
public:
	game();
	~game();
	void start();          //´¦ÀíÓÎÏ·¿ªÊ¼µÄ½çÃæ£¬ºÍ°´ÔÝÍ£¼üºóµÄ½çÃæ
	void init();           //³õÊ¼»¯¸÷Ïî±äÁ¿
	void move();           //¿ØÖÆÖ÷½ÇÒÆ¶¯
	void show();           //ÏÔÊ¾»­Ãæ
	int isdie();           //ÅÐ¶ÏÖ÷½ÇÊÇ·ñÒÑËÀ
	int  GetCommand();	   // »ñÈ¡¿ØÖÆÃüÁî¡£²ÎÔÄeasyx
	void left();           //Ö÷½ÇÏò×óÔË¶¯
	void right();          //Ö÷½ÇÏòÓÒÔË¶¯
	void up();             //Ö÷½ÇÌøÔ¾
	void init_shoot();     //³õÊ¼»¯·¢Éä×Óµ¯
	void fall();	       //Ö÷½Ç×ÔÓÉÂäÌå»òÕßÏòÉÏÌøÔ¾
	int is_l_touch(int id);//Ö÷½ÇµÄ×ó±ßÊÇ·ñÅöµ½Ç½»òµÐÈË£¬ÒÔ¼°µÐÈËÊÇ·ñÅöµ½Â½µØµÄ×ó±ß½ç
	int is_r_touch(int id);//Ö÷½ÇµÄÓÒ±ßÊÇ·ñÅöµ½Ç½»òµÐÈË£¬ÒÔ¼°µÐÈËÊÇ·ñÅöµ½Â½µØµÄÓÒ±ß½ç
	int is_t_touch();      //Ö÷½ÇµÄÍ·ÊÇ·ñÅöµ½Ç½
	int is_b_touch(int id);//Ö÷½ÇÊÇ·ñ²Èµ½µÐÈË¡£
	int is_touch();        //Ö÷½ÇÊÇ·ñ³Ôµ½½ð±Ò
	int is_land(ENEMY e);  //µÐÈËÊÇ·ñÕ¾ÔÚÂ½µØÉÏ
	void getbullet();      //»ñÈ¡×Óµ¯
	void shoot();          //·¢Éä×Óµ¯
	int eat(BULLET b);     //×Óµ¯ÊÇ·ñ´òµ½µÐÈË»òÕßÇ½±Ú
	void end();            //´¦ÀíÓÎÏ·½áÊø
};
game::game()
{
	initgraph(XSIZE,YSIZE);
}
game::~game()
{
	closegraph();
}
void game::start()
{
	if(pause==1)//Èç¹û°´ÁËÔÝÍ£¼ü
	{
		BeginBatchDraw();
		int points[8]={XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3,XSIZE/2+45,YSIZE/3+90,XSIZE/2-45,YSIZE/3+90};
		setfillstyle(GREEN);
		fillpoly(4, points);
		setbkmode(TRANSPARENT);
		setfont(20,0,"ºÚÌå");
    	RECT r2={XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30};rectangle(XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30);
    	drawtext("»Øµ½ÓÎÏ·", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    	RECT r3={XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60};rectangle(XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60);
        drawtext("ÖØÐÂ¿ªÊ¼", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	    RECT r4={XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90};rectangle(XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90);
    	drawtext(" Ö÷ ²Ë µ¥ ", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		FlushBatchDraw();

	    MOUSEMSG m;
    	while(true)
    	{
			BeginBatchDraw();
    		m=GetMouseMsg();
    		switch(m.uMsg)
            {
			case WM_LBUTTONDOWN:
				EndBatchDraw();
				if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3&&m.y<YSIZE/3+30)
    	     	    return;
    			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+30&&m.y<YSIZE/3+60)
    			{
					mciSendString("close all", NULL, 0, NULL);	
    				pause=0;
					score=0;
    				return;
     			}
    			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+60&&m.y<YSIZE/3+90)
				{
					mciSendString("close all", NULL, 0, NULL);	
					pause=0;
					score=0;
					life=0;
					cleardevice();
    				break;
				}
    			else
    				break;
			case WM_MOUSEMOVE:
				RECT r;
				int i;
				for(i=0;i<3;i++)
				{
				    if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+i*30&&m.y<YSIZE/3+30+i*30)
				    {
					    r.left=XSIZE/2-45;
					    r.top=YSIZE/3+i*30;
					    r.right=XSIZE/2+45;
					    r.bottom=YSIZE/3+30+i*30;
						int points[8]={r.left,r.top,r.right,r.top,r.right,r.bottom,r.left,r.bottom};
					    setfillstyle(RED);
						fillpoly(4, points);
						setbkmode(TRANSPARENT);
						switch(i)
						{
						case 0:
						    drawtext("»Øµ½ÓÎÏ·", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						    break;
						case 1:
						    drawtext("ÖØÐÂ¿ªÊ¼", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
						    drawtext(" Ö÷ ²Ë µ¥ ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
				    else
				    {
						if(getpixel(XSIZE/2-45+1,YSIZE/3+i*30+1)==RED)
						{
				       	    r.left=XSIZE/2-45;
				            r.top=YSIZE/3+i*30;
				       	    r.right=XSIZE/2+45;
				       	    r.bottom=YSIZE/3+30+i*30;
				       		int points[8]={r.left,r.top,r.right,r.top,r.right,r.bottom,r.left,r.bottom};
			   		        setfillstyle(GREEN);
			       			fillpoly(4, points);
			       			setbkmode(TRANSPARENT);
			        		switch(i)
						    {
				    		case 0:
				    		    drawtext("»Øµ½ÓÎÏ·", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    		case 1:
				    			drawtext("ÖØÐÂ¿ªÊ¼", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    		case 2:
				    			drawtext(" Ö÷ ²Ë µ¥ ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
							}						
						}
						FlushBatchDraw();
					}
				}
			}
			if(pause==0)
				break;
		}
	}
	if(life==1||life==2)
		return;
	life=3;
	score=0;
	setfont(40,0,"·½ÕýÊæÌå");	
	RECT r1 = {0, 0, XSIZE, YSIZE/3};
	drawtext("³¬¼¶Ä¢¹½", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfont(20,0,"ËÎÌå");
	RECT r2={XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30};rectangle(XSIZE/2-45,YSIZE/3,XSIZE/2+45,YSIZE/3+30);
	drawtext("¿ªÊ¼ÓÎÏ·", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r3={XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60};rectangle(XSIZE/2-45,YSIZE/3+30,XSIZE/2+45,YSIZE/3+60);
    drawtext("ÓÎÏ·½éÉÜ", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r4={XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90};rectangle(XSIZE/2-45,YSIZE/3+60,XSIZE/2+45,YSIZE/3+90);
	drawtext("²Ù×÷ËµÃ÷", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r5={XSIZE/2-45,YSIZE/3+90,XSIZE/2+45,YSIZE/3+120};rectangle(XSIZE/2-45,YSIZE/3+90,XSIZE/2+45,YSIZE/3+120);
	drawtext("ÍË³öÓÎÏ·", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int flag1=1,flag2=0,flag3=0;
	MOUSEMSG m;
	while(flag1==1)
	{
		BeginBatchDraw();
		m=GetMouseMsg();
		switch(m.uMsg)
		{
		case WM_LBUTTONDOWN:
			EndBatchDraw();
		    if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3&&m.y<YSIZE/3+30&&flag1==1&&flag2==0&&flag3==0)
		    {
				flag1=0;
				break;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+30&&m.y<YSIZE/3+60&&flag1==1&&flag3==0)
			{
			    flag2=1;
				cleardevice();
				rectangle(50,50,213,220);
				outtextxy(52,52,"ÓÎÏ·½éÉÜ£º");
				outtextxy(52,82,"³¬¼¶ÂêÀö±ä");
				outtextxy(52,102,"Éí³¬¼¶Ä¢¹½¡£");
				
				RECT R1={XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2};rectangle(XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2);
	            drawtext("·µ»Ø", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+60&&m.y<YSIZE/3+90&&flag1==1&&flag2==0)
			{
				flag3=1;
				cleardevice();
				rectangle(50,50,213,220);
				outtextxy(52,52,"²Ù×÷ËµÃ÷£º");
				outtextxy(52,72,"×óÒÆ£ºA¼ü");
				outtextxy(52,92,"ÓÒÒÆ£ºD¼ü");
				outtextxy(52,112,"·¢Éä£ºJ¼ü");
				outtextxy(52,132,"ÌøÔ¾£ºW¼ü/K¼ü");
				outtextxy(52,152,"ÔÝÍ££ºEsc¼ü");
				RECT R2={XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2};rectangle(XSIZE-46,YSIZE-26,XSIZE-2,YSIZE-2);
	            drawtext("·µ»Ø", &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+90&&m.y<YSIZE/3+120&&flag1==1&&flag2==0&&flag3==0)
			    exit(0);
			else if(m.x>XSIZE-46&&m.x<XSIZE-3&&m.y>YSIZE-26&&m.y<YSIZE-3&&(flag2==1||flag3==1))
			{
			    cleardevice();
				flag1=0,flag2=0,flag3=0;
				start();
			}
			else
			    break;
		case WM_MOUSEMOVE:
			RECT r;
			if(flag2==1||flag3==1)
			{
			    if(m.x>XSIZE-46&&m.x<XSIZE-3&&m.y>YSIZE-26&&m.y<YSIZE-3)
				{
				    r.left=XSIZE-46;
				    r.top=YSIZE-26;
			        r.right=XSIZE-2;
					r.bottom=YSIZE-2;
					int points[8]={r.left,r.top,r.right,r.top,r.right,r.bottom,r.left,r.bottom};
				    setfillstyle(RED);
					fillpoly(4, points);
					setbkmode(TRANSPARENT);
					drawtext("·µ»Ø", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					if(getpixel(XSIZE-46+1,YSIZE-26+1)==RED)
					{
					    r.left=XSIZE-46;
			         	r.top=YSIZE-26;
			            r.right=XSIZE-2;
			        	r.bottom=YSIZE-2;
						int points[8]={r.left,r.top,r.right,r.top,r.right,r.bottom,r.left,r.bottom};
						setfillstyle(BLACK);
					    fillpoly(4, points);
					    setbkmode(TRANSPARENT);
					    drawtext("·µ»Ø", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}
			else
			{
			    for(int i=0;i<4;i++)
				{
				    if(m.x>XSIZE/2-45&&m.x<XSIZE/2+45&&m.y>YSIZE/3+i*30&&m.y<YSIZE/3+30+i*30)
				    {
					    r.left=XSIZE/2-45;
					    r.top=YSIZE/3+i*30;
					    r.right=XSIZE/2+45;
					    r.bottom=YSIZE/3+30+i*30;
						int points[8]={r.left,r.top,r.right,r.top,r.right,r.bottom,r.left,r.bottom};
					    setfillstyle(RED);
						fillpoly(4, points);
						setbkmode(TRANSPARENT);
						switch(i)
						{
						case 0:
						    drawtext("¿ªÊ¼ÓÎÏ·", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						    break;
						case 1:
						    drawtext("ÓÎÏ·½éÉÜ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
						    drawtext("²Ù×÷ËµÃ÷", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
					   	case 3:
							drawtext("ÍË³öÓÎÏ·", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
				    else
				    {
						if(getpixel(XSIZE/2-45+1,YSIZE/3+i*30+1)==RED)
						{
				       	    r.left=XSIZE/2-45;
				            r.top=YSIZE/3+i*30;
				       	    r.right=XSIZE/2+45;
				       	    r.bottom=YSIZE/3+30+i*30;
				       		int points[8]={r.left,r.top,r.right,r.top,r.right,r.bottom,r.left,r.bottom};
			   		        setfillstyle(BLACK);
			       			fillpoly(4, points);
			       			setbkmode(TRANSPARENT);
			        		switch(i)
						    {
				    		case 0:
				    		    drawtext("¿ªÊ¼ÓÎÏ·", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    		case 1:
				    			drawtext("ÓÎÏ·½éÉÜ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    		case 2:
				    			drawtext("²Ù×÷ËµÃ÷", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
				    	   	case 3:
				    			drawtext("ÍË³öÓÎÏ·", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				    			break;
							}
						}
					}
				}
			}
			FlushBatchDraw();
			break;
			default:
			    break;
		 }
	}
}
void game::init()
{
	if(pause==1)
		return;
	role.id=1;
	role.x=X;
	role.y=Y;
	role.w=W;
	role.h=H;
	role.xleft=0;
	role.xright=role.w*6+STEP;
	role.iframe=1;
	role.turn=1;
	role.jump=0;

	xmapsky=0;
	xmap=0;
	v0=0;
	h=0;
	t=0;
	ibullet=-1;
	icoint.x=-1;
	icoint.y=-1;
	score_frame=0;
	bomb_frame=1;
	mapbk_frame=1;
	temp.x=-1;
	temp.y=-1;
	xbullet=41*role.w-10;
	ybullet=4*role.h-25;
	get_bullet=0;
	win=0;
	pause=0;
	score=0;
	int i;
	for(i=0;i<350;i++)
    {
		map[i].id=0;
		map[i].x=-1;
		map[i].y=-1;
		if(i<50)
		{
			coint[i].x=-1;
			coint[i].y=-1;
			coint[i].iframe=1;
		}
		if(i<20)
		{
			bullet[i].id=0;
			bullet[i].x=-1;
	        bullet[i].y=-1;
	        bullet[i].iframe=1;
	        bullet[i].turn=-1;  

			enemy[i].id=0;
   			enemy[i].x=-1;
	        enemy[i].y=-1;
         	enemy[i].turn=1;
	        enemy[i].iframe=1;

			bomb[i].x=-1;
			bomb[i].y=-1;
	 	}
	}
	loadimage(&img_mapsky,"res\\mapsky.bmp",XSIZE,YSIZE*4);
	loadimage(&img_p,"res\\role.bmp");
	loadimage(&img_map,"res\\map.bmp");
	loadimage(&img_ani,"res\\ani.bmp");
	loadimage(&img_mapbk,"res\\mapbk.bmp");
	loadimage(&img_home,"res\\home.bmp",XSIZE,YSIZE*5);

    mciSendString("open ±³¾°ÒôÀÖ.mp3 alias mymusic1", NULL, 0, NULL);
	mciSendString("open ×Óµ¯.mp3 alias mymusic2", NULL, 0, NULL);
	mciSendString("open ½ð±Ò.mp3 alias mymusic3", NULL, 0, NULL);
	mciSendString("open Ìø.mp3 alias mymusic4", NULL, 0, NULL);
	mciSendString("open ×Óµ¯´òµ½µÐÈË.mp3 alias mymusic5", NULL, 0, NULL);
	mciSendString("open ×Óµ¯×²Ç½.mp3 alias mymusic6", NULL, 0, NULL);
	mciSendString("open ²ÈµÐÈË.mp3 alias mymusic7", NULL, 0, NULL);
	mciSendString("open ³Ôµ½ÎäÆ÷.mp3 alias mymusic8", NULL, 0, NULL);	
	mciSendString("open Ê¤Àû.mp3 alias mymusic9", NULL, 0, NULL);
	mciSendString("open ËÀÍö1.mp3 alias mymusic10", NULL, 0, NULL);
	mciSendString("open ËÀÍö2.mp3 alias mymusic11", NULL, 0, NULL);
	                                       
	for(i=0;i<300;i++)                 //ÒÔÏÂ¶¼ÊÇ±à¼­µØÍ¼
	{
		map[i].id=1;
		map[i].x=i%100*role.w;
		if(i<100)
			map[i].y=9*role.h;
		else if(i>=100&&i<200)
			map[i].y=10*role.h;
		else
			map[i].y=11*role.h;
	}
	map[15].id=1,map[15].x=18*role.w,map[15].y=8*role.h;
	map[115].id=1,map[115].x=19*role.w,map[115].y=8*role.h;
	map[215].id=1,map[215].x=20*role.w,map[215].y=8*role.h;

	map[16].id=1,map[16].x=21*role.w,map[16].y=8*role.h;
	map[116].id=1,map[116].x=22*role.w,map[116].y=8*role.h;
	map[216].id=1,map[216].x=23*role.w,map[216].y=8*role.h;

	map[17].id=1,map[17].x=24*role.w,map[17].y=8*role.h;
	map[117].id=1,map[117].x=25*role.w,map[117].y=8*role.h;
	map[217].id=1,map[217].x=26*role.w,map[217].y=8*role.h;

	map[300].id=2,map[300].x=10*role.w,map[300].y=6*role.h;
	map[301].id=2,map[301].x=11*role.w,map[301].y=6*role.h;
	map[302].id=2,map[302].x=12*role.w,map[302].y=6*role.h;

	map[303].id=3,map[303].x=36*role.w,map[303].y=7*role.h;
	map[304].id=3,map[304].x=44*role.w,map[304].y=7*role.h;

	map[305].id=2,map[305].x=40*role.w,map[305].y=4*role.h;
	map[306].id=2,map[306].x=41*role.w,map[306].y=4*role.h;
	map[307].id=2,map[307].x=42*role.w,map[307].y=4*role.h;

	map[308].id=2,map[308].x=13*role.w,map[308].y=6*role.h;

	map[309].id=4,map[309].x=15*role.w,map[309].y=10*role.h;

	map[310].id=5,map[310].x=19*role.w,map[310].y=6*role.h;
	map[311].id=5,map[311].x=23*role.w,map[311].y=6*role.h;
	map[312].id=5,map[312].x=32*role.w,map[312].y=7*role.h;
	map[313].id=5,map[313].x=48*role.w,map[313].y=7*role.h;
	map[314].id=5,map[314].x=52*role.w,map[314].y=7*role.h;
	map[315].id=5,map[315].x=56*role.w,map[315].y=7*role.h;

	map[316].id=3,map[316].x=80*role.w,map[316].y=7*role.h;
	map[317].id=3,map[317].x=90*role.w,map[317].y=7*role.h;

	map[318].id=2,map[318].x=62*role.w,map[318].y=6*role.h;

	map[319].id=2,map[319].x=65*role.w,map[319].y=3*role.h;
	map[320].id=2,map[320].x=66*role.w,map[320].y=3*role.h;
	map[321].id=2,map[321].x=67*role.w,map[321].y=3*role.h;
	map[322].id=2,map[322].x=68*role.w,map[322].y=3*role.h;
	map[323].id=2,map[323].x=69*role.w,map[323].y=3*role.h;

	map[349].id=6,map[349].x=97*role.w,map[349].y=7*role.h;

	for(i=64;i<300;i+=100)
	{
		map[i].id=0;map[i].x=-1;map[i].y=-1;
		map[i+1].id=0;map[i+1].x=-1;map[i+1].y=-1;
		map[i+2].id=0;map[i+2].x=-1;map[i+2].y=-1;

		map[i+7].id=0;map[i].x=-1;map[i].y=-1;
		map[i+8].id=0;map[i+1].x=-1;map[i+1].y=-1;
		map[i+9].id=0;map[i+1].x=-1;map[i+1].y=-1;

		map[i+11].id=0;map[i].x=-1;map[i].y=-1;
		map[i+12].id=0;map[i+1].x=-1;map[i+1].y=-1;
		map[i+13].id=0;map[i+1].x=-1;map[i+1].y=-1;
	}
	map[64].id=4,map[64].x=64*role.w,map[64].y=10*role.h;
	map[71].id=4,map[71].x=71*role.w,map[71].y=10*role.h;
	map[75].id=4,map[75].x=75*role.w,map[75].y=10*role.h;

	enemy[0].id=1;enemy[0].x=6*role.w;enemy[0].y=8*role.h;enemy[0].turn=1;enemy[0].iframe=1;
	enemy[1].id=1;enemy[1].x=8*role.w;enemy[1].y=8*role.h;enemy[1].turn=1;enemy[1].iframe=1;
	enemy[2].id=1;enemy[2].x=27*role.w;enemy[2].y=8*role.h;enemy[2].turn=1;enemy[2].iframe=1;
	enemy[3].id=1;enemy[3].x=29*role.w;enemy[3].y=8*role.h;enemy[3].turn=1;enemy[3].iframe=1;
	enemy[4].id=1;enemy[4].x=31*role.w;enemy[4].y=8*role.h;enemy[4].turn=1;enemy[4].iframe=1;
	enemy[5].id=1;enemy[5].x=33*role.w;enemy[5].y=8*role.h;enemy[5].turn=1;enemy[5].iframe=1;
	enemy[6].id=1;enemy[6].x=35*role.w;enemy[6].y=8*role.h;enemy[6].turn=1;enemy[6].iframe=1;
	enemy[7].id=1;enemy[7].x=40*role.w;enemy[7].y=8*role.h;enemy[7].turn=1;enemy[7].iframe=1;
	enemy[8].id=1;enemy[8].x=82*role.w;enemy[8].y=8*role.h;enemy[8].turn=1;enemy[8].iframe=1;
	enemy[9].id=1;enemy[9].x=65*role.w;enemy[9].y=2*role.h;enemy[9].turn=1;enemy[9].iframe=1;
	enemy[10].id=1;enemy[10].x=69*role.w;enemy[10].y=2*role.h;enemy[10].turn=1;enemy[10].iframe=1;
	enemy[11].id=1;enemy[11].x=85*role.w;enemy[11].y=8*role.h;enemy[11].turn=1;enemy[11].iframe=1;

	for(i=0;i<4;i++)
	{
		coint[i].x=(10+i)*role.w;
	    coint[i].y=5*role.h;

		coint[i+4].x=(67+i)*role.w;
		coint[i+4].y=8*role.w;

		coint[i+8].x=74*role.w;
		coint[i+8].y=(4+i)*role.w;
	}
	for(i=12;i<18;i++)
	{
		coint[i].x=(83-12+i)*role.w;
	    coint[i].y=6*role.h;

		coint[i+6].x=(83-12+i)*role.w;
		coint[i+6].y=7*role.w;
	}
}
void game::move()
{	
	MyTimer tt;
	int c;
	int k=0;                              //¿ØÖÆ·¢Éä×Óµ¯µÄÆµÂÊºÍµÐÈËµÄÒÆ¶¯ËÙ¶È
	int n=0;                              //¿ØÖÆ·¢Éä×Óµ¯µÄÆµÂÊ
	while(true)
	{
		tt.Sleep(25);
		t=sqrt(2*HIGH/G)/14;		
		k++;
		if(k==1000)
	    	k=0;
		if(kbhit()&&win==0)
		{
			c=GetCommand();
	    	if(c&CMD_LEFT)
	    		left();
	    	if(c&CMD_RIGHT)
				right();
	    	if((c&CMD_UP)&&role.jump==0)
				up();
			if(c&CMD_ESC)
			{
				pause=1;
				break;
			}
			if(c&CMD_SHOOT&&get_bullet==1)
			{
				if(n==0)
			    {
			    	init_shoot();
			    	n=1;
			    }
				n++;
				if(k%10==0&&n>10)
			    {
			    	init_shoot();
			    }
			}
			else
			    n=0;
		}
		if(-xmap+role.x==97*role.w)
		{
		    mciSendString("stop mymusic1", NULL, 0, NULL);
			mciSendString("play mymusic9", NULL, 0, NULL);
		}
		if(-xmap+role.x>95*role.w)
		{
			
            win=1;
			role.x+=STEP;
			if(role.x-STEP>XSIZE)
				break;
		}
		if(is_b_touch(1)==0)
			role.jump=1;
		if(role.jump==1)
			fall();
		if(isdie()==1)
		{
			mciSendString("stop mymusic1", NULL, 0, NULL);
			mciSendString("play mymusic11", NULL, 0, NULL);
			life--;
			return;	
		}
		if(k%2==0)               //µÐÈËµÄÔË¶¯
		{
	    	for(int i=0;i<20;i++)
	    	{
		    	if(enemy[i].id==1)
		    	{
		            if(is_land(enemy[i])==1)
		            {
		    	        if(enemy[i].turn==1)
			    	        enemy[i].x+=STEP;
			            else
				            enemy[i].x-=STEP;
			    	}
		            if(is_land(enemy[i])==0||is_l_touch(3)==1||is_r_touch(3)==1)
		            {
		            	if(enemy[i].turn==1)
		            		enemy[i].x-=STEP;
		            	else
		            		enemy[i].x+=STEP;
		            	enemy[i].turn*=-1;
		            }
				    enemy[i].iframe*=-1;
			    }
	    	}
		}
		int boom=0;
		if(is_b_touch(2)==1)                     //Èç¹ûÖ÷½Ç¡°²Èµ½¡±µÐÈË
			boom=1;
		getbullet();                             //»ñÈ¡×Óµ¯
		if(get_bullet==1)
			shoot();

		BeginBatchDraw();
		show();
		FlushBatchDraw();

		if((is_l_touch(2)==1||is_r_touch(2)==1))
		{
			mciSendString("stop mymusic1", NULL, 0, NULL);
			mciSendString("play mymusic10", NULL, 0, NULL);
			life--;
			pause=0;
			putimage(role.x,role.y,role.w,role.h,&img_p,2*role.w,role.h,SRCAND);
	        putimage(role.x,role.y,role.w,role.h,&img_p,2*role.w,0,SRCPAINT);
		    return;
		}
	}
}
void game::show()
{
	if(xmapsky==-XSIZE)
		xmapsky=0;
    putimage(xmapsky,0,&img_mapsky);     //ÏÔÊ¾±³¾°
	putimage(XSIZE+xmapsky,0,&img_mapsky);

	if(is_touch()==1)
		score_frame=1;
	if(score_frame!=0)                  //Åöµ½Ó²±Ò£¬ÏÔÊ¾µÃ·Ö
	{		
		switch((int)score_frame)
		{
		case 1:
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,0,11*role.h,SRCAND);
		    putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,0,10*role.h,SRCPAINT);
			break;
		case 2:
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,role.w,11*role.h,SRCAND);
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,role.w,10*role.h,SRCPAINT);
			break;
		case 3:
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,2*role.w,11*role.h,SRCAND);
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,2*role.w,10*role.h,SRCPAINT);
			break;
		case 4:
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,3*role.w,11*role.h,SRCAND);
			putimage(xmap+icoint.x,icoint.y,role.w,role.h,&img_ani,3*role.w,10*role.h,SRCPAINT);
			break;
		default:
			break;
		}	
		score_frame+=0.2;
	    if(score_frame==5)
		    score_frame=0;
	}
	int i;
	for(i=0;i<350;i++)             //ÏÔÊ¾µØÍ¼£¬Ìì¿ÕÉÏµÄµØÍ¼ºÍÓ²±Ò
	{
		if(map[i].id==1)
		{
			putimage(xmap+map[i].x,map[i].y,role.w,role.h,&img_map,0,0);
		}
		else if(map[i].id==2)
		{
			putimage(xmap+map[i].x,map[i].y,role.w,role.h,&img_map,0,role.h);
		}
		else if(map[i].id==3)
		{
			putimage(xmap+map[i].x,map[i].y,2*role.w,2*role.h,&img_map,0,9*role.h);
		}
		else
		{
		    if(map[i].id==4)
		    {
		    	switch((int)mapbk_frame)
		    	{
		    	case 1:
		    		putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,0,10*role.h,SRCAND);
		    	    putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,0,8*role.h,SRCPAINT);
		    		break;
		    	case 2:
		   	     	putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,3*role.w,10*role.h,SRCAND);
		     		putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,3*role.w,8*role.h,SRCPAINT);
			    	break;
	    		default:
	    			break;
	    		}	
		    }
			else if(map[i].id==5)
		    {
		    	switch((int)mapbk_frame)
		    	{
		    	case 1:
		    		putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,0,2*role.h,SRCAND);
		    	    putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,0,0,SRCPAINT);
		    		break;
		    	case 2:
		   	     	putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,3*role.w,2*role.h,SRCAND);
		     		putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,3*role.w,0,SRCPAINT);
			    	break;
	    		default:
	    			break;
	    		}	
		    }
			else if(map[i].id==6)
			{
				switch((int)mapbk_frame)
		    	{
		    	case 1:
		    		putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,0,6*role.h,SRCAND);
					putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,0,4*role.h,SRCPAINT);
		    		break;
		    	case 2:
		   	     	putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,3*role.w,6*role.h,SRCAND);
					putimage(xmap+map[i].x,map[i].y,3*role.w,2*role.h,&img_mapbk,3*role.w,4*role.h,SRCPAINT);
			    	break;
	    		default:
	    			break;
	    		}	
			}
			mapbk_frame+=0.003;
	        if(mapbk_frame>2.9)
	   		{
	   			mapbk_frame=1;
	   		}
		}
		if(i<50)
		{
			if(coint[i].x!=-1||coint[i].y!=-1)
			{
				switch((int)coint[i].iframe)
				{
				case 1:
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,0,9*role.h,SRCAND);
				    putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,0,8*role.h,SRCPAINT);
					break;
				case 2:
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,role.w,9*role.h,SRCAND);
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,role.w,8*role.h,SRCPAINT);
					break;
				case 3:
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,2*role.w,9*role.h,SRCAND);
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,2*role.w,8*role.h,SRCPAINT);
					break;
				case 4:
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,3*role.w,9*role.h,SRCAND);
					putimage(xmap+coint[i].x,coint[i].y,role.w,role.h,&img_ani,3*role.w,8*role.h,SRCPAINT);
					break;
				default:
					break;
				}	
				coint[i].iframe+=0.125;
			    if(coint[i].iframe==5)
				    coint[i].iframe=1;
			}
		}
	}
	if(get_bullet==0)
	{

		switch((int)mapbk_frame)
	 	{
		    case 1:
		   		putimage(xmap+xbullet,ybullet,52,25,&img_ani,0,12*role.h+25,SRCAND);
                putimage(xmap+xbullet,ybullet,52,25,&img_ani,0,12*role.h,SRCPAINT);
		    	break;
		    case 2:
     	     	putimage(xmap+xbullet,ybullet,52,25,&img_ani,52,12*role.h+25,SRCAND);
                putimage(xmap+xbullet,ybullet,52,25,&img_ani,52,12*role.h,SRCPAINT);
			   	break;
	    	default:
	    	    break;
	  	}	

	}
	for(i=0;i<20;i++)             //ÏÔÊ¾×Óµ¯
	{
		if(get_bullet==1)
		{
		    if(bullet[i].id==1)
	        {
	            if(bullet[i].iframe==1)
    	        {
	            	putimage(bullet[i].x,bullet[i].y,role.w,role.h,&img_ani,0,3*role.h,SRCAND);
    		        putimage(bullet[i].x,bullet[i].y,role.w,role.h,&img_ani,0,2*role.h,SRCPAINT);
	            }
    	        else
    	        {
    	        	putimage(bullet[i].x,bullet[i].y,role.w,role.h,&img_ani,role.w,3*role.h,SRCAND);
    	        	putimage(bullet[i].x,bullet[i].y,role.w,role.h,&img_ani,role.w,2*role.h,SRCPAINT);
        	    }	
            }
		}
		if(enemy[i].id==1)
		{
	    	if(enemy[i].iframe==1)                           //ÏÔÊ¾µÐÈË
	        { 
		        putimage(xmap+enemy[i].x,enemy[i].y,role.w,role.h,&img_ani,0,role.h,SRCAND);
    	        putimage(xmap+enemy[i].x,enemy[i].y,role.w,role.h,&img_ani,0,0,SRCPAINT);
	        }
	        else
	        {
	        	putimage(xmap+enemy[i].x,enemy[i].y,role.w,role.h,&img_ani,role.w,role.h,SRCAND);
	        	putimage(xmap+enemy[i].x,enemy[i].y,role.w,role.h,&img_ani,role.w,0,SRCPAINT);
	        }
	        
		}
		if(bomb[i].x!=-1||bomb[i].y!=-1)
		{
			switch((int)bomb_frame)
			{
			case 1:
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,0,6*role.h,SRCAND);
			    putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,0,4*role.h,SRCPAINT);
				break;
			case 2:
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,2*role.w,6*role.h,SRCAND);
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,2*role.w,4*role.h,SRCPAINT);
				break;
			case 3:
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,4*role.w,6*role.h,SRCAND);
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,4*role.w,4*role.h,SRCPAINT);
				break;
			case 4:
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,6*role.w,6*role.h,SRCAND);
				putimage(xmap+bomb[i].x-role.w/2,bomb[i].y-role.h/2,2*role.w,2*role.h,&img_ani,6*role.w,4*role.h,SRCPAINT);
				break;
			default:
				break;
			}	
			bomb_frame+=0.25;
		    if(bomb_frame==5)
			{
				bomb[i].x=-1;
			    bomb[i].y=-1;
				bomb_frame=1;
			}
		}
	}
	int n=score;
	char s1[20]="µ±Ç°µÃ·Ö£º";
	char s2[10];
	itoa(n,s2,10);
	RECT r1={10,10,110,40};
	RECT r2={110,10,150,40};
	setfont(20, 0,"ËÎÌå");
	drawtext(s1, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(s2, &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	if(role.iframe==1)                           //ÏÔÊ¾Ö÷½Ç
	{
		if(role.turn==1)
		{
		    putimage(role.x,role.y,role.w,role.h,&img_p,0,role.h,SRCAND);
    	    putimage(role.x,role.y,role.w,role.h,&img_p,0,0,SRCPAINT);
		}
		else
		{
			putimage(role.x,role.y,role.w,role.h,&img_p,4*role.w,role.h,SRCAND);
    	    putimage(role.x,role.y,role.w,role.h,&img_p,4*role.w,0,SRCPAINT);
		}
	}
	else
	{
		if(role.turn==1)
		{
			putimage(role.x,role.y,role.w,role.h,&img_p,role.w,role.h,SRCAND);
    	    putimage(role.x,role.y,role.w,role.h,&img_p,role.w,0,SRCPAINT);
		}
		else
		{
			putimage(role.x,role.y,role.w,role.h,&img_p,3*role.w,role.h,SRCAND);
    	    putimage(role.x,role.y,role.w,role.h,&img_p,3*role.w,0,SRCPAINT);
		}
	}
}  
int game::isdie()
{
	if(role.y>=YSIZE)
		return 1;
	else
		return 0;
}
int game::GetCommand()
{
	int c = 0;

	if (GetAsyncKeyState('A') & 0x8000)
		c |= CMD_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)
    	c |= CMD_RIGHT;
	if ((GetAsyncKeyState('W') & 0x8000)||(GetAsyncKeyState('K') & 0x8000))
    	c |= CMD_UP;
	if (GetAsyncKeyState('S') & 0x8000)
    	c |= CMD_DOWN;
	if (GetAsyncKeyState('J') & 0x8000)
    	c |= CMD_SHOOT;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    	c |= CMD_ESC;
	return c;
}
void game::left()
{
	role.iframe*=-1;	
	role.turn=-1;
	role.x-=STEP;
	
	if(is_l_touch(1)==1)
		role.x+=STEP;	
	if(role.x<role.xleft)
      	role.x+=STEP;
}
void game::right()
{
	role.iframe*=-1;	
	role.turn=1;
	role.x+=STEP;
	
	if(is_r_touch(1)==1)
		role.x-=STEP;
	if(role.x>role.xright&&(-xmap+role.x<90*role.w))
	{
    	role.x-=STEP;
		xmapsky-=1;
	    xmap-=STEP;
	}
}
void game::up()
{
	mciSendString("play mymusic4 from 0", NULL, 0, NULL);
	role.iframe*=-1;	
	v0=-sqrt(2*G*HIGH);
	role.jump=1;
}
void game::init_shoot()
{
	mciSendString("play mymusic2 from 0", NULL, 0, NULL);
	ibullet++;
	if(ibullet==20)
		ibullet=0;
	bullet[ibullet].id=1;
	bullet[ibullet].y=role.y+8;
    bullet[ibullet].turn=role.turn;

	if(bullet[ibullet].turn==1)
		bullet[ibullet].x=role.x+10;
	else
		bullet[ibullet].x=role.x-26;
}
int game::is_l_touch(int id)
{
	int x,y;
	int i;
	if(id==1)                                    //id==1±íÊ¾Ö÷½ÇÊÇ·ñÅöµ½idÎª1µÄµØÍ¼£¬¼°ÓÎÏ·ÖÐ»ÆÉ«µÄµØÍ¼
	{
    	x=-xmap+role.x;
    	y=role.y;
    	for(i=0;i<350;i++)
    	{
    		if(map[i].id!=0&&map[i].id<4)
    		{
    			POINT m[2];

			    m[0].x=map[i].x;
			    m[0].y=map[i].y;

		    	m[1].x=map[i].x+role.w;
		    	m[1].y=map[i].y;

				if(map[i].id==3)
				{
					if(((y-m[1].y)/role.h==0||(y-m[1].y-role.h)/role.h==0)&&x>m[1].x&&x<m[1].x+role.w)
			    	    return 1;
				}
				else
				{
					if((y-m[1].y)/role.h==0&&x>m[0].x&&x<m[1].x)
			    	    return 1;
				}
		    }
	    }
	    return 0;
	}
   	else if(id==2)                                 //id==2±íÊ¾Ö÷½ÇÊÇ·ñÅöµ½µÐÈËµÄ×ó±ß
   	{
        x=-xmap+role.x;
       	y=role.y;
       	for(i=0;i<20;i++)
		{
        	if(enemy[i].id!=0)
        	{
		    	POINT m[2];  

	    		m[0].x=enemy[i].x;
	    		m[0].y=enemy[i].y;

	    	   	m[1].x=enemy[i].x+role.w;
		    	m[1].y=enemy[i].y;

		    	if((y-m[1].y)/role.h==0&&x>m[0].x&&x<m[1].x)  		
	    			return 1;
			}
		}
	    return 0;
	}
	else                                        //id==3±íÊ¾µÐÈËÊÇ·ñÅöµ½µØÍ¼µÄ×ó±ß
	{
		int j;
		for(j=0;j<20;j++)
		{
			if(enemy[j].id!=0)
			{
    	        x=enemy[j].x;
    	        y=enemy[j].y;
			
    	        for(i=0;i<350;i++)
		        {
            		if(map[i].id!=0&&map[i].id<4)
            		{
            			POINT m[2];
     
			            m[0].x=map[i].x;
	        		    m[0].y=map[i].y;

		            	m[1].x=map[i].x+role.w;
		            	m[1].y=map[i].y;

			         	if(map[i].id==3)
			           	{
			        		if(((y-m[1].y)/role.h==0||(y-m[1].y-role.h)/role.h==0)&&x>m[1].x&&x<m[1].x+role.w)
			    	            return 1;
		         		}   
			        	else
			        	{
			        		if((y-m[1].y)/role.h==0&&x>m[0].x&&x<m[1].x)
			    	            return 1;
				        }   
					}
				}
			}
		}
		return 0;
	}
}
int game::is_r_touch(int id)
{
	int x,y;
	int i;
	if(id==1)
	{    
	    x=-xmap+role.x+role.w;
	    y=role.y;

	    for(i=0;i<350;i++)
    	{
	    	if(map[i].id!=0&&map[i].id<4)
		    {
			    POINT m[2];
 
    			m[0].x=map[i].x;
    			m[0].y=map[i].y;

    			m[1].x=map[i].x+role.w;
    			m[1].y=map[i].y;

				if(map[i].id==3)
				{
					if(((y-m[0].y)/role.h==0||(y-m[0].y-role.h)/role.h==0)&&x>m[0].x&&x<m[1].x)
			    	    return 1;
				}
				else
				{
					if((y-m[0].y)/role.h==0&&x>m[0].x&&x<m[1].x)
		    		    return 1;
				}
		    }
	    }
	    return 0;
	}
	else if(id==2)
	{
	    x=-xmap+role.x+role.w;
	    y=role.y;

	    for(i=0;i<20;i++)
	    {
		    if(enemy[i].id!=0)
		    {
		    	POINT m[2];

			    m[0].x=enemy[i].x;
		    	m[0].y=enemy[i].y;
 
    			m[1].x=enemy[i].x+role.w;
    			m[1].y=enemy[i].y;

	    		if((y-m[0].y)/role.h==0&&x>m[0].x&&x<m[1].x)
		    		return 1;
	    	}
	    }
	    return 0;
	}
	else
	{
		int j;
		for(j=0;j<20;j++)
		{
			if(enemy[j].id!=0)
			{
			    x=enemy[j].x+role.w;
	            y=enemy[j].y;
			
	            for(i=0;i<350;i++)
		        {
	            	if(map[i].id!=0&&map[i].id<4)
        		    {
		        	    POINT m[2];
 
            			m[0].x=map[i].x;
            			m[0].y=map[i].y;

            			m[1].x=map[i].x+role.w;
    	        		m[1].y=map[i].y;

			        	if(map[i].id==3)
			        	{
				        	if(((y-m[0].y)/role.h==0||(y-m[0].y-role.h)/role.h==0)&&x>m[0].x&&x<m[1].x)
			    	            return 1;
				        }
				        else
				        {
					        if((y-m[0].y)/role.h==0&&x>m[0].x&&x<m[1].x)
		    		            return 1;
				        }
					}
	            }
			}
		}
		return 0;
	}
}
int game::is_t_touch()
{
	int x,y;
	x=-xmap+role.x;
	y=role.y;

	for(int i=0;i<350;i++)
	{
		if(map[i].id!=0&&map[i].id<4)
		{
			POINT m[2];

			m[0].x=map[i].x;
			m[0].y=map[i].y;

			m[1].x=map[i].x;
			m[1].y=map[i].y+role.h;

			if((x-m[1].x)/role.w==0&&y>m[0].y&&y<m[1].y)
				return 1;
		}
	}
	return 0;
}
int game::is_b_touch(int id)
{
	if(id==1)
	{
	    int x,y;
	    x=-xmap+role.x;
    	y=role.y+role.h;

	    for(int i=0;i<350;i++)
	    {
    		if(map[i].id!=0&&map[i].id<4)
    		{
    			POINT m[2];  

    			m[0].x=map[i].x;
	    		m[0].y=map[i].y;

	    		m[1].x=map[i].x; 
		    	m[1].y=map[i].y+role.h;

				if(map[i].id==3)
				{
					if(((x-m[0].x)/role.w==0||(x+role.w-m[0].x-2*role.w)/role.w==0)&&y>=m[0].y&&y<m[1].y)
			    	    return 1;
				}
				else
				{
					if((x-m[0].x)/role.w==0&&y>=m[0].y&&y<m[1].y)
			    	    return 1;
				}
			}
		}
	    return 0;
	}
	else if(id==2)
	{
		int x,y;
	    x=-xmap+role.x;
	    y=role.y+role.h;

    	for(int i=0;i<20;i++)
    	{
    		if(enemy[i].id!=0)
    		{
    			POINT m[2];

	    		m[0].x=enemy[i].x;
	    		m[0].y=enemy[i].y;

		    	m[1].x=enemy[i].x;
		    	m[1].y=enemy[i].y+role.h;

		    	if((x-m[0].x)/role.w==0&&y>m[0].y&&y<m[1].y)
				{
			    	mciSendString("play mymusic7 from 0", NULL, 0, NULL);
					score+=10;
					bomb[i].x=enemy[i].x;
		        	bomb[i].y=enemy[i].y;
					enemy[i].id=0;
			        enemy[i].iframe=-1;
			        enemy[i].turn=1;
		        	enemy[i].x=-1;
		        	enemy[i].y=-1;
		    		return 1;
				}
		    }
		}
	    return 0;
	}
	return 0;
}
int game::is_touch()
{
	int i,j;
	POINT r[2];
	r[0].x=-xmap+role.x;
	r[0].y=role.y;
	r[1].x=-xmap+role.x+role.w;
	r[1].y=role.y+role.h;
	for(i=0;i<50;i++)
	{
		if(coint[i].x!=-1||coint[i].y!=-1)
		{
			POINT c[4];

			c[0].x=coint[i].x;
			c[0].y=coint[i].y;

			c[1].x=coint[i].x+role.w;
			c[1].y=coint[i].y;

			c[2].x=coint[i].x;
			c[2].y=coint[i].y+role.h;

			c[3].x=coint[i].x+role.w;
			c[3].y=coint[i].y+role.h;

		    for(j=0;j<4;j++)
			{
				if(c[j].x>=r[0].x&&c[j].y>=r[0].y&&c[j].x<=r[1].x&&c[j].y<=r[1].y)
				{
			    	mciSendString("play mymusic3 from 0", NULL, 0, NULL);
					score+=20;
					icoint.x=coint[i].x;
					icoint.y=coint[i].y;

					coint[i].x=-1;
					coint[i].y=-1;
					coint[i].iframe=1;

					return 1;
				}
			}
		}
	}
	return 0;
}
int game::is_land(ENEMY e)
{
	POINT r[2];
	r[0].x=e.x;
	r[0].y=e.y+role.h;

	r[1].x=e.x+role.h;
	r[1].y=e.y+role.h;

	for(int i=0;i<350;i++)
	{
		if(map[i].id!=0&&map[i].id<4)
		{
			POINT m[3];

			m[0].x=map[i].x;
			m[0].y=map[i].y;
			
			m[1].x=map[i].x+role.w;
			m[1].y=map[i].y;

			m[2].x=map[i].x;
			m[2].y=map[i].y+role.h;

			if(e.turn==1)
			{
				if((r[1].x-m[0].x)/role.w==0&&r[1].y>=m[0].y&&r[1].y<m[2].y)
			    	return 1;
			}
			else
			{
				if((r[0].x-m[1].x)/role.w==0&&r[0].y>=m[0].y&&r[0].y<m[2].y)
			    	return 1;
			}
		}
	}
	return 0;
}
void game::getbullet()
{
	int i;
	POINT r[2];
	r[0].x=-xmap+role.x;
	r[0].y=role.y;
	r[1].x=-xmap+role.x+role.w;
	r[1].y=role.y+role.h;

	POINT b[4];
	b[0].x=xbullet;
	b[0].y=ybullet;

	b[1].x=xbullet+52;
	b[1].y=ybullet;

	b[2].x=xbullet;
	b[2].y=ybullet+25;

	b[3].x=xbullet+52;
	b[3].y=ybullet+25;
	
	for(i=0;i<4;i++)
	{
		if(b[i].x>=r[0].x&&b[i].y>=r[0].y&&b[i].x<=r[1].x&&b[i].y<=r[1].y)
		{
			mciSendString("play mymusic8 from 0", NULL, 0, NULL);
			get_bullet=1;
			xbullet=0;
			ybullet=0;
		}
	}
}
void game::fall()           
{

	h=v0*t+G*pow(t,2)/2;
	
    role.y+=(int)(h+0.5);
	if(v0>=0)   //×ÔÓÉÂäÌå
	{
		if(isdie()==1)
			return;    
		if(is_b_touch(1)==1)
		{
			v0=0;
			role.y=role.y/role.h*role.h;
			role.jump=0;
		}
	}
	else       //ÏòÉÏÌøÔ¾
	{
		if(v0>=0)
			h=0;
		else
			role.y+=(int)(h+0.5);

		if(is_t_touch()==1)
		{
			v0=0;
			h=0;
			role.y=role.y/role.h*role.h+role.h;
		}
	}
	v0=v0+G*t;
}
void game::shoot()
{
	int i;
	for(i=0;i<20;i++)
	{
		if(bullet[i].id==1)
		{
			
	        if(bullet[i].turn==1)
			{
	     	    bullet[i].x+=2*STEP;
			}
			else
			{
		        bullet[i].x-=2*STEP;
			}
			if((bullet[i].x<(-3*role.w))||(bullet[i].x>XSIZE))
			{
		        bullet[i].id=0;
				bullet[i].x=-1;
	            bullet[i].y=-1;
	            bullet[i].iframe=1;
	            bullet[i].turn=1;	
			}
	        if(eat(bullet[i])==1)
			{
		        bullet[i].id=0;
				bullet[i].x=-1;
	            bullet[i].y=-1;
	            bullet[i].iframe=1;
	            bullet[i].turn=1;

				bomb[i].x=temp.x;
				bomb[i].y=temp.y;
			}
			bullet[i].iframe*=-1;
		}
	}
}

int game::eat(BULLET b)
{
	POINT r[4];
	r[0].x=-xmap+b.x+role.w/2;
	r[0].y=b.y;
	r[1].x=-xmap+b.x+role.w;
	r[1].y=b.y;
	r[2].x=-xmap+b.x+role.w/2;
	r[2].y=b.y+role.h/2;
	r[3].x=-xmap+b.x+role.w;
	r[3].y=b.y+role.h/2;

	int i;
	for(i=0;i<350;i++)
	{
		if(map[i].id!=0&&map[i].id<4)
		{
			POINT m[2];

			m[0].x=map[i].x;
			m[0].y=map[i].y;

			if(map[i].id==3)
			{
				m[1].x=map[i].x+2*role.w;
			    m[1].y=map[i].y+2*role.h;
			}
			else
			{
				m[1].x=map[i].x+role.w;
			    m[1].y=map[i].y+role.h;
			}

			for(int j=0;j<4;j++)
			{
				if(r[j].x>m[0].x&&r[j].x<m[1].x&&r[j].y>m[0].y&&r[j].y<m[1].y)
				{
	                mciSendString("play mymusic6 from 0", NULL, 0, NULL);
					temp.x=r[0].x-role.w/4;
			        temp.y=r[0].y-role.w/4;
					return 1;
				}
			}
		}
		if(i<20)
		{
	    	if(enemy[i].id==1)
	    	{
	    		POINT e[2];

    			e[0].x=enemy[i].x;
    			e[0].y=enemy[i].y;

	    		e[1].x=enemy[i].x+role.w;
				e[1].y=enemy[i].y+role.h;

				for(int j=0;j<4;j++)
			    {
				    if(r[j].x>e[0].x&&r[j].x<e[1].x&&r[j].y>e[0].y&&r[j].y<e[1].y)
			    	{
						
	                    mciSendString("play mymusic5 from 0", NULL, 0, NULL);
						score+=10;
			    		temp.x=enemy[i].x;
			            temp.y=enemy[i].y;

			    		enemy[i].id=0;
		    			enemy[i].iframe=-1;
		    			enemy[i].turn=1;
		    			enemy[i].x=-1;
		    			enemy[i].y=-1;

				    	return 1;
				    }
			    }
		    }
		}
	}
	return 0;
}
void game::end()
{
	MyTimer tt;
	EndBatchDraw();
	if(isdie()==1||win==1)
		pause=0;
	if(pause==1)
		return;
	if(win==1)
		tt.Sleep(5000);
	else
		tt.Sleep(2700);
	mciSendString("close all", NULL, 0, NULL);	
	tt.Sleep(1000);
	
	if(win==1)
	{
		pause=0;
		score=0;
		life=0;
		mciSendString("open Í¨¹Ø.mp3 alias mymusic13", NULL, 0, NULL);
	    mciSendString("play mymusic13", NULL, 0, NULL);
		putimage(0,-3*YSIZE,&img_home);
		tt.Sleep(7000);
		mciSendString("close mymusic13", NULL, 0, NULL);
	}
	else
	{
		score=0;
		if(life==0)
		{
			mciSendString("open ÓÎÏ·½áÊø.mp3 alias mymusic12", NULL, 0, NULL);
	        mciSendString("play mymusic12", NULL, 0, NULL);
			putimage(0,-YSIZE,&img_home);
		    tt.Sleep(5500);
	        mciSendString("close mymusic12", NULL, 0, NULL);
		}
		else
		{
			cleardevice();
			outtextxy(XSIZE/2-43,YSIZE/3,"ÉúÃü»¹Ê£ÏÂ:");
			if(life==1)
				outtextxy(XSIZE/2,YSIZE/2-20,"1");
			else if(life==2)
				outtextxy(XSIZE/2,YSIZE/2-20,"2");
			tt.Sleep(2000);
		}
	}
	cleardevice();
}
void main()

{
	game g;
	while(true)
	{
		g.start();
	    g.init();
		mciSendString ("play mymusic1 repeat", NULL,0, NULL);
	    g.show();
		g.move();
		g.end();
	}
}
