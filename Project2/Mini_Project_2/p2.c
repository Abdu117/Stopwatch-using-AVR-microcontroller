#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char secU=0;
unsigned char secT=0;
unsigned char minU=0;
unsigned char minT=0;
unsigned char hourU=0;
unsigned char hourT=0;

//Activating timer1 to tick each 1sec
void Timer1_Init()
{
	TCCR1A=(1<<FOC1A)|(1<<FOC1B);
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TIMSK|=(1<<OCIE1A);
	TCNT1=0;
	OCR1A=977;
	SREG|=(1<<7);
}

void Int0_INIT() //intrupt0 to reset the stop watch
{
	DDRD&=~(1<<2);
	PORTD|=(1<<2);
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC11);

}

void Int1_INIT() //intrupt1 to pause it 
{
	DDRD&=~(1<<3);
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC11)|(1<<ISC10);

}

void Int2_INIT() //intrupt2 to resume it
{
	DDRB&=~(1<<2);
	PORTB|=(1<<2);
	GICR|=(1<<INT2);


}

ISR(INT0_vect)
{
	 secU=0;
	 secT=0;
	 minU=0;
	 minT=0;
	 hourU=0;
	 hourT=0;
	 TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12); // to resume the stop watch if it was paused

}
ISR(INT1_vect){
	TCCR1B=(1<<WGM12);//pause the clk
}
ISR(INT2_vect){
	 TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
}
ISR(TIMER1_COMPA_vect){
	secU++;
	if(secU==10)
	{
		secU=0;
		secT++;
		if(secT==6)
		{
			secT=0;
			minU++;
			if(minU==10)
			{
				minU=0;
				minT++;
				if(minT==6)
				{
					minT=0;
					hourU++;
					if(hourU==10)
					{
						hourU=0;
						hourT++;
					}
				}
			}
		}
	}



}
int main(){

	DDRC|=0x0f;
	DDRA|=(0x3f);
	PORTC&=~(0X0F);
	Timer1_Init();
	Int0_INIT();
	Int1_INIT();
	Int2_INIT();

	while(1){

		PORTA=(1<<5);
		PORTC=(PORTC & 0xf0)|(secU & 0x0f); // show a number on each 7-segment each 2ms
		_delay_ms(2);

		PORTA=(1<<4);
		PORTC=(PORTC & 0xf0)|(secT & 0x0f);
		_delay_ms(2);
		PORTA=(1<<3);
		PORTC=(PORTC & 0xf0)|(minU & 0x0f);
		_delay_ms(2);

		PORTA=(1<<2);
		PORTC=(PORTC & 0xf0)|(minT & 0x0f);
		_delay_ms(2);

		PORTA=(1<<1);
		PORTC=(PORTC & 0xf0)|(hourU & 0x0f);
		_delay_ms(2);

		PORTA=(1<<0);
		PORTC=(PORTC & 0xf0)|(hourT & 0x0f);
		_delay_ms(2);

	}
}
