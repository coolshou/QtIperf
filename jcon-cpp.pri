QT += core widgets network websockets testlib

CONFIG += c++14

INCLUDEPATH += \
    $$PWD/jcon-cpp/src/jcon/

HEADERS += \
    $$PWD/jcon-cpp/src/jcon/jcon_assert.h \
    $$PWD/jcon-cpp/src/jcon/jcon.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_client.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_debug_logger.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_endpoint.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_error.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_file_logger.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_logger.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_request.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_result.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_server.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_socket.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_success.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_tcp_client.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_tcp_server.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_tcp_socket.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_websocket_client.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_websocket_server.h \
    $$PWD/jcon-cpp/src/jcon/json_rpc_websocket.h \
    $$PWD/jcon-cpp/src/jcon/string_util.h

SOURCES += \
    $$PWD/jcon-cpp/src/jcon/json_rpc_client.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_debug_logger.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_endpoint.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_error.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_file_logger.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_logger.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_request.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_server.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_success.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_tcp_client.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_tcp_server.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_tcp_socket.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_websocket_client.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_websocket_server.cpp \
    $$PWD/jcon-cpp/src/jcon/json_rpc_websocket.cpp \
    $$PWD/jcon-cpp/src/jcon/string_util.cpp
