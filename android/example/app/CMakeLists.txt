cmake_minimum_required(VERSION 3.4.1)

set(RAOP_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)
set(MDNS_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/mDNSResponder)
set(FDK_AAC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/fdk-aac)

add_subdirectory(${RAOP_LIB_PATH} raop_out)
add_subdirectory(${MDNS_LIB_PATH} mdns_out)

#include_directories(${RAOP_LIB_PATH}
#        ${FDK_AAC_LIB_PATH}libAACdec/include
#        ${FDK_AAC_LIB_PATH}libAACenc/include
#        ${FDK_AAC_LIB_PATH}libFDK/include
#        ${FDK_AAC_LIB_PATH}libMpegTPDec/include
#        ${FDK_AAC_LIB_PATH}libMpegTPEnc/include
#        ${FDK_AAC_LIB_PATH}libPCMutils/include
#        ${FDK_AAC_LIB_PATH}libSBRdec/include
#        ${FDK_AAC_LIB_PATH}libSBRenc/include
#        ${FDK_AAC_LIB_PATH}libSYS/include
#
#        ${FDK_AAC_LIB_PATH}libArithCoding/include
#        ${FDK_AAC_LIB_PATH}libDRCdec/include
#        ${FDK_AAC_LIB_PATH}libSACdec/include
#        ${FDK_AAC_LIB_PATH}libSACenc/include)
add_library( raop_server
        SHARED
        src/main/cpp/jni_raop_server.cpp
        )

find_library( log-lib
              log )

target_link_libraries( raop_server
                       ${log-lib}
        play-lib
        jdns_sd
        ed25519
        fdk-aac)