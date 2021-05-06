/*
 * 180305_fnd.c
 *
 * Created: 2018-03-05 ���� 9:30:31
 * Author : KCCI
 */ 


#include <avr/io.h>  
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>  //sei�� �������Ͽ�



//��ó�����þ�� ������ �س��� �ڵ��� �������� ������ ���ؼ�
#define FND_DATA_DDR DDRA  //data direct resistor  FND �����͹����� portA�� ����
#define FND_DATA_PORT PORTA  //#�� ��ó�� ���þ� ������ �ϱ����� ó���ϴ°�, FND_DATA_PORT�� �ؿ� �������� ã�Ƽ� PORTA�� �ٲ��ش�.
#define FND_CONTROL_DDR DDRG //FND ��Ʈ�ѹ����� portG�� ����
#define FND_CONTROL_PORT PORTG //���̳��ͱ����� ���� control �� ������ش�.(port ����)
#define LED_PORT PORTC
//


//#define FND_FONT_ZERO 0b11000000  //����ȭ�� ���� �޸� �Ѱ��� ���ش�.(���� ���°ź��� define�����ִ°�)

char FND[4]; //���� fnd(�ڵ�)�� ���� ���������Ʈ fnd(Ĩ)�� �ٲ��, �迭�� �Ἥ �޸�����(�ּҰ��� �ϳ��� �ޱ⶧����)

char FND_FONT[10] = {0b11000000, 0b11111001 , 0b10100100, 0b10110000, 
					0b10011001,0b10010010 ,0b10000010 ,0b11111000 ,0b10000000 ,0b10010000 };

 /*
 unsigned char sec=0, min=0, hour=0;
 unsigned char cnt=0;  //cnt�� 8ms
*/

char hour_inc_flag;

int main(void){
	
 led_init();	
	
  FND_init();
  Timer0_Init();
  Timer1A_Init(2);
  
  FND_UPDATE(0);  //�ʱ�ȭ�� ���൵ while���� �Լ��� ���� �ʾƵ� ISR�� �����ָ� �ϵ��� ȣ���ϱ⶧���� �����ȴ�.
  sei();  //set enable interrupt(�۷ι� ���ͷ�Ʈ) �̰Ծ����� FND���� ���Ѵ�. ���ͷ�Ʈ�� �ȵ���
 
 
  
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
 FND_CONTROL_DDR = 0x0f;  //led 4��
 FND_CONTROL_PORT=0; //led �ʱ�ȭ

}