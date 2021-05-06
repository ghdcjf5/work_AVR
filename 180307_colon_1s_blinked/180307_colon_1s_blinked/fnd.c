/*
 * 180305_fnd.c
 *
 * Created: 2018-03-05 오전 9:30:31
 * Author : KCCI
 */ 


#include <avr/io.h>  
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>  //sei를 쓰기위하여



//전처리지시어로 재정의 해놓음 코드의 가독성을 높히기 위해서
#define FND_DATA_DDR DDRA  //data direct resistor  FND 데이터버스는 portA에 연결
#define FND_DATA_PORT PORTA  //#은 전처리 지시어 컴파일 하기전에 처리하는것, FND_DATA_PORT를 밑에 구절에서 찾아서 PORTA로 바꿔준다.
#define FND_CONTROL_DDR DDRG //FND 콘트롤버스는 portG에 연결
#define FND_CONTROL_PORT PORTG //다이나믹구동을 위해 control 을 만들어준다.(port 절약)
#define LED_PORT PORTC
//


//#define FND_FONT_ZERO 0b11000000  //최적화를 위해 메모리 한개를 써준다.(변수 쓰는거보다 define시켜주는것)

char FND[4]; //가상 fnd(코드)를 만들어서 리얼오브젝트 fnd(칩)도 바뀌도록, 배열을 써서 메모리절약(주소값을 하나만 받기때문에)

char FND_FONT[10] = {0b11000000, 0b11111001 , 0b10100100, 0b10110000, 
					0b10011001,0b10010010 ,0b10000010 ,0b11111000 ,0b10000000 ,0b10010000 };

 /*
 unsigned char sec=0, min=0, hour=0;
 unsigned char cnt=0;  //cnt는 8ms
*/

char hour_inc_flag;

int main(void){
	
 led_init();	
	
  FND_init();
  Timer0_Init();
  Timer1A_Init(2);
  
  FND_UPDATE(0);  //초기화만 해줘도 while문에 함수가 들어가지 않아도 ISR로 정해주면 하드웨어가 호출하기때문에 구동된다.
  sei();  //set enable interrupt(글로벌 인터럽트) 이게없으면 FND동작 안한다. 인터럽트가 안들어가서
 
 
  
}

void FND_UPDATE(unsigned int data){
	FND[0] = FND_FONT[(data/1000)%10];
	FND[1] = FND_FONT[(data/100)%10];
	FND[2] = FND_FONT[(data/10)%10];
	FND[3] = FND_FONT[data%10];
}

void FND_OUTPUT(void){
	int i=0;
	for (i=0;i<4;i++){
		FND_CONTROL_PORT = 0b00000001<<i;
		FND_DATA_PORT = FND[i];
		_delay_ms(2);
		}
}

void FND_init(void){
 FND_DATA_DDR = 0xff;
 FND_DATA_PORT =0;
 FND_CONTROL_DDR = 0x0f;  //led 4개
 FND_CONTROL_PORT=0; //led 초기화

}