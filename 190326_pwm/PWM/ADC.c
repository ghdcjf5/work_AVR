#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define Sensor_power_DDR DDRB
#define Sensor_power_PORT PORTB
#define CDS_power 5

void ADC_Init(void){
	//Sensor_power_DDR  |= 1 << CDS_power;
	ADMUX  = (1<<REFS1)|(1<<REFS0);                                 
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);     // ADC enable, 125kHz
	_delay_us(200);
}
int ADC_Value(int old_ADC_value){
	int sum;
	//Sensor_power_PORT |= 1 << CDS_power;
	ADCSRA |= (1<<ADSC);                        // ADC start
	while((ADCSRA & (1<<ADIF)) != (1<<ADIF));
	sum = old_ADC_value*9 + ADCL + ADCH*256;
	//새로 읽는(측정)값을 기존의 값의 10분의1정도로 반영되게 하여 평균을 낸다.
	return sum/10;
}


//analog 전위값을 dc로 바꿔주는것 mux로 채널변경해서 각 채널의 sensor전위값 측정해준다.
//cds를 이용하여 조도를 읽어들인다.
//써미스터(온도에 따른 저항값 바뀌기떄문에) : 온도에 따른 보정을 위해