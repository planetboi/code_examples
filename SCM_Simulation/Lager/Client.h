#include <iostream>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TTransportUtils.h>
#include "Service.h"
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include "Lager.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace Service;

class Client{

    public:

    Client(Lager * lager, bool * end){

this->lager = lager;
this->endRun = end;

    };
    ~Client(){
        delete lager;
        delete endRun;
    };

    void addStock(std::string & _return, const std::string & product, const int32_t amount, const std::string & warehouse);
    void rpc();

    Lager * lager;
    bool * endRun;

};