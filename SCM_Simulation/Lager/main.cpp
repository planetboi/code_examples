#include <iostream>
#include "SensorListener.h"
#include "Webserver.h"
#include <thread>


#include "Service.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/Thrift.h>
#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "service_types.h"
#include "Service_server.cpp"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <vector>
#include <sstream>
#include <ctime>
#include <mutex>
#include "Client.h"
#include "MQTT_pub.h"
#include "MQTT_sub.h"
#include <ctime>
#include <sstream>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace Service;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::Service;

mutex * mtx = new mutex();
mutex * mtx2 = new mutex();

void stopServer(TSimpleServer * server, bool * end){
    while(true){
if(*end){
    server->stop();
    cout << "RPC server was stopped\n";
    return;
     }
    }  
}

void rpcServer(bool * end){

//get port
        char* serverPortStr = getenv ("SERVERPORT");
        if (serverPortStr==nullptr) {
            printf ("No port number sent");
            throw runtime_error ("Sensor Error: Port number missing");
        }
        int serverPort = atoi(serverPortStr);

    ::std::shared_ptr<ServiceHandler> handler(new ServiceHandler());
    ::std::shared_ptr<TProcessor> processor(new ServiceProcessor(handler));
    ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(serverPort));
    ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    std::thread stop(stopServer, &server, end);
    server.serve();    
    stop.join();
}

void tcp(Webserver * webserver) {
    webserver->runServer();
}

void udp(SensorListener * listener) {
    listener->receiveSensorInformation();
}

void rpcClient(Client * client){
    
client->rpc();
       
}

void mqttPub(MQTT_pub * pub){

pub->init();
pub->loop();

}

void mqttSub(MQTT_sub * sub){
    sub->sub_init();
}

int main() {

bool * endRun = new bool();
*endRun = false;

    try {
       // std::cout << "Hallo, ich bin ein Lager. Ich werde Sensor-Informationen erhalten und diese an den Browser übermitteln :)"
       //           << std::endl;

        Lager * lager = new Lager(mtx, mtx2);
        Webserver * webserver = new Webserver(lager, endRun);
        SensorListener * listener = new SensorListener(lager, endRun);
        Client * client = new Client(lager, endRun);
        MQTT_pub * pub = new MQTT_pub(lager, endRun);
        MQTT_sub * sub = new MQTT_sub(endRun);

char* d = getenv ("DURATION");
        if (d==nullptr) {
            printf ("No duration set");
            throw runtime_error ("Missing sensor duration");
        }

double duration = atof(d); //run program for ca. 30 seconds
time_t startTime = time(NULL);

std::thread sensorlistener(udp, listener);
        std::thread tcpserver(tcp, webserver);
        std::thread service(rpcServer, endRun);
        std::thread publisher(mqttPub, pub);
        std::thread subscriber(mqttSub, sub);
        std::thread rpc(rpcClient, client);

while(difftime(time(NULL), startTime) < duration){}
        
        *endRun = true;
        rpc.join();
        publisher.join();
        subscriber.join();
        service.join();
        sensorlistener.join();
        tcpserver.detach();

        stringstream ss;
        
        double pkgPercent = 0;
        pkgPercent = ((double)listener->packetCounterL/(double)listener->sensorAmount) * 100;

        ss << "\nOVERVIEW\n";
        ss << "[Sensor Listener] message counter: " << listener->packetCounterL << endl;
        ss << "Sensor said it sent " << listener->sensorAmount << " packages\n";
        ss << pkgPercent << "% of sensor packages were received.\n";
        ss << "[MQTT Publisher] message counter: " << pub->pubCounter << endl;
        ss << "[MQTT Subscriber] message counter: " << sub->subCounter << endl;
        
        if((pub->pubCounter) == (sub->subCounter))
        {
            ss << "MQTT TEST: ALL MESSAGES RECEIVED" << endl;
        }
        else
        {
            ss << "MQTT TEST: " << ((pub->pubCounter) - (sub->subCounter)) << "MESSAGES WERE NOT RECEIVED" << endl;
        }

        ss << "Topics subscribed to: ";
        int a = 0;
        while(a < sub->topicVec.size()){
            ss << sub->getTopic(a) << "\t";
            a++;
        }
        ss << "\nTimestamp: " << lager->timestamp;
        ss << "Schuhe: " << lager->schuhe << " Hose: " << lager->hose << " Jeans: " << lager->jeans;
        ss << " Shirt: " << lager->shirt << " Pullover: " << lager->pullover;
        ss << " Hemd: " << lager->hemd << "\nKleid: " << lager->kleid << " Rock: " << lager->rock << " Jacke: " << lager->jacke << " Mantel: " << lager->mantel << endl;
        ss << "Lager was online for " << difftime(time(NULL), startTime) << " seconds.\n";
        
        cout << ss.str();

        ofstream summary;
    summary.open("summary.txt");
    if (summary.is_open()) {
      summary << ss.str();
        }
        summary.close();

        cout << "Summary saved. LAGER SHUTDOWN" << endl;

    }

    catch (const std::exception &e) {
        cout << "ERROR in main::" << e.what() << endl;
        return -1;
    }
    return 0;
}
