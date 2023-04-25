#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <softPwm.h>

int main(int argc, char const *argv[])
{
    wiringPiSetupPhys();
    pinMode(3,PWM_OUTPUT);
    digitalWrite(3,1);
    softPwmCreate(3,0,200);
    softPwmWrite(3,13);
    delay(2000);
    for(;;){
         softPwmWrite(3,4);
         delay(1000);
         softPwmWrite(3,13);
         delay(1000);
    }
    return 0;
}

