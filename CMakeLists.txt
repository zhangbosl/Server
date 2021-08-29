CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(MyServer)

FILE(GLOB SRC_LIST_CPP ${PROJECT_SOURCE_DIR}/*.cpp) # /src
FILE(GLOB SRC_LIST_C ${PROJECT_SOURCE_DIR}/*.c) # /src

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}) # /include

# SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(Server ${SRC_LIST_CPP} ${SRC_LIST_C}) # 
target_link_libraries(Server /usr/lib/x86_64-linux-gnu/libmysqlclient.so)

# ADD_LIBRARY(myprint SHARED ${SRC_LIST_CPP} ${SRC_LIST_C})
