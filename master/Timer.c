/*! Timer.c  
*****************************************************************************
//
// File Name	: 'Timer.c'
// Title		: Timer driver
// Author		: KYOUNG YOON JEONG
// Created		: 7/25/2017
// Version		: 1.1
// Target MCU	: Atmel AVR ATMega128
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
// *************************************************/ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

#define FND_DATA_PORT PORTA		//FND �����͹����� PORTA�� ����
#define FND_DATA_DDR DDRA
#define FND_CONTROL_PORT PORTG	//FND ��Ʈ�ѹ����� PORTG�� ����
#define FND_CONTROL_DDR DDRG

volatile char sec, min, hour;
volatile char csec, sw_sec, sw_min, sw_hour;
volatile int dmsec;
volatile char harf_sec;
extern char FND[4];
extern char change_time_flag=0;
extern volatile char start_flag;
/*SIGNAL(SIG_OUTPUT_COMPARE1A)   //winavr ���� ���ͷ�Ʈ ���� ��ƾ
{

}
*/
ISR(TIMER0_COMP_vect){			//AtmelStudio ���� ���ͷ�Ʈ ���� ��ƾ
	static char i=0;
	FND_CONTROL_PORT &= 0b11110000;			
	FND_CONTROL_PORT |= 0b00000001<<i;
	FND_DATA_PORT = FND[i];
	i++;
	if(i==4)i=0;
}
ISR(TIMER1_COMPA_vect){			//AtmelStudio ���� ���ͷ�Ʈ ���� ��ƾ
	dmsec++;
	if(dmsec>=500){
		dmsec=0;
		sec++;
		if(sec>=60){
			sec=0;
			min++;
			if(min>=60){
				min=0;
				hour++;
				if(hour>=24){
					hour=0;
				}
			}
		}
		FND_update(min*100+sec);
	}
	if(!(dmsec%250)){
		change_time_flag = 1;
		harf_sec ^= 1;
		FND_CONTROL_PORT ^= 0b00010000;
	}
	if(start_flag){
		if(!(dmsec%5)){
			csec++;
			change_time_flag = 1;
			if(csec>=100){
				csec=0;
				sw_sec++;
				if(sw_sec>=60){
					sw_sec=0;
					sw_min++;
					if(sw_min>=60){
						sw_min=0;
						sw_hour++;
						if(sw_hour>=100)sw_hour=0;
					}
				}
			}
		}
	}
}
void Timer0_Init(void){
	TCCR0 = 0b00001110;
	OCR0 =124;
	TIMSK |= 1<<OCIE0;
	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}
void Timer1A_Init(int ms){	//�Ű������� �Ѱܹ޴� �ð� �ֱ�� ���ͷ�Ʈ �߻�(���� ms)
	int divider;

	TCCR1A |= 0;              //don't output OC1A
	TCCR1B |= (1<<WGM12) | (1<<CS12);  //Timer1A CTC_mod
	TCCR1C |= 0;       

	switch(TCCR1B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default : break;
	}

 	OCR1AH = (((F_CPU/divider)*ms/1000-1) >> 8);  
	OCR1AL = (((F_CPU/divider)*ms/1000-1) & 0XFF); 
	
	TCNT1H = 0x00;                               // clear Timer/Counter1
	TCNT1L = 0x00;

	TIMSK |= 1<<OCIE1A;                          // enable OC1A interrupt

	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}	
void Timer3A_Init(int ms){	
	int divider;
	
	
	TCCR3A |= (1<<COM3A0) ;
	TCCR3B |= (1<<WGM32) | (1<<CS31);
 	      
	switch(TCCR3B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default : break;
	}

 	OCR3AH = ((F_CPU/divider/1000*ms-1) >> 8);  
	OCR3AL = (F_CPU/divider/1000*ms-1) & 0XFF; 
	
	TCNT3H = 0X00;
 	TCNT3L = 0X00;
  
	ETIMSK |= 1<<OCIE3A;
	TIFR = 0X00;
	ETIFR = 0X00;
}
void PWM_OC3A_init(void){
	TCCR3A |= (1<<COM3A1) | (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33) | (1<<CS31);
	
	OCR3A = 0;
	ICR3 = 19999;
}
	

