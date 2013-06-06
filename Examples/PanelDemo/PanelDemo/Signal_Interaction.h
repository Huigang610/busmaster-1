#pragma once

/* Project includes */
#include "stdafx.h"
#include "BUSMASTER_Interface.h"
#include "BUSMASTER_Interface.c"

/* COM interface reference */
extern IApplication * m_IApplication;

/* Physical signal variables */
extern int          signal_Charge;
extern unsigned int signal_Clamp_15;
extern unsigned int signal_GearSelected;
extern unsigned int signal_Lamp_BlinkerLeft;
extern unsigned int signal_Lamp_BlinkerRight;
extern unsigned int signal_MotorSpeed;
extern int          signal_Temp;
extern unsigned int signal_VehicleMode;
extern int          signal_VehicleSpeed;
extern int			signal_dayLight;
extern int			signal_highBeam;
extern int			signal_esp;
extern int			signal_abs;
extern int			signal_brake;

/* Environmental variables */
extern unsigned int variable_Gear;

/* This function initializes the constants in each of the messages. */
void initialize_messages(void);

/* These functions are for sending explicit message. */
void send_BodyComputer01_message(void);
void send_BodyComputer02_message(void);
void send_BreakingSystem01_message(void);
void send_EngineControl01_message(void);
void send_EngineControl03_message(void);
void send_GearboxControl01_message(void);

/* This function sends out all messages at once. */
void send_all_messages(void);

/* These functions where formerly the key handlers. */
void set_vehicle_off();							// on key 0
void set_vehicle_ignition_on();					// on key 1
void set_vehicle_engine_running();				// on key 2
void set_vehicle_switch_into_reverse_gear();	// on key 3
void set_vehicle_switch_into_neutral_gear();	// on key 4
void set_vehicle_switch_into_drive_gear();		// on key 5
void accelerate_vehicle();						// on key a
void decelerate_vehicle();						// on key d
void set_lamp_left();
void set_lamp_right();
void set_day_light();
void set_high_beam_light();
void set_esp();
void set_abs();
void set_brake();
