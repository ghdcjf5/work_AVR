/*
 * PWM.c
 *
 * Created: 2018-03-20 오전 9:23:19
 * Author : user
 */ 
#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>

extern char sec, min, hour;
extern volatile char harf_sec;
extern volatile char csec, sw_sec, sw_min, sw_hour;
extern volatile signed char timer_sec, timer_min, timer_hour;
extern int He_Pirate[];
extern int He_Pirate1_Beat[];
extern int School_Bell_Tune[];
extern const int School_Bell_Beats[];
extern int Elise_Tune[];
extern const int Elise_Beats[];
extern int GrandFa_Tune[];
extern const int GrandFa_Beats[];

int main(void)
{
	int lumi = 300;
	FND_init();
	led_init();
	Timer1A_Init(2);
	Timer0_Init();
	//Timer3A_Init(2);
	//Speaker_Init();
	ADC_Init();
	PWM_OC3A_init();
	FND_update(0);
	LCD_init();
	LCD_string(0x80," ADC & PWM Exam ");
	LCD_string(0xC0,"                ");
	DDRD &= 0b00001111;
	sei();
    while (1) 
    {
		/*lumi = ADC_Value(lumi);
		LCD_command(0xC4);
		LCD_data('0'+lumi/1000%10);
		LCD_data('0'+lumi/100%10);
		LCD_data('0'+lumi/10%10);
		LCD_data('0'+lumi%10);
		_delay_ms(200);
		OCR3A = lumi*10;*/
		/*for(int i=1999;i<19999;i++){
			OCR3A = i;
			OCR_LCD_Out(i);
		}
		for(int i=19999;i>=1999;i--){
			OCR3A = i;
			OCR_LCD_Out(i);
		}*/
		for(int i=4999;i<19999;i++){
			OCR3A = i;
			OCR_LCD_Out(i);
		}
		for(int i=19999;i>=4999;i--){
			OCR3A = i;
			OCR_LCD_Out(i);
		}
    }
}
void OCR_LCD_Out(int value){
	LCD_command(0xC4);
	LCD_data('0'+value/10000);
	LCD_data('0'+value/1000%10);
	LCD_data('0'+value/100%10);
	LCD_data('0'+value/10%10);
	LCD_data('0'+value%10);
}
