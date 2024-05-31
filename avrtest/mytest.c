

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



unsigned char seconds=0;
unsigned char minutes=0;
unsigned char hours=0;

void s7seg(void){
	PORTA=(1<<0);
			PORTC=seconds%10;
			_delay_ms(3);
			PORTA=(1<<1);
			PORTC=(seconds-(seconds%10))/10;
			_delay_ms(3);
			PORTA=(1<<2);
			PORTC=minutes%10;
			_delay_ms(3);
			PORTA=(1<<3);
			PORTC=(minutes-(minutes)%10)/10;
			_delay_ms(3);
			PORTA=(1<<4);
			PORTC=hours%10;
			_delay_ms(3);
			PORTA=(1<<5);
			PORTC=(hours-(hours%10))/10;
			_delay_ms(3);

}

ISR(INT0_vect){
	seconds=0;
	minutes=0;
	hours=0;
}
ISR(INT1_vect){
	TIMSK &= ~(1<<OCIE1A);



}
ISR(INT2_vect){
	TIMSK |= (1<<OCIE1A);

}


ISR(TIMER1_COMPA_vect)
{



	if(seconds ==59){
		seconds=0;

		if(minutes==59){
			minutes=0;
			hours++;

		}
		else{
			minutes++;
		}
	}
	else{
		seconds++;
	}



}
void INT2_init(void){
	SREG   &= ~(1<<7);
	DDRB   &= (~(1<<PB2));
	GICR   |= (1<<INT2);
	MCUCSR &=~(1<<ISC2);
	SREG   |= (1<<7);

}
void INT1_init(void){
	SREG   &= ~(1<<7);
	DDRB   &= (~(1<<PD3));
	GICR   |= (1<<INT1);
	MCUCR |= (1<<ISC11)|(1<<ISC10);
	SREG   |= (1<<7);


}
void INT0_init(void){

		SREG   &= ~(1<<7);       
		DDRB   &= (~(1<<PD2));   
		GICR   |= (1<<INT0);	 
		MCUCSR |= (1<<ISC01);     
		SREG   |= (1<<7);        

}



void Timer1_CTC_Init(void)
{

	TCNT1 = 0;

	OCR1A = 15625;

	TIMSK |= (1<<OCIE1A);

	TCCR1A = (1<<FOC1A );


	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
}

int main(void)
{
	DDRC  |= 0x0F;
	INT0_init();
	INT1_init();
	INT2_init();




	DDRA |= 0x3f;
	PORTA |= 0x3f;
	Timer1_CTC_Init();
	SREG  |= (1<<7);








while(1){


s7seg();
}
    }

