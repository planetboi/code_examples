//
// Created by Cheryl on 22.11.2021.
//

#ifndef LAGER_WEBSERVER_H
#define LAGER_WEBSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include "HTTPHandler.h"
#include "Lager.h"

using namespace std;

#define SERV_TCP_PORT   50000
#define BUFFERSIZE      1024

class Webserver {
public:
    Webserver(Lager * lager, bool * end){

        this->lager = lager;
        this->endRun = end;

    }
    ~Webserver(){
        delete lager;
        delete endRun;
    };
    int runServer();

    int processRequest(int socket);

    int sockfd, newsockfd, childpid;
    struct sockaddr_in cli_addr, serv_addr;
    socklen_t clilen;

    Lager * lager;
    bool * endRun;

};


#endif //LAGER_WEBSERVER_H
