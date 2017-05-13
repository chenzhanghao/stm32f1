#define ST7564R_HERE
#include "st7565r.h"

static void gpio_init(void)
{
	OUT(CS);
	//OUT(RST);
	OUT(RS);
	OUT(SCK);
	OUT(SDA);
}
/*дָ�LCDģ��*/
void st7565r_write_cmd(u8 data)   
{
	char i;
	RESET(RS);
	for(i=0;i<8;i++)
   {
		RESET(SCK);
		SCK_DELAY;
		if(data&0x80) SET(SDA);
		else RESET(SDA);
		SCK_DELAY;
		SET(SCK);
	 	data=data<<=1;
   }
}

/*д���ݵ�LCDģ��*/
void st7565r_write_data(u8 data)
{
	char i;
	SET(RS);
	for(i=0;i<8;i++)
   {
		RESET(SCK);
	   SCK_DELAY
		if(data&0x80) SET(SDA);
		else RESET(SDA);
	   SCK_DELAY
		SET(SCK);
	 	data=data<<=1;
    }
}

void st7565r_write_ndata(u8 *data,int n)
{
	for(int i=0; i<n; i++,data++)
		st7565r_write_data(*data);
}


/*LCDģ���ʼ��*/
void st7565r_init()
{
	gpio_init();
	RESET(CS);
	//RESET(RST);        /*�͵�ƽ��λ*/
	POWERUP_DELAY;
	//SET(RST);		    /*��λ���*/
	POWERUP_DELAY;        
	st7565r_write_cmd(0xe2);	 /*����λ*/
	POWERUP_DELAY;
	st7565r_write_cmd(0x2f);  /*��ѹ����3*/
	POWERUP_DELAY;
	st7565r_write_cmd(0x23);  /*�ֵ��Աȶȣ������÷�Χ0x20��0x27*/
	st7565r_write_cmd(0x81);  /*΢���Աȶ�*/
	st7565r_write_cmd(0x2a);  /*0x28,΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f*/
	st7565r_write_cmd(0xa2);  /*1/9ƫѹ�ȣ�bias��*/
	st7565r_write_cmd(0xc8);  /*��ɨ��˳�򣺴��ϵ���*/
	st7565r_write_cmd(0xa0);  /*��ɨ��˳�򣺴�����*/  
	st7565r_write_cmd(0xa6);
	st7565r_write_cmd(0xa4);
	st7565r_write_cmd(0x40);  /*��ʼ�У���һ�п�ʼ*/
	st7565r_write_cmd(0xaf);  /*����ʾ*/
	SET(CS);
}

void st7565r_set_addr(u8 page,u8 column)
{
	column=column-0x01;
	st7565r_write_cmd(0xb0+page-1);   /*����ҳ��ַ*/
	st7565r_write_cmd(0x10+(column>>4&0x0f));	/*�����е�ַ�ĸ�4λ*/
	st7565r_write_cmd(column&0x0f);	/*�����е�ַ�ĵ�4λ*/	
}
void st7565r_clear_area(u8 page_start, u8 page_end, u8 column_start, u8 column_end )
{
	int i;
	RESET(CS);
	u8 page_len = page_end - page_start + 1;
	u8 column_len = column_end - column_start +1;
	for(i = 0; i < page_len; i++)
	{
		st7565r_set_addr(page_start + i,column_start);
		for(int i=0; i < column_len; i++)
			st7565r_write_data(0);
	}
	SET(CS);
}
void st7565r_clear_screen()
{
	unsigned char i,j;
	RESET(CS);
	for(i=1;i<9;i++)
	{
		st7565r_set_addr(i,1);
		for(j=0;j<128;j++)
		{
		  	st7565r_write_data(0);
		}
	}
 	SET(CS);
}

void st7565r_show_5x7(uchar page,uchar column,uchar *dp)
{
	uint col_cnt;	
	RESET(CS);	
	st7565r_set_addr(page, column);
	for (col_cnt=0;col_cnt<6;col_cnt++)
	{	
		st7565r_write_data(*dp);
		dp++;
	}
	SET(CS);
}
void st7565r_show_8x16(uchar page,uchar column,uchar *dp)
{
	uint i,j;
	RESET(CS);	
	for(j=0;j<2;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<8;i++)
		{	
			st7565r_write_data(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_12x24(uchar page,uchar column,uchar *dp)
{
	uint i,j;
	RESET(CS);	
	for(j=0;j<3;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<12;i++)
		{	
			st7565r_write_data(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_16x16(uchar page,uchar column,uchar *dp)
{
	uint i,j;
 	RESET(CS);	
	for(j=0;j<2;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<16;i++)
		{	
			st7565r_write_data(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_16x32(uchar page,uchar column,uchar *dp)
{
	uint i,j;
 	RESET(CS);	
	for(j=0;j<4;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<16;i++)
		{	
			st7565r_write_data(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_20x40(uchar page,uchar column,uchar *dp)
{
	uint i,j;
 	RESET(CS);	
	for(j=0;j<5;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<20;i++)
		{	
			st7565r_write_data(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_24x24(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+3;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 24);
		dp += 24;
	}
	SET(CS);
}
void st7565r_show_24x48(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+6;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 24);
		dp += 24;
	}
	SET(CS);
}
void st7565r_show_32x12(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+2;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 32);
		dp += 32;
	}
	SET(CS);
}
void st7565r_show_32x32(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+4;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 32);
		dp += 32;
	}
	SET(CS);
}

/*��ʾ128x64����ͼ��*/
void st7565r_flush(uchar *data)
{
	uint i,j;
	RESET(CS);
	for(j=0;j<8;j++)
	{
		st7565r_set_addr(j+1,1);
		for (i=0;i<128;i++)
		{	
			st7565r_write_data(*data);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			data++;
		}
	}
	SET(CS);
}



void st7565r_show_page()
{
	RESET(CS);
	for(int n = 1; n <= 8; n++)
	{
		st7565r_set_addr(n, 1);
		for(int i = 1;i <= 128; i++)
		{
			st7565r_write_data(0x80);
		}
	}
	SET(CS);
}