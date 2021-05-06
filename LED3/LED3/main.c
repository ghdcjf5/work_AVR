/*
 * led3.c
 *
 * Created: 2018-02-23 오전 9:32:50
 * Author : Ro Hong Cheol
 */ 

#include <avr/io.h>
#define F_CPU 16000000L  //16MHz의 크리스탈이 동작하는것을 동기화 시켜주는것? delay를 쓰려면 이 define을 해줘야한다. 16M는 8bit넘어가니까 long으로 변환(L)
#include <util/delay.h>   //delay를 쓸수있도록 include

int main(void)
{
    /* Replace with your application code */
	
	DDRC = 0b11111111;    //data direct resiter(데이터 방향을 지시하는것)  현재는 내부의 스위치를 선택하여 출력을 하니까 8핀 모두 1로 출력으로 쓰는것(입력으로쓸떄는 버튼스위치)
	PORTC = 0b00000000;  //초기화를 하기위해 8bit 0(0~7까지의 led꺼놓는것 즉, 두줄은 led를 쓰기위한 초기화 작업
	
	//여기까지 초기화시켜놓는것
	//int a,b,result;
		char i;
	//	char a;
	//	char b;
	//	char result;
		
		
	//		a=0b11110101;  //245  f5
			//b=0b00110011;
	//a=5;          //gcc(c언어)에서는 이진 bynery로 출력하는 것이 없다.(printf에서 0b같은게 없다) 하지만 atmel에서는 십진수로 집어넣어도 알아서 2진수로 번역해서 들어간다.
	
	//		b=0b11001100;     //204     cc

//a=0xc5;  //16진수로 입력해도 2진수로 번역해서 (led)보여준다

//c에서는 printf에서 출력을 보여주는것 지금 atmel에는 elvis를 이용하여 led로 보여주고있다.
    while (1) 
    {
		//result =a^b;
		
		/*
		PORTC |= 0b10000000; //계절이 어떤지 현재 들어있는 비트가 뭔지모르니까 초기의 상태를 유지하면서 원하는 bit만 제어하기 위해서 제어하고자하는부분만 1로(00100000)하고 or연산
		_delay_ms(10000);  //10초 delay
		PORTC &= 0b01111111;  //커튼이 올라가는데 끝에 다다르면 모터가 꺼져야하니까 and연산 (제어하고자 하는 부분만 0을하고)
		*/
		//프로젝트 스크린도 이와같은 원리
		
		
		
		for(i=0;i<9;i++)
		{
			
			PORTC = ( PORTC <<i);
			_delay_ms(200);
			
		}
		
		for(i=0;i<9;i++)
		{
			
			PORTC = (PORTC<<1) | 0b00000001;   //or로 +1해도된다. 
			_delay_ms(200);
		}
		  /*
		  for(i=0;i<9;i++)
		  {
			  
			  PORTC = 0b11111111<<(8-i);  //밀어도되고
			  _delay_ms(200);
		  
		  */
		
		for(i=0;i<9;i++)
		{
			
			PORTC = (0b11111111>>1) | 0b10000000;
			_delay_ms(200);
		}
		
			for(i=0;i<9;i++)
			{
				
				PORTC = (0b11111111>>1) ;
				_delay_ms(200);
			}
			
		
		
		
		
		//toggle : on/off를 바꿔주는것 (현재 0인것을 1로 1인것을 0으로 ) exclusive or사용(xor)하는데 토글하고싶은곳만 1을 주고 나머지는 0을준다.(나머지는 건드리지 않을것이기에)
    }
}


/* 모터에서 bit연산이 사용된다. 
히터, 계절, 도어, 커튼, 음악, 창문 등(8bit에 하나씩 제어가 들어있고) 이를 컨트롤 할때 초기상태를 유지한 상태에서 어떤 한개만 컨트롤 하려면 커트롤 하는 bit에 1을 넣고 나머지는 0넣고 or연산
10초뒤에 커튼이 열린다하면 delay 10초를 주고 port1에 값 넣어주면 커튼이 열리고 ...
*/

 