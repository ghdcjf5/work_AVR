/*! \file UART0.c  
*****************************************************************************
//
// File Name	: 'UART.c'
// Title	: UART0 driver for Zigbex Platform
// Author	: Kyoung Yoon Jeong
// Created	: 8/20/2017
// Version	: 1.0
// Target MCU	: Atmel AVR ATMega128
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
// *************************************************/ 
/*         UART0_Init(BAUD)			UART0 설정 및 초기화(통신속도 BAUD, 수신 인터럽트 사용, 문자크기 8비트)
		   TX0_char('char')       	한 문자 출력
		   TX0_string("string")   	문자열 출력
		   TX0_5D()				  	5자리 십진수 출력
		   TX0_2D()				  	2자리 십진수 출력
		   TX0_BIN()			  	8자리 이진수 출력
		   TX0_HEX() 			  	4자리 16진수 출력  
		   							출력함수에 줄바꿈 없음    */
		    
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*ISR(USART0_RX_vect)   //UART0 수신 인터럽트 서비스 루틴
{
	
}*/
ISR(USART1_RX_vect)		//UART1 수신 인터럽트 서비스 루틴
{
	char data = UDR1;
	//TX1_char(data);
	LCD_data(data);
	PORTC<<=1;
}

void TX0_char(unsigned char data)		/* transmit a character by USART0 */
{
	while((UCSR0A & 0x20) == 0x00);		// data register empty ?
	UDR0 = data;
}
void TX1_char(unsigned char data)		/* transmit a character by USART1 */
{
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
	UDR1 = data;
}
void TX0_string(unsigned char *string)          /* transmit a string by USART0 */
{
	while(*string != '\0')
	{	
		TX0_char(*string);
		string++;
	}
}
void TX1_string(unsigned char *string)          /* transmit a string by USART1 */
{
	while(*string != '\0')
	{
		TX1_char(*string);
		string++;
	}
}
void TX0_5D(unsigned int data)
{
	TX0_char('0'+data/10000); 
	TX0_char('0'+data%10000/1000);
	TX0_char('0'+data%1000/100); 
	TX0_char('0'+data%100/10);
	TX0_char('0'+data%10);
}
void TX1_5D(unsigned int data)
{
	TX1_char('0'+data/10000);
	TX1_char('0'+data%10000/1000);
	TX1_char('0'+data%1000/100);
	TX1_char('0'+data%100/10);
	TX1_char('0'+data%10);
}
void UART0_Init(unsigned int Baud)
{
	char i;
	
	UBRR0H = F_CPU/16/Baud-1>>8;					//UBRR0 <UBRR=FOSC/16/BAUD-1>
	UBRR0L = F_CPU/16/Baud-1&0xFF;
	
	UCSR0A = 0x00;                                  // asynchronous normal mode
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);     // Rx/Tx enable, 8 data
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);               // no parity, 1 stop, 8 data
	i = UDR0;
	_delay_ms(1);
}
void UART1_Init(unsigned int Baud)
{
	char i;
	UBRR1H = F_CPU/16/Baud-1>>8;                    //UBRR1 <UBRR=FOSC/16/BAUD-1>
	UBRR1L = F_CPU/16/Baud-1&0xFF;
	UCSR1A = 0x00;                                  // asynchronous normal mode
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);     // Rx/Tx enable, 8 data
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);               // no parity, 1 stop, 8 data
	i = UDR1;
	_delay_ms(1);
}
void TX0_BIN(unsigned char data)
{
	TX0_char('0'+data/128); 
	TX0_char('0'+data%128/64);
	TX0_char('0'+data%64/32); 
	TX0_char('0'+data%32/16);
	TX0_char('0'+data%16/8);
	TX0_char('0'+data%8/4);
	TX0_char('0'+data%4/2);
	TX0_char('0'+data%2);
}
void TX0_HEX(unsigned int data)
{
	if(data/4096 > 9)TX0_char('A'-10+data/4096);
	else             TX0_char('0'   +data/4096);
	if(data%4096/256 > 9)TX0_char('A'-10+data%4096/256);
	else                 TX0_char('0'   +data%4096/256);
	if(data%256/16 > 9)TX0_char('A'-10+data%256/16);
	else               TX0_char('0'   +data%256/16);
	if(data%16 > 9)TX0_char('A'-10+data%16);
	else           TX0_char('0'   +data%16);
}
void TX0_2D(unsigned char data)
{
	TX0_char('0'+data/10); 
	TX0_char('0'+data%10);
}
