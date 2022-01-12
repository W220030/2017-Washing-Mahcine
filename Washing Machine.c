#include<reg51.h>
//****************************//
#define uchar unsigned char
#define uint unsigned int


//***************************//
sbit mo_r = P3^2;	  //电机右控制线
sbit mo_l = P3^3;		//电机左控制线

//****************************//
sbit key_menu = P3^4; // 菜单按键
sbit key_on = P3^5;	  // 开始按键
sbit key_off = P3^6; //	 结束按键
sbit key_se = P3^7;	 //	 菜单选择按键

//***************************//
sbit led_in = P0^0;		//	进水指示灯
sbit led_xi = P0^1;	   //  洗衣指示灯
sbit led_pao = P0^2;	//	泡洗指示灯
sbit led_xx = P0^3;	   //  脱水指示灯
sbit led_out = P0^4;  //   出水指示灯
sbit led_over = P0^5;  // 洗衣结束指示灯
sbit led_work = P0^6;  // 电机工作指示灯
sbit led_wring = P0^7;	 //	报警指示灯

sbit other = P3^1;	 //	 脱水盖子保护开关
sbit anther = P3^0;	 //	洗衣电源控制开关

//******************************//
uchar code num[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
								   //
//*****************************//
char sec = 0;		  //  时间秒
char min = 0;		   // 时间分
uchar count=0;		  //  中断计数
uchar flag0=0;		  //  洗衣机工作状态标志
uchar flag1=0;		  //  进水次数标志
uchar flag2=0;		  //  出水次数标志
uchar flag3=0;		 //	  泡洗次数标志
uchar err =0;	   //	报警标志
uchar quan = 0;//正反转计数  
//**********************************//
//	 函数声明
//*************************************//
void delay();//	延时函数
void in();	//	  进水子程序
void out();	 //	  出水子程序
void over();  //  结束子程序
void xi();	 //	  洗衣子程序
void pao();	//	 泡衣子程序
void xx(); //	脱水子程序
void on(); //  工作on处理子程序
void se(); //  显示菜单选择
void SEG_display();	 //显示时间子程序
void key_scan();   //  按键扫描子程序

//*****************************//
//     延时函数
//****************************//
void delay(uint i)
{
	uint x,y;
	for(x=i;x>0;x--)
	 for(y=50;y>0;y--);
}

//******************************//
//	   工作on处理子程序
//********************************//
void on()
{
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;

	P0 = 0xff;
	if(flag0 == 0)
	    in();
	if(flag0 == 1)
	    xi();
	if(flag0 == 2)
	    pao();
	if(flag0 == 3)
	    xx();
	if(flag0 == 4)
	    out();
		
}

//*******************************//
//		 结束子程序
//*********************************//
void over()
{	other=0;
	anther=0;
	P0 = 0xff;
	mo_r=0;
	mo_l=0;
	led_over = 0;
	led_wring=0;
	EA=0;

}

//*********************************//
//	   进水子程序
//*********************************//
void in()
{	anther=0;
	other=1;
	P0 = 0xff;
	led_in = 0;
	flag1++;
	mo_r = 0;
	mo_l = 0  ;
	min = 0;
	sec = 5;
}
//*********************************//
//		洗衣子程序
//*********************************//
void xi()
{	 anther=1;
	 other=1;
     P0 = 0xff;
	 led_work = 0;
	 led_xi = 0;
	 mo_r = 1;
	 mo_l = 0;
	 min = 0;
	 sec = 30;
	 quan = 0;
}
//*********************************//
//	   泡衣（漂洗）子程序
//*********************************//
void pao()
{	
    anther=1;
	other=1;
	P0 = 0xff;
	led_pao = 0;
	led_work = 0;
	flag3++;
	mo_r = 1;
	mo_l = 0;
	min = 0;
	sec = 20;
	quan = 0;
}
//*********************************//
//		脱水子程序
//*********************************//
void xx()
{	other=0;
	anther=0;
	P0 = 0xff;
	led_xx = 0;
	led_out=0;
	mo_r = 0;
	mo_l = 1;
	min = 0;
	sec = 30;
}
//*********************************//
//		 出水子程序
//*********************************//
void out()
{	anther=0;
	other=0;
	P0 = 0xff;
	led_out = 0;
	flag2++;
	mo_r = 0;
	mo_l = 0;
	min = 0;
	sec = 5;
}
//*********************************//
//	   显示菜单选择
//*********************************//
void se()
{
	P0 = 0xff;
	if(flag0 >= 5)
		flag0 = 0; 
	if(flag0 == 0)
	{
		led_in = 0;
	}
	if(flag0 == 1)
	{
		led_xi=0;
	}
	if(flag0 == 2)
	{
		led_pao=0;
	}
	if(flag0 == 3)
	{
		led_xx=0;
	}
	if(flag0 == 4)
	{
		led_out=0;
	}
}
//********************************//
//	   菜单处理子程序
//**********************************//
void menu()
{	
    min = 0;
	sec = 0;
	mo_r=0;
	mo_l=0;
	SEG_display();

   	while(1)
	{
	   if(key_on == 0)
		{
			delay(5);
			if(key_on == 0)
			{
				while(!key_on);
				on();
				break;
			}
		}
//**************************//
		if(key_off == 0)
		{
			delay(5);
			if(key_off == 0)
			{
				while(!key_off);
				over();
				break;
			}
		}
//****************************//
		if(key_se == 0)
		{
			delay(5);
			if(key_se == 0)
			{
				while(!key_se);
				flag0++;
				se();
			}
		}				
	}
}
//*********************************//
//		   按键扫描子程序
//*********************************//
void key_scan()
{ 
	if(key_menu == 0)
	{
		delay(5);
		if(key_menu == 0)
		{
			while(!key_menu);
			menu();
		}
	}
//********************************//
	if(key_on == 0)
	{
		delay(5);
		if(key_on == 0)
		{
			while(!key_on);
			on();
		}
	}
//*********************************//
	if(key_off == 0)
	{
		delay(5);
		if(key_off == 0)
		{
			while(!key_off);
			over();
		}
	}
}
//*******************************//
//	显示子程序
//*********************************//
void SEG_display()
{
	P1=0x01;
	P2 = num[min/10];
	delay(10);
	P1 = 0x02;
	P2 = num[min%10];
	delay(10);
	P1 = 0x04;
	P2 = num[sec/10];
	delay(10);
	P1 = 0x08;
	P2 = num[sec%10];
	delay(10);
}
//*********************************//
//       主函数
//*********************************//
void main()
{
	led_in=1;
	anther=0;
	other=0;
	while(1)
	{
		
		SEG_display();
		key_scan();
	}
}
//**********************************//
//	    定时器0中断处理程序
//**********************************//
void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	count++;
    if(count==20)
	{	
	 	count = 0;
		sec--;
		if((flag0==1)||(flag0==2))
		{
		    quan++;
			switch(quan)
			 {
			 	case 1:mo_r=1;mo_l=0;break;	  
				case 5:mo_r=0;mo_l=0;break;
				case 7:mo_r=0;mo_l=1;break;
				case 12:mo_r=0;mo_l=0;break;
			    case 14:mo_r=1;mo_l=0;break;
				case 19:mo_r=0;mo_l=0;break;
				case 21:mo_r=0;mo_l=1;break;
			    case 26:mo_r=0;mo_l=0;break;
				default:;
			 }

			if(quan==30)
			{
			   quan=0;
			}

		}
	 //**********************************//
		if((sec == 0)&&(min != 0))
		{
		  min--;
		  sec = 59;
		 }
	  //**********************************//
		if((sec<0)&&(min==0)&&(flag0==0))	 //进水结束
		{	 
		     switch(flag1)
			 {
			 	case 1:flag0=1;xi();break;
				case 2:flag0=2;pao();break;
				case 3:flag0=2;pao();break;
				case 4:flag0=2;pao();break;
				default: err=1;led_wring = 0;
			 }
		}
	 //**********************************//
		if((sec<0)&&(min==0)&&(flag0==1))   //洗衣结束
		{	 
		     flag0 = 4;
			 out();
		}
	 //**********************************//
		if((sec<0)&&(min==0)&&(flag0==2))   //泡衣结束
		{	 
		     switch(flag3)
			 {
			 	case 1:flag0=4;out();break;
				case 2:flag0=4;out();break;
				case 3:flag0=4;out();break;
				default: err=1;led_wring = 0;
			 }
		}
	 //**********************************//
		if((sec<0)&&(min==0)&&(flag0==4))	 //出水结束
		{	
			switch(flag2)
			 {
			 	case 1:flag0=0;in();break;
				case 2:flag0=0;in();break;
				case 3:flag0=0;in();break;
				case 4:flag0=3;xx();break;

				default: err= 1;led_wring = 0;
			 }
		}
	 //***********************************//
		if((sec<0)&&(min==0)&&(flag0==3))
		{	 sec = 0;
			 over();
		}

	}
}
