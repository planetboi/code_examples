//
// Created by Cheryl on 22.11.2021.
//

#ifndef LAGER_HTTPHANDLER_H
#define LAGER_HTTPHANDLER_H

#include <cstdio>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include "WebpageGenerator.h"
#include <ctype.h>
#include <cstring>
#include <fstream>

using std::ifstream;

using namespace std;

#define BUFFERSIZE  1024

class HTTPHandler {

public:
    HTTPHandler() {}

    ~HTTPHandler() = default;

    void parseHTTPRequest(const char *buffer);

    void generateHTTPResponse();

    vector<sensorMessage *>
    fetchSensorData(); //z.B einzeln: /sensors/id/index /sensors/type/index alle: sensors/all Historie: /sensors/id /sensors/type
    string generateStatusPage(vector<sensorMessage *> s);
    vector<string> getStock();
    void processURL();

    string response = "", url = "", userAgent = "", sensorID = "", sensorType = "", sensorProduct = "";
    vector<string> urlInfo;
    bool showInventory = false;

};


#endif //LAGER_HTTPHANDLER_H
