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
	event->message = 0;
	cleanEventHandlerQueue(event);
}

void processEventHandler(EventHandler* event) {
	event->message = fetchBasicMessageEventHandler(event);
	if(event->message != 0) {
		parseIncomingMessageEventHandler(event, event->message);
		event->message = 0;
	}
}

void setEventHandlerMessageBroker(EventHandler* event, MBroker* broker)  {
	event->broker = broker;
}

void incomingBasicMessagEventHandler(EventHandler* event, BasicMessage* message) {
	appendIncomingBasicMessageEventHandler(event, message);
}
void parseIncomingMessageEventHandler(EventHandler* event, BasicMessage* message) {
	/*
	SensorDistanceReadingPayload* reading = 0;
	float dist = 0;
	if(message->type == PAYLOAD_DISTANCEREADING) {
		reading = (SensorDistanceReadingPayload*)message->payload;
		dist = reading->distance;
	}
	freeBasicMessage(message);
	*/
}

void appendIncomingBasicMessageEventHandler(EventHandler* event, BasicMessage* message) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		if(event->incoming_messages[i] == 0) {
			event->incoming_messages[i] = message;
			return;
		}
	}
	//If incoming buffer is full, free the message
	freeBasicMessage(message);
}

BasicMessage* fetchBasicMessageEventHandler(EventHandler* event) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		if(event->incoming_messages[i] != 0) {
			return event->incoming_messages[i];
		}
	}
	return 0;
}

void cleanEventHandlerQueue(EventHandler* event) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		event->incoming_messages[i] = 0;
	}
}
