#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS	85
#define PIN_TEMP		18

int dht11_dat[5] = { 0, 0, 0, 0, 0 };
 
char* read_temp()
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 
	
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
    wiringPiSetupPhys();
	pinMode(PIN_TEMP, OUTPUT);
    digitalWrite(PIN_TEMP, LOW);
    delay(18);
    digitalWrite(PIN_TEMP, HIGH);
    delayMicroseconds(40);
    pinMode(PIN_TEMP, INPUT);
 
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( PIN_TEMP ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( PIN_TEMP );
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) )
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
	char* res;
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		sprintf(res,"%d.%d",dht11_dat[2],dht11_dat[3]);
		 return res;
	}
	else
	{
		return "0";
	}
	
}
