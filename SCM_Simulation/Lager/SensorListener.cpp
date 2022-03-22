//
// Created by Cheryl on 09.11.2021.
//

#include "SensorListener.h"

#define MAXLINE 2000
#define EINLIEFERUNG "Einlieferung"
#define AUSLIEFERUNG "Auslieferung"
#define INVENTARISIERUNG "Inventarisierung"


int SensorListener::receiveSensorInformation() {

    try {

        /**
         * creates a socket
         * AF_INET = socket_family -> IPv4
         * SOCK_DGRAM = socket_type -> UDP
         * 0 = protocol
         */
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            cout << "The socket creation failed." << endl;
            return -1;
        }

        timeval tv;
tv.tv_sec = 20;
tv.tv_usec = 0;
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,(const char*) &tv, sizeof(tv));

        /**
         * struct to hold server and client address
         */
        struct sockaddr_in servaddr, cliaddr;

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        int port = 8090;

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);

        int bindResult = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        if (bindResult < 0) {
            cout << "The socket bind failed." << endl;
            return -1;
        }

        unsigned int n, len;
        len = sizeof(cliaddr);
        char buffer[MAXLINE];

        cout << "[Sensor Listener] Online! Warte auf Sensor-Informationen..." << endl;
        while (true) {

            if(*endRun){
                cout << "Sensorlistener thread ending" << endl;
                return 0;
            }

            n = recvfrom(sockfd, (char *) buffer, MAXLINE,
                         0, (struct sockaddr *) &cliaddr,
                         &len);
            
            if (n < 0 || errno != 0) {
                if((errno == 11)){
                if(*endRun){
                cout << "Sensorlistener thread ending" << endl;
                close(sockfd);
                return 0;
            }
            cout << "Socket reached timeout" << endl;

                }
                else {
                    perror("Message receive failed.");
                return -1;
                }
            }
            else{
                
                            buffer[n] = '\0';
            // cout << buffer << endl;
            packetCounterL++;
           //cout << "Anzahl der erhaltenen Nachrichten: " << packetCounterL << endl;
        
            processSensorInformation(buffer);
            }
                
        
        }
        

    }
    catch (const exception &e) {

        cout << "Exception: " << e.what();
    }
    return 0;

}

string sensorMessageToString(sensorMessage &message) {

    string s = "Timestamp: " + message.scannedTime
               + "Sensor ID: " + message.sensorID
               + "\nIP: " + message.sensorIP
               + "\nPort: " + message.sensorPort
               + "\nSensor Typ: " + message.sensorType
               + "\nProdukt: " + message.scannedProduct + "\n";

    return s;
}

string sensorMessageToLog(sensorMessage &message) {

    string s = message.scannedTime
               + message.sensorID
               + "\n" + message.sensorType
               + "\n" + message.scannedProduct + "\n";

    return s;
}

void SensorListener::adjustStock(sensorMessage &message){

    string type = message.sensorType;
    string product = message.scannedProduct;
    string timestamp = message.scannedTime;
    lager->timestamp = timestamp;

    if(type == EINLIEFERUNG){
        lager->changeStock(product, 1);
        // add timestamp
        lager->saveStock(timestamp);
        lager->in = lager->in + 1;
    }
    else if(type == AUSLIEFERUNG){
        lager->changeStock(product, -1);
        lager->saveStock(timestamp);
        lager->out = lager->in + 1;
    }

    lager->saveOverview();
//  int i = lager->countStock(product);
    //    cout << "Produkt: " << product << " Anzahl: " << i << endl;

}

void SensorListener::processSensorInformation(char *buffer) {

    //cout << "Processing sensor info...." << endl;

    vector<string> data;

    char *p = strtok(buffer, ";");
    while (p != NULL) {
        //cout << p << endl;
        data.push_back(p);
        p = strtok(NULL, ";");
    }

    if (data.size() < 6) {

if(data[0] == "TOTAL"){

this->sensorAmount += stol(data[1]);
packetCounterL--;
return;
}
else{
throw invalid_argument("Received too few arguments from sensor.");
}
        
    }

    sensorMessage message;
    message.sensorIP = data[0];
    message.sensorPort = data[1];
    message.sensorType = data[2];
    message.scannedProduct = data[3];
    message.scannedTime = data[4];
    message.sensorID = data[5];

    adjustStock(message);
    string s = sensorMessageToString(message);
    string l = sensorMessageToLog(message);
    printSensorInformation(s);
    saveSensorInformation(l);
}


void SensorListener::printSensorInformation(string message) {

    cout << message << '\n';

}

void SensorListener::saveSensorInformation(string message) {

    ofstream logfile;
    logfile.open("sensorLog.txt", ios::app);
    if (logfile.is_open()) {
        logfile << message << ";\n";
        logfile.close();
        //cout << "Information saved to log file" << endl;
    } else {
        cout << "Unable to open log file." << endl;
    }

}

