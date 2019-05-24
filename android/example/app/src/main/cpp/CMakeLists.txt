cmake_minimum_required(VERSION 3.4.1)
add_definitions(-DNDEBUG -D_USRDLL -DMDNS_DEBUGMSGS=0 -DWIN32_LEAN_AND_MEAN -DUSE_TCP_LOOPBACK -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1 -DNOT_HAVE_SA_LEN)

add_subdirectory(lib/crypto)
add_subdirectory(lib/curve25519)
add_subdirectory(lib/ed25519)
add_subdirectory(lib/playfair)
add_subdirectory(lib/plist)
add_subdirectory(lib/fdk-aac)
add_subdirectory(lib)

include_directories(lib
        lib/fdk-aac/libAACdec/include
        lib/fdk-aac/libAACenc/include
        lib/fdk-aac/libFDK/include
        lib/fdk-aac/libMpegTPDec/include
        lib/fdk-aac/libMpegTPEnc/include
        lib/fdk-aac/libPCMutils/include
        lib/fdk-aac/libSBRdec/include
        lib/fdk-aac/libSBRenc/include
        lib/fdk-aac/libSYS/include

        lib/fdk-aac/libArithCoding/include
        lib/fdk-aac/libDRCdec/include
        lib/fdk-aac/libSACdec/include
        lib/fdk-aac/libSACenc/include)
#add_library( raop_server SHARED airplay_server.cpp )
add_executable( raop_server raop_server.cpp)
add_subdirectory(mDNSResponder)
#find_library( log-lib
 #             log )
find_library(WS2_32_LIBRARY ws2_32)
target_link_libraries( raop_server
        raopserver
        jdns_sd
        ed25519
        fdk-aac
		ws2_32)