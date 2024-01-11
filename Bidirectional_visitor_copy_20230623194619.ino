#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
connect Vcc to Vin
GND to GND
SDA to D2
SCL to D1
*/
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define MAX_LIMIT  40   //Maximum number of persons allowed in the hall or room

#define in 14    //IR Sensor 1
#define out 13   //IR sensor 2
#define buz 12   //Buzzer pin

int count=0, pos=0;

//Function to Display the count on LCD

void displayCount(int num){
	char str[6];        //To hold count to be displayed
    	int i, rem, len = 0, n;

 	/*Convert integer (count) to string for displaying on LCD*/
    	n = num;
    	while (n != 0){
        	len++;
        	n /= 10;
    	}
    	for (i = 0; i < len; i++){
        	rem = num % 10;
        	num = num / 10;
        	str[len - (i + 1)] = rem + '0';
    	}
      	str[len] = '\0';	//Put NULL character to end the string

	/*If count in room or hall is less than maximum limit of the hall or room, 
	  then display the message on LCD to print Number of persons in room */
      	if(count < MAX_LIMIT){
     		lcd.clear();
      		lcd.setCursor(0,0);
      		lcd.print("No. of Persons");
      		lcd.setCursor(0,1);
      		lcd.print("in Room: ");
      	}

    	/*If number of persons in room is equal to MAX_LIMIT of the room,
          then display the message as Room Full */
    	if(count==4){
      		lcd.clear();
      		lcd.setCursor(0,0);
      		lcd.print("Room Full ");
      		lcd.setCursor(0,1);
      		lcd.print("No.of Persons:");
      	}

	/*If number of persons in room is greater than MAX_LIMIT,
          then display the message as Over Crowded Room and also turn on Buzzer*/
     	if(count>4){
        	digitalWrite(buz, LOW);	//Buzzer made ON
	        lcd.clear();
      		lcd.setCursor(0,0);
	      	lcd.print("Over CrowdedRoom");
      		lcd.setCursor(0,1);
	      	lcd.print("No.of Persons:");
     	}
      	else{	//If count is less than or equal to MAX_LIMIT, the turnoff buzzer
           	digitalWrite(buz, HIGH);
      	}

	//Display the count on LCD
    	lcd.print(str);
}

//Setup function for initial setup
void setup(){
	//IR sensor pins are made as input pins
  	pinMode(in, INPUT);	
  	pinMode(out, INPUT);

	//Buzzer pin is made as output pin
  	pinMode(buz, OUTPUT);

	// Buzzer made ON for small duration to indicate start of the counting
  	digitalWrite(buz,LOW);
  	delay(1000);
  	digitalWrite(buz,HIGH);
  	delay(50);

	//Initial Message on LCD
  	lcd.init();
      lcd.backlight();
  delay(200);
  	lcd.print("Visitor Counter");
  	delay(100);
   	lcd.setCursor(0,1);
  	lcd.print("EC DEPT BLDEACET");
  	delay(100);
}

//Loop function 
void loop(){  
 
  	if((digitalRead(in))==0){
    	delay(20);
    	while((digitalRead(in))==0);
    	/*
    	Arrangement or placing of sensors: while entering the room from outside, sensor1 will be 
    	encounterd first and sensor2 will be next.
    	pos will tell the position of a person, entering/leaving the room

    	If pos=0, default value; No person is entering/leaving the room/hall
    	If pos=1, person is entering the room and crossed sensor1 (in)
    	If pos=2, person has entered the room after crossing both the sensors
    	If pos=3, person is going out of the room and crossed the sensor2 (out)
    	If pos=4, person has gone out of the room after crossing both the sensors
    	*/
    	if(pos==0)	
       		pos=1;
       	else if(pos==3)
       		pos=4;
  	}
  	if(pos==4 && count!=0){
    		count--;		//person has left the room, decrement the count
    		displayCount(count);
    		pos=0;
  	}
   	if((digitalRead(out))==0){
      		delay(20);
      		while((digitalRead(out))==0);
       		if(pos==1)
       			pos=2;
       		else if(pos==0)
       			pos=3;
    	}
   
    	if(pos==2){
      		count++;		//person has entered the room, increment the count
      		displayCount(count);
      		pos=0;
    	}
    	else if(pos==4 && count!=0){
      		count--;		//person has left the room, decrement the count
      		displayCount(count);
      		pos=0;
    	}
    	delay(50);
}

