#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <pthread.h>
#include <softPwm.h>
#include <mqtt.h>
#include <string.h>
#include <sys/socket.h>
#include "../MQTT-C/examples/templates/posix_sockets.h"
#include "temp.h"

#define PIN_AIRE 13
#define PIN_SWITCH 15
#define PIN_TEMP 18
#define PIN_SERVO 16

struct mqtt_client client;
int sockfd;
uint8_t sendbuf[2048];
uint8_t recvbuf[1024];
enum MQTTErrors errors;
pthread_t thread [2];

void *publish_temp(void *vargp)
{
    while (1)
    {
        char *temp = read_temp();
        errors = mqtt_publish(&client, "temperatura", temp, strlen(temp), MQTT_PUBLISH_QOS_1);
        printf("%s\n", mqtt_error_str(errors));
        if (client.error != MQTT_OK)
        {
            fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
            break;
        }
        delay(1000);
    }
}

void *control_servo(){
    pinMode(PIN_SERVO,PWM_OUTPUT);
    digitalWrite(PIN_SERVO,1);
    softPwmCreate(PIN_SERVO,0,200);
    softPwmWrite(PIN_SERVO,13);
    while (1){
        if (digitalRead(PIN_SWITCH) == 1){
            softPwmWrite(PIN_SERVO,4);
            delay(1000);
            softPwmWrite(PIN_SERVO,13);
            delay(1000);
        }
        delay(10);
    }
}

void publish_callback(void **unused, struct mqtt_response_publish *published)
{
}

void exit_example(int status, int sockfd, pthread_t *client_daemon)
{
    if (sockfd != -1)
        close(sockfd);
    if (client_daemon != NULL)
    pthread_cancel(*client_daemon);
    exit(status);
}

void init_mqtt_client()
{

    const char *addr = "localhost";
    const char *port = "1884";
    sockfd = open_nb_socket(addr, port);
    if (sockfd == -1)
    {
        perror("Failed to open socket: ");
        exit_example(EXIT_FAILURE, sockfd, NULL);
    }
    mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);
    mqtt_connect(&client, "publishing_client", NULL, NULL, 0, NULL, NULL, 0, 400);
    if (client.error != MQTT_OK)
    {
        fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
        exit_example(EXIT_FAILURE, sockfd, NULL);
    }
}

int main(int argc, char const *argv[])
{
    wiringPiSetupPhys();
    pinMode(PIN_AIRE, INPUT);
    pullUpDnControl(PIN_AIRE, PUD_UP);
    pinMode(PIN_SWITCH, INPUT);
    pullUpDnControl(PIN_SWITCH, PUD_DOWN);
    init_mqtt_client();
    pthread_create(&thread[0],NULL,publish_temp,NULL);
    pthread_create(&thread[1],NULL,control_servo,NULL);
    int x[2] = {0, 0};
    int y[2];
    printf("%i\n", digitalRead(PIN_AIRE));
    do
    {
        y[0] = digitalRead(PIN_AIRE);
        y[1] = digitalRead(PIN_SWITCH);
        char *topic_aire = "air";
        char *state;

        if (x[0] != y[0])
        {
            if (y[0] == 1)
            {
                state = "up";
            }
            else
            {
                state = "down";
            }
            errors = mqtt_publish(&client, topic_aire, state, strlen(state), MQTT_PUBLISH_QOS_1);
            printf("%s\n", mqtt_error_str(errors));
            if (client.error != MQTT_OK)
            {
                fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
                exit_example(EXIT_FAILURE, sockfd, NULL);
            }
            x[0] = y[0];
        }
        if (x[1] != y[1])
        {
            if (y[1] == 1)
            {
                state = "up";
            }
            else
            {
                state = "down";
            }
            errors = mqtt_publish(&client, "limpiaparabrisas", state, strlen(state), MQTT_PUBLISH_QOS_1);
            printf("%s\n", mqtt_error_str(errors));
            if (client.error != MQTT_OK)
            {
                fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
                exit_example(EXIT_FAILURE, sockfd, NULL);
            }
            x[1] = y[1];
        }
        mqtt_sync(&client);
        delay(10);
    } while (1);
    return 0;
}
