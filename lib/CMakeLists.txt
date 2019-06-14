cmake_minimum_required(VERSION 3.4.1)

set(MDNS_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/mDNSResponder)
set(FDK_AAC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fdk-aac)

add_subdirectory(${MDNS_LIB_PATH} build_mdns)
add_subdirectory(crypto)
add_subdirectory(curve25519)
add_subdirectory(ed25519)
add_subdirectory(playfair)
add_subdirectory(plist)
add_subdirectory(${FDK_AAC_LIB_PATH} fdk_out)

include_directories(crypto curve25519 ed25519 playfair plist/plist
        ${FDK_AAC_LIB_PATH}/libAACdec/include
        ${FDK_AAC_LIB_PATH}/libAACenc/include
        ${FDK_AAC_LIB_PATH}/libFDK/include
        ${FDK_AAC_LIB_PATH}/libMpegTPDec/include
        ${FDK_AAC_LIB_PATH}/libMpegTPEnc/include
        ${FDK_AAC_LIB_PATH}/libPCMutils/include
        ${FDK_AAC_LIB_PATH}/libSBRdec/include
        ${FDK_AAC_LIB_PATH}/libSBRenc/include
        ${FDK_AAC_LIB_PATH}/libSYS/include
        #${FDK_AAC_LIB_PATH}/libArithCoding/include
        #${FDK_AAC_LIB_PATH}/libDRCdec/include
        #${FDK_AAC_LIB_PATH}/libSACdec/include
        #${FDK_AAC_LIB_PATH}/libSACenc/include
        ${MDNS_LIB_PATH}/mDNSPosix
        ${MDNS_LIB_PATH}/mDNSCore
        ${MDNS_LIB_PATH}/mDNSShared
        )

aux_source_directory(. play_src)
set(DIR_SRCS ${play_src})

if(WIN32)
    add_library( raopserver
            STATIC
            ${DIR_SRCS}
            )
elseif(APPLE)
    add_library( raopserver
            STATIC
            ${DIR_SRCS}
            )
else()
    add_library( raopserver
            SHARED
            ${DIR_SRCS}
            )
endif()

if(ANDROID)
    find_library( log-lib
            log )
    target_link_libraries( raopserver
            crypto
            curve25519
            ed25519
            playfair
            plist
            fdk-aac
            jdns_sd
            ${log-lib})
elseif(APPLE)
    find_library(WS2_32_LIBRARY ws2_32)
    target_link_libraries( raopserver
            crypto
            curve25519
            ed25519
            playfair
            plist
            fdk-aac
            jdns_sd
            ws2_32)
else()
    find_library(WS2_32_LIBRARY ws2_32)
    target_link_libraries( raopserver
            crypto
            curve25519
            ed25519
            playfair
            plist
            fdk-aac
            jdns_sd
            ws2_32)
endif()


target_include_directories(raopserver
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)