#!/bin/sh
export LD_LIBRARY_PATH=/usr/local/lib/:${LD_LIBRARY_PATH}
cd ../usr/src/lager
g++ -std=c++11 -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H -Wall -I/usr/local/include/thrift SensorListener.cpp Webserver.cpp HTTPHandler.cpp WebpageGenerator.cpp Lager.cpp Lager.h Service_server.cpp service_types.cpp service_types.h Service.cpp Service.h Client.h Client.cpp MQTT_pub.cpp MQTT_pub.h main.cpp MQTT_sub.h MQTT_sub.cpp -o lager -L/usr/local/lib/ -pthread -ldl -lthrift -lssl -lmosquitto
./lager