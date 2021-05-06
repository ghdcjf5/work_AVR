/*
 * Calculator.c
 *
 * Created: 2018-04-02 오전 9:53:36
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>

#define Scr_Shift_Left 0x18
#define LCD_Clear 0x01
#define zero 10
#define add 11
#define sub 12
#define mul 13
#define div 14
#define op 15
#define clear 16

int main(void)
{
    Key_Init();
	LCD_init();
	LCD_string(0x80,"KCCI Calculator ");
	LCD_string(0xC0,"                ");
	
    while (1) 
    {
		Key_process(Key_scan());
    }
}
unsigned char Key_trans(unsigned char key_value){
	switch(key_value){
		case 1: return 7; break;
		case 2: return 4; break;
		case 3: return 1; break;
		case 4: return zero; break;
		case 5: return 8; break;
		case 6: return 5; break;
		case 7: return 2; break;
		case 8: return clear; break;
		case 9: return 9; break;
		case 10: return 6; break;
		case 11: return 3; break;
		case 12: return op; break;
		case 13: return add; break;
		case 14: return sub; break;
		case 15: return mul; break;
		case 16: return div; break;
		default: return 0; break;
	}
}
void Key_process(unsigned char key_value){
	static unsigned char old_key=0, opcode=0;
	static int result = 0, number = 0, tmp = 0;
	int cursor = 0;
	_delay_us(10);
	if(key_value==Key_scan()){
		if(old_key != key_value){
			old_key = key_value;
			if(number == 0 && opcode == 0){
				LCD_command(LCD_Clear);
				_delay_ms(5);
				LCD_command(0xCF);
			}
			char trans_value = Key_trans(old_key);
			if(1 <= trans_value && trans_value <= 10){
				switch(trans_value){
					case zero : 	LCD_data('0');
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					number = number*10;
					break;
					case 1:
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('1');
					number = number*10 + 1;
					break;
					case 2 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('2');
					number = number*10 + 2;
					break;
					case 3 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('3');
					number = number*10 + 3;
					break;
					case 4 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('4');
					number = number*10 + 4;
					break;
					case 5 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('5');
					number = number*10 + 5;
					break;
					case 6 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('6');
					number = number*10 + 6;
					break;
					case 7 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('7');
					number = number*10 + 7;
					break;
					case 8 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('8');
					number = number*10 + 8;
					break;
					case 9 :
					if(number)LCD_command(Scr_Shift_Left);
					else {
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
					}
					LCD_data('9');
					number = number*10 + 9;
					break;
				}
			}
			else if(trans_value>=11 && trans_value<=16){
				if(number){
					if(opcode){
						switch(opcode){
							case add : result = result + number; break;
							case sub : result = result - number; break;
							case mul : result = result * number; break;
							case div : result = result / number; break;
						}
						tmp = result;
						while(tmp){
							tmp /= 10;
							cursor++;
						}
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xD0);
						switch(cursor){
							case 5 : LCD_data('0' + result/10000);LCD_command(Scr_Shift_Left);
							case 4 : LCD_data('0' + result/1000%10);LCD_command(Scr_Shift_Left);
							case 3 : LCD_data('0' + result/100%10);LCD_command(Scr_Shift_Left);
							case 2 : LCD_data('0' + result/10%10);LCD_command(Scr_Shift_Left);
							case 1 : LCD_data('0' + result%10);LCD_command(Scr_Shift_Left);break;
							default: LCD_string(0x80,"Too Big"); break;
						}
						number = result;
						opcode = 0;
					}
					switch(trans_value){
						case add :
						opcode = add; result = number; number = 0;
						break;
						case sub :
						opcode = sub; result = number; number = 0;
						break;
						case mul :
						opcode = mul; result = number; number = 0;
						break;
						case div :
						opcode = div; result = number; number = 0;
						break;
						case '=' :
						switch(opcode){
							case add : result = result + number; break;
							case sub : result = result - number; break;
							case mul : result = result * number; break;
							case div : result = result / number; break;
						}
						tmp = result;
						cursor = 0;
						while(tmp){
							tmp /= 10;
							cursor++;
						}
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xD0);
						switch(cursor){
							case 5 : LCD_data('0' + result/10000);LCD_command(Scr_Shift_Left);
							case 4 : LCD_data('0' + result/1000%10);LCD_command(Scr_Shift_Left);
							case 3 : LCD_data('0' + result/100%10);LCD_command(Scr_Shift_Left);
							case 2 : LCD_data('0' + result/10%10);LCD_command(Scr_Shift_Left);
							case 1 : LCD_data('0' + result%10);LCD_command(Scr_Shift_Left);break;
							default: LCD_string(0x80,"Too Big"); break;
						}
						number = result;
						opcode = 0;
						break;
						case clear :  opcode = 0;
						number = 0;
						result = 0;
						LCD_command(LCD_Clear);
						_delay_ms(5);
						LCD_command(0xCF);
						break;
					}
				}
				else{
					switch(trans_value){
						case add :
						opcode = 1;break;
						case sub :
						opcode = 2; break;
						case mul :
						opcode = 3; break;
						case div :
						opcode = 4; break;
					}
				}
			}
			if(trans_value == clear){
				number = 0;
				result = 0;
				opcode = 0;
				cursor = 0;
				LCD_command(LCD_Clear);
				_delay_ms(5);
				LCD_command(0xCF);
			}
		}
	}
	return;
}

