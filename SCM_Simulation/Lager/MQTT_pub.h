#include <stdio.h>
#include <mosquitto.h>
#include <sstream>
#include "Lager.h"
#include <iostream>
#include <exception>
#include <thread>
#include <chrono>
#include <string>

using namespace std;
using std::cerr;
using std::string;

class MQTT_pub{

public:
MQTT_pub(Lager * lager, bool * end){
this->lager = lager;
this->endRun = end;
}
~MQTT_pub(){
mosquitto_disconnect(mosq);
mosquitto_destroy(mosq);
mosquitto_lib_cleanup();
delete lager;
delete endRun;
}

int init();
void publish();
void loop();

struct mosquitto * mosq; 

Lager * lager;
char* clientID;
int pubCounter = 0;
bool * endRun;
};