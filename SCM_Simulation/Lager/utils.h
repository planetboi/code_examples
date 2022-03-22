//
// Created by Cheryl on 09.11.2021.
//

#include <string>

using namespace std;

#ifndef LAGER_UTILS_H
#define LAGER_UTILS_H

struct sensorMessage {
    string sensorID;
    string sensorIP;
    string sensorPort;
    string sensorType;
    string scannedTime;
    string scannedProduct;
};

#endif //LAGER_UTILS_H
