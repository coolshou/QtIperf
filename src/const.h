#ifndef CONST_H
#define CONST_H

#define QTIPERF "QtIperf"
const int IPERF_BCAST_PORT=58080; //broadcast port
const int JCONRPC_PORT=58082;
//const int MAIAXMLRPC_PORT=58082;
//iperf port
const int BASE_PORT=5201;


#define DEFAULT_PAIR_CFG "\
<pair managerIP2=\"192.168.0.1\" endpoint1=\"192.168.0.1\" idx=\"0\" managerIP1=\"192.168.0.1\" endpoint2=\"192.168.0.1\"> \
    <port name=\"-p\" value=\"5201\"/> \
    <format name=\"-f\" value=\"k\"/> \
    <verbose name=\"-V\" value=\"1\"/> \
    <json name=\"-J\" value=\"0\"/> \
    <forceflush name=\"--forceflush\" value=\"1\"/> \
    <debug name=\"-d\" value=\"0\"/> \
    <bind name=\"-B\" value=\"\"/> \
    <client> \
      <ServerIP name=\"-c\" value=\"192.168.0.1\"/> \
      <testMode name=\"TCP\" value=\"\"/> \
      <testMethod name=\"-t\" value=\"10\"/> \
      <omit name=\"-O\" value=\"0\"/> \
      <buffer name=\"-l\" value=\"0\"/> \
      <window name=\"-w\" value=\"0\"/> \
      <reverse name=\"-R\" value=\"0\"/> \
      <bitrate name=\"--bitrate\" value=\"0\"/> \
    </client> \
</pair>"


#endif // CONST_H
