/*
 * Unions_for_Node_Simulation.h is automatically created based on the database.
 * It is not used in this project, as this is not a node simulation, but a demo using the COM interface.
 * The unions and structs therein are based on the STCAN_MSG struct.
 *
 * Unions_for_COM_Interface.h is based the aforementioned file.
 * As CAN messages via the COM Interface are based upon CAN_MSGS struct, rather than STCAN_MSG struct, manual changes are done to reflect this difference.
 * Actually this involves the following changes to each union and struct:
 * - Reduce m_aucData[64] to m_aucData[8] in message union
 * - Exchange m_ucChannel with message union in message struct
 * - Remove m_ulTimeStamp and m_bCANFD from message struct
 */

#pragma once
#pragma pack(pop,8)

#ifndef MESSAGENAME_EngineControl03
#define MESSAGENAME_EngineControl03
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int CoolantTemperature :  8 ;
	    unsigned int                  : 32 ;
	    unsigned int                  : 24 ;

	};
}EngineControl03_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	EngineControl03_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} EngineControl03;
#endif /* MESSAGENAME_EngineControl03 */


#ifndef MESSAGENAME_GearboxControl01
#define MESSAGENAME_GearboxControl01
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int GearSelected     :  4 ;
	    unsigned int                  : 32 ;
	    unsigned int                  : 28 ;

	};
}GearboxControl01_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	GearboxControl01_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} GearboxControl01;
#endif /* MESSAGENAME_GearboxControl01 */


#ifndef MESSAGENAME_EngineControl01
#define MESSAGENAME_EngineControl01
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int CO2Emission      :  8 ;
	    unsigned int LampWarning_CoolantTemp :  2 ;
	    unsigned int LampWarning_Engine :  2 ;
	    unsigned int LampWarning_GlowPlug :  2 ;
	    unsigned int LampWarning_OilPressure :  2 ;
	    unsigned int EnginePower      :  8 ;
	    unsigned int EngineTorque     :  8 ;
	    unsigned int FuelConsumption  :  8 ;
	    unsigned int MotorSpeed       : 14 ;
	    unsigned int                  :  2 ;
	    unsigned int OilPressure      :  8 ;

	};
}EngineControl01_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	EngineControl01_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} EngineControl01;
#endif /* MESSAGENAME_EngineControl01 */


#ifndef MESSAGENAME_Kombi_02
#define MESSAGENAME_Kombi_02
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 24 ;

	};
}Kombi_02_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	Kombi_02_     m_sWhichBit     ;
	unsigned char m_ucChannel	;
} Kombi_02;
#endif /* MESSAGENAME_Kombi_02 */


#ifndef MESSAGENAME_Kombi_01
#define MESSAGENAME_Kombi_01
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 32 ;
	    unsigned int                  : 32 ;

	};
}Kombi_01_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	Kombi_01_     m_sWhichBit     ;
	unsigned char m_ucChannel	;
} Kombi_01;
#endif /* MESSAGENAME_Kombi_01 */


#ifndef MESSAGENAME_BodyComputer02
#define MESSAGENAME_BodyComputer02
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    int          SteeringWheelAngle : 10 ;
	    unsigned int Minutes          :  6 ;
	    unsigned int TankFillingLevel :  8 ;
	    unsigned int Battery_Voltage  :  8 ;
	    unsigned int DisplayMeasurementSystem :  1 ;
	    unsigned int Day              :  5 ;
	    unsigned int                  :  2 ;
	    unsigned int Hours            :  5 ;
	    unsigned int                  :  3 ;
	    unsigned int Year             :  8 ;
	    unsigned int Month            :  4 ;
	    unsigned int                  :  4 ;

	};
}BodyComputer02_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	BodyComputer02_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} BodyComputer02;
#endif /* MESSAGENAME_BodyComputer02 */


#ifndef MESSAGENAME_BodyComputer01
#define MESSAGENAME_BodyComputer01
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int LampWarning_Airbag :  2 ;
	    unsigned int LampWarning_AntiLock :  2 ;
	    unsigned int LampWarning_Battery :  2 ;
	    unsigned int Lamp_BlinkerLeft :  2 ;
	    unsigned int BatteryChargeLevel :  8 ;
	    int          BatteryTemperature :  8 ;
	    unsigned int Lamp_BlinkerRight :  2 ;
	    unsigned int Lamp_DayLights   :  2 ;
	    unsigned int LampWarning_DoorsOpen :  2 ;
	    unsigned int Lamp_FrontFog    :  2 ;
	    unsigned int LampWarning_FuelLevel :  2 ;
	    unsigned int Lamp_HighBeam    :  2 ;
	    unsigned int LampWarning_LightsDefect :  2 ;
	    unsigned int LampWarning_LowTire :  2 ;
	    unsigned int MultiFunctionControl :  3 ;
	    unsigned int VehicleMode      :  3 ;
	    unsigned int LampWarning_WashFluidLvl :  2 ;
	    unsigned int Clamp_15         :  2 ;
	    unsigned int LampWarning_SafetyBelt :  2 ;
	    unsigned int Lamp_RearFog     :  2 ;
	    unsigned int LampWarning_PowerSteering :  2 ;
	    int          OutdoorTemperature :  8 ;

	};
}BodyComputer01_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	BodyComputer01_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} BodyComputer01;
#endif /* MESSAGENAME_BodyComputer01 */


#ifndef MESSAGENAME_BreakingSystem01
#define MESSAGENAME_BreakingSystem01
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int LampWarning_Abs  :  2 ;
	    unsigned int LampWarning_Brake :  2 ;
	    unsigned int LampWarning_BrakeLights :  2 ;
	    unsigned int LampWarning_BrakePads :  2 ;
	    unsigned int AccelerationPedalTravel :  8 ;
	    unsigned int BrakePedalTravel :  8 ;
	    unsigned int Lamp_CruiseControl :  2 ;
	    unsigned int LampWarning_Esp  :  2 ;
	    unsigned int LampWarning_EspOff :  2 ;
	    unsigned int                  :  2 ;
	    int          GforceLateral    :  8 ;
	    int          GforceLongitudinal :  8 ;
	    unsigned int VehicleSpeed     : 14 ;
	    unsigned int                  :  2 ;

	};
}BreakingSystem01_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	BreakingSystem01_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} BreakingSystem01;
#endif /* MESSAGENAME_BreakingSystem01 */


#ifndef MESSAGENAME_HEC_StatusAndCMD
#define MESSAGENAME_HEC_StatusAndCMD
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  : 16 ;

	};
}HEC_StatusAndCMD_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	HEC_StatusAndCMD_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} HEC_StatusAndCMD;
#endif /* MESSAGENAME_HEC_StatusAndCMD */


#ifndef MESSAGENAME_RCM_EventMessage
#define MESSAGENAME_RCM_EventMessage
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int                  :  8 ;

	};
}RCM_EventMessage_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	RCM_EventMessage_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} RCM_EventMessage;
#endif /* MESSAGENAME_RCM_EventMessage */


#ifndef MESSAGENAME_ISO_Kombi_Req
#define MESSAGENAME_ISO_Kombi_Req
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    __int64      ISO_Kombi_Req_Data : 64 ;

	};
}ISO_Kombi_Req_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	ISO_Kombi_Req_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} ISO_Kombi_Req;
#endif /* MESSAGENAME_ISO_Kombi_Req */


#ifndef MESSAGENAME_EngineControl02
#define MESSAGENAME_EngineControl02
typedef union 
{
	unsigned char      m_aucData[8];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    int          OilTemperature   :  8 ;
	    unsigned int RecuperationLevel :  8 ;
	    unsigned int TorqueVectoring  :  3 ;
	    unsigned int                  :  5 ;
	    int          TurboPressure    :  8 ;
	    unsigned int Vehicle_Odometer : 32 ;

	};
}EngineControl02_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	EngineControl02_  m_sWhichBit     ;
	unsigned char m_ucChannel	;
} EngineControl02;
#endif /* MESSAGENAME_EngineControl02 */
