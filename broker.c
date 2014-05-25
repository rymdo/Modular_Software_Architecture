#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sensor.h"
#include "broker.h"
#include "event.h"

void initMessageBroker(MBroker* broker) {
	broker->message = 0;
	cleanMessageBrokerQueue(broker);
	cleanMessageBrokerEndpoints(broker);
}

void processMessageBroker(MBroker* broker) {
	broker->message = fetchMessageFromQueue(broker);
	if(broker->message != 0) {
		deliverBrokerMessageToModule(broker, broker->message);
		broker->message = 0;
	}
}

uint_fast8_t registerModuleEndpoint(MBroker* broker, uint_fast8_t endpoint, uint_fast8_t type, void* module) {
	uint_fast16_t i;

	ModuleEndpoint* NewEndpoint = calloc(1,sizeof(ModuleEndpoint));

	for(i = 0; i < MBROKER_MOUDLE_ENDPOINTS; i++) {
		if(broker->Endpoints[i] == 0) {
			NewEndpoint->endpoint;
			NewEndpoint->moduleType = type;
			NewEndpoint->module = module;
			broker->Endpoints[i] = NewEndpoint;
			return 0;
		}
	}
	return 1;
}

uint_fast8_t publishToBroker(MBroker* broker, BasicMessage* message) {
	uint_fast8_t ret = appendMessageToQueue(broker, message);
	if(ret == MBROKER_PUBLISH_SUCCESS) {
		return MBROKER_PUBLISH_SUCCESS;
	}
	return MBROKER_PUBLISH_FAILED | ret;
}

uint_fast8_t appendMessageToQueue(MBroker* broker, BasicMessage* message) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_BASIC_MESSAGE_QUEUE_SIZE; i++) {
		if(broker->Queue[i] == 0) {
			broker->Queue[i] = message;
			return MBROKER_PUBLISH_SUCCESS;
		}
	}
	return MBROKER_PUBLISH_QUEUE_FULL;
}

BasicMessage* fetchMessageFromQueue(MBroker* broker) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_BASIC_MESSAGE_QUEUE_SIZE; i++) {
		if(broker->Queue[i] != 0) {
			return broker->Queue[i];
		}
	}
	return 0;
}

void deliverBrokerMessageToModule(MBroker* broker, BasicMessage* message) {
	ModuleEndpoint* endpoint = getEndpointRegistration(broker, message->target);
	if(endpoint != 0) {
		if(endpoint->moduleType == ENDPOINT_EVENT) {
			incomingBasicMessagEventHandler((MODULE_TYPE_0)endpoint->module, message);
		}
	}
	deleteMessageFromQueue(broker, message);
}

ModuleEndpoint* getEndpointRegistration(MBroker* broker, uint_fast8_t endpoint) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_MOUDLE_ENDPOINTS; i++) {
		if(broker->Endpoints[i]->endpoint == endpoint) {
			return broker->Endpoints[i];
		}
	}
	return 0;
}


void deleteMessageFromQueue(MBroker* broker, BasicMessage* message) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_BASIC_MESSAGE_QUEUE_SIZE; i++) {
		if(broker->Queue[i] == message) {
			//freeBasicMessage(message);
			broker->Queue[i] = 0;
			fixMessageBrokerQueue(broker);
			return;
		}
	}
}

void freeBasicMessage(BasicMessage* message) {
	free(message->payload);
	free(message);
}

void fixMessageBrokerQueue(MBroker* broker) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_BASIC_MESSAGE_QUEUE_SIZE-1; i++) {
		if(broker->Queue[i] == 0) {
			if(broker->Queue[i+1] != 0) {
				broker->Queue[i] = broker->Queue[i+1];
				broker->Queue[i+1] = 0;
			}
		}
	}
}

void cleanMessageBrokerQueue(MBroker* broker) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_BASIC_MESSAGE_QUEUE_SIZE; i++) {
		broker->Queue[i] = 0;
	}
}

void cleanMessageBrokerEndpoints(MBroker* broker) {
	uint_fast16_t i;
	for(i = 0; i < MBROKER_MOUDLE_ENDPOINTS; i++) {
		broker->Endpoints[i] = 0;
	}
}
