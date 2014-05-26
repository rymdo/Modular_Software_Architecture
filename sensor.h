#ifndef SENSOR_H
#define SENSOR_H

#define ENDPOINT_DISTANCESENSOR 1

#define PAYLOAD_DISTANCEREADING 10

#include "broker.h"

#define SENSOR_INCOMING_MESSAGES_SIZE 5

typedef struct SensorDistanceReadingPayload {
	float distance;
} SensorDistanceReadingPayload;


typedef struct DistanceSensor {
	float latest_reading;
	//uint_fast8_t (*MB_Publish)(BasicMessage*);
	MBroker* broker;
	BasicMessage* incoming_messages[SENSOR_INCOMING_MESSAGES_SIZE];

} DistanceSensor;

void initDistaceSensor(DistanceSensor* sensor);
void processDistanceSensor(DistanceSensor* sensor);
void setDistanceSensorMessageBroker(DistanceSensor* sensor, MBroker* broker);

void sendLatestReadingToEventHandler(DistanceSensor* sensor);

//Receive function
void incomingBasicMessageDistanceSensor(DistanceSensor* sensor, BasicMessage* message);
void parseIncomingMessagesDistanceSensor(DistanceSensor* sensor, BasicMessage* message);

void cleanDistanceSensorIncomingMessages(DistanceSensor* sensor);

/*
void setDistanceSensorPublishFunction(uint8_t (*PublishFunc)(BasicMessage*));
 getDistanceSensorReceiveFunction(void);
*/
#endif
