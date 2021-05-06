#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define Sensor_power_DDR DDRB
#define Sensor_power_PORT PORTB
#define CDS_power 6

void ADC_Init(void){
	//Sensor_power_DDR  |= 1 << CDS_power;
	ADMUX  = (1<<REFS1)|(1<<REFS0);                                 
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);     // ADC enable, 125kHz
	_delay_us(200);
}
int ADC_Value(void){
	char i;
	int sum;
	//Sensor_power_PORT |= 1 << CDS_power;
	for(i=0;i<16;i++){
			ADCSRA |= (1<<ADSC);                        // ADC start
			while((ADCSRA & (1<<ADIF)) != (1<<ADIF));
			sum += ADCL + ADCH*256;               // add A/D result 16 times
		}
	return sum/16;
}


