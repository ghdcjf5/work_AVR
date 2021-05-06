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




#define FND_DATA_DDR DDRA  //data direct resistor  FND 데이터버스는 portA에 연결
#define FND_DATA_PORT PORTA  //#은 전처리 지시어 컴파일 하기전에 처리하는것, FND_DATA_PORT를 밑에 구절에서 찾아서 PORTA로 바꿔준다.
#define FND_CONTROL_DDR DDRG //FND 콘트롤버스는 portG에 연결
#define FND_CONTROL_PORT PORTG //다이나믹구동을 위해 control 을 만들어준다.(port 절약)
#define LED_PORT PORTC

volatile char sec, min;
volatile int dmsec;
extern char FND[4]; //다른 프로젝트 폴더안의 .c파일의 FND를 가져다 쓰고싶을때 적어줘야함
/*SIGNAL(SIG_OUTPUT_COMPARE1A)   //winavr 사용시 인터럽트 서비스 루틴
{

}
*/

ISR(TIMER1_COMPA_vect){			//AtmelStudio 사용시 인터럽트 서비스 루틴 //timer1_compa_vect가 주소값이다.
	dmsec++;
	if(dmsec>=500){
		dmsec=0;
		sec++;
		if(sec>=60){
			sec=0;
			min++;
			if(min>=60){
				min=0;
			}  //무조건 2ms가 걸린다.
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
	OCR0 =124;  //2ms마다ㅏ 돌아가게 되있는것
	TIMSK |= 1<<OCIE0;
	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}
void Timer1A_Init(int ms){	//매개변수로 넘겨받는 시간 주기로 인터럽트 발생(단위 ms)
	int divider;

	TCCR1A |= 0;     
	         //don't output OC1A  //timercounter control register
	TCCR1B |= (1<<WGM12) | (1<<CS12);  //Timer1A CTC_mod 분주는 256분주
	//WGM12의 비트수(3)을 쉬프트하니까 1000이 되고 이를 TCCR1B와 or연산
	//CS12는 분주계산을 위해서
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
  
	ETIMSK |= 1<<OCIE3A;  //인터럽트 쓰는것
	TIFR = 0X00;
	ETIFR = 0X00;
}
void PWM_OC3A_init(void){
	TCCR3A |= (1<<COM3A1) | (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33) | (1<<CS31);
	
	OCR3A = 0;
	ICR3 = 19999;
}
	

