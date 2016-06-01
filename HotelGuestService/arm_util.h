/*
* Team Id : eYRC-HS#3081
* Author List : Sujata Shrinivas Regoti
* Filename: HotelGuestService.c
* Theme: HD – eYRC Specific
* Functions:
* Global Variables: <List of global variables defined in this file, none if no global
* variables>
*/


//TIMER1 initialization in 10 bit fast PWM mode  
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 52.25Hz 
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}


//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)
{
	float PositionPanServo = 0;
	PositionPanServo = ((float)degrees / 1.86) + 35.0;
	OCR1AH = 0x00;
	OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
void servo_2(unsigned char degrees)
{
	float PositionTiltServo = 0;
	PositionTiltServo = ((float)degrees / 1.86) + 35.0;
	OCR1BH = 0x00;
	OCR1BL = (unsigned char) PositionTiltServo;
}



//servo_free functions unlocks the servo motors from the any angle
//and make them free by giving 100% duty cycle at the PWM. This function can be used to
//reduce the power consumption of the motor if it is holding load against the gravity.

void servo_1_free (void) //makes servo 1 free rotating
{
	OCR1AH = 0x03;
	OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
	OCR1BH = 0x03;
	OCR1BL = 0xFF; //Servo 2 off
}



void moveArmDown(){
	
	unsigned int i;
	for (i = 0; i <55; i++)//servo motor 2 for the downward the arm
	{
		servo_2(i);
		_delay_ms(30);
	}
}

void grip(){
	
	unsigned int i;
	for (i = 0; i <25; i++)//servo_1 for gripping
	{
		servo_1(i);
		_delay_ms(30);
	}
}
void moveArmUp(){
	
	unsigned int i;
	for(i = 0;i<8;i++) //servo_2 for upward motion of arm
	{
		servo_2(i);
		_delay_ms(30);
	}
}

void ungrip(){
	
	unsigned int i;
	for(i = 0;i<5;i++)//servo_1 for relase the grip
	{
		servo_1(i);
		_delay_ms(30);
	}
}
void pick(){
	
	moveArmDown();
	_delay_ms(1000);
	grip();
	_delay_ms(1000);
	moveArmUp();
	_delay_ms(1000);
}
void place(){
	moveArmDown();
	_delay_ms(1000);
	ungrip();
	_delay_ms(1000);
}