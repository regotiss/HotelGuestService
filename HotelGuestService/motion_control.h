/*
* Team Id : eYRC-HS#3081
* Author List : Sujata Shrinivas Regoti
* Filename: HotelGuestService.c
* Theme: HD – eYRC Specific
* Functions:
* Global Variables: <List of global variables defined in this file, none if no global
* variables>
*/

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;


//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}


// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}




void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F; //set direction of the PORTA 3 to PORTA 0 pins as output
	PORTA = PORTA & 0xF0; // set initial value of the PORTA 3 to PORTA 0 pins to logic 0
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM
}


//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
	DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
	PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
	DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
	PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}


//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 			    // removing upper nibbel as it is not needed
	PortARestore = PORTA; 			// reading the PORTA's original status
	PortARestore &= 0xF0; 			// setting lower direction nibbel to 0
	PortARestore |= Direction; 	    // adding lower nibbel for direction command and restoring the PORTA status
	PORTA = PortARestore; 			// setting the command to the port
}


void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

void forward (void) //both wheels forward
{
	motion_set(0x06);
}
void back (void) //both wheels backward
{
	motion_set(0x09);
}
void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void stop (void) //hard stop
{
	motion_set(0x00);
}

void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;

	while (1)
	{
		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
		break;
	}
	stop(); //Stop robot
}

void left_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	left(); //Turn left
	angle_rotate(Degrees);
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}
void get_white_sensor_values()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	
}


//--------------------------Line follow-----------------------//
void line_follow(){
	int var=20;
	unsigned char flag = 0;
	while(1)
	{

		get_white_sensor_values();
		flag=0;

		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		
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
				if(Left_white_line <var && Center_white_line>var && Right_white_line<var)
				{
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
	}
}
void lineFollower(){
	get_white_sensor_values();
	unsigned char flag = 0;
	unsigned char var = 20;
	print_sensor(1,1,3);	//Prints value of White Line Sensor1
	print_sensor(1,5,2);	//Prints Value of White Line Sensor2
	print_sensor(1,9,1);	//Prints Value of White Line Sensor3
	
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
		
	}
	
	if((Left_white_line>var) && (flag==0))
	{
		//flag=1;
		left();
		velocity(130,130);
		while(1)
		{
			get_white_sensor_values();
			if(Left_white_line <var && Center_white_line>var && Right_white_line<var)
			{
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
		
	}
}

void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	right(); //Turn right
	angle_rotate(Degrees);
}
