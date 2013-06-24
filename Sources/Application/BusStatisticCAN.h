/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @brief     This File contains the BusStatics Class Declaration.
 * @author    venkatanarayana Makam
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This File contains the BusStatics Class Declaration.
 */

#pragma once

/* Project includes */
#include "Utility/Utility_Thread.h"
#include "Include/struct_can.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "Include/Basedefs.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "BaseBusStatisticCAN.h"

class CBusStatisticCAN : public CBaseBusStatisticCAN
{
public:
	/**
	 * Initialises critical section and event mechanism.
	 */
    CBusStatisticCAN(void);

	/**
	 * Deletes critical section and terminates the thread.
	 */
    virtual ~CBusStatisticCAN(void);

	/**
	 * @return contain Error information
	 *
	 * Does the client registration with driver and starts the Read
	 * Thread.
	 *
	 * This function should be called for the very first time.
	 */
    HRESULT doInitialization(void);

	/**
	 * @brief This function resets the bus statistics
	 * @return contain Error information
	 *
	 * This function resets the bus statistics structure using
	 * initializeBusStatisticsData() member function.
	 */
    HRESULT resetBusStatistics(void);

	/**
	 * @brief Get the total number of valid messages transmitted to or received from the bus
	 * @param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
	 * @param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
	 * @param[in] byIdType can be TYPE_ID_CAN_STANDARD, TYPE_ID_CAN_EXTENDED, TYPE_ID_CAN_ALL
	 * @param[in] byMsgType can be TYPE_MSG_CAN_RTR, TYPE_MSG_CAN_NON_RTR, TYPE_MSG_CAN_ALL
	 * @param[out] nMsgCount Referance variable contains the resulted Total message count
	 * @req RS_24_08 Standard frames are considered.
	 * @req RS_24_09 Extended frames are considered.
	 * @req RS_24_10 RTR frames are considered.
	 * @req RS_24_16 Total data shall be presented.
	 *
	 * Returns the total number of valid messages transmitted to or
	 * received from the bus.
	 */
    HRESULT getTotalMessageCount(UINT unChannelIndex, eDirection eDir, BYTE byIdType,
                                 BYTE byMsgType, UINT& nMsgCount);

	/**
	 * @brief Get the total number of error messages occurred while receiving or transmitting
	 * @param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
	 * @param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
	 * @param[out] nErrCount contain the Total error count on function return
	 * @return contain Error information
	 * @req RS_24_11 Error frames occurs under the purview of status data
	 * @req RS_24_16 Total data shall be presented.
	 *
	 * Returns the total number of error messages occurred while
	 * receiving or transmitting
	 */
    HRESULT getTotalErrorCount(UINT unChannelIndex, eDirection eDir, UINT& nErrCount);

	/**
	 * @brief Get average number of msgs per second(Msg/s)
	 * @param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
	 * @param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
	 * @param[in] byIdType can be TYPE_ID_CAN_STANDARD, TYPE_ID_CAN_EXTENDED, TYPE_ID_CAN_ALL
	 * @param[out] fMsgCount on return it will contain the Average message count per second.
	 * @req RS_24_08 Standard frames are considered.
	 * @req RS_24_09 Extended frames are considered.
	 * @req RS_24_17 Present moment data shall be presented.
	 *
	 * Returns average number of msgs per second (Msg/s)
	 */
    HRESULT getAverageMessageCountPerSecond(UINT unChannelIndex, eDirection eDir,
                                     BYTE byIdType, double& dMsgCount);

	/**
	 * @brief Get average number of errors per second(Err/s)
	 * @param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
	 * @param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
	 * @param[out] fErrCount on return it contain the Average Error count per Second.
	 * @return contain Error information
	 * @req RS_24_11 Error frames occurs under the purview of status data
	 * @req RS_24_17 Present moment data shall be presented.
	 *
	 * Returns average number of errors per second (Err/s)
	 */
    HRESULT getAverageErrorCountPerSecond(UINT unChannelIndex, eDirection eDir, double& dErrCount);

	/**
	 * @brief Get the bus load
	 * @param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
	 * @param[in] eLoad can be CURRENT, AVERAGE, PEAK
	 * @param[out] dBusLoad on function return it contain Bus Load
	 * @return contain Error information
	 * @req RS_24_12 Bus load status data set includes current load
	 * @req RS_24_13 Bus load status data set includes peak load
	 * @req RS_24_14 Bus load status data set includes average load
	 *
	 * Returns the bus load
	 */
    HRESULT getBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad);

	/**
	 * @brief Get error counter
	 * @param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
	 * @param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
	 * @param[in] eLoad can be CURRENT, AVERAGE, PEAK
	 * @param[out] ucErrCounter on return contains the error counter
	 * @return contain Error information
	 * @req RS_24_11 Error frames occurs under the purview of status data
	 * @req RS_24_12 Bus load status data set includes current load
	 * @req RS_24_13 Bus load status data set includes peak load
	 * @req RS_24_14 Bus load status data set includes average load
	 *
	 * Get error counter
	 *
	 * Note: eLoad can have only CURRENT, PEAK.
	 */
    HRESULT getErrorCounter(UINT unChannelIndex, eDirection eDir, eLOAD eLoad, UCHAR& ucErrCounter);

	/**
	 * @brief To Set the Baud Rate
	 * @return return the error state
	 *
	 * To Set the Baud Rate
	 */
    HRESULT setBaudRate(UINT unChannelIndex, double dBaudRate);

	/**
	 * @brief Starts or kills the timer.
	 * @param[in] bStart if ture starts the TIMER otherwise kill the thread
	 *
	 * Updates the Bus Statistics on every received or transmitted Message
	 */
    BOOL startUpdation(BOOL bStart);

	/**
	 * @brief Get the controller status
	 * @return Controller status
	 * @req RS_24_18 Controller status data covers present controller status
	 *
	 * This Function renders controller status
	 */
    UCHAR getControllerStatus(UINT unChannelIndex);

	/**
	 * @brief Get the required channel's BusStistic structure.
	 * @param[in] sBusStatistics will have the required channel's
	 * @return function return status
	 *
	 * This Function returns the required channel's BusStatistics
	 */
    HRESULT getBusStatistics(UINT unChannelIndex, SBUSSTATISTICS& sBusStatistics);

	/**
	 * @brief Updates the Bus Statics Structure on every Tx/Rx Message.
	 * @req RS_24_08 Standard frames are considered.
	 * @req RS_24_09 Extended frames are considered.
	 * @req RS_24_10 RTR frames are considered.
	 * @req RS_24_11 Error frames occurs under the purview of status data
	 *
	 * Calculate the Bus statistics in m_sBusStatistics structure
	 */
    void updateBusStatistics(STCANDATA& sCanData);

	/**
	 * @brief Calculate the Bus statistics on timer
	 * @req RS_24_08 Standard frames are considered.
	 * @req RS_24_09 Extended frames are considered.
	 * @req RS_24_10 RTR frames are considered.
	 * @req RS_24_11 Error frames occurs under the purview of status data
	 * @req RS_24_12 Bus load status data set includes current load
	 * @req RS_24_13 Bus load status data set includes peak load
	 * @req RS_24_14 Bus load status data set includes average load
	 * @req RS_24_18 Controller status data covers present controller status
	 * @req RS_24_19 Controller status data covers present controller Tx Error Counter (peak)
	 * @req RS_24_20 Controller status data covers present controller Tx Error Counter (present)
	 * @req RS_24_21 Controller status data covers present controller Rx Error Counter (peak)
	 * @req RS_24_22 Controller status data covers present controller Rx Error Counter (present)
	 *
	 * Calculates the Bus Statistics using m_sBusStatistics structure
	 */
    void calculateBusParameters(void);

	/**
	 * @brief Temporary function called by the SetTimer function.
	 *
	 * This Function will be called by the Timer event
	 */
    static void CALLBACK onTimeWrapper(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

    CCANBufFSE m_ouCanBufFSE;

private:
    /**
	 * used to Creating threads
	 */
    CPARAM_THREADPROC m_ouReadThread;

	/**
	 * Critical section
	 */
    CRITICAL_SECTION m_omCritSecBS;

	/**
	 * Bus Statictics Structure
	 */
    SBUSSTATISTICS m_sBusStatistics[ defNO_OF_CHANNELS ];

	/**
	 * Sub Bus Statistics
	 */
    SSUBBUSSTATISTICS m_sSubBusStatistics[ defNO_OF_CHANNELS ];

	/**
	 * Temporary Referance of STCAN_MSG
	 */
    STCAN_MSG m_sCurrEntry;

	/**
	 * variable to hold current channel
	 */
	CBaseDIL_CAN* m_pouDIL_CAN;

	/**
	 * To keep previous bus data
	 */
    SBUSSTATISTICS m_sPrevStatData[ defNO_OF_CHANNELS ];

	int m_nFactorSTDFrame;
    int m_nFactorEXTDFrame;
    int m_nFactorSTDRTRFrame;
    int m_nFactorEXTDRTRFrame;
    int m_nFactorErrorFrame;
    double m_dDiffTime;
    UINT_PTR m_nTimerHandle;

	/* Just for an intermediate calculations */
    UINT m_unPrevStandardCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevExtendedCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevStandardRTRCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevExtendedRTRCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevErrorTotalCount[ defNO_OF_CHANNELS ] ;
    INT  m_unPreviousTime;
    static void* sm_pouBSCan;

    UINT m_unBitsStdMsg[9];
    UINT m_unBitsExdMsg[9];

    UINT m_unBitsFDMsgCRC15[2][9];
    UINT m_unBitsFDMsgCRC17[2][2];
    UINT m_unBitsFDMsgCRC21[2][5];

	/**
	 * @brief Initialise or resets the bus statistics.
	 * @req RS_24_08 Standard frames are considered.
	 * @req RS_24_09 Extended frames are considered.
	 * @req RS_24_10 RTR frames are considered.
	 * @req RS_24_11 Error frames occurs under the purview of status data
	 *
	 * Initialises the m_sbusstatistics structor
	 */
    void initializeBusStatisticsData(void);

	/**
	 * @brief Calculates the Time differance.
	 *
	 * Calculates the differential time in sec
	 */
    void calculateDifferentialTime(void);

	/**
	 * @brief Starts the BS Read Thread
	 * @req RS_24_15 Measurement period always begins (or resets) from the time of connection
	 * @return return the error state
	 *
     * Starts the Read thread.
	 */
    BOOL startBusStatisticsReadThread(void);
};
