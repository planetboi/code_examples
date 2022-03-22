#include "MQTT_pub.h"

int MQTT_pub::init(){

int rc;

mosquitto_lib_init();

clientID = getenv ("LAGERID");
        if (clientID==nullptr) {
            printf ("No client ID");
            throw runtime_error ("MQTT Pub Error: Client ID missing");
        }

this->mosq = mosquitto_new(clientID, true, NULL);

//cout << "topic " << clientID << endl; 

rc = mosquitto_connect(mosq, "mqtt", 1883, 60);
//if no errors, function will return 0
if(rc != 0){
    printf("[MQTT Pub] Error. Could not connect to broker. Error code: %d\n", rc);
    mosquitto_destroy(mosq);
    return -1;
}
printf("[MQTT Pub] Connected to the broker!\n");

return 0;

}

void MQTT_pub::publish(){
//client instance, messageID, topic, payload length, message (string)

try{

stringstream msg;

ifstream mqttData;
string line;

this->lager->mtx2->lock();

    mqttData.open("overview.txt");

    if (!mqttData) {
        cerr << "Error: file could not be opened" << endl;
    } else {
    
    int i = 0;
    while (!mqttData.eof())
        {
            getline(mqttData, line);
            switch(i){
                case 0:
                msg << "Timestamp: " << line << "\n";
                break;
                case 1:
                msg << "Einlieferungen: " << line << "\n";
                break;
                case 2:
                msg << "Auslieferungen: " << line << "\n";
                break;
                case 3:
                msg << "Bestand\nSchuhe: " << line << "\n";
                break;
                case 4:
                msg << "Hose: " << line << "\n";
                break;
                case 5:
                msg << "Jeans: " << line << "\n";
                break;
                case 6:
                msg << "Shirt: " << line << "\n";
                break;
                case 7:
                msg << "Pullover: " << line << "\n";
                break;
                case 8:
                msg << "Hemd: " << line << "\n";
                break;
                case 9:
                msg << "Kleid: " << line << "\n";
                break;
                case 10:
                msg << "Rock: " << line << "\n";
                break;
                case 11:
                msg << "Jacke: " << line << "\n";
                break;
                case 12:
                msg << "Mantel: " << line << "\n";
                break;
                default:
                break;
            }
            i++;

        }
    
    }

    mqttData.close();

    this->lager->mtx2->unlock();

//cout << "[MQTT] Publishing....\n" << msg.str().c_str() << endl;
string clientStr = clientID;

int msglen = sizeof(char) * msg.str().length();
//cout << "Size of string is " << msglen << endl;
//topic is ClientID/container name
mosquitto_publish(mosq, NULL, clientID, msglen, msg.str().c_str(), 0, false);
//mosquitto_publish(mosq, NULL, clientID, 6, "Hello", 0, false);
pubCounter++;
//cout << "Pub counter: " << pubCounter << endl;
cout << "Published to topic: " << clientStr << endl << endl;
}
catch(exception &e)
{
		cout << "ERROR: " << e.what() << endl;
}

}

void MQTT_pub::loop(){

while(true){

    if(*endRun){
                cout << "MQTT Pub thread ending" << endl;
                return;
            }
    this->publish();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

}
}