/*
 * DS1302.c
 *
 * Created: 2018-04-03 오후 4:25:25
 * Author : user
 */ 
extern volatile char sec, min, hour, date, day, month, year, time_flag;
#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>
//void ds1302_init_settime();
int main(void)
{
    Timer0_Init();
	Timer1A_Init(2);
	LCD_init();
	LCD_string(0x80," 2018-04-03 Thu ");
	LCD_string(0xC0,"    00:00:00    ");
	FND_init();
	ds1302_init();
	//ds1302_init_settime();
	ds1302_init_gettime();
	sei();
    while (1) 
    {
		if(time_flag){
			time_flag = 0;
			LCD_Time_print();
		}
    }
}
void ds1302_init_settime()
{
	ds1302_setsec(10);
	ds1302_setmin(45);
	ds1302_sethour(11);
	ds1302_setdate(9);
	ds1302_setday(1);
	ds1302_setmonth(4);
	ds1302_setyear(18);
}

void ds1302_init_gettime()
{
	
	sec = ds1302_getsec();
	min = ds1302_getmin();
	hour = ds1302_gethour();
	date = ds1302_getdate();
	day = ds1302_getday();
	month = ds1302_getmonth();
	year = ds1302_getyear();
}
void LCD_Time_print(void){
	LCD_command(0x83);
	LCD_data('0'+year/10);
	LCD_data('0'+year%10);
	LCD_command(0x86);
	LCD_data('0'+month/10);
	LCD_data('0'+month%10);
	LCD_command(0x89);
	LCD_data('0'+date/10);
	LCD_data('0'+date%10);
	switch(day){
		case 0:LCD_string(0x8C,"Sun"); break;
		case 1:LCD_string(0x8C,"Mon"); break;
		case 2:LCD_string(0x8C,"Thu"); break;
		case 3:LCD_string(0x8C,"Wen"); break;
		case 4:LCD_string(0x8C,"Thi"); break;
		case 5:LCD_string(0x8C,"Fri"); break;
		case 6:LCD_string(0x8C,"Sat"); break;
	}
	LCD_command(0xC4);
	LCD_data('0'+hour/10);
	LCD_data('0'+hour%10);
	LCD_command(0xC7);
	LCD_data('0'+min/10);
	LCD_data('0'+min%10);
	LCD_command(0xCA);
	LCD_data('0'+sec/10);
	LCD_data('0'+sec%10);
}
