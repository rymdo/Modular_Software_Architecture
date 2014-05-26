#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "broker.h"
#include "event.h"
#include "motor.h"

void initMotorHandler(MotorHandler* motor) {
	motor->broker = 0;
	cleanMotorHandlerQueue(motor);
}
void processMotorHandler(MotorHandler* motor) {
	motor->message = fetchBasicMessageMotorHandler(motor);
	if(motor->message != 0) {
		parseIncomingMessageMotorHandler(motor, motor->message);
		motor->message = 0;
	}
}

void setMotorHandlerMessageBroker(MotorHandler* motor, MBroker* broker) {
	motor->broker = broker;
}

//Receive function
void incomingBasicMessagMotorHandler(MotorHandler* motor, BasicMessage* message) {
	appendIncomingBasicMessageMotorHandler(motor, message);
}

void parseIncomingMessageMotorHandler(MotorHandler* motor, BasicMessage* message) {
	freeBasicMessage(message);
	cleanMotorHandlerQueue(motor);
}

void appendIncomingBasicMessageMotorHandler(MotorHandler* motor, BasicMessage* message) {
	uint_fast16_t i;
	for(i = 0; i < MOTORHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		if(motor->incoming_messages[i] == 0) {
			motor->incoming_messages[i] = message;
			return;
		}
	}
	//If incoming buffer is full, free the message
	freeBasicMessage(message);
}
BasicMessage* fetchBasicMessageMotorHandler(MotorHandler* motor) {
	uint_fast16_t i;
	for(i = 0; i < MOTORHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		if(motor->incoming_messages[i] != 0) {
			return motor->incoming_messages[i];
		}
	}
	return 0;
}
void cleanMotorHandlerQueue(MotorHandler* motor) {
	uint_fast16_t i;
	for(i = 0; i < MOTORHANDLER_INCOMING_MESSAGES_SIZE; i++) {
		motor->incoming_messages[i] = 0;
	}
}
