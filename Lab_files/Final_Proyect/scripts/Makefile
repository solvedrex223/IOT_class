build:
	gcc test.c temp.c -g -lwiringPi -lpthread -lmqttc

install:
	mosquitto -d -c mosquitto.conf
	cd ../MQTT-C/bin && cmake .. && make install
