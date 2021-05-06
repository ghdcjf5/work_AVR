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




#define FND_DATA_DDR DDRA  //data direct resistor  FND �����͹����� portA�� ����
#define FND_DATA_PORT PORTA  //#�� ��ó�� ���þ� ������ �ϱ����� ó���ϴ°�, FND_DATA_PORT�� �ؿ� �������� ã�Ƽ� PORTA�� �ٲ��ش�.
#define FND_CONTROL_DDR DDRG //FND ��Ʈ�ѹ����� portG�� ����
#define FND_CONTROL_PORT PORTG //���̳��ͱ����� ���� control �� ������ش�.(port ����)
#define LED_PORT PORTC

volatile char sec, min;
volatile int dmsec;
extern char FND[4]; //�ٸ� ������Ʈ �������� .c������ FND�� ������ ��������� ���������
/*SIGNAL(SIG_OUTPUT_COMPARE1A)   //winavr ���� ���ͷ�Ʈ ���� ��ƾ
{

}
*/

ISR(TIMER1_COMPA_vect){			//AtmelStudio ���� ���ͷ�Ʈ ���� ��ƾ //timer1_compa_vect�� �ּҰ��̴�.
	dmsec++;
	if(dmsec>=500){
		dmsec=0;
		sec++;
		if(sec>=60){
			sec=0;
			min++;
			if(min>=60){
				min=0;
			}  //������ 2ms�� �ɸ���.
		}
		FND_UPDATE(min*100+sec);
	}
}
ISR(TIMER0_COMP_vect){			
	static char i=0;
	
	FND_CONTROL_PORT = 0b00000001 <<i;
	FND_DATA_PORT = FND[i];
	i++;
	if(i==4){i=0;}
		
}

void Timer0_Init(void){
	TCCR0 = 0b00001110;
	OCR0 =124;  //2ms���٤� ���ư��� ���ִ°�
	TIMSK |= 1<<OCIE0;
	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}
void Timer1A_Init(int ms){	//�Ű������� �Ѱܹ޴� �ð� �ֱ�� ���ͷ�Ʈ �߻�(���� ms)
	int divider;

	TCCR1A |= 0;     
	         //don't output OC1A  //timercounter control register
	TCCR1B |= (1<<WGM12) | (1<<CS12);  //Timer1A CTC_mod ���ִ� 256����
	//WGM12�� ��Ʈ��(3)�� ����Ʈ�ϴϱ� 1000�� �ǰ� �̸� TCCR1B�� or����
	//CS12�� ���ְ���� ���ؼ�
	TCCR1C |= 0;       

	switch(TCCR1B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default : break;
	}

 	OCR1AH = (((F_CPU/divider)*ms/1000-1) >> 8);  //2ms
	OCR1AL = (F_CPU/divider/1000*ms-1) & 0XFF; 
	
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
  
	ETIMSK |= 1<<OCIE3A;  //���ͷ�Ʈ ���°�
	TIFR = 0X00;
	ETIFR = 0X00;
}
void PWM_OC3A_init(void){
	TCCR3A |= (1<<COM3A1) | (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33) | (1<<CS31);
	
	OCR3A = 0;
	ICR3 = 19999;
}
	

