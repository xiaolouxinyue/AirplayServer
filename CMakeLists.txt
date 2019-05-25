cmake_minimum_required(VERSION 3.4.1)
#add_definitions(-DNDEBUG -D_USRDLL -DMDNS_DEBUGMSGS=0 -DWIN32_LEAN_AND_MEAN -DUSE_TCP_LOOPBACK -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1 -DNOT_HAVE_SA_LEN)
add_definitions(-D_USRDLL -DMDNS_DEBUGMSGS=0 -DWIN32_LEAN_AND_MEAN -DUSE_TCP_LOOPBACK -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1 -DNOT_HAVE_SA_LEN)
set(RAOP_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(MDNS_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mDNSResponder)
set(FDK_AAC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fdk-aac)

add_subdirectory(${RAOP_LIB_PATH} build_raop)
add_subdirectory(${MDNS_LIB_PATH} build_mdns)

add_executable( raop_server airplay_server.cpp )

find_library(WS2_32_LIBRARY ws2_32)
target_link_libraries( raop_server
        play-lib
        jdns_sd
        ed25519
        fdk-aac
	ws2_32)