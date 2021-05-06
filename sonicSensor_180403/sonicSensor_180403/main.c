/*
 * sonicSensor_180403.c
 *
 * Created: 2018-04-03 오전 10:41:04
 * Author : KCCI
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile char distance_time; //거리 8bit니까 char로
volatile char distance_m;

ISR(INT4_vect){ //rising
	//PORTC = 0xff;//다켜기
	TCNT2 = 0;
}

ISR(INT5_vect){//faling
	//PORTC = 0x00; //다끄기
	distance_time = TCNT2;
	//distance_m = (distance_time+1)/15625*1000000;
	distance_m = (distance_time+1)* 64/58;
	
	
	
}




int main(void)
{
   EICRB = 0b00001011; //4번이 rising(하위비트) 5번 상위비트가 falling edge 를 쓸것이다.
   //에코는 4,5번핀을 병렬로(묶어서) 연결
   EIMSK = 0b00110000; //external interrupt mask
   DDRE = 0b01000000; //pe6번핀 사용
   Timer0_Init(); //타이머0가 있어야 동작한다. output이 연관되어 있는듯
   Timer2_Init();  //타이머가 초기화하고 tcnt0값이 올라간다.
   FND_init();
   
   FND_UPDATE(0); //fnd에 0000출력
 
   led_init();
   
  
   sei();
    while (1) 
    {
		
		
		PORTE |=0b01000000;
		_delay_us(10);
		PORTE&=0b10111111;  //triger 들어가는 코드  pe6번핀
		_delay_ms(1000);
		
		
		
	//	FND_UPDATE(distance_m); //타이머의 값을 출력해준다.
		//	FND_UPDATE(trans_m_s());
		FND_UPDATE(distance_m);
		
		
    }
}
// 
//void trans_m_s(){
// 	
// 	distance_m =(distance_time+1);
// }
