#include "MQTT_sub.h"

vector<string> * tv;

void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    if(rc)
    {
        printf("MQTT Sub Error: %d\n", rc);
        return;
    }

char* topics = getenv ("TOPICS");
        if (topics==nullptr) {
            printf ("No topic set");
            throw runtime_error ("MQTT Sub Error: Topic missing");
        }

    char *p = strtok(topics, ";");
    while (p != NULL) {
        //cout << p << endl;
        //string a = p;
        tv->push_back(p);
        p = strtok(NULL, ";");
    }

int i = 0;
    //mosquitto_subscribe(mosq, NULL, topic, 0);
    while(i < tv->size()){
    mosquitto_subscribe(mosq, NULL, (*tv)[i].c_str(), 0);
cout << "Subbed to " << (*tv)[i] << endl;;
i++;
    }

}

int * counter;

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    (*counter)++;
   // cout << "Sub counter: " << (*counter) << endl;
    printf("[MQTT Sub] New message with topic %s:\n%s\n", msg->topic, (char *) msg->payload);
}

void MQTT_sub::sub_init()
{
    int rc;
    counter = &this->subCounter;
    tv = &this->topicVec;

    mosquitto_lib_init();

    char* clientID = getenv ("MQTTSUB");
        if (clientID==nullptr) {
            printf ("No client ID");
            throw runtime_error ("MQTT Sub Error: Client ID missing");
        }
        
    mosq = mosquitto_new(clientID, true, NULL);

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "mqtt", 1883, 60);

    if(rc)
    {
        printf("MQTT Sub Error: Could not connect to the Broker. Error code: %d\n", rc);
        return;
    }

    printf("[MQTT Sub] Connected to the broker!\n");

  //mosquitto_loop_forever(mosq, 5000, 1);
  mosquitto_loop_start(mosq);
  while(!(*endRun)){

  }
  mosquitto_loop_stop(mosq, true);
  if(*endRun){
                cout << "MQTT Sub thread ending" << endl;
            }

}

