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
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

/* Project includes */
#include "DIL_J1939_stdafx.h"
#include "J1939_UtilityFuncs.h"
#include "TransferLayer.h"
#include "NetworkMgmt.h"
#include "NodeConManager.h"
#include "DIL_Interface/BaseDIL_CAN.h"

// Forward declarations
class CConnectionDet;

const int nSIZE_OF_BYTE = sizeof(BYTE);

CTransferLayer::CTransferLayer()
{
    m_pIDIL_CAN = NULL;
}

CTransferLayer::~CTransferLayer(void)
{
}

CTransferLayer& CTransferLayer::ouGetTransLayerObj()
{
    static CTransferLayer ouTransferLayerObj;
    return ouTransferLayerObj;
}

LONG CTransferLayer::lTConnectReq(short /*sConNumber*/, char /*cBlockSize*/, eCON_MODE /*eSMmode*/)
{
    return 0L;
}

void CTransferLayer::vTConnectCon(short /*sConNumber*/, eCON_STATUS /*ConStatus*/,
                                  char /*cBlockSize*/, eCON_MODE /*eServiceMode*/)
{
}

void CTransferLayer::vTConnectInd(short /*sConNumber*/,char /*cBlockSize*/, BOOL /*bIsSMEnhance*/)
{
}

void CTransferLayer::vTDisconnectInd(short /*sConNumber*/,eREASON /*eReason*/)
{
}

void CTransferLayer::vTLongDataCon(short /*sConNumber*/, char /*cTransferResult*/)
{
}

void CTransferLayer::vTBroadDataInd(short /*sBroadcastCANId*/,short /*sDataLength*/, BYTE* /*pbData*/)
{
    //Indicate a RX broad cast data
    //CAdaptationLayer::ouGetAdaptLayerObj().vABroadDataInd(sBroadcastCANId, *pbData,
    //    pbData, sDataLength);
}

void CTransferLayer::vTConTestCon(short /*sConNumber*/, char /*cConnectionStatus*/, char /*cBlockSize*/, char /*cServiceMode*/)
{
    //Confirm the connection request
}

void CTransferLayer::vTransmitCANMsg(DWORD dwClientID, UINT unID,
                                     UCHAR ucDataLen, BYTE* pData, UINT unChannel)
{
    STCAN_MSG sMsgCAN = {'\0'};

    sMsgCAN.m_unMsgID = unID;
    sMsgCAN.m_ucDataLen = ucDataLen;
    memcpy(sMsgCAN.m_ucData, pData, ucDataLen);
    //sMsgCAN.m_ucChannel = unChannel;
    sMsgCAN.m_ucChannel = (UCHAR)unChannel;
    sMsgCAN.m_ucRTR = 0;
    sMsgCAN.m_ucEXTENDED = 1;
    m_pIDIL_CAN->sendMessage(dwClientID, sMsgCAN);
}

void CTransferLayer::vSetIDIL_CAN(CBaseDIL_CAN* pIDIL_CAN)
{
    m_pIDIL_CAN = pIDIL_CAN;
}
