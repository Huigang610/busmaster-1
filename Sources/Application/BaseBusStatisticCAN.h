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
 * @file      BaseBusStatisticCAN.h
 * @brief     CBaseBusStatisticCAN declaration.
 * @author    Venkatanarayana makam
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CBaseBusStatisticCAN declaration.
 */

#pragma once

enum eLOAD
{
    CURRENT = 0,
    AVERAGE,
    PEAK
};
class CBaseBusStatisticCAN
{
    /* NOTE:
        1. unChannelIndex can be CAN_CHANNEL_0, CAN_CHANNEL_1, CAN_CHANNEL_ALL
        2. eDir can be DIR_RX, DIR_TX, DIR_ALL
        3. byIdType can be TYPE_ID_CAN_STANDARD, TYPE_ID_CAN_EXTENDED, TYPE_ID_CAN_ALL
        4. byMsgType can be TYPE_MSG_CAN_RTR, TYPE_MSG_CAN_NON_RTR, TYPE_MSG_CAN_ALL
        5. eLoad can be CURRENT, AVERAGE, PEAK
    */
public:

    /* This function should be called for the very first time. */
    virtual HRESULT doInitialization(void) = 0;
    /* This function resets the bus statistics */
    virtual HRESULT resetBusStatistics(void) = 0;
    /* Get the total number of valid messages transmitted to or
       received from the bus */
    virtual HRESULT getTotalMessageCount(UINT unChannelIndex, eDirection eDir, BYTE byIdType,
                                         BYTE byMsgType, UINT& nMsgCount) = 0;
    /* Get the total number of error messages occurred while receiving or
       transmitting */
    virtual HRESULT getTotalErrorCount(UINT unChannelIndex, eDirection eDir, UINT& nErrCount) = 0;

    /* Get average number of msgs per second(Msg/s)*/
    virtual HRESULT getAverageMessageCountPerSecond(UINT unChannelIndex, eDirection eDir,
            BYTE byIdType, double& dMsgCount) = 0;
    /* Get average number of errors per second(Err/s)*/
    virtual HRESULT getAverageErrorCountPerSecond(UINT unChannelIndex, eDirection eDir, double& dErrCount) = 0;
    /* Get the bus load */
    virtual HRESULT getBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad) = 0;
    /* Get controller status */
    /* Note: eLoad can have only CURRENT, PEAK.*/
    virtual HRESULT getErrorCounter(UINT unChannelIndex, eDirection eDir, eLOAD eLoad, UCHAR& ucErrCounter) = 0;
    //Setting BaudRate
    virtual HRESULT setBaudRate(UINT unChannelIndex, double dBaudRate) = 0;
    //Start updating BusStatistics data
    virtual BOOL startUpdation(BOOL bStart)= 0;
    //Get the controller status
    virtual UCHAR getControllerStatus(UINT unChannelIndex) = 0;
    //Get the required channel's BusStistic structure.
    virtual HRESULT getBusStatistics(UINT unChannelIndex, SBUSSTATISTICS& sBusStatistics) = 0;
};
