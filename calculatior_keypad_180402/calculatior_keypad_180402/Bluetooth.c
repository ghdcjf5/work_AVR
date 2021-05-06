/*
 * BlueTooth.c
 *
 * Created: 2017-08-21 오전 10:10:57
 * Author : user
 */ 
/*UART.c와 함께 동작*/

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BlueTooth_string TX1_string	/*UART.c와 함께 동작*/
#define BlueTooth_char TX1_char		/*UART.c와 함께 동작*/

void BlueToothInit(void){
	UART1_Init(9600L);
}
void setName(unsigned char *string){
	BlueTooth_string("AT+NAME");
	while(*string != '\0')
	{
		BlueTooth_char(*string);
		string++;
	}
}
void setPin(unsigned char *string){
	BlueTooth_string("AT+PIN");
	while(*string != '\0')
	{
		BlueTooth_char(*string);
		string++;
	}
}
void connectTest(void){
	BlueTooth_string("AT");
}
void setBAUD(unsigned char BAUD){ 
	/*BAUD value
	사용 예 setBAUD(4); -> 9600
	1 : 1200
	2 : 2400
	3 : 4800
	4 : 9600     (default)
	5 : 19200
	6 : 38400
	7 : 57600
	8 : 115200
	9 : 230400
	A : 460800
	B : 921600
	C : 1382400	*/
	BlueTooth_string("AT+BAUD");			
	BlueTooth_char('0' + BAUD);
}
void getVersion(void){
	BlueTooth_string("AT+VERSION");
}

