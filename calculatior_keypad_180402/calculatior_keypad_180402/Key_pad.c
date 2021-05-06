#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define Key_DDR DDRD
#define Key_PORT PORTD  //�����Ʈ
#define Key_PIN  PIND  //�Է���Ʈ

#define ADD 11
#define SUB 12
#define MUL 13
#define DIV 14
#define EQUAL 15
#define CLEAR 16
#define ZERO 10


void Key_Init(void){
	Key_DDR = 0b11110000;  //�Է��� ���� 4��Ʈ ����� ���� 4��Ʈ �����̴�. �ʱ�ȭ
	Key_PORT = 0x00;  //������ �ȵ����ִ°�
}

unsigned char Key_scan_sub(void){
	char i;
	i = Key_PIN & 0x0F;  //���� 4��Ʈ�� ����.
	if(i == 0x01)return 1;
	else if(i == 0x02)return 2;
	else if(i == 0x04)return 3;
	else if(i == 0x08)return 4;
	else return 0;  //�� �ش��ɿ��� 1,2,3,4�� return
}
unsigned char Key_scan(void){  //0~16���� 17�� ����
	char i;
	for(i=0;i<4;i++){
		Key_PORT = 0x10 << i;
		_delay_ms(10);  //chattering����
		if(Key_scan_sub()){
			_delay_ms(2);  //chattering ����
			return i*4+Key_scan_sub();
		}
	}
	return 0;
}

unsigned char Key_trans(unsigned char key_value){
	switch(key_value){
		case 1 : return ZERO; break;//return�ϸ� �Լ� �����ϱ� break�ʿ����.
		case 2 : return CLEAR; break;
		case 3 : return EQUAL; break;
		case 4 : return DIV; break;
		case 5 : return 1; break;
		case 6 : return 2; break;
		case 7 : return 3; break;
		case 8 : return MUL; break;
		case 9 : return 4; break;
		case 10 : return 5; break;
		case 11 : return 6; break;
		case 12 : return SUB; break;
		case 13 : return 7; break;
		case 14 : return 8; break;
		case 15 : return 9; break;
		case 16 : return ADD; break;
		
		default: return 0; break;
		
		
	}
	
}

