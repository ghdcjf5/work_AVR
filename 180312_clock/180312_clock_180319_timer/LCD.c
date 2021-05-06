#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 //text lcd는 명령메모리, 데이터메모리 2개가있따. 명령메모리에 명령써주고, 데이터메모리에 데이터넣어주면 출력
 //데이터메모리는 7bit체계 : 그다음 왼쪽 0,1(윗줄아래줄) 이어서 총 7개의 주소값을 가진다.총 8칸중에서 맨왼쪽은 1을 써야 
 //데이터를 써준다. 나머지 6bit(64개)로 표현 (총 40개까지 표현가능) 데이터는 askii코드를 쓴다.
 //RS에 1주면 데이터,0을주면 명령에 집어넣는것
 //R/W는 읽는것을 안하기에 0을주어 쓰기만한다.
 //enable은 H->L가 될때 데이터를 읽는다.
 //
 
 
#define LCD_DATABUS	PORTB // LCD/GLCD data
#define LCD_CONTROL	PORTE // LCD/GLCD control signal
#define LCD_DATABUS_DDR DDRB
#define LCD_CONTROL_DDR DDRE
#define RS 5
#define RW 6
#define EN 7

void LCD_command(unsigned char command)		/* write a command(instruction) to text LCD */
{
	LCD_CONTROL &= ~((1 << EN) | (1 << RS) | (1 << RW));				// E = 0, Rs = 0
	_delay_us(1);
	LCD_DATABUS = command;			// output command
	LCD_CONTROL |= (1 << EN);				// E = 1
	_delay_us(1);
	LCD_CONTROL &= ~(1 << EN);			// E = 0
	_delay_us(40); // 대부분의 명령의 delay가 37us가 걸리기에 40us를 준것
}

void LCD_data(unsigned char data)		/* display a character on text LCD */
{
	LCD_CONTROL &= ~((1 << EN) | (1 << RW));  //en =0, rw=0
	LCD_CONTROL |= (1 << RS);		// E = 0, Rs = 1 
	_delay_us(1);
	LCD_DATABUS = data;				// output data
	LCD_CONTROL |= (1 << EN);				// E = 1
	_delay_us(1);
	LCD_CONTROL &= ~(1 << EN);				// E = 0  //falling edge에 데이터가 들어간다. 주소는 명령어로준다.
	_delay_us(40);
}
void LCD_string(unsigned char command, unsigned char *string) /* display a string on LCD */
//					출력위치                    문자열
{
	LCD_command(command);				// start position of string
	while(*string != '\0')			// display string
	{ 
		LCD_data(*string);
		string++;
	}
}
void LCD_init(void)			/* initialize text LCD module */
{
	LCD_DATABUS_DDR = 0XFF; //FF 8개다 출력으로쓴다.
	LCD_CONTROL_DDR |= (1 << EN) | (1 << RS) | (1 << RW); //portE lcd 콘트롤 ddr 567번에 1을 넣어 출력으로 쓰겠따.
	//lc
	
	_delay_us(50);
	LCD_CONTROL |= (1 << EN) | (1 << RS);		// E = 1, Rs = 1 (dummy write)
	_delay_us(50);
	LCD_CONTROL &= ~(1 << EN);				// E = 0, Rs = 1
	_delay_us(50);

	LCD_command(0x38);				// function set(8 bit, 2 line, 5x7 dot)
	LCD_command(0x0C);				// display control(display ON, cursor OFF)
	LCD_command(0x06);				// entry mode set(increment, not shift)
	LCD_command(0x01);				// clear display
	_delay_ms(5);
}
//lcd를 초기화해주어야 쓸수있다.

