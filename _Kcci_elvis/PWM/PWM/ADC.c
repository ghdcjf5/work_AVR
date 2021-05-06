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
	
	return sum/10;
}


