/*
 * 180326_RC_exam.c
 *속도나 방향을 제어하기 위해서는 드라이버가 필요하다.
 밑의 코딩은 한쪽 모터를 끔으로써 방향제어를 하였기에 드라이버는 사용하지 않았다.
 * Created: 2018-03-26 오전 9:57:34
 * Author : KCCI
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>


volatile char UART_RX_data;

#define F 'F'
#define B 'B'
#define L 'L'
#define R 'R'
#define S 'S'


int main(void)
{
  UART1_Init(9600);  //송신속도 9600
   led_init();
 //  LCD_init();
  // LCD_command(0xc0);
  
   
   
   
   
   
   
   
   /*
   setName("rouetooth");
   setPin("1234");
   _delay_ms(1000);
   LCD_command(0x80);
   */
   sei();
   
    while (1) {	
	
	if(UART_RX_data){
		switch(UART_RX_data){
			case  F :  Foward(); break;
			case B : Backward(); break;
			case L :  TurnLeft() ; break;
			case R : TurnRight() ; break;
			case S : Stop(); break;
			
			
		}
		
	}

	
		/*
		connectTest();
		_delay_ms(2000);
		*/
	}
	
}
	//왼쪽11이 후진
	//왼쪽01이 전진
//오른쪽이 11일때 전진
//오른쪽이 01일떄 후진
void Foward(void){  //leftmotor 시계, rightmotor 시계
	/*
	left_forward();
	right_forward();
	*/
	PORTC |= 0b00001101;
	//오른쪽이 11일때 전진
	//오른쪽이 01일떄 후진
}

void Backward(void){
	/*
	left_backward();
	right_backward();
	*/
	PORTC |= 0b00000111;
	//왼쪽11이 후진
	//왼쪽01이 전진 
}


void TurnLeft(void){
	right_forward();
	left_stop();
	
	
}


void TurnRight(void){
	left_forward();
	right_stop();

	
}


void Stop(void){
	left_stop();
	right_stop();
	
	
}


void left_forward(void){
	PORTC |= 0b00000001;
	//오른쪽이 11일때 전진
	//오른쪽이 01일떄 후진
}

void left_backward(void){
	PORTC |= 0b00000010;
	//오른쪽이 11일때 전진
	//오른쪽이 01일떄 후진
}

void right_forward(void){
	PORTC |= 0b00001100;
	//오른쪽이 11일때 전진
	//오른쪽이 01일떄 후진
}


void right_backward(void){
	PORTC |= 0b00000100;
	//오른쪽이 11일때 전진
	//오른쪽이 01일떄 후진
}

void left_stop(void){
PORTC &= 0b11111100;
//오른쪽이 11일때 전진
//오른쪽이 01일떄 후진
}

void right_stop(void){
PORTC &= 0b11110011;

}