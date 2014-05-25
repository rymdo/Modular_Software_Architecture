#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "broker.h"
#include "sensor.h"
#include "event.h"

MBroker 		messageBroker;
DistanceSensor 	sensorFront;
EventHandler 	eventHandler;

int main(void) {
	initDistaceSensor(&sensorFront);
	initMessageBroker(&messageBroker);
	initEventHandler(&eventHandler);

	setDistanceSensorMessageBroker(&sensorFront, &messageBroker);
	setEventHandlerMessageBroker(&eventHandler,&messageBroker);

	registerModuleEndpoint(&messageBroker, ENDPOINT_EVENT, 0, &eventHandler);
	registerModuleEndpoint(&messageBroker, ENDPOINT_DISTANCESENSOR, 1, &sensorFront);

    while(1) {
    	processMessageBroker(&messageBroker);
    	processEventHandler(&eventHandler);
    	processDistanceSensor(&sensorFront);
    }
}
