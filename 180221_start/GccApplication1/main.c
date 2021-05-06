/*
 * GccApplication1.c
 *
 * Created: 2018-02-21 오후 2:36:29
 * Author : KCCI
 */ 

#include <avr/io.h>  //io.h에 DDRC,PORTC가 들어어있다.
#define F_CPU 16000000L  //16MHz의 크리스탈이 동작하는것을 동기화 시켜주는것? delay를 쓰려면 이 define을 해줘야한다.
#include <util/delay.h>   //delay를 쓸수있도록 include
int main(void)
{
    unsigned int i;
	DDRC = 0xFF;
	PORTC = 0xFF;
	//DDRC PORTC를 레지스터라한다.
	
	
	
	/* Replace with your application code */
    while (1)   //while로 무한반복을 시켜줌으로써 다른 메모리용량을 사용하지 못하게 해줘야한다. 임베디드에서는 특히 더 중시해야한다.
    {
		for(i=0;i<10;i++)
		{
			PORTC = 0b111111111 >> i;
			_delay_ms(500);
		}
		
		
		
		
		
		
		
		
		
		
	/*	PORTC = 0b11111111;
		_delay_ms(500);  //뒤에 괄호가있으니까 함수겠네  //0.5초 켜지고
		PORTC = 0b00000000;
		_delay_ms(500);     //0.5초 꺼진다
	*/	
		
		
	/*	i = 65000;
		while(i)	i-=0.1;
			
        
	     PORTC = 0b11111111;  //0b는 2진수
	    i=65000;
	    while(i)	i-=0.1;
	 */  
    }
	
	
}


/* atmega에서 a~f까지는 8개의 port로 구성 8bit, g는 0~4까지 구성
	gpio(general perpose input output) 
	프로그램메모리 128kB있다.
	16MHZ의 속도 atmega의 타원형에 16.000이 이것 (크리스탈이라고한다.) 전압을 걸어주면 진동을 하는데 일정하게 진동한다.
	
	
	
	
	
	  */