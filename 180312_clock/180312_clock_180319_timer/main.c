/*
 
 *
 * Created: 2018-03-12~2018-03-13 오전 9:30:31
 * Author : KCCI
 */ 


#include <avr/io.h>  
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>  //sei를 쓰기위하여



//전처리지시어로 재정의 해놓음 코드의 가독성을 높히기 위해서
#define FND_DATA_DDR DDRA  //data direct resistor  FND 데이터버스는 portA에 연결
#define FND_DATA_PORT PORTA  //#은 전처리 지시어 컴파일 하기전에 처리하는것, FND_DATA_PORT를 밑에 구절에서 찾아서 PORTA로 바꿔준다.
#define FND_CONTROL_DDR DDRG //FND 콘트롤버스는 portG에 연결
#define FND_CONTROL_PORT PORTG //다이나믹구동을 위해 control 을 만들어준다.(port 절약)
#define LED_PORT PORTC
#define LCD_DATABUS	PORTB // LCD/GLCD data
#define LCD_CONTROL	PORTE // LCD/GLCD control signal
#define LCD_DATABUS_DDR DDRB
#define LCD_CONTROL_DDR DDRE
#define RS 5
#define RW 6
#define EN 7
#define CLOCK 0
#define CORRECT 1 
#define STOPWATCH 2
#define COUNTDOWN 3
#define START 0b00100000
#define LAB 0b01000000
#define CLEAR 0b10000000
//


//#define FND_FONT_ZERO 0b11000000  //최적화를 위해 메모리 한개를 써준다.(변수 쓰는거보다 define시켜주는것)

char FND[4]; //가상 fnd(코드)를 만들어서 리얼오브젝트 fnd(칩)도 바뀌도록, 배열을 써서 메모리절약(주소값을 하나만 받기때문에)


//char FND_FONT_EXTRA[2]={0,1};


char FND_FONT[10] = {0b11000000, 0b11111001 , 0b10100100, 0b10110000, 
					0b10011001,0b10010010 ,0b10000010 ,0b11111000 ,0b10000000 ,0b10010000 };
					
					
char LCD[6]	;
char LCD_FONT[10] = {'0','1','2','3','4','5','6','7','8','9'};				
					
volatile char lab_time_flag=1;					
char volatile  change_time_flag;	  //시간변화		
volatile char sw_change_time_flag;
volatile char cd_change_time_flag;
volatile char play_flag,cd_flag;

extern char sec, min,hour;	
extern char half_sec;
extern char half_sec2;
extern char dmsec;
extern char sw_dmsec;
extern char cd_hour,cd_min,cd_sec;



extern char csec,sw_sec,sw_min,sw_hour;


extern int  He_Pirate1_Beat[];
extern int He_Pirate[];		

extern int School_Bell_Beats[];
extern int School_Bell_Tune[];			
char key_value = 0;	

char old_key = 0;				
					
char long_key = 0;

char cnt = 0;

volatile char mode=0;

volatile char cd_mode = 0;


 /*
 unsigned char sec=0, min=0, hour=0;
 unsigned char cnt=0;  //cnt는 8ms
*/

//char hour_inc_flag;

int main(void){
	
	
	
  FND_init();
  Timer0_Init();
  Timer1A_Init(2);
//  Timer3A_Init(2);//2ms
 // OCR3A = 15288;  //7643(4C)
 // DDRE = 0x08;
  
  //FND_COLON();
  led_init();
  Speaker_Init();
   LCD_init(); //초기화를 먼저 해주어야 string 함수가 실행된다.
  LCD_string(0b10000000,"  Digital Clock"); //1은 커서를 움직이는것 각 비트는 커서위치
  //LCD_string(0b11000000,"00Hr 00Min 00Sec");
  
  LCD_string(0b11000000,"    00:00:00    ");//시계출력
  DDRD &=0b00001111;
  
  //LCD_command(0b11001111); //curser entry mode set을 계산기처럼 만드려고
 
 //Music_Player(He_Pirate,He_Pirate1_Beat);
 //Music_Player(School_Bell_Tune,School_Bell_Beats);
  FND_UPDATE(0);  //초기화만 해줘도 while문에 함수가 들어가지 않아도 ISR로 정해주면 하드웨어가 호출하기때문에 구동된다.
  sei();  //set enable interrupt(글로벌 인터럽트) 이게없으면 FND동작 안한다. 인터럽트가 안들어가서
 
 //LCD_update(0);
 
 
 
	while(1){
	/* 
	 LCD_data(0x30);
	 _delay_ms(200);
	 LCD_data(0x31);
	 LCD_command(0b00000111); // display shift 왼쪽으로(increment)
	 _delay_ms(200);
	 LCD_data(0x32);
	 LCD_command(0b00000111); // display shift 왼쪽으로(increment)
	 _delay_ms(200);
	 LCD_data(0x33);
	 LCD_command(0b00000111); // display shift 왼쪽으로(increment)
	 _delay_ms(200);
	 LCD_data(0x34);
	 LCD_command(0b00000111); // display shift 왼쪽으로(increment)
	 _delay_ms(200);
	*/
	
	
	//	LCD_command(0xc4);//00:00:00의 맨처음 0위치
	/*
	LCD_data(LCD[0]);
	LCD_data(LCD[1]);
	LCD_data(':');
	LCD_data(LCD[2]);
	LCD_data(LCD[3]);
	LCD_data(':');
	LCD_data(LCD[4]);
	LCD_data(LCD[5]);
	*/
	
		if(change_time_flag){
			change_time_flag =0;
			print_time();
		}
		if(play_flag){
				Beep(7);
				play_flag=0;
		}
		
		/*
		if(sw_change_time_flag){
			sw_change_time_flag = 0;
			print_sw();
		}
		*/
		
		/*
		if(mode == COUNTDOWN){
			
		
		}
		*/
		
		if(PIND &0b00010000){
			long_key++;
			_delay_ms(10);
			if(long_key >=200 ){
				
				long_key = 0;
				
				mode++;
				if(mode>=4) mode=CLOCK;
				if(mode == CLOCK){
					 LCD_string(0b10000000,"  Digital Clock");
					 LCD_string(0b11000000,"    00:00:00    ");
					 
					 }
				
				else if (mode==STOPWATCH){
					LCD_string(0b11000000," 00:00:00:00    ");
					LCD_string(0b10000000,"             ");
					}
				else if (mode == CORRECT){
					
					LCD_string(0b10000000,"  clock setting ");
				//	LCD_string(0b11000000,"    00:00:00    ");
				}	
				else if (mode == COUNTDOWN){
					LCD_string(0b11000000,"    00:00:00    ");
					LCD_string(0b10000000,"     TIMER      ");
					
				}	
					
					
			/*	
				if(mode == CLOCK){
					mode = CORRECT;
				}
				else if(mode == CORRECT){
					mode = STOPWATCH;
				}
				else {
					mode = CLOCK;
				}
				
			*/	
			//	mode ^=1; //exclusive or
	
			}
		} 
	
		
		
		//중복키 막는것 chattering 막는것, 두개를 사용했다.
		if(old_key != PIND)	{  //button을 누를때 chattering(잡음)생겨서 pulse에 under damping or noise 끼는현상 이 일어날것
			_delay_ms(1);					//하드웨어적으로 막는방법은 cap달아준다. 솦트에어적으로는 delay준다.
			
			old_key = PIND;  //PIND:0x80를 key에 넣는다.
			
		
		
		/*
			
			
	*/			
	
			if(mode ==CORRECT){
				//LCD_string(0b10000000,"  clock setting ");
			//	LCD_string(0b11000000,"      :  :      ");
				if(PIND &0b10000000){
				
				//	LED_PORT = PIND;  
					hour++;  //while이 1초에 10만번 돌테니(while한번 누를때 수십us니까) hour가 막 올라갈것이다.
					if(hour>=24){
						hour=0;
					} //한바퀴돌면 old_key = PIND니까 if문이 안돌아감 
				}
				if(PIND &0b01000000){
				
				//	LED_PORT = PIND;
					min++;  //while이 1초에 10만번 돌테니(while한번 누를때 수십us니까) hour가 막 올라갈것이다.
					if(min>=60){
						min=0;
					} //한바퀴돌면 old_key = PIND니까 if문이 안돌아감
				}
			
				if(PIND &0b00100000){
				
				//	LED_PORT = PIND;
					sec++;  //while이 1초에 10만번 돌테니(while한번 누를때 수십us니까) hour가 막 올라갈것이다.
					if(sec>=60){
					//>=가 아닌 ==로 비교가능 다만 60이됐을때 넘어가기 전에 인터럽트가 걸리면 61이 될수도있다.
						sec=0;
					} //한바퀴돌면 old_key = PIND니까 if문이 안돌아감
				}
			}
			
			
			
			
			
			if(mode ==COUNTDOWN){
				

				if(PIND &0b00100000){  //cursor shift
					cnt++;
					if(cnt>=4) cnt = 0;		
					
				}
				
				if(PIND &0b01000000){
					//키올라가는것
					//시 분 쵸가 cursor가 각 위치에 있으면 올라가게 

							if(cnt==1){
								cd_hour++;
								if(cd_hour>=100){
									cd_hour =0;
								}
							}
							else if(cnt==2){
								cd_min++;
								if(cd_min>=60){
									cd_min = 0;
								}
							
							}
							else if (cnt==3){
								cd_sec++;
								if(cd_sec>=60){
									cd_sec = 0;
								}
							}
						
					
				}
				

				if(PIND &0b10000000){//start / stop  토글
				
						cd_flag ^= 1; 
						cnt=0;
						}
				
						
					
					
			/*	
					if (cd_change_time_flag ==1 & cd_sec==0&cd_min==0&cd_hour==0){
						Beep(7) ;
						sec=0; min=0;hour=0;
					}
			*/		
					
				}
				
				
				
			}
			
			
			
			
			if(mode==STOPWATCH){	
				
				
		
				if(PIND & START){	//stop or start
					
					sw_change_time_flag ^= 1;  //start toggle start_flag^=1;
					//1일때 시간이가는것
					lab_time_flag = 1;
					
				}
		
				if(PIND &LAB){	//lab 밑에줄
					
					lab_time_flag ^= 1;
				
				/*
					sw_change_time_flag = 0;
					LCD_command(0x8C);
					LCD_string(0b10000000,"lab");
			
			
			
			
					LCD_command(0x84);
					LCD_data('0'+(sw_hour/10));
					LCD_data('0'+sw_hour%10);
			
			
					LCD_command(0x87);
					LCD_data('0'+(sw_min/10));
					LCD_data('0'+sw_min%10);
			
			
					LCD_command(0x8A);
					LCD_data('0'+(sw_sec/10));
					LCD_data('0'+sw_sec%10);
			
			
					LCD_command(0x8D);
					LCD_data('0'+(csec/10));
					LCD_data('0'+csec%10);
			
			
			*/
			
				}
		
		
				if(PIND &CLEAR){	//clear
					csec=0;
					sw_sec=0;
					sw_min = 0;
					sw_hour = 0;
					
	
							/*
					//밑줄
					LCD_command(0xc0);
					LCD_string(0b11000000," 00:00:00:00     ");
			
					// 00: 00 : 00: 00	을 블랭크로
			
			
			
					//윗줄
					LCD_command(0x80);
					LCD_string(0b11000000,"                 ");
			
					//빈칸으로 냄기기
			*/
			
				}	
	

			}
			
			
			
	
			
		
			
			
		
		}
	
	//	LED_PORT = 0;
	
	
	
	/*
	 LCD_command(0b00000001);  //clear(지우는데 시간이 쫌 걸린다. 맨앞의 data가 나와도 클리어중이어서 표시가안된다.
	_delay_ms(30); //따라서 delay준다.
	*/
	
	/*
	LCD_command(0b00000010); // curser home
	_delay_ms(30);
	*/
	
	
	
	/*
	
	
	
	*/
	
 

	
	

	}


/*

void print_sw(void){
	
	if(mode == STOPWATCH){
		
		LCD_string(0b10000000,"            S/W");  //0x80
		LCD_string(0b11000000," 00:00:00:00");
	
	
	
		LCD_command(0xC1);
		LCD_data('0'+(sw_hour/10));
		LCD_data('0'+sw_hour%10);
	
	
		LCD_command(0xC4);
		LCD_data('0'+(sw_min/10));
		LCD_data('0'+sw_min%10);
	
	
		LCD_command(0xC7);
		LCD_data('0'+(sw_sec/10));
		LCD_data('0'+sw_sec%10);
	
	
		LCD_command(0xCA);
		LCD_data('0'+(csec/10));
		LCD_data('0'+csec%10);
	
	
		}
		else {
		LCD_string(0xc1,"  :  :  :  ");
	
		}
	
}
	
*/	











void print_time(void){
	
	
	
	
	
	
	 if(mode == STOPWATCH ){
		LCD_string(0b10000000,"             s/w");
		//LCD_string(0b11000000," 00:00:00:00    ");
		
		if(lab_time_flag){
			
		/*	
			LCD_command(0x81);
			LCD_data('0'+(sw_hour/10));
			LCD_data('0'+sw_hour%10);
			
			
			LCD_command(0x84);
			LCD_data('0'+(sw_min/10));
			LCD_data('0'+sw_min%10);
			
			
			LCD_command(0x87);
			LCD_data('0'+(sw_sec/10));
			LCD_data('0'+sw_sec%10);
			
			
			LCD_command(0x8a);
			LCD_data('0'+(csec/10));
			LCD_data('0'+csec%10);
			
		*/
		
		
			LCD_command(0xC1);
			LCD_data('0'+(sw_hour/10));
			LCD_data('0'+sw_hour%10);
			
			
			LCD_command(0xC4);
			LCD_data('0'+(sw_min/10));
			LCD_data('0'+sw_min%10);
			
			
			LCD_command(0xC7);
			LCD_data('0'+(sw_sec/10));
			LCD_data('0'+sw_sec%10);
			
			
			LCD_command(0xCa);
			LCD_data('0'+(csec/10));
			LCD_data('0'+csec%10);
			
		
			
		}
		
		
		
	
	}
	
	
	
	

	
	

	
	else if(mode == CLOCK || mode == CORRECT){
		if(half_sec || mode==CLOCK){
			
			//LCD_string(0b11000000,"    00:00:00    ");
			LCD_command(0xC4);
			LCD_data('0'+(hour/10));
			LCD_data('0'+hour%10);
			
			
			LCD_command(0xC7);
			LCD_data('0'+(min/10));
			LCD_data('0'+min%10);
			
			
			LCD_command(0xCA);
			LCD_data('0'+(sec/10));
			LCD_data('0'+sec%10);
			
			
		}
		
	
		
		
		
		
	
		else {
			
			
				LCD_string(0xc4,"  :  :  ");
		
			
		}
		
	}
	
	else {  //countdown(timer) 모드일때
		
			LCD_command(0xC4);
			LCD_data('0'+(cd_hour/10));
			LCD_data('0'+cd_hour%10);
			
			
			LCD_command(0xC7);
			LCD_data('0'+(cd_min/10));
			LCD_data('0'+cd_min%10);
			
			
			LCD_command(0xCA);
			LCD_data('0'+(cd_sec/10));
			LCD_data('0'+cd_sec%10);
			
		if(half_sec){

			if(cnt==1){
				LCD_string(0xc4,"  ");
				
			}
			else if(cnt==2){
				
				LCD_string(0xc7,"  ");
			}
			else if (cnt==3){
				LCD_string(0xcA,"  ");
			}
	
			
		
		
		
	}	
		
		
	
}
}
		
		
		
/*
if(mode == COUNTDOWN){
	if(half_sec2 ){
		
		
		LCD_command(0xC4);
		LCD_data('0'+(cd_hour/10));
		LCD_data('0'+cd_hour%10);
		
		
		LCD_command(0xC7);
		LCD_data('0'+(cd_min/10));
		LCD_data('0'+cd_min%10);
		
		
		LCD_command(0xCA);
		LCD_data('0'+(cd_sec/10));
		LCD_data('0'+cd_sec%10);
		
		
	}
	
	
	else  {
		
		
		if(cnt==1){
			LCD_string(0xc4,"  ");
		}
		
		
		else if(cnt==2){
			LCD_string(0xc7,"  ");
		}
		
		
		
		else if(cnt==3){
			LCD_string(0xcA,"  ");
		}
		
		
	}
	
	
	
	
}


*/












		
	
	
	
	

	
	
	




void LCD_update(unsigned int number){
	LCD[0]=LCD_FONT[(number/1000000)%10];
	LCD[1]=LCD_FONT[(number/10000)%10];
	LCD[2]=LCD_FONT[(number/1000)%10];
	LCD[3]=LCD_FONT[(number/100)%10];
	LCD[4]=LCD_FONT[(number/10)%10];
	LCD[5]=LCD_FONT[(number)%10];
	
	
	
	
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
 FND_CONTROL_DDR = 0x1f;  //pin 갯수(16bit+1개더) //0x?f에서 ?가 1이면 켜지고 2이면 꺼지고 f면 켜진다???
 FND_CONTROL_PORT=0; //pin 초기화

}

/*void FND_COLON(unsigned int cnt){
	
	FND[4] = FND_FONT_EXTRA[(cnt)];
	
}*/




