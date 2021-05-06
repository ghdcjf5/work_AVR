/*
 * 180305_fnd.c
 *
 * Created: 2018-03-05 오전 9:30:31
 * Author : KCCI
 */ 


#include <avr/io.h>  
#define F_CPU 16000000L
#include <util/delay.h>


#define FND_DATA_DDR DDRA  //data direct resistor  FND 데이터버스는 portA에 연결
#define FND_DATA_PORT PORTA  //#은 전처리 지시어 컴파일 하기전에 처리하는것, FND_DATA_PORT를 밑에 구절에서 찾아서 PORTA로 바꿔준다.
#define FND_CONTROL_DDR DDRG //FND 콘트롤버스는 portG에 연결
#define FND_CONTROL_PORT PORTG //다이나믹구동을 위해 control 을 만들어준다.(port 절약)

//


//#define FND_FONT_ZERO 0b11000000  //최적화를 위해 메모리 한개를 써준다.(변수 쓰는거보다 define시켜주는것)

char FND[4]; //가상 fnd를 만들어서 리얼 fnd도 바뀌도록

char FND_FONT[10] = {0b11000000, 0b11111001 , 0b10100100, 0b10110000, 
					0b10011001,0b10010010 ,0b10000010 ,0b11111000 ,0b10000000 ,0b10010000 };
unsigned int x=0;
 unsigned char sec=0, min=0, hour=0;
 unsigned char cnt=0;  //cnt는 8ms


char hour_inc_flag;

int main(void)
{
	/*
	char FND_FONT_0 =0b11000000;
	char FND_FONT_1 =0b11111001;
	char FND_FONT_2 =0b10100100;
	char FND_FONT_3 =0b10110000;
	char FND_FONT_4 =0b10011001;
	char FND_FONT_5 =0b10010010;
	char FND_FONT_6 =0b10000010;
	char FND_FONT_7 =0b11111000;
	char FND_FONT_8 =0b10000000;
	char FND_FONT_9 =0b10010000;
	*/
	
	
	
  FND_init();
  int i=0;
  
	
 
    while (1) 
    {
		/*
		for(char i=0;i<8;i++){
			PORTA= ~(0b00000001<<i);  //회로 테스트용
			_delay_ms(200);
		}
		*/
			
		
	/*
		FND_CONTROL_PORT=0b00000001;
		for( i=0;i<10;i++){
			FND_CONTROL_PORT= FND_FONT[i];  //회로 테스트용
			_delay_ms(200);
		}
	*/
	
	//x=min*100+sec;
	x=hour*100+min;
	
	FND_UPDATE(x);  //

	FND_OUTPUT();	
	cnt++;
	if(cnt==1){  //actually time's cnt=125
		cnt=0;
		
		sec++;
		if(sec==60){
			sec=0;
			min++;
		
		if(min==60){
			min=0;
			hour++;
		//	inc_hour_flag=1;
		
		if(hour==24){
			hour=0;
		}
		
		}
		
		
		}
		
		
	/*	
		if(inc_hour_flag==1){
			
			inc_hour_flag=0;
			sec--;  //시간 보정장치 375us/s가 생겨서 따라서 정확한 시간을 뽑아내기가 cpu로는 힘들다. time을 써야한다.
		}
	*/
	
		
	}
	
	
		
	/*
		FND_CONTROL_PORT = 0b00000001;
		FND_DATA_PORT = FND[0];
		_delay_ms(2);
		
		FND_CONTROL_PORT = 0b00000010;
		FND_DATA_PORT = FND[1];
		_delay_ms(2);
		
		FND_CONTROL_PORT = 0b00000100;
		FND_DATA_PORT = FND[2];
		_delay_ms(2);
		
		FND_CONTROL_PORT = 0b00001000;
		FND_DATA_PORT = FND[3];
		_delay_ms(2);
		*/
	
				
		/*
		FND_DATA_PORT = FND_FONT_ZERO; //0
		_delay_ms(5000);
		
		
		FND_DATA_PORT = FND_FONT_1; //1
		_delay_ms(5000);
		
		
		FND_DATA_PORT = FND_FONT_2; //2
		_delay_ms(5000);
		
		
		FND_DATA_PORT = FND_FONT_3; //3
		_delay_ms(5000);
		
		
		FND_DATA_PORT = FND_FONT_4; //4
		_delay_ms(5000);
		
		
		FND_DATA_PORT = FND_FONT_5;//5
		_delay_ms(5000);
		
		
		FND_DATA_PORT= FND_FONT_6;//6
		_delay_ms(5000);
		
		FND_DATA_PORT = FND_FONT_7;//7
		_delay_ms(5000);
		
		FND_DATA_PORT = FND_FONT_8;//8
		_delay_ms(5000);
		
		FND_DATA_PORT = FND_FONT_9;//9
		_delay_ms(5000);
		
		*/
		
		/*
		FND_CONTROL_PORT = 0b00000001;
		FND_DATA_PORT = FND_FONT[1];
		_delay_ms(2);
	
		FND_CONTROL_PORT = 0b00000010;
		FND_DATA_PORT = FND_FONT[2];
		_delay_ms(2);
		
		FND_CONTROL_PORT = 0b00000100;
		FND_DATA_PORT = FND_FONT[3];
		_delay_ms(2);
		
		FND_CONTROL_PORT = 0b00001000;
		FND_DATA_PORT = FND_FONT[4];
		_delay_ms(2);			
		//delay를 2ms이하로 주면 잔상효과로 인해 깜빡임이 보이지않고 계속 켜져있는것처럼 보인다. 다만 빛은 약간 약해질수도있다.
		
		
		*/
		
		
		
		}
		
		
		
		
		
    
}

void FND_UPDATE(int data){
FND[0] = FND_FONT[(x/1000)%10];
FND[1] = FND_FONT[(x/100)%10];
FND[2] = FND_FONT[(x/10)%10];
FND[3] = FND_FONT[x%10];
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