/*
* Team Id : eYRC-HS#3081
* Author List : Sujata Shrinivas Regoti
* Filename: HotelGuestService.c
* Theme: HD – eYRC Specific
* Functions:buzzer_pin_config () , buzzer_on() , buzzer_off() , end_buzzer()
* Global Variables: none
*/


void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}
void buzzer(){
	buzzer_on();
	_delay_ms(250);
	buzzer_off();
	_delay_ms(250);
}

void end_buzzer(){
	buzzer_on();
	_delay_ms(5000);
	buzzer_off();
	_delay_ms(250);
}