SET(ADM_LIB ADM_UI_Cli6)

SET(${ADM_LIB}_SRCS 
	DIA_dialogFactory.cpp
	DIA_flyDialogCli.cpp
	DIA_none.cpp
)

add_compiler_export_flags()
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include)
ADD_LIBRARY(${ADM_LIB}  SHARED ${${ADM_LIB}_SRCS})
ADD_SOURCE_CFLAGS(DIA_flyDialogCli.cpp " -DADM_UI_TYPE_BUILD=1")
TARGET_LINK_LIBRARIES(${ADM_LIB} ADM_core6 ADM_coreUI6  ADM_render6_cli ADM_coreVideoFilter6)
ADM_INSTALL_LIB(${ADM_LIB} )
