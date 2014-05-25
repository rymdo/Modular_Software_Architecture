#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sensor.h"
#include "broker.h"
#include "event.h"

void initEventHandler(EventHandler* event) {
	event->broker = 0;
	cleanEventHandlerQueue(event);
}

void processEventHandler(EventHandler* event) {

}

void setEventHandlerMessageBroker(EventHandler* event, MBroker* broker)  {
	event->broker = broker;
}

void incomingBasicMessagEventHandler(EventHandler* event, BasicMessage* message) {
	SensorDistanceReadingPayload* reading = 0;
	float dist = 0;
	if(message->type == PAYLOAD_DISTANCEREADING) {
		reading = (SensorDistanceReadingPayload*)message->payload;
		dist = reading->distance;
	}
	freeBasicMessage(message);
}
void parseIncomingMessagesEventHandler(EventHandler* event, BasicMessage* message) {

}

void cleanEventHandlerQueue(EventHandler* event) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		event->incoming_messages[i] = 0;
	}
}
