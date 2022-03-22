//
// Created by Cheryl on 09.11.2021.
//

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include "utils.h"
#include <vector>
#include "Lager.h"
#include <errno.h>

using namespace std;

#ifndef LAGER_SENSORLISTENER_H
#define LAGER_SENSORLISTENER_H


class SensorListener {

public:

    long int packetCounterL = 0;
    long int sensorAmount = 0;

    SensorListener(Lager * lager, bool * end) {

        this->lager = lager;
        this->endRun = end;

    };

    ~SensorListener(){
        delete lager;
        delete endRun;
    };

    /**
     * sensor information is received through an active UDP socket
     */
    int receiveSensorInformation();

    /**
     * received sensor information is rearranged into sensorMessage struct
     * @param buffer
     * @return sensorMessage struct containing the received information
     */
    void processSensorInformation(char *buffer);

    /**
     * the received sensor information is printed on the console
     */
    void printSensorInformation(string message);

    /**
     * the received information is added to a log text file
     */
    void saveSensorInformation(string message);

    void adjustStock(sensorMessage &message);

    Lager * lager;
    bool * endRun;
};


#endif //LAGER_SENSORLISTENER_H
