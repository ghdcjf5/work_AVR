/*
 * RC_Exam.c
 *
 * Created: 2018-03-26 오전 9:56:38
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>

#define add 1
#define sub 2
#define mul 3
#define div 4

volatile char UART_RX_data;

int main(void)
{
    unsigned int input_number=0, result=0;
	char opcode=0;
	UART1_Init(9600);
	LCD_init();
	LCD_string(0x80, "   Calculator   ");
	LCD_string(0xC0, "                ");
	//LCD_command(0xD0);
	led_init();
	sei();
    while (1) 
    {
		if(UART_RX_data){
			if(UART_RX_data >= '0' && UART_RX_data <= '9'){
				LCD_command(0x18);
				switch(UART_RX_data){
					case '0':input_number = input_number *10 + 0;
					LCD_data('0'); break;
					case '1':input_number = input_number *10 + 1;
					LCD_data('1'); break;
					case '2':input_number = input_number *10 + 2;
					LCD_data('2'); break;
					case '3':input_number = input_number *10 + 3;
					LCD_data('3'); break;
					case '4': input_number = input_number *10 + 4;
					LCD_data('4'); break;
					case '5': input_number = input_number *10 + 5;
					LCD_data('5'); break;
					case '6': input_number = input_number *10 + 6;
					LCD_data('6'); break;
					case '7': input_number = input_number *10 + 7;
					LCD_data('7'); break;
					case '8': input_number = input_number *10 + 8;
					LCD_data('8'); break;
					case '9': input_number = input_number *10 + 9;
					LCD_data('9'); break;
				}
				UART_RX_data = 0;
			}
			else if(UART_RX_data=='+' || UART_RX_data=='-' || UART_RX_data=='*' 
					|| UART_RX_data=='/' || UART_RX_data=='='){
				if(input_number){
					if(opcode){
						switch (opcode){
							case add: result = result + input_number;
								opcode = add; break;
							case sub: result = result - input_number;
								opcode = sub; break;
							case mul: result = result * input_number;
								opcode = mul; break;
							case div: result = result / input_number;
								opcode = div; break;
						}
						LCD_command(0x01);				// clear display
						_delay_ms(5);
						LCD_command(0xCB);
						LCD_5D(result);
						TX1_5D(result);
						TX1_char('\n');
						TX1_char('\r');
						input_number = result;
						opcode = 0;
					}
					switch(UART_RX_data){
						case '+':result = input_number; input_number = 0;
							LCD_command(0x18);
							LCD_data('+');
							opcode = add; break;
						case '-':result = input_number; input_number = 0;
							LCD_command(0x18);
							LCD_data('-');
							opcode = sub; break;
						case '*':result = input_number; input_number = 0;
							LCD_command(0x18);
							LCD_data('*');
							opcode = mul; break;
						case '/':result = input_number; input_number = 0;
							LCD_command(0x18);
							LCD_data('/');
							opcode = div; break;
						case 'C' : break;
					}
					UART_RX_data = 0;
				}
				else {
					switch(UART_RX_data){
						case '+': opcode = add; break;
						case '-': opcode = sub; break;
						case '*': opcode = mul; break;
						case '/': opcode = div; break;
						case 'C': break;
					}
				}
			}
		}
	}
}

