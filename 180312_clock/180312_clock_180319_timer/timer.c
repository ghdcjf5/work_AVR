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
#define CLOCK 0
#define CORRECT 1
#define STOPWATCH 2
#define COUNTDOWN 3
volatile char signed cd_hour, cd_min, cd_sec;
volatile char csec,sw_sec,sw_min,sw_hour;
			//centisec(1/100��)
volatile char sec, min,hour;  //volatile�� �����Ϸ��� �����ϴ°� , ������ ���������� �����ϸ�
//���μ����ȿ��ִ� working regist���� ó���ؼ� ����������. ���� working reg�� �ƴ� �޸𸮿� �Ҵ��϶�� �����ϴ°� 
volatile int dmsec;
extern char FND[4]; //�ٸ� ������Ʈ �������� .c������ FND�� ������ ��������� ���������
extern char LCD[6];
extern volatile char change_time_flag;
volatile char half_sec=0;  //������ �����.
volatile char half_sec2=0;
volatile int sw_dmsec;
extern	volatile char sw_change_time_flag;
extern volatile char lab_time_flag;
extern volatile char cd_change_time_flag;
extern volatile char start_flag, play_flag,cd_flag;

extern char mode;

/*SIGNAL(SIG_OUTPUT_COMPARE1A)   //winavr ���� ���ͷ�Ʈ ���� ��ƾ
{

}
*/

ISR(TIMER1_COMPA_vect){			//AtmelStudio ���� ���ͷ�Ʈ ���� ��ƾ //timer1_compa_vect�� �ּҰ��̴�.
	
	
		dmsec++;
		if(dmsec>=500){
			dmsec=0;
			if(cd_flag){
				cd_sec--;
				
				if(cd_sec<0){
					
					cd_sec = 59;
					cd_min--;
					if(cd_min<0){
						cd_min=59;
						cd_hour--;
						if(cd_hour<0){
								cd_sec = 0;
								cd_min = 0;
								cd_hour = 0;
							play_flag=1;
							cd_flag = 0;
						}
					
					}
				}
				
				
				
			}
			
			
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
					/*
					  LCD_command(0xC4);
					  LCD_data('0'+(hour/10));
					  LCD_data('0'+hour%10);
					*/
				
				}
			
			
			
				  //������ 2ms�� �ɸ���.
			  
				  /*
				  LCD_command(0xC7);
				  LCD_data('0'+(min/10));
				  LCD_data('0'+min%10);
				  */
			}
		
			FND_UPDATE(min*100+sec);
			//LCD_update(hour*10000+min*100+sec);
		
			

		
		}
		
		
		
		
	if(!(dmsec%250)){
		change_time_flag = 1; //ȭ�鰻��
		half_sec ^= 1;
		
		FND_CONTROL_PORT ^=0b00010000;
	} //dmsec%250 �������� 1�̵����� ���
	//250���� ���� �������� 0�̵Ǿ� if�� ����ȴ�. 0�϶��� 250�϶� ���� ,1�ʰ��Ǹ� clear�Ǵϱ� 0���� �ٲ�ϱ� 0�� 250�� �۵�	
	
	
	
	
	
	if(sw_change_time_flag){
		if(!(dmsec%5)){  //dmsec�� 2ms�� �ö󰡴ϱ� dmsec�� 5�� ���� �������� 0�� �϶�
			csec++;
			change_time_flag = 1; //ȭ�鰻��
			
			if(csec>=100){
				csec = 0;
				sw_sec++;
				if(sw_sec>=60){
					sw_sec=0;
					sw_min++;
					if(sw_min>=60){
						sw_min = 0;
						sw_hour++;
						if(sw_hour>=100){
							sw_hour =0;
						}
					}
					
				}
				
	
			}
		}
		
	}
	

	
	
	

	
	
	
	
}

//INTERRRUPT SERVICE ROUTINE�ȿ����� delay�� ���̵��� �ȵȴ�. �� ���� �ڵ��� ���� �����ʴ�.
//Ư�� 2ms�� delay�� �ְ��ִµ� �̸� �Ѿ������ �ð��� �����ʰԵȴ�.
ISR(TIMER0_COMP_vect){			
	static char i=0;
	
	FND_CONTROL_PORT &= 0b11110000; 
	FND_CONTROL_PORT |= 0b00000001 <<i;
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
void Timer3A_Init(int ms){	//Ÿ�̸� �ڵ�
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
	

