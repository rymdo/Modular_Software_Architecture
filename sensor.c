#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sensor.h"
#include "broker.h"
#include "event.h"

void initDistaceSensor(DistanceSensor* sensor) {
	sensor->latest_reading = 153.54;
	sensor->broker = 0;
	cleanDistanceSensorIncomingMessages(sensor);
}

void processDistanceSensor(DistanceSensor* sensor) {
	sendLatestReadingToEventHandler(sensor);
	sensor->latest_reading += 1.1;
}

void setDistanceSensorMessageBroker(DistanceSensor* sensor, MBroker* broker) {
	sensor->broker = broker;
}

void sendLatestReadingToEventHandler(DistanceSensor* sensor) {
	if(sensor->broker != 0) {
		BasicMessage* message = calloc(1,sizeof(BasicMessage));
		SensorDistanceReadingPayload* sensor_payload = calloc(1,sizeof(SensorDistanceReadingPayload));
		if(message != 0 && sensor_payload != 0) {
			sensor_payload->distance = sensor->latest_reading;
			message->target = ENDPOINT_EVENT;
			message->type = PAYLOAD_DISTANCEREADING;
			message->payload = sensor_payload;
			uint_fast8_t ret = publishToBroker(sensor->broker, message);
			if(ret == MBROKER_PUBLISH_SUCCESS) {
				//Successful publish!
			} else {
				//Something went wrong!
			}
		}
	}
}

void incomingBasicMessageDistanceSensor(DistanceSensor* sensor, BasicMessage* message) {

}
void parseIncomingMessagesDistanceSensor(DistanceSensor* sensor, BasicMessage* message) {

}


void cleanDistanceSensorIncomingMessages(DistanceSensor* sensor) {
	uint_fast16_t i;
	for(i = 0; i < SENSOR_INCOMING_MESSAGES_SIZE; i++) {
		sensor->incoming_messages[i].type = 0;
		sensor->incoming_messages[i].target = 0;
		sensor->incoming_messages[i].payload = 0;
	}
}
