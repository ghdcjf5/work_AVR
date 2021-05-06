/*
 * 180326_RC_exam.c
 *
 * Created: 2018-03-26 오전 9:57:34
 * Author : KCCI
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>
volatile char UART_RX_data;
#define add 1
#define sub 2
#define mul 3
#define div 4


int main(void)
{
unsigned int input_number=0,result=0;
char opcode=0;  //초기화 중요!!!!
   UART1_Init(9600);  //송신속도 9600
   led_init();
   LCD_init();
   LCD_string(0x80,"   calculator   ");
   LCD_string(0xc0,"                ");
  // LCD_command(0xd0);
   sei();
   
    while (1) 
    {	

		if(UART_RX_data){
			//수신이 있을때 라는 뜻이다.
			//switch case에 break가 없으면 0누를때 0~7까지 다 들어간다.
			//adc할때 led바를 사용하면 level meter가 된다.
			
			if(UART_RX_data>= '0'&& UART_RX_data<= '9'){
				LCD_command(0x18);
				switch(UART_RX_data){
					case '0':input_number = input_number * 10+0; 
					LCD_data('0');break;
					//커서 1번미는것
					case '1':input_number = input_number * 10+1;
					LCD_data('1');break;
					case '2':input_number = input_number * 10+2;
					LCD_data('2');break;
					case '3':input_number = input_number * 10+3;
					LCD_data('3');break;
					case '4':input_number = input_number * 10+4;
					LCD_data('4');break;
					case '5':input_number = input_number * 10+5;
					LCD_data('5');break;
					case '6':input_number = input_number * 10+6;
					LCD_data('6');break;
					case '7':input_number = input_number * 10+7;
					LCD_data('7');break;
					case '8':input_number = input_number * 10+8;
					LCD_data('8');break;
					case '9':input_number = input_number * 10+9;
					LCD_data('9');break;
	
				}
				
				UART_RX_data = 0;
			}
		
			else if(UART_RX_data=='+'||UART_RX_data=='-'||UART_RX_data=='*'||
			UART_RX_data=='/'||UART_RX_data=='='){
				if(input_number){  //input넘버에 무언가가 있을때
					
					
					if(opcode){ //opcode가 뭔가있으면 그opcode에대한 case문
						switch(opcode){
							
							 case add:
							 result = result + input_number;
							 opcode = add;
							 break;
							 case sub:
							 result = result - input_number;
							 opcode = sub;
							 break;
							 case mul:
							 result = result * input_number;
							 opcode = mul;
							 break;
							 case div:
							 result = result / input_number;
							 opcode = div;
							 break;
							
						} 
						
						
						 LCD_command(0x01);  //화면초기화
						 _delay_ms(5);
						 
						 LCD_command(0xcb); //커서가 아랫줄 맨오른쪽에서 5번째 에 위치시킨다.
						 LCD_5D(result);
						 TX1_5D(result);
						 TX1_char('\n');
						 TX1_char('\r');
						 input_number = result;
						 opcode =0;
						 
					}
					
					switch(UART_RX_data){
						case '+': result = input_number;input_number = 0;
							LCD_command(0x18);
							LCD_data('+');
							
							opcode = add; break;
							
						case '*': result = input_number;input_number = 0;
							LCD_command(0x18);
							LCD_data('*');
							
							opcode = mul; break;
							
						case '-': result = input_number;input_number = 0;
							LCD_command(0x18);
							LCD_data('-');
							
							opcode = sub; break;
							
						case '/': result = input_number;input_number = 0;
							LCD_command(0x18);
							LCD_data('/');
							
							opcode = div; break;
							
							
							/*	
								case '=' :
								if(opcode==add){
									result = result+input_number;
								}
								else if(opcode==sub){
									result = result-input_number;
								}
								else if(opcode==div){
									result = result/input_number;
								}
								else if(opcode==mul){
									result = result*input_number;
								}
							
								LCD_command(0x01);  //화면초기화
								_delay_ms(10);
							
								LCD_command(0xcb); //커서가 아랫줄 맨오른쪽에서 5번째 에 위치시킨다.
								LCD_5D(result);
							
								input_number = 0;
								opcode = 0; break;
								*/
						case 'c' : result=0;input_number=0;opcode=0;
							LCD_command(0x01);  //화면초기화
							_delay_ms(5);
							LCD_string(0x80,"   calculator   ");
							// LCD_command(0xcf);
							break;

						}
						
						
						
					UART_RX_data = 0;
				}
			
			
				
			else{ //inputnumber=0
					switch(UART_RX_data){
						case '+' : opcode = add; break;
						case '-' : opcode = sub; break;
						case '*' : opcode = mul; break;
						case '/' : opcode = div; break;
						case 'c' : break;
 					}
				}
					
			}  //else if꺼
					
		}
			
			
			/*
				case '0':PORTC |= 0b10000000; 
				case '1':PORTC |= 0b01000000; 
				case '2':PORTC |= 0b00100000; 
				case '3':PORTC |= 0b00010000; 
				case '4':PORTC |= 0b00001000; 
				case '5':PORTC |= 0b00000100; 
				case '6':PORTC |= 0b00000010;
				case '7':PORTC |= 0b00000001; 
				*/
		
	
	
		
		
		
		
		/*
		TX1_char('a');
		TX1_string("hello");
		_delay_ms(500);
		*/
	
	}
	
}
