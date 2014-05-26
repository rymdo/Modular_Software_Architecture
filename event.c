#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sensor.h"
#include "broker.h"
#include "event.h"
#include "motor.h"

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

	SensorDistanceReadingPayload* reading = 0;

	if(message->type == PAYLOAD_DISTANCEREADING) {
		reading = (SensorDistanceReadingPayload*)message->payload;

		uint_fast8_t state;
		if(reading->distance >= 10.0) {
			state = 2;
		} else {
			state = 1;
		}

		if(event->broker != 0) {
			BasicMessage* motor_message = calloc(1,sizeof(BasicMessage));
			MotorMovementCommand* motor_payload = calloc(1,sizeof(MotorMovementCommand));
			if(motor_message != 0 && motor_payload != 0) {
				motor_payload->state = state;
				motor_message->target = ENDPOINT_MOTOR;
				motor_message->type = PAYLOAD_MOTORCOMMAND;
				motor_message->payload = motor_payload;
				uint_fast8_t ret = publishToBroker(event->broker, motor_message);
				if(ret == MBROKER_PUBLISH_SUCCESS) {
					//Successful publish!
				} else {
					//If publish fail, free the message from memory
					freeBasicMessage(motor_message);
				}
			}
		}
	}
	clearMessageFromEventHandlerIncomingQueue(event,message);
	freeBasicMessage(message);
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

void clearMessageFromEventHandlerIncomingQueue(EventHandler* event, BasicMessage* message) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		if(event->incoming_messages[i] == message) {
			//freeBasicMessage(message);
			event->incoming_messages[i] = 0;
			fixEventHandlerIncomingQueue(event);
			return;
		}
	}
}

void fixEventHandlerIncomingQueue(EventHandler* event) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE-1; i++) {
		if(event->incoming_messages[i] == 0) {
			if(event->incoming_messages[i+1] != 0) {
				event->incoming_messages[i] = event->incoming_messages[i+1];
				event->incoming_messages[i+1] = 0;
			}
		}
	}
}

void cleanEventHandlerQueue(EventHandler* event) {
	uint_fast16_t i;
	for(i = 0; i < EVENTHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		event->incoming_messages[i] = 0;
	}
}

