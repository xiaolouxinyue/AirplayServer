cmake_minimum_required(VERSION 3.4.1)

set(RAOP_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)
set(MDNS_LIB_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/mDNSResponder)
set(FDK_AAC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/fdk-aac)

add_subdirectory(${RAOP_LIB_PATH} build_raop)
add_subdirectory(${MDNS_LIB_PATH} build_mdns)

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