#include <stdio.h>
#include <mosquitto.h>
#include <sstream>
#include "Lager.h"
#include <iostream>
#include <exception>
#include <thread>
#include <chrono>
#include <vector>
#include <cstring>

using namespace std;
using std::cerr;

class MQTT_sub
{
public:
MQTT_sub(bool * end){
    this->endRun = end;
}

~MQTT_sub()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    delete endRun;
    
}


//void on_connect(struct mosquitto *mosq, void *obj, int rc);
//void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
void sub_init();
string getTopic(int i){
    return topicVec[i];
}


struct mosquitto * mosq; 
string clientID;
int subCounter;
bool * endRun;
vector<string> topicVec;
};