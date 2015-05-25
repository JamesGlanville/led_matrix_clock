
#include "RTClib.h"
#include <Wire.h>

RTC_DS1307 RTC;

// ledmatrix.ino
int rows[]={9,10,11,12,13,14,15,16};
int dai = 8;
int dck = 7;
int lat = 6;

int hour_ten = 1;
int hour_one = 7;
int minute_ten = 30;
int minute_one = 0;
int second_ten = 0;
int second_one = 0;

uint8_t numbers[]={	14,17,19,21,25,17,14,0, //0
					14,4,4,4,4,6,4,0, //1
					31,2,4,8,16,17,14,0, //2
					14,17,16,8,4,8,31,0, //3
					8,8,31,9,10,12,8,0, //4
					14,17,16,16,15,1,31,0, //5
					14,17,17,15,1,2,12,0, //6
					4,4,4,8,16,17,31,0, //7
					14,17,17,14,17,17,14,0, //8
					6,8,16,30,17,17,14,0};//9

void tick()
{
	second_one++;
	if (second_one==10)
	{
		second_one=0;
		second_ten++;
	}
	if (second_ten==6)
	{
		second_ten=0;
		minute_one++;
	}
	if (minute_one==10)
	{
		minute_one=0;
		minute_ten++;
	}
	if (minute_ten==6)
	{
		minute_ten=0;
		hour_one++;
	}
	if (hour_one==10)
	{
		hour_one=0;
		hour_ten++;
	}
	if (hour_ten==2 && hour_one==4)
	{
		hour_one=0;
		hour_ten=0;
	}

}

void ticky()
{
	Serial.print("tick\n");
}

//#define SETTIME

void setup() {
	Wire.begin();
	RTC.begin();
#ifdef SETTIME
	DateTime now = DateTime(__DATE__, __TIME__);
	DateTime future(now.unixtime()+34);
	RTC.adjust(future);
	RTC.setSqwOutLevel(HIGH);
	RTC.setSqwOutSignal();
#endif //SETTIME


    DateTime nowh = RTC.now();
    second_one=nowh.second()%10;
    second_ten=nowh.second()/10;
    minute_one=nowh.minute()%10;
    minute_ten=nowh.minute()/10;
    hour_one=nowh.hour()%10;
    hour_ten=nowh.hour()/10;

	//Serial.begin(115200);
	attachInterrupt(0, tick, RISING);
	//Timer3.initialize(1000000);
	//Timer3.attachInterrupt(tick);
	for(int i=0;i<8;i++)
	{
		digitalWrite(rows[i], HIGH);
		pinMode(rows[i],OUTPUT);
		digitalWrite(rows[i], HIGH);
	}
	pinMode(dai, OUTPUT);
	pinMode(dck, OUTPUT);
	pinMode(lat, OUTPUT);
}

void loop() {
	for (int i=0;i<8;i++)
	{
		display(i);
		//delay(10);
	}
}

void display(int row)
{

	shift_blank();

	shift5_out(numbers[second_one*8+row]);
	shift_blank();
	shift5_out(numbers[second_ten*8+row]);

	shift_blank();
	if (row==2 || row==4){shift_spot();}else{shift_blank();}
	shift_blank();

	shift5_out(numbers[minute_one*8+row]);
	shift_blank();
	shift5_out(numbers[minute_ten*8+row]);
	shift_blank();
	if (row==2 || row==4){shift_spot();}else{shift_blank();}
	shift_blank();
	shift5_out(numbers[hour_one*8+row]);
	shift_blank();
	shift5_out(numbers[hour_ten*8+row]);
//	digitalWrite(dck, HIGH);digitalWrite(dck, LOW);
//	digitalWrite(dck, HIGH);digitalWrite(dck, LOW);




	digitalWrite(dai, LOW);
//	digitalWrite(dck, HIGH);
//	digitalWrite(dck, LOW);
	digitalWrite(lat, HIGH);
	if(row>100 && row <6){
	digitalWrite(rows[row], LOW);}
	else{digitalWrite(rows[row], HIGH);delayMicroseconds(20);}
	digitalWrite(lat, LOW);
	delayMicroseconds(500);
//	delay(row>0 && row <6);
//	delay(2);
	if(row>100 && row <6){
	digitalWrite(rows[row], HIGH);}
	else{digitalWrite(rows[row], LOW);}
}

inline void shift_out(unsigned char myDataOut) {
  int i;
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //clear everything out just in case to
  //prepare shift register for bit shifting

  //data a0, clock a1
 // PORTA&=B11111100; //data=0,clock=0
 //	digitalWrite(dai, LOW);
 	digitalWrite(dck, LOW);

    //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
 //   PORTA&=B11111101; //clock=0
 	digitalWrite(dck, LOW);
    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
//      PORTA|=B00000001; //data=1
 	digitalWrite(dai, HIGH);
    }
    else {	
//      PORTA&=B11111110; //data=0
 	digitalWrite(dai, LOW);

    }

  //  PORTA|=B00000010; //clock=1
   	digitalWrite(dck, HIGH);
 //  	digitalWrite(dai, LOW);
//    PORTA&=11111110; //data=0
  }

  //stop shifting
 // PORTA&=B11111101; //clock=0
 digitalWrite(dck, HIGH);
 digitalWrite(dck, LOW);
}

inline void shift5_out(unsigned char myDataOut) {
  int i;
 	digitalWrite(dck, LOW);
  for (i=4; i>=0; i--)  {
 	digitalWrite(dck, LOW);
    if ( myDataOut & (1<<i) ) {
 	digitalWrite(dai, HIGH);
    }
    else {	
 	digitalWrite(dai, LOW);
    }
   	digitalWrite(dck, HIGH);
  }
 digitalWrite(dck, HIGH);
 digitalWrite(dck, LOW);
}

inline void shift_blank()
{
	digitalWrite(dai, LOW);
   	digitalWrite(dck, HIGH);digitalWrite(dck, LOW);

}

inline void shift_spot()
{
	digitalWrite(dai, HIGH);
   	digitalWrite(dck, HIGH);digitalWrite(dck, LOW);

}
