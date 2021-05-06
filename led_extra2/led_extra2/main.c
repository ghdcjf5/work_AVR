/*
 * led3.c
 *
 * Created: 2018-02-23 오전 9:32:50
 * Author : Ro Hong Cheol
 */ 

#include <avr/io.h>
#define F_CPU 16000000L  //16MHz의 크리스탈이 동작하는것을 동기화 시켜주는것? delay를 쓰려면 이 define을 해줘야한다. 16M는 8bit넘어가니까 long으로 변환(L)
#include <util/delay.h>   //delay를 쓸수있도록 include




void led_move_upto()
	{
		char i;
		
		_delay_ms(200);
		
		for(i=0;i<9;i++)
		{
		
		PORTC = 0b00000001<<i;
		_delay_ms(200);
		}
	}
void led_on_upto()
	{
		char i;
		for(i=0;i<9;i++)
		{
			
			
			PORTC = (0b11111111>>(8-i));
			_delay_ms(200);
		}
	}
		void led_off_downto()
		{
			char i;
			for(i=0;i<9;i++)
			{
				
				PORTC = 0b11111111>>i;
				_delay_ms(200);
			}
		}
			void led_move_downto()
			{
				char i;
				for(i=0;i<9;i++)
				{
					
					PORTC = 0b10000000>>i;
					_delay_ms(200);
				}
			}


int main(void)
{
    /* Replace with your application code */
	
	DDRC = 0b11111111;    //data direct resiter(데이터 방향을 지시하는것)  현재는 내부의 스위치를 선택하여 출력을 하니까 8핀 모두 1로 출력으로 쓰는것(입력으로쓸떄는 버튼스위치)
	PORTC = 0b00000000;  //초기화를 하기위해 8bit 0(0~7까지의 led꺼놓는것 즉, 두줄은 led를 쓰기위한 초기화 작업

	
    while (1) 
    {
		led_on_upto();
		led_on_downto();
		led_off_upto();
		led_move_upto();
		
		led_off_downto();
		led_move_downto();
		
	}

}
