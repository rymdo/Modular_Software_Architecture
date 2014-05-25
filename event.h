#ifndef EVENT_H
#define EVENT_H

#define ENDPOINT_EVENT 0

#define EVENTHANDLER_INCOMING_MESSAGES_SIZE 5

typedef struct EventHandler {
	MBroker* broker;
	BasicMessage* incoming_messages[EVENTHANDLER_INCOMING_MESSAGES_SIZE];
} EventHandler;

void initEventHandler(EventHandler* event);
void processEventHandler(EventHandler* event);

void setEventHandlerMessageBroker(EventHandler* event, MBroker* broker);

//Receive function
void incomingBasicMessagEventHandler(EventHandler* event, BasicMessage* message);
void parseIncomingMessagesEventHandler(EventHandler* event, BasicMessage* message);

void cleanEventHandlerQueue(EventHandler* event);

#endif
