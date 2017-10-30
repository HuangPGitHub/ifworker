cmake_minimum_required(VERSION 2.8)
include(${PROJECT_SOURCE_DIR}/env.txt)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LIB_SRC)

set (LINK_LIBS client msg_tools ngn_interface czmq zmq)

link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})
include_directories(${INC_DIR})
add_library(rpc ${LIB_SRC})

