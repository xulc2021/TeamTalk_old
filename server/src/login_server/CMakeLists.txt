cmake_minimum_required(VERSION 2.6)
PROJECT(loginserver)

# CMAKE的输出路径为TeamTalk\server\src\bin\${name}，${name}代表项目名称，如login_server
## ../../定位到src下，并且只需要连接静态库时，增加该前缀
SET(SRC_DIR  ../../)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)

#SET(LIBRARY_OUTPUT_PATH ./)
SET(EXECUTABLE_OUTPUT_PATH  ./)

SET(PB_INCLUDE_DIR ../protobuf/include)
SET(PB_LIB_DIR ${SRC_DIR}/protobuf/lib)

SET(SLOG_LIB_DIR ${SRC_DIR}/lib)

#设置自己mac os的openssl的路径
if(APPLE)
    SET(OPENSSL_INCLUDE_DIR /usr/local/Cellar/openssl/1.0.2r/include)
    SET(OPENSSL_LIB_DIR /usr/local/Cellar/openssl/1.0.2r/lib)
endif()

ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

INCLUDE_DIRECTORIES(./ ../base ../base/pb/protocol ../base/jsoncpp/ ${PB_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
LINK_DIRECTORIES(${SRC_DIR}/lib ${SLOG_LIB_DIR} ${PB_LIB_DIR} ${OPENSSL_LIB_DIR})

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面，否则会报错
#ADD_LIBRARY(${PROJECTNAME} SHARED/STATIC ${SRC_LIST})
ADD_EXECUTABLE(login_server ${SRC_LIST})

TARGET_LINK_LIBRARIES(login_server base protobuf-lite slog pthread ssl crypto) 