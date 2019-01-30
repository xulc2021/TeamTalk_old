cmake_minimum_required(VERSION 2.6)
PROJECT(websocket_server)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
AUX_SOURCE_DIRECTORY(../msg_server/src MSG_SERVER_SRC_LIST)

#SET(LIBRARY_OUTPUT_PATH ./)
SET(EXECUTABLE_OUTPUT_PATH  ./)

# CMAKE的输出路径为TeamTalk\server\src\bin\${name}，${name}代表项目名称，如login_server
## ../../定位到src下，并且只需要连接静态库时，增加该前缀
SET(SRC_DIR  ../../)
SET(PB_INCLUDE_DIR ../protobuf/include)
SET(PB_LIB_DIR ${SRC_DIR}/protobuf/lib)
SET(MSG_SERVER_INCLUDE_DIR ../msg_server)
SET(SLOG_LIB_DIR ${SRC_DIR}/lib)

#设置自己mac os的openssl的路径
if(APPLE)
    SET(OPENSSL_INCLUDE_DIR /usr/local/Cellar/openssl/1.0.2n/include)
    SET(OPENSSL_LIB_DIR /usr/local/Cellar/openssl/1.0.2n/lib)
endif()


if(APPLE)
    SET(SECURITY_LIB_DIR ../base/security/lib/mac)
    SET(OTHER_INCLUDE /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift-migrator/sdk/MacOSX.sdk/usr/include/)
elseif(UNIX)
    SET(SECURITY_LIB_DIR ../base/security/lib/linux)
endif()


ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

INCLUDE_DIRECTORIES(./ ${MSG_SERVER_INCLUDE_DIR} ../base ./websocket ../base/pb/protocol ${PB_INCLUDE_DIR} ../base/security/ ${OPENSSL_INCLUDE_DIR}  ${OTHER_INCLUDE})
LINK_DIRECTORIES(./ ./websocket ${SLOG_LIB_DIR} ${PB_LIB_DIR} ${SECURITY_LIB_DIR} ${OPENSSL_LIB_DIR})

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面，否则会报错
#ADD_LIBRARY(${PROJECTNAME} SHARED/STATIC ${SRC_LIST})

LIST(REMOVE_ITEM MSG_SERVER_SRC_LIST ../msg_server/msg_server.cpp)
#LIST(REMOVE_ITEM MSG_SERVER_SRC_LIST ../msg_server/LoginServConn.cpp)

ADD_EXECUTABLE(websocket_server ${SRC_LIST} ${MSG_SERVER_SRC_LIST} ${WEB_SOCKET_LIST} ${WEB_SOCKET_LIST2} ${WEB_SOCKET_LIST3} ${WEB_SOCKET_LIST4})

TARGET_LINK_LIBRARIES(websocket_server base websocket protobuf-lite slog crypto ssl pthread )
 
