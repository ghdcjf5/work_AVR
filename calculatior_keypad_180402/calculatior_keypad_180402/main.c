/*
 * calculatior_keypad_180402.c
 *
 * Created: 2018-04-02 오전 10:48:57
 * Author : KCCI
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char value=10;
#define add 1
#define sub 2
#define mul 3
#define div 4
#define c 5
#define equal 6

int tmp = 0;
//int number={1,2,3,4,5,6,7,8,9,0};
//extern unsigned volatile char key_data = 0;
// volatile char input_number = 0;
volatile int  cursor=0 ;

int main(void)
{
	unsigned int input_number=0,result=0;
	 volatile char opcode=0 ; //초기화 중요!!!!
   Key_Init();
   LCD_init();
   LCD_string(0x80,"   calculator   ");
  // LCD_string(0xc0,"               0");
	
  	LCD_command(0xcf); 
	  
    while (1) 
    {
		value = Key_trans(Key_scan());
		
			
			if(value>=1&&value<=10){
				
				LCD_command(0x18);
				_delay_ms(3000);
				
				switch(value){
					case 10 :
					input_number = input_number * 10;LCD_data('0');
					break;
					case 1:input_number = input_number * 10+1;LCD_data('1');
					break;
					case 2:input_number = input_number * 10+2;LCD_data('2');
					break;
					case 3:input_number = input_number * 10+3;LCD_data('3');
					break;
					case 4:input_number = input_number * 10+4;LCD_data('4');
					break;
					case 5:input_number = input_number * 10+5;LCD_data('5');
					break;
					case 6:input_number = input_number * 10+6;LCD_data('6');
					break;
					case 7:input_number = input_number * 10+7;LCD_data('7');
					break;
					case 8:input_number = input_number * 10+8;LCD_data('8');
					break;
					case 9:input_number = input_number * 10+9;LCD_data('9');
					break;	
				}
			
			}
			
			if(value>=11&&value<=16){
				if(input_number){
					switch(value){
						 case 11 :
						 opcode = add; result = input_number;input_number = 0;LCD_data('+');
						 break;
						 case 12:
						  opcode = sub;result = input_number;input_number = 0;LCD_data('-');
						 break;
						 case 13:
						  opcode = mul;result = input_number;input_number = 0;LCD_data('*');
						 break;
						 case 14:
						  opcode = div;result = input_number;input_number = 0;LCD_data('/');
						 break;
						 case 15:  //equal
						 
						 if(opcode==add){
							 result = result+input_number; LCD_data(result);
							 input_number = 0;
							 }
						 else if(opcode==sub){
							 result = result-input_number; LCD_data(result);
							 input_number = 0;
						 }	 
						 else if(opcode==mul){
							 result = result*input_number; LCD_data(result);
							 input_number = 0;
						 }	 
						 else if(opcode==div){
							 result = result/input_number; LCD_data(result);
							 input_number = 0;
						 }	
						 
						 break;
						
					}
					
					
				}
				
// 				else if(input_number){
// 					
// 					
// 				}
			}
	
	
	
	
	}//while의 괄호
}//main의 괄호