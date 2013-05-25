@Remarks
  AFX_MANAGE_STATE:
  Call this macro to protect an exported function in a DLL.
    e.g. AFX_MANAGE_STATE(AfxGetStaticModuleState());
  Absolutly necessary for dialogs etc.
  
  
List with function which should be filled or reworked:
---------------------------------------------------------------------------------------  

CDIL_CAN_IXXAT_VCI::CAN_SelectHwInterface  
No dialog is shown and the first CAN controller found in list is selected.
Wait till the hardware selection dialog was moved to the busmaster application itself.


CDIL_CAN_IXXAT_VCI::CAN_DisplayConfigDlg
Only the configuration for the first CAN controller can changed.
No Mask/Value filter available.


CDIL_CAN_IXXAT_VCI::CAN_GetControllerParams
Check if this is correct. Only copied from another DLL.
