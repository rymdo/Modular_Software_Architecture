#ifndef EVENT_H
#define EVENT_H

#define ENDPOINT_EVENT 0

#define EVENTHANDLER_INCOMING_MESSAGES_SIZE 5

typedef struct EventHandler {
	MBroker* broker;
	BasicMessage* incoming_messages[EVENTHANDLER_INCOMING_MESSAGES_SIZE];
	BasicMessage* message;
} EventHandler;

void initEventHandler(EventHandler* event);
void processEventHandler(EventHandler* event);

void setEventHandlerMessageBroker(EventHandler* event, MBroker* broker);

//Receive function
void incomingBasicMessagEventHandler(EventHandler* event, BasicMessage* message);
void parseIncomingMessageEventHandler(EventHandler* event, BasicMessage* message);
void appendIncomingBasicMessageEventHandler(EventHandler* event, BasicMessage* message);
BasicMessage* fetchBasicMessageEventHandler(EventHandler* event);
void clearMessageFromEventHandlerIncomingQueue(EventHandler* event, BasicMessage* message);
void fixEventHandlerIncomingQueue(EventHandler* event);
void cleanEventHandlerQueue(EventHandler* event);

#endif
