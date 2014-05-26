#ifndef MOTOR_H
#define MOTOR_H

#define ENDPOINT_MOTOR 2
#define PAYLOAD_MOTORCOMMAND 20
#define MOTORHANDLER_INCOMING_MESSAGES_SIZE 5

typedef struct MotorMovementCommand {
	uint_fast8_t state;
} MotorMovementCommand;

typedef struct MotorHandler {
	MBroker* broker;
	BasicMessage* incoming_messages[MOTORHANDLER_INCOMING_MESSAGES_SIZE];
	BasicMessage* message;
} MotorHandler;

void initMotorHandler(MotorHandler* motor);
void processMotorHandler(MotorHandler* motor);

void setMotorHandlerMessageBroker(MotorHandler* motor, MBroker* broker);

//Receive function
void incomingBasicMessagMotorHandler(MotorHandler* motor, BasicMessage* message);
void parseIncomingMessageMotorHandler(MotorHandler* motor, BasicMessage* message);
void appendIncomingBasicMessageMotorHandler(MotorHandler* motor, BasicMessage* message);
BasicMessage* fetchBasicMessageMotorHandler(MotorHandler* motor);
void cleanMotorHandlerQueue(MotorHandler* motor);

#endif
