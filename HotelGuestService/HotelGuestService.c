/*
* Team Id : eYRC-HS#3081
* Author List : Sujata Shrinivas Regoti
* Filename: HotelGuestService.c
* Theme: HD – eYRC Specific 
* Functions: provide_service_to_RoomA(),provide_service_to_RoomC(),provide_service_to_RoomD()
* Global Variables: none
* 
*/

#define F_CPU 14745600
//-----------------Required Header Files--------------------//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

/*
* Function Name: provide_service_to_RoomA
* Input : none
* Output : robot provide service to room A and returns to home
* Logic: 1st robot do line following then pick service and do line following followed by wall folowing then enter into 
* room A place Service and returns to home and stop
* Example Call: provide_service_to_RoomA()
*/

void provide_service_to_RoomA(){
	
	pick_service_A();
	go_to_roomA_and_place();
	return_To_Home();
	end_buzzer();
	
	
}


/*
* Function Name: provide_service_to_RoomC
* Input : none
* Output : robot provide service to room C and returns to home
* Example Call: provide_service_to_RoomC()
*/

void provide_service_to_RoomC(){
	
	rotate_and_pick_service_C();
	to_center_SS_from_right();
	go_to_room_C_and_place_service();
	garbage_dump_C();
	
}


/*
* Function Name: provide_service_to_RoomD
* Input : none
* Output : robot provide service to room  D and pick garbage and dump into dumping section
* Example Call: provide_service_to_RoomD()
*/

void provide_service_to_RoomD(){
	
	start_move_to_SS();
	move_left_and_pick_service1();
	return_to_center_of_SS();
	enterIntoRoomD_and_PlaceService();
	pick_garbage_D();
	dump_in_DS();
}


void initilize(){
	init_devices();
	lcd_set_4bit();
	lcd_init();
}
int main(void)
{
   initilize();
   
   provide_service_to_RoomD();
   provide_service_to_RoomC();
   provide_service_to_RoomA();
}