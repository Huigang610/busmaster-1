include_directories(EXTERNAL/Include)

add_library(CAN_ETAS_BOA SHARED
  CAN_ETAS_BOA.cpp CAN_ETAS_BOA.h CAN_ETAS_BOA.rc
  CAN_ETAS_BOA_Extern.h
  CAN_ETAS_BOA_Resource.h
  CAN_ETAS_BOA_stdafx.h
  ChangeRegDefines.h
  ChangeRegisters_CAN_ETAS_BOA.cpp ChangeRegisters_CAN_ETAS_BOA.h
  ContrConfigPeakUsbDefs.h
  HardwareListing.cpp HardwareListing.h
  ../Application/MultiLanguage.cpp ../Application/MultiLanguage.h)

target_link_libraries(CAN_ETAS_BOA
  Utility)
