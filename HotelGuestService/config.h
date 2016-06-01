/*
* Team Id : eYRC-HS#3081
* Author List : Sujata Shrinivas Regoti
* Filename: HotelGuestService.c
* Theme: HD – eYRC Specific
* Functions:
* Global Variables: <List of global variables defined in this file, none if no global
* variables>
*/

#include "buzzer.h"
#include "lcd_util.h"
#include "adc_util.h"
#include "HGS_util.h"

//Initialize the ports
void port_init(void)
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
	servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation
	buzzer_pin_config();
}

//Function to initialize all the peripherals
void init_devices(void)
{
	cli(); //disable all interrupts
	port_init();
	timer1_init();
	timer5_init();
	adc_init();
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	sei(); //re-enable interrupts
}
void return_to_center_of_SS(){
	left_degrees(110);
	line_follow();
	
	forward();
	_delay_ms(400);
	right_degrees(90);
	
	line_follow();
	
}

