#ifndef ETAS_CSI_BASE_H
#define ETAS_CSI_BASE_H

#include "csitypes.h"

/**
* @file       csibase.h
* @brief      Public declaration of the Connection Service Interface 
*             (CSI), which must be implemented on any node in the 
*             system.
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 5976 $
*
*/

#include "..\Common\boatypes.h"
#include "csidefs.h"
#include "csisfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_CSI_COMMON Enumerate Sub Service (ESS)
* @brief      Common declaration of a service, 
*             which must be implemented on any node in the system.
 The @ref GROUP_CSI_SFS and @ref GROUP_CSI_CTS APIs of the Connection Service Interface require this API to offer their higher level services
 to the application / binding process in any node that is represented in the API.
* @{
*/

/**
 Get the handle to access a node. This handle can be used to bind the CSI_ESS-Interface
 of a node found by a previous call of CSI_ServiceGetSubItems() for any element, that
 matches to the CSI_ESS-API.
 This function describes the interface used by the application for the access to the node.
 The CSI framework will resolve the uriLocation into a BOA_ProtocolStack. So the driver has
 to implement the CSI_ServiceOpenNodeBind() command.

 @param[in]  uriLocation uriPath to the Node, which will be opened. This path was found by a 
                         previous CSI_ServiceGetSubItems() call or can be "/" to open the 
                         root node.
                         The node at the destiantion must offer the CSI_ESS-Interface. In other
                         cases the function will return OCI_ERR_INVALID_PARAM
 @param[out] handle handle to the CSI_ESS-API of the node. CSI_ServiceGetSubItems() can be used 
                    to enumerate all services, that are available at this node.
*/
CSI_DECLSPEC BOA_ResultCode 
CSI_CALL CSI_ServiceOpenNode 
    (const char* uriLocation, CSI_Handle* handle);

typedef BOA_ResultCode (CSI_CALL *PF_CSI_ServiceOpenNode)(const char *uriLocation, CSI_Handle *handle);

/**
 Get the handle to access a node. This handle can be used to bind the CSI_ESS-Interface
 of a node found by a previous call of CSI_ServiceGetSubItems() for any element, that
 matches to the CSI_ESS-API.
 This function describes the interface used by the driver to implement the node.
 The CSI framework will resolve a uriLocation into a BOA_ProtocolStack. So the application has
 to implement the CSI_ServiceOpenNode() command with the user visible uriPath.

 @param[in]  stack Protocol stack to open the node. In most cases this stack will consist of
                   a single element and the node itself will use the information to access the
                   correct element inside of the specific implementation. When the implementation
                   uses a protocol stack children of the stack may be required to bind APIs at
                   the lower end of this implementation and the sub-tree of the stack has to be 
                   forwarded to the initialization routine of the bound interface.

 @param[out] handle handle to the CSI_ESS-API of the node. CSI_ServiceGetSubItems() can be used 
                    to enumerate all services, that are available at this node. When the API binding
                    process fails (e.g. because a binding of a lower interface failed) the call
                    returns OCI_NO_HANDLE
*/
CSI_DECLSPEC BOA_ResultCode 
CSI_CALL CSI_ServiceOpenNodeBind 
    (const BOA_ProtocolStack* stack, CSI_ESS_Handle* handle);

typedef BOA_ResultCode (CSI_CALL *PF_CSI_ServiceOpenNodeBind)(const BOA_ProtocolStack* stack, CSI_ESS_Handle *handle);

/** Release the access to a node. */
CSI_DECLSPEC BOA_ResultCode 
CSI_CALL CSI_ServiceCloseNode (CSI_ESS_Handle handle);

typedef BOA_ResultCode (CSI_CALL *PF_CSI_ServiceCloseNode)(CSI_ESS_Handle handle);

/**
 Find all sub-services of a specific node. The node has to be opened by the CSI_ServiceOpenNode()
 call. The result represents a snapshot of the currently accessible sub-services. The sub-services
 include the sub-nodes, which offer the CSI_ESS-API again and any protocol that may be used on this
 node and that may depend on the detection of other protocols in the stack. The number of 
 sub-services may change, when some hardware is connected or disconnected to the host. 

 @param[in]  handle         A handle to an implementation of CSI_ESS, created via CSI_ServiceOpenNode().
 @param[out] result         Sub-services of the node whose implementation of CSI_ESS is identified by @p handle.
                            The memory for the result is supplied by the caller. It is large enough to
                            receive @p size elements.
 @param[in]  size           The number of elements in @p result[].
 @param[out] count          The number of elements in @p result[] which were used.
 @param[in]  szUriNameHint  If this is non-NULL it provides a hint that the caller is looking for a sub-service
                            which has a particular URI name. The function may choose to ignore the hint and
                            output all sub-services regardless, but the hint allows the function to optimise its
                            implementation, if possible, by only outputting those sub-services whose URI name
                            matches @p szUriNameHint, or which can be used to reach the sub-service named
                            by @p szUriNameHint.
                            NOTE that an implementation of this function should ignore @p szUriNameHint unless it
                            is absolutely certain that the hint can be applied. If in doubt, do not apply the hint.
 
 @return When the function succeeds OCI_SUCCESS is returned and the result and count parameter
         deliver the requested information. When the function fails an error code is returned 
         and the out parameter may contain undefined values.  
*/
CSI_DECLSPEC BOA_ResultCode 
CSI_CALL CSI_ServiceGetSubItems
    (CSI_ESS_Handle handle, CSI_SubItem result[], uint32 size, uint32* count, const char* szUriNameHint );


typedef BOA_ResultCode (CSI_CALL *PF_CSI_ServiceGetSubItems)(CSI_ESS_Handle handle, CSI_SubItem result[], uint32 size, uint32 *count, const char* szUriNameHint );

#define UUID_CSI_ESS { 0x07, 0xdb, 0x65, 0x38, 0x46, 0x3c, 0x4c, 0x8b,\
                       0x88, 0xdc, 0x63, 0xd0, 0xf9, 0x22, 0x6b, 0x4d }

typedef struct CSI_ESS_VTable
{
    PF_CSI_ServiceOpenNode serviceOpenNode;
    PF_CSI_ServiceOpenNodeBind serviceOpenNodeBind;
    PF_CSI_ServiceGetSubItems serviceGetSubItems;
    PF_CSI_ServiceCloseNode serviceCloseNode;
} CSI_ESS_VTable;

/** 
* @} 
*/

#ifdef __cplusplus
}
#endif

#endif
