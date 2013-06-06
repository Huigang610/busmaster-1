/**
 * @brief Signal Interaction Layer
 * @author Tobias Lorenz
 *
 * This files contains Signal Interaction implementation for the
 * required messages and signals.
 */

/* Project includes */
#include "stdafx.h"
#include "Unions_for_COM_Interface.h"
#include "BUSMASTER_Interface.h"

/* COM interface reference */
IApplication * m_IApplication;

/* Message variables */
CAN_MSGS message_BodyComputer01;
CAN_MSGS message_BodyComputer02;
CAN_MSGS message_BreakingSystem01;
CAN_MSGS message_EngineControl01;
CAN_MSGS message_EngineControl03;
CAN_MSGS message_GearboxControl01;

/* Raw signal variables */
BodyComputer01_   signals_BodyComputer01;
BodyComputer02_   signals_BodyComputer02;
BreakingSystem01_ signals_BreakingSystem01;
EngineControl01_  signals_EngineControl01;
EngineControl03_  signals_EngineControl03;
GearboxControl01_ signals_GearboxControl01;

/* Physical signal variables */
int          signal_Charge;
unsigned int signal_Clamp_15;
unsigned int signal_GearSelected;
unsigned int signal_Lamp_BlinkerLeft;
unsigned int signal_Lamp_BlinkerRight;
unsigned int signal_MotorSpeed;
int          signal_Temp;
unsigned int signal_VehicleMode;
int          signal_VehicleSpeed;
short int	 signal_DayLight;
short int	 signal_highBeam;
short int	 signal_esp;
short int	 signal_abs;
short int	 signal_brake;

/* Environmental variables */
unsigned int variable_Gear; // 0..6

/* Constants */
unsigned int const_GearRatio[7] = {0, 9000/70, 9000/140, 9000/210, 9000/280, 9000/350, 9000/420};

/**
 * This function initializes the constants in each of the messages.
 * Constants are for message ids, standard/extended message, and Data Length Code (DLC).
 */
void initialize_messages(void)
{
	/* Initialize message BodyComputer01 */
	message_BodyComputer01.m_unMsgID = 0x101;              // Message ID
	message_BodyComputer01.m_bEXTENDED = false;            // Standard Message type
	message_BodyComputer01.m_bRTR = false;                 // Not RTR type
	message_BodyComputer01.m_ucDataLen = 8;                // Length is 8 Bytes
	message_BodyComputer01.m_ucChannel = 1;                // First CAN channel

	/* Initialize message BodyComputer02 */
	message_BodyComputer02.m_unMsgID = 0x102;              // Message ID
	message_BodyComputer02.m_bEXTENDED = false;            // Standard Message type
	message_BodyComputer02.m_bRTR = false;                 // Not RTR type
	message_BodyComputer02.m_ucDataLen = 8;                // Length is 8 Bytes
	message_BodyComputer02.m_ucChannel = 1;                // First CAN channel

	/* Initialize message BreakingSystem01 */
	message_BreakingSystem01.m_unMsgID = 0x201;            // Message ID
	message_BreakingSystem01.m_bEXTENDED = false;          // Standard Message type
	message_BreakingSystem01.m_bRTR = false;               // Not RTR type
	message_BreakingSystem01.m_ucDataLen = 8;              // Length is 8 Bytes
	message_BreakingSystem01.m_ucChannel = 1;              // First CAN channel

	/* Initialize message EngineControl01 */
	message_EngineControl01.m_unMsgID = 0x301;             // Message ID
	message_EngineControl01.m_bEXTENDED = false;           // Standard Message type
	message_EngineControl01.m_bRTR = false;                // Not RTR type
	message_EngineControl01.m_ucDataLen = 8;               // Length is 8 Bytes
	message_EngineControl01.m_ucChannel = 1;               // First CAN channel

	/* Initialize message EngineControl03 */
	message_EngineControl03.m_unMsgID = 0x303;             // Message ID
	message_EngineControl03.m_bEXTENDED = false;           // Standard Message type
	message_EngineControl03.m_bRTR = false;                // Not RTR type
	message_EngineControl03.m_ucDataLen = 8;               // Length is 8 Bytes
	message_EngineControl03.m_ucChannel = 1;               // First CAN channel

	/* Initialize message GearboxControl01 */
	message_GearboxControl01.m_unMsgID = 0x401;            // Message ID
	message_GearboxControl01.m_bEXTENDED = false;          // Standard Message type
	message_GearboxControl01.m_bRTR = false;               // Not RTR type
	message_GearboxControl01.m_ucDataLen = 8;              // Length is 8 Bytes
	message_GearboxControl01.m_ucChannel = 1;              // First CAN channel
}

/**
 * This sends the BodyComputer01 message as follows:
 * - Collection of physical signals
 * - Calculation into raw signals
 * - Integration of raw signals into CAN message content.
 * - Triggering of transmission.
 */
void send_BodyComputer01_message(void)
{
	/* Calculate physical to raw signals and copy them into raw signal structure */
	signals_BodyComputer01.VehicleMode = (signal_VehicleMode - (0)) / 1;
	signals_BodyComputer01.Clamp_15 = (signal_Clamp_15 - (0)) / 1;
	signals_BodyComputer01.Lamp_BlinkerLeft = signal_Lamp_BlinkerLeft;
	signals_BodyComputer01.Lamp_BlinkerRight = signal_Lamp_BlinkerRight;
	signals_BodyComputer01.Lamp_DayLights = signal_DayLight;
	signals_BodyComputer01.Lamp_HighBeam = signal_highBeam;

	/* Copy raw signals into message */
	memcpy(message_BodyComputer01.m_ucData, signals_BodyComputer01.m_aucData, 8);

	/* Send message */
	m_IApplication->SendCANMSg(&message_BodyComputer01);
}

/**
 * This sends the BodyComputer02 message as follows:
 * - Collection of physical signals
 * - Calculation into raw signals
 * - Integration of raw signals into CAN message content.
 * - Triggering of transmission.
 */
void send_BodyComputer02_message(void)
{
	/* Calculate physical to raw signals and copy them into raw signal structure */
	// TODO

	/* Copy raw signals into emssage */
	memcpy(message_BodyComputer02.m_ucData, signals_BodyComputer02.m_aucData, 8);

	/* Send message */
	m_IApplication->SendCANMSg(&message_BodyComputer02);
}

/**
 * This sends the BreakingSystem01 message as follows:
 * - Collection of physical signals
 * - Calculation into raw signals
 * - Integration of raw signals into CAN message content.
 * - Triggering of transmission.
 */
void send_BreakingSystem01_message(void)
{
	/* Calculate physical to raw signals and copy them into raw signal structure */
	signals_BreakingSystem01.VehicleSpeed = (signal_VehicleSpeed - (-81.9)) / 0.1 + 0.5; // +0.5 for correct round up/down
	signals_BreakingSystem01.LampWarning_Esp = signal_esp;
	signals_BreakingSystem01.LampWarning_Abs = signal_abs;
	signals_BreakingSystem01.LampWarning_Brake = signal_brake;

	/* Copy raw signals into message */
	memcpy(message_BreakingSystem01.m_ucData, signals_BreakingSystem01.m_aucData, 8);

	/* Send message */
	m_IApplication->SendCANMSg(&message_BreakingSystem01);
}

/**
 * This sends the EngineControl01 message as follows:
 * - Collection of physical signals
 * - Calculation into raw signals
 * - Integration of raw signals into CAN message content.
 * - Triggering of transmission.
 */
void send_EngineControl01_message(void)
{
	/* Calculate physical to raw signals and copy them into raw signal structure */
	signals_EngineControl01.MotorSpeed = (signal_MotorSpeed - (0)) / 1;

	/* Copy raw signals into message */
	memcpy(message_EngineControl01.m_ucData, signals_EngineControl01.m_aucData, 8);

	/* Send message */
	m_IApplication->SendCANMSg(&message_EngineControl01);
}

/**
 * This sends the EngineControl03 message as follows:
 * - Collection of physical signals
 * - Calculation into raw signals
 * - Integration of raw signals into CAN message content.
 * - Triggering of transmission.
 */
void send_EngineControl03_message(void)
{
	/* Calculate physical to raw signals and copy them into raw signal structure */
	// TODO

	/* Copy raw signals into message */
	memcpy(message_EngineControl03.m_ucData, signals_EngineControl03.m_aucData, 8);

	/* Send message */
	m_IApplication->SendCANMSg(&message_EngineControl03);
}

/**
 * This sends the GearboxControl01 message as follows:
 * - Collection of physical signals
 * - Calculation into raw signals
 * - Integration of raw signals into CAN message content.
 * - Triggering of transmission.
 */
void send_GearboxControl01_message(void)
{
	/* Calculate physical to raw signals and copy them into raw signal structure */
	signals_GearboxControl01.GearSelected = (signal_GearSelected - (0)) / 1;

	/* Copy raw signals into message */
	memcpy(message_GearboxControl01.m_ucData, signals_GearboxControl01.m_aucData, 8);

	/* Send message */
	m_IApplication->SendCANMSg(&message_GearboxControl01);
}

/**
 * This sends out all messages.
 */
void send_all_messages(void)
{
	send_BodyComputer01_message();
	send_BodyComputer02_message();
	send_BreakingSystem01_message();
	send_EngineControl01_message();
	send_EngineControl03_message();
	send_GearboxControl01_message();
}

/**
 * Set all physical signals to "Vehicle Off" state and send messages accordingly.
 */
void set_vehicle_off()
{
	/* Modify physical signals */
	signal_Clamp_15 = 0;
	signal_GearSelected = 1; // P
	signal_MotorSpeed = 0;
	signal_VehicleMode = 0;
	signal_VehicleSpeed = 0;

	/* Modify environmental variables */
	variable_Gear = 1;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to "Ignition On" state and send messages accordingly.
 */
void set_vehicle_ignition_on()
{
	/* Modify physical signals */
	signal_Clamp_15 = 1;
	signal_GearSelected = 1; // P
	signal_MotorSpeed = 0;
	signal_VehicleMode = 0;
	signal_VehicleSpeed = 0;

	/* Modify environmental variables */
	variable_Gear = 1;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to "Engine Running" state and send messages accordingly.
 */
void set_vehicle_engine_running()
{
	/* Modify physical signals */
	signal_Clamp_15 = 1;
	signal_GearSelected = 1; // P
	signal_MotorSpeed = 700;
	signal_VehicleMode = 1;
	signal_VehicleSpeed = 0;

	/* Modify environmental variables */
	variable_Gear = 1;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to "Reverse Gear" state and send messages accordingly.
 */
void set_vehicle_switch_into_reverse_gear()
{
	/* Modify physical signals */
	signal_Clamp_15 = 1;
	signal_GearSelected = 2; // R
	signal_MotorSpeed = 700;
	signal_VehicleMode = 1;
	signal_VehicleSpeed = 0;

	/* Modify environmental variables */
	variable_Gear = 1;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to "Neutral Gear" state and send messages accordingly.
 */
void set_vehicle_switch_into_neutral_gear()
{
	/* Modify physical signals */
	signal_Clamp_15 = 1;
	signal_GearSelected = 3; // N
	signal_MotorSpeed = 700;
	signal_VehicleMode = 1;
	signal_VehicleSpeed = 0;

	/* Modify environmental variables */
	variable_Gear = 1;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to "Drive Gear" state and send messages accordingly.
 */
void set_vehicle_switch_into_drive_gear()
{
	/* Modify physical signals */
	signal_Clamp_15 = 1;
	signal_GearSelected = 4; // D
	signal_MotorSpeed = 700;
	signal_VehicleMode = 1;
	signal_VehicleSpeed = 0;

	/* Modify environmental variables */
	variable_Gear = 1;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to accelerate the car and send messages accordingly.
 */
void accelerate_vehicle()
{
	/* Increase speed */
	if (signal_VehicleSpeed < 320)
		signal_VehicleSpeed += 10;

	/* Calculate motor speed at that gear */
	signal_MotorSpeed = signal_VehicleSpeed * const_GearRatio[variable_Gear];

	/* Switch gear */
	if ((signal_MotorSpeed > 6000) && (variable_Gear < 6)) {
		variable_Gear++;

		/* Calculate motor speed at that gear */
		signal_MotorSpeed = signal_VehicleSpeed * const_GearRatio[variable_Gear];
	}

	/* Never drop below idle rpm */
	if (signal_MotorSpeed < 700)
		signal_MotorSpeed = 700;

	/* Send messages */
	send_all_messages();
}

/**
 * Set all physical signals to decelerate the car and send messages accordingly.
 *
 * @return Gear environment variable
 */
void decelerate_vehicle()
{
	/* Decrease speed */
	signal_VehicleSpeed -= 10;
	if (signal_VehicleSpeed < 0)
	  signal_VehicleSpeed = 0;

	/* Calculate motor speed at that gear */
	signal_MotorSpeed = signal_VehicleSpeed * const_GearRatio[variable_Gear];

	/* Switch gear */
	if ((signal_MotorSpeed < 3000) && (variable_Gear>1)) {
		variable_Gear--;

		/* Calculate motor speed at that gear */
		signal_MotorSpeed = signal_VehicleSpeed * const_GearRatio[variable_Gear];
	}

	/* Never drop below idle rpm */
	if (signal_MotorSpeed < 700)
		signal_MotorSpeed = 700;

	/* Send messages */
	send_all_messages();
}

void set_lamp_left()
{
	if (signal_Lamp_BlinkerLeft == 0)
		signal_Lamp_BlinkerLeft = 1;
	else
		signal_Lamp_BlinkerLeft = 0;

	/* Send messages */
	send_all_messages();
}

void set_lamp_right()
{
	if (signal_Lamp_BlinkerRight == 0)
		signal_Lamp_BlinkerRight = 1;
	else
		signal_Lamp_BlinkerRight = 0;

	/* Send messages */
	send_all_messages();
}

void set_day_light()
{
	if (signal_DayLight == 0)
		signal_DayLight = 1;
	else
		signal_DayLight = 0;

	/* Send messages */
	send_all_messages();
}

void set_high_beam_light()
{
	if (signal_highBeam == 0)
		signal_highBeam = 1;
	else
		signal_highBeam = 0;

	/* Send messages */
	send_all_messages();
}

void set_esp()
{
	if (signal_esp == 0)
		signal_esp = 1;
	else
		signal_esp = 0;

	/* Send messages */
	send_all_messages();
}

void set_abs()
{
	if (signal_abs == 0)
		signal_abs = 1;
	else
		signal_abs = 0;

	/* Send messages */
	send_all_messages();
}

void set_brake()
{
	if (signal_brake == 0)
		signal_brake = 1;
	else
		signal_brake = 0;

	/* Send messages */
	send_all_messages();
}
