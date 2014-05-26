#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "broker.h"
#include "sensor.h"
#include "event.h"
#include "motor.h"

MBroker 		messageBroker;
DistanceSensor 	sensorFront;
EventHandler 	eventHandler;
MotorHandler 	motorHandler;

int main(void) {
	initDistaceSensor(&sensorFront);
	initMessageBroker(&messageBroker);
	initEventHandler(&eventHandler);
	initMotorHandler(&motorHandler);

	setDistanceSensorMessageBroker(&sensorFront, &messageBroker);
	setEventHandlerMessageBroker(&eventHandler,&messageBroker);
	setMotorHandlerMessageBroker(&motorHandler, &messageBroker);

	registerModuleEndpoint(&messageBroker, ENDPOINT_EVENT, 0, &eventHandler);
	registerModuleEndpoint(&messageBroker, ENDPOINT_DISTANCESENSOR, 1, &sensorFront);
	registerModuleEndpoint(&messageBroker, ENDPOINT_MOTOR, 2, &motorHandler);

    while(1) {
    	processMessageBroker(&messageBroker);
    	processEventHandler(&eventHandler);
    	processDistanceSensor(&sensorFront);
    	processMotorHandler(&motorHandler);
    }
}
