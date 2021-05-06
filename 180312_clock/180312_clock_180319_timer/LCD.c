#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 //text lcd�� ��ɸ޸�, �����͸޸� 2�����ֵ�. ��ɸ޸𸮿� ��ɽ��ְ�, �����͸޸𸮿� �����ͳ־��ָ� ���
 //�����͸޸𸮴� 7bitü�� : �״��� ���� 0,1(���پƷ���) �̾ �� 7���� �ּҰ��� ������.�� 8ĭ�߿��� �ǿ����� 1�� ��� 
 //�����͸� ���ش�. ������ 6bit(64��)�� ǥ�� (�� 40������ ǥ������) �����ʹ� askii�ڵ带 ����.
 //RS�� 1�ָ� ������,0���ָ� ��ɿ� ����ִ°�
 //R/W�� �д°��� ���ϱ⿡ 0���־� ���⸸�Ѵ�.
 //enable�� H->L�� �ɶ� �����͸� �д´�.
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
	_delay_us(40); // ��κ��� ����� delay�� 37us�� �ɸ��⿡ 40us�� �ذ�
}

void LCD_data(unsigned char data)		/* display a character on text LCD */
{
	LCD_CONTROL &= ~((1 << EN) | (1 << RW));  //en =0, rw=0
	LCD_CONTROL |= (1 << RS);		// E = 0, Rs = 1 
	_delay_us(1);
	LCD_DATABUS = data;				// output data
	LCD_CONTROL |= (1 << EN);				// E = 1
	_delay_us(1);
	LCD_CONTROL &= ~(1 << EN);				// E = 0  //falling edge�� �����Ͱ� ����. �ּҴ� ��ɾ���ش�.
	_delay_us(40);
}
void LCD_string(unsigned char command, unsigned char *string) /* display a string on LCD */
//					�����ġ                    ���ڿ�
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
	LCD_DATABUS_DDR = 0XFF; //FF 8���� ������ξ���.
	LCD_CONTROL_DDR |= (1 << EN) | (1 << RS) | (1 << RW); //portE lcd ��Ʈ�� ddr 567���� 1�� �־� ������� ���ڵ�.
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
//lcd�� �ʱ�ȭ���־�� �����ִ�.

