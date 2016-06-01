/*
* Team Id : eYRC-HS#3081
* Author List : Sujata Shrinivas Regoti
* Filename: HotelGuestService.c
* Theme: HD – eYRC Specific
* Functions:
* Global Variables: <List of global variables defined in this file, none if no global
* variables>
*/

#include "motion_control.h"
#include "arm_util.h"

unsigned int lim1=5,lim2=780;


void wallFollowS5(int d){
	get_sharp_sensor_values();
	get_white_sensor_values();
	while(s5 < lim2 && (d>=0 && d <  400 ))
	{
		if (s5 < 328)
		{
			forward();
			velocity(150,165);
			while (s5<328 && (d>=0 && d <  400 ) )
			{
				get_sharp_sensor_values();
				lcd_print(1,1,s1,3);
				lcd_print(1,5,s3,3);
				lcd_print(1,10,s5,3);
				lcd_print(2,1,d,3);
				if(Center_white_line >= 20 || Left_white_line > 20 || Right_white_line > 20){
					d=500;
					lineFollower();
					break;
				}
				d--;
			}
		}
		if(s5 > 328 && s5 < 700)
		{
			forward();
			velocity(170,140);
			while (s5 > 328  && (d>=0 && d <  400 ) )
			{
				lcd_print(1,1,s1,3);
				lcd_print(1,5,s3,3);
				lcd_print(1,10,s5,3);
				lcd_print(2,1,d,3);
				get_sharp_sensor_values();
				if(Center_white_line >= 20 || Left_white_line > 20 || Right_white_line > 20){
					d=500;
					lineFollower();
					break;
				}
				d--;
			}
		}
		if( s5 >= 700)
		break;
		if(s5 == 328)
		{
			forward();
			velocity(175,165);
			d--;
		}
		lcd_print(1,1,s1,3);
		lcd_print(1,5,s3,3);
		lcd_print(1,10,s5,3);
		lcd_print(2,1,d,3);
		
		if(Center_white_line >= 20 || Left_white_line > 20 || Right_white_line > 20){
			d=500;
			lineFollower();
			break;
		}
		get_white_sensor_values();
		get_sharp_sensor_values();
	}
	buzzer_on();
	_delay_ms(200);
	buzzer_off();
	_delay_ms(200);
	stop();
	
}
void wallFollowS1(signed int d){
	get_sharp_sensor_values();
	get_white_sensor_values();
	while(s1 < lim2 && (d>=0 && d < 400 ))
	{
		if (s1 < 328)
		{
			forward();
			velocity(170,140);
			while (s1<328 && (d>=0 && d <  400 ) )
			{
				get_sharp_sensor_values();
				lcd_print(1,1,s1,3);
				lcd_print(1,5,s3,3);
				lcd_print(1,10,s5,3);
				lcd_print(2,1,d,3);
				if(Center_white_line >= 20 || Left_white_line > 20 || Right_white_line > 20){
					d=500;
					lineFollower();
					break;
				}
				d--;
			}
		}
		if(s1 > 328 && s1 < 700)
		{
			forward();
			
			velocity(150,165);
			while (s1 > 328  && (d>=0 && d <  400 ) )
			{
				lcd_print(1,1,s1,3);
				lcd_print(1,5,s3,3);
				lcd_print(1,10,s5,3);
				lcd_print(2,1,d,3);
				get_sharp_sensor_values();
				if(Center_white_line >= 20 || Left_white_line > 20 || Right_white_line > 20){
					d=500;
					lineFollower();
					break;
				}
				d--;
			}
		}
		if( s1 >= 700)
		break;
		if(s1 == 328)
		{
			forward();
			velocity(175,165);
			d--;
		}
		lcd_print(1,1,s1,3);
		lcd_print(1,5,s3,3);
		lcd_print(1,10,s5,3);
		lcd_print(2,1,d,3);
		
		if(Center_white_line >= 20 || Left_white_line > 20 || Right_white_line > 20){
			d=500;
			lineFollower();
			break;
		}
		get_white_sensor_values();
		get_sharp_sensor_values();
	}
	buzzer_on();
	_delay_ms(200);
	buzzer_off();
	_delay_ms(200);
	stop();																																		
	
}

void garbageCollection(){
	right_degrees(100);
	
	line_follow();
	forward();
	_delay_ms(200);
	
	line_follow();
	back();
	_delay_ms(400);
	stop();
	_delay_ms(100);
	place();
	moveArmUp();
	

}
void HomeToSS()
{
	forward();
	velocity(175,175);
	_delay_ms(2000);
	stop();
	wallFollowS5(45);
	velocity(230,230);
	line_follow();
		
	//---------Center of SS----------//
	line_follow();
	
	forward();
	_delay_ms(400);
	
}

void wallDetection(){
	int var=20,lim1=50,lim2=500;
	while(1)
	{

		get_white_sensor_values();
		get_sharp_sensor_values();
		unsigned char flag=0;

		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		
		if ((s1 < lim2 && s1 > lim1) && (s5 < lim2 && s1 > lim1)){
			buzzer();
			lcd_wr_command(0x01);
			lcd_print(2,1,1,1);
			stop();
			break;
			
		}
		if(Center_white_line>var)
		{
			flag=1;
			forward();
			velocity(220,220);
			//velocity(170,170);
		}

		if (Left_white_line>var && Center_white_line>var && Right_white_line>var)
		{
			lcd_cursor(2,1);
			lcd_string("All");
			stop();
			buzzer_on();
			_delay_ms(500);
			buzzer_off();
			_delay_ms(500);
			break;
		}
		
		if((Left_white_line>var) && (flag==0))
		{
			//flag=1;
			left();
			velocity(130,130);
			while(1)
			{
				get_white_sensor_values();
				get_sharp_sensor_values();
				if ((s1 < lim2 && s1 > lim1) && (s5 < lim2 && s5 > lim1))
				{
					lcd_wr_command(0x01);
					lcd_print(2,1,2,1);
					buzzer();
					stop();
					break;
				}
				if(Left_white_line <var && Center_white_line>var && Right_white_line<var)
				{
					lcd_wr_command(0x01);
					stop();
					break;
				}
			}
			//velocity(175,175);
		}

		if((Right_white_line>var) && (flag==0))
		{
			//flag=1;
			right();
			velocity(130,130);
			while(1)
			{
				get_white_sensor_values();
				get_sharp_sensor_values();
				if ((s1 < lim2 && s1 > lim1) && (s5 < lim2 && s5 > lim1))
				{
					lcd_print(2,1,3,1);
					buzzer();
					stop();
					break;
				}
				if(Left_white_line <var && Center_white_line>var && Right_white_line<var)
				{
					stop();
					break;
				}
			}//velocity(175,175);
		}
		if((Left_white_line>var) && (flag==1))
		{
			stop();
			lcd_cursor(2,1);
			lcd_string("Left");
			buzzer_on();
			_delay_ms(500);
			buzzer_off();
			_delay_ms(500);
			break;
		}

		if((Right_white_line>var) && (flag==1))
		{
			stop();
			lcd_cursor(2,1);
			lcd_string("Right");
			buzzer_on();
			_delay_ms(500);
			buzzer_off();
			_delay_ms(500);
			break;
		}
		get_sharp_sensor_values();
		if ((s1 < lim2 && s1 > lim1) && (s5 < lim2 && s5 > lim1))
		{
			lcd_print(2,1,4,1);
			buzzer();
			stop();
			break;
		}
	}
}

void SSToHome(){
	line_follow();
	wallFollowS1(70);
	line_follow();
	
}


void wallfollow_S1(){
	get_sharp_sensor_values();
	get_white_sensor_values();
	while(Center_white_line < 20 && Left_white_line < 20 && Right_white_line < 20)
	{
		if (s1 < 328)
		{
			forward();
			velocity(190,165);
			while (s1<328)
			{
				get_sharp_sensor_values();
				lcd_print(1,1,s1,3);
				lcd_print(1,5,s3,3);
				lcd_print(1,10,s5,3);
			
			}
		}
		if(s1 > 328 && s1 < 700)
		{
			forward();
			
			velocity(170,185);
			while (s1 > 328)
			{
				lcd_print(1,1,s1,3);
				lcd_print(1,5,s3,3);
				lcd_print(1,10,s5,3);
				get_sharp_sensor_values();
		
			}
		}
		if( s1 >= 500){
			
			buzzer();
			break;
		}
		
		if(s1 == 328)
		{
			forward();
			velocity(190,190);
			
		}
		lcd_print(1,1,s1,3);
		lcd_print(1,5,s3,3);
		lcd_print(1,10,s5,3);

		get_white_sensor_values();
		get_sharp_sensor_values();
	}
	buzzer_on();
	_delay_ms(200);
	buzzer_off();
	_delay_ms(200);
	stop();
	
	
}
void start_move_to_SS(){
	left_degrees(90);
	HomeToSS();
}

void move_left_and_pick_service1(){
	left_degrees(100);
	line_follow();
	
	forward();
	_delay_ms(210);
	moveArmUp();
	
	ungrip();
	left_degrees(105);
	pick();

}

void enterIntoRoomD_and_PlaceService(){
	wallFollowS1(20);
	right_degrees(100);
	forward();
	_delay_ms(1000);
	
	line_follow();
	
	forward();
	_delay_ms(400);
	
	right_degrees(95);
	place();

}

void pick_garbage_D(){
	
		moveArmUp();
		right_degrees(90);
		back();
		_delay_ms(100);
		
		right_degrees(95);
		
		pick();
}
void dump_in_DS(){
	left_degrees(108);
	
	wallDetection();
	

	forward();
	_delay_ms(900);
	stop();
	
	left_degrees(90);
	
	wallFollowS5(30);
	line_follow();
	
	forward();
	_delay_ms(400);
	
	
	garbageCollection();
}

void rotate_and_pick_service_C(){
	right_degrees(180);
	line_follow();
	
	forward();
	_delay_ms(350);
	
	moveArmUp();
	ungrip();
	
	left_degrees(95);
	
	
	pick();
}

void to_center_SS_from_right(){
	right_degrees(95);
	
	line_follow();
	forward();
	_delay_ms(400);
	left_degrees(90);
	
}
void go_to_room_C_and_place_service()
{
	SSToHome();
	forward();
	_delay_ms(400);
	right_degrees(95);
	
	forward();
	_delay_ms(1500);
	
	wallFollowS5(21);
	
	left_degrees(100);
	
	forward();
	_delay_ms(900);
	
	line_follow();
	
	forward();
	_delay_ms(250);
	left_degrees(100);
	place();
	
}
void garbage_dump_C(){
	
	moveArmUp();
	left_degrees(90);
	back();
	_delay_ms(100);
	
	left_degrees(110);
	
	pick();
	right_degrees(95);
	
	wallDetection();

	forward();
	_delay_ms(1000);
	
	right_degrees(100);
	
	wallFollowS1(60);
	line_follow();

	forward();
	_delay_ms(400);
	left_degrees(95);
	
	HomeToSS();
	garbageCollection();
}

void pick_service_A(){
	right_degrees(180);
	
	line_follow();//1
	forward();
	_delay_ms(200);
	
	line_follow();//2
	forward();
	_delay_ms(300);
	
	line_follow();//3
	_delay_ms(300);
	
	line_follow();//4
	_delay_ms(300);
	
	//rotate_left();
	forward();
	_delay_ms(300);
	moveArmUp();
	ungrip();
	left_degrees(100);
	pick();
	
}

void go_to_roomA_and_place(){
	
	left_degrees(110);
	line_follow();
	
	forward();
	_delay_ms(200);
	
	line_follow();
	
	forward();
	_delay_ms(400);
	right_degrees(95);
	
	SSToHome();
	forward();
	_delay_ms(350);
	
	left_degrees(90);
	
	forward();
	_delay_ms(1000);
	
	wallFollowS5(30);
	
	left_degrees(90);
	
	forward();
	_delay_ms(1000);
	line_follow();
	
	forward();
	_delay_ms(300);
	left_degrees(90);
	place();
	
	
}

void return_To_Home(){
	left_degrees(100);

	wallDetection();
	
	moveArmDown();
	forward();
	_delay_ms(1200);
	
	right_degrees(100);
	
	
	wallfollow_S1();
	
}