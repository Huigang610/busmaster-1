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
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 */

#pragma once

/* Project includes */
#include "Include/BaseDefs.h"
#include "Datatypes/J1939_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "Datatypes/Base_WrapperErrorLogger.h"
#include "DataTypes/BaseMsgBufAll.h"
#include "DataTypes/Dil_Datatypes.h"

class CBaseDILI_J1939
{
public:
    /**
     * Initializes J1939 network
     *
     * @req RS_26_01
     *   Initialise
     * @param[in] pILog
     *   pointer to wrapper_error object
     * @return
     *   S_OK for success or S_FALSE for failure
    */
    virtual HRESULT initialize(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pIDIL_CAN) = 0;

    /**
     * Uninitializes J1939 network
     *
     * @req RS_26_02
     *   Uninitialise
     * @return
     *   S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT uninitialize(void) = 0;

    /**
     * Registers / unregisters a client. This is necessary to simulate a node
     * and to receive messages. Only registered client's buffer will be updated
     * on receive of a msg in the bus.
     *
     * @param[in] bRegister
     *   TRUE to register, else FALSE.
     * @param[in] pacNodeName
     *   Client node name.
     * @param[in] un64ECUName
     *   64 bit ECU name.
     * @param[in] byPrefAdres
     *   8 bit node address (0 - 253)
     * @param[out] dwClientId
     *   Client's Id rendered.
     * @return
     *   ERR_CLIENT_ EXISTS, ERR_NO_CLIENT_EXIST, ERR_NO_ MORE_CLIENT_ALLOWED, or S_OK
     */
    virtual HRESULT registerClient(BOOL bRegister, char* pacNodeName,
                                         UINT64 un64ECUName, BYTE byPrefAdres,
                                         DWORD& dwClientId) = 0;

    /**
     * Manages the target client buffer list. Call this function to open a
     * data channel to receive messages.
     *
     * @param[in] bAction
     *   When MSGBUF_ADD, adds pBufObj to the target message buffer list.
     *   Removes when MSGBUF_CLEAR.
     * @param[in] ClientID
     *   Client ID
     * @param[in] pBufObj
     *   Interface to message buffer object.
     * @return
     *   S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT manageMessageBuffer(BYTE bAction, DWORD ClientID,
                                       CBaseMsgBufVSE* pBufObj) = 0;

    /**
     * Sends a J1939 message.
     *
     * @param[in] dwClient Id of registered client
     * @param[in] unChannel Channel number
     * @param[in] eMsgType COMMAND, BROADCAST, DATA, REQUEST
     * @param[in] unPGN PGN to be sent or received
     * @param[in] pbyData Data
     * @param[in] unDLC Data length in number of bytes
     * @param[in] byPriority Priority of the message (0-7)
     * @param[in] bySrc From address, This is ignored if client is not a monitor client
     * @param[in] byDest Destination address
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT sendJ1939Message(DWORD dwClient, UINT unChannel,
                                        EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
                                        BYTE* pbyData, UINT unDLC,
                                        BYTE byPriority = DEFAULT_PRIORITY,
                                        BYTE bySrc = ADDRESS_NULL,
                                        BYTE byDestAdress = ADDRESS_ALL) = 0;

    /**
     * Sends Positive/Negative acknowledgement msg.
     *
     * @param[in] dwClient Client Id of the sender node
     * @param[in] eAckType ACK_POS, ACK_NEG
     * @param[in] unPGN PGN to be acknowledged.
     * @param[in] bySrc From address, This is ignored if client is not a monitor node.
     * @param[in] byAddresAck Address to be acknowledged.
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT sendAcknowledgementMessage(DWORD dwClient, UINT unChannel,
                                     ETYPE_ACK eAckType, UINT32 unPGN,
                                     BYTE bySrc,
                                     BYTE byAddresAck) = 0;

    /**
     * Requests a PGN from the node.
     *
     * @param[in] dwClient Client Id of the sender node
     * @param[in] nPGN PGN to be requested.
     * @param[in] bySrc From address, This is ignored if the client is not monitor node.
     * @param[in] byDestAddress From Node address.
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT requestPgn(DWORD dwClient, UINT unChannel,
                                     UINT32 unPGN, BYTE byPriority = DEFAULT_PRIORITY,
                                     BYTE bySrc = ADDRESS_NULL,
                                     BYTE byDestAddress = ADDRESS_ALL) = 0;

    /**
     * Starts J1939 network. All nodes start sending according to the configuration.
     *
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT goOnline() = 0;

    /**
     * Stops J1939 network. All nodes stop sending msgs.
     *
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT goOffline() = 0;

    /**
     * Gets the node name from 8 bit address from J1939 network.
     *
     * @param[in] byAddress 8 bit node address (0 - 253)
     * @param[out] acNodeName Nodes name
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT getNodeName(BYTE byAddress, char* acNodeName) = 0;

    /**
     * Gets the 8 bit address from node name from J1939 network.
     *
     * @param[out] byAddress 8 bit node address (0 - 253)
     * @param[in] dwClient Client Id
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT getNodeAddress(BYTE& byAddress, DWORD dwClient) = 0;

    /**
     * Returns whether the address is already claimed by another node.
     * 
     * @return TRUE for already claimed or FALSE for not claimed.
     */
    virtual BOOL isAddressClaimed(BYTE byAddress) = 0;

    /**
     * Tries to claim 8 bit address to a corresponding node.
     *
     * @param[in] dwClientId Already register node's client Id
     * @param[in] byAddress New address (0 - 253)
     * @param[in] byPriority Priority
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT claimAddress(DWORD dwClientId, UINT unChannel,
                                          BYTE byAddress,
                                          BYTE byPriority = DEFAULT_PRIORITY) = 0;

    /**
     * Requests address from the node.
     *
     * @param[in] dwClientId
     *   Senders's client Id
     * @param[in] byPriority
     *   Priority
     * @param[in] bySrc
     *   Src Address for monitor node.
     *   This is ignored if client is not a monitor node.
     * @param[in] byDestAddress
     *   Destination address (global by default)
     * @return
     *   S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT requestAddress(DWORD dwClient, UINT unChannel,
                                            BYTE byPriority = DEFAULT_PRIORITY,
                                            BYTE bySrc = ADDRESS_NULL,
                                            BYTE byDestAddress = ADDRESS_ALL) = 0;

    /**
     * Commands a node to assume a address.
     *
     * @param[in] dwClientId Senders's client Id
     * @param[in] unECU_NAME 64 bit NAME of the ECU to which command has to be sent.
     * @param[in] byNewAddress The new address it has to assume.
     * @param[in] byPriority Priority
     * @param[in] bySrc This is ignored if client is not monitor node.
     * @param[in] byDestAddress Destination address (global by default).
     * @return S_OK for success or S_FALSE for failure.
     */
    virtual HRESULT commandAddress(DWORD dwClient, UINT unChannel,
                                            UINT64 unECU_NAME,
                                            BYTE byNewAddress,
                                            BYTE byPriority = DEFAULT_PRIORITY,
                                            BYTE bySrc = ADDRESS_NULL,
                                            BYTE byDestAddress = ADDRESS_ALL) = 0;

    /**
     * Get J1939 timeouts.
     *
     * @param[in] eTimeOutType
     *   Type of the timeout:
     *   - TYPE_TO_BROADCAST
     *   - TYPE_TO_RESPONSE
     *   - TYPE_TO_HOLDING
     *   - TYPE_TO_T1
     *   - TYPE_TO_T2
     *   - TYPE_TO_T3
     *   - TYPE_TO_T4
     * @param[out] unMiliSeconds
     *   Value of the timeout
     * @return
     *   S_OK for success or S_FALSE for failure
    */
    virtual HRESULT getTimeout(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds) = 0;

    /**
     * Configure J1939 timeouts.
     *
     * @param[in] eTimeOutType
     *   Type of the timeout:
     *   - TYPE_TO_BROADCAST
     *   - TYPE_TO_RESPONSE
     *   - TYPE_TO_HOLDING
     *   - TYPE_TO_T1
     *   - TYPE_TO_T2
     *   - TYPE_TO_T3
     *   - TYPE_TO_T4
     * @param[in] unMiliSeconds
     *   Value of the timeout
     * @return
     *   S_OK for success or S_FALSE for failure
     */
    virtual HRESULT configureTimeout(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds) = 0;

    /**
     * Get the time mode mapping
     */
    virtual HRESULT getTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime) = 0;

    /**
     * Returns J1939 network status
     *
     * @return
     *   TRUE if online, FALSE if offline
     */
    virtual BOOL isOnline(void) = 0;

    /**
     * Set call back function pointer
     *
     * @param eClBckFnType[in]
     *   Call back function type
     *   (Long data & broadcast data indication & type)
     * @param pvClBckFn[in]
     *   Call back function pointer
     * 
     * @return
     *   S_OK for success or S_FALSE for failure
     */
    virtual HRESULT setCallbackFunction(DWORD dwClientId, ETYPE_CLBCK_FN eClBckFnType,
                                            void* pvClBckFn) = 0;
};
