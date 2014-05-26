#ifndef BROKER_H
#define BROKER_H

#define MBROKER_BASIC_MESSAGE_QUEUE_SIZE 10
#define MBROKER_MOUDLE_ENDPOINTS 4

//Returns for publishToBroker
#define MBROKER_PUBLISH_SUCCESS 0x00
#define MBROKER_PUBLISH_FAILED 0x01
#define MBROKER_PUBLISH_QUEUE_FULL 0x02

//Module Types
#define MODULE_TYPE_0 EventHandler*
#define MODULE_TYPE_1 DistanceSensor*
#define MODULE_TYPE_2 MotorHandler*

typedef struct BasicMessage {
	uint_fast8_t type;
	uint_fast8_t target;
	void* payload;
} BasicMessage;

typedef struct ModuleEndpoint {
	uint_fast8_t endpoint;
	uint_fast8_t moduleType;
	void* module;
} ModuleEndpoint;

typedef struct MBroker {
	BasicMessage* Queue[MBROKER_BASIC_MESSAGE_QUEUE_SIZE];
	ModuleEndpoint* Endpoints[MBROKER_MOUDLE_ENDPOINTS];
	BasicMessage* message;
} MBroker;

void initMessageBroker(MBroker* broker);
void processMessageBroker(MBroker* broker);
uint_fast8_t registerModuleEndpoint(MBroker* broker, uint_fast8_t endpoint, uint_fast8_t type, void* module);
uint_fast8_t publishToBroker(MBroker* broker, BasicMessage* message);
uint_fast8_t appendMessageToQueue(MBroker* broker, BasicMessage* message);
BasicMessage* fetchMessageFromQueue(MBroker* broker);
void deliverBrokerMessageToModule(MBroker* broker, BasicMessage* message);
ModuleEndpoint* getEndpointRegistration(MBroker* broker, uint_fast8_t endpoint);
void deleteMessageFromQueue(MBroker* broker, BasicMessage* message);
void freeBasicMessage(BasicMessage* message);
void fixMessageBrokerQueue(MBroker* broker);
void cleanMessageBrokerQueue(MBroker* broker);
void cleanMessageBrokerEndpoints(MBroker* broker);
#endif
