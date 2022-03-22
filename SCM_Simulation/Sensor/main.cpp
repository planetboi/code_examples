#include <iostream>
#include <cstdio>      //for printf
#include <cstdlib>     //for srand(), rand()
#include <unistd.h>
#include <cstring>     //for memset()
#include <sys/socket.h> //for socket(), bind(), sendto(), close()
#include <arpa/inet.h>  //for htons()
#include <ctime>
#include <netdb.h>
#include <thread>
#include <chrono>
#include "utils.h"
#include "sensor.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;


//Sensor hat IP, Typ, Port -> muss als Nachricht ans Lager
//Sensor bekommt Info über Produkte -> Barcode, ID, Datum

/*
1. Create UDP socket
    -> int socket(int domain, int type, int protocol)       //creates an unbound socket in the specified domain, returns FD

2. Server assigns address to the unbound socket
    -> int bind(int sockFD, const struct sockaddr *addr, socklen_t addrLength)

3. Send message to server
    -> ssize_t sendto(int sockFD, const void *buf, size_t bufLength, int flags, const struct sockaddr *destAddr, socklen_t addrLength)
        -> buf: Buffer containing the data to be sent
        -> ssize_t: Returns size in bytes or error value

4. Close socket descriptor and exit     
    -> int close(int fd)
*/

time_t startTime, simulatedTime;

long int packetCounterS = 0;

time_t simulateTime(void *){
    return (900 * (time(nullptr) - startTime)) + startTime;
}

int main() {
    try {

        int sockFD;
        struct sockaddr_in serverAddr;
        //char buffer[MAXLINE];

        //creates socket file descriptor sockFD
        if ((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) <
            0)    //AF_INET: IPv6    SOCK_DGRAM: UDP     protocol = 0 -> default
        {
            perror("socket creation failed");   //PrintERROR
            exit(EXIT_FAILURE);
        }

        //fills memory that serverAddr points to
        memset(&serverAddr, 0, sizeof(serverAddr));

        //get name of server to get ip address
        char* servername = getenv ("LAGERNAME");
        if (servername==nullptr) {
            printf ("No server name sent");
            throw runtime_error ("Sensor Error: Lager name missing");
        }

        int port = 8090;

        //Filling server information
        serverAddr.sin_family = AF_INET;
        //->sin_family: address family for the transport address (should always be set to AF_INET)
        serverAddr.sin_port = htons(port);
        //-> sin_port: transport protocol port number
        //-> htons: host-to-network-short

        //Get server IP address from container name
        //https://cppsecrets.com/users/18989711511997116104103495564103109971051084699111109/C00-Socket-Programming-getaddrinfo.php
        struct addrinfo hints, *res, *p;
        int status;
        char ipstr[INET6_ADDRSTRLEN];

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(servername, nullptr, &hints, &res)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 2;
        }

        for(p = res;p != nullptr; p = p->ai_next) {
            void *addr;

            // get the pointer to the address itself,
            // different fields in IPv4 and IPv6:
            if (p->ai_family == AF_INET) { // IPv4
                auto *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
            } else { // IPv6
                auto *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
            }

            // convert the IP to a string and print it:
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            serverAddr.sin_addr.s_addr = inet_addr(ipstr);


        }

        freeaddrinfo(res); // free the linked list
       // cout << "Set up client to send to ip: " << ipstr << endl;


        time_t timeNow;
        startTime = time(nullptr);

        char* sensorID = getenv ("SENSORID");
        if (sensorID==nullptr) {
            printf ("No sensor id set");
            throw runtime_error ("Missing sensor ID");
        }

        //seed to initialise randomizer
        string seed = "";
        int a = 0;
        while (sensorID[a] != '\0'){
            seed += sensorID[a];
            a++;
        }

srand(stoi(seed));

char* d = getenv ("DURATION");
        if (d==nullptr) {
            printf ("No duration set");
            throw runtime_error ("Missing sensor duration");
        }

            std::this_thread::sleep_for(std::chrono::milliseconds(12000));

        std::cout << "Hallo, ich bin ein Sensor. Ich werde regelmässig Updates an das Lager senden :)" << std::endl;


double duration = atof(d); //eg. run program for ca. 30 seconds
while(difftime(time(NULL), startTime) < duration){
    
time_t start_Time = time(NULL);

//generate the necessary information and send at random times (1 hour = 4 seconds)


            simulatedTime = simulateTime(nullptr);

            /**
             * convert string message to char array
             */
            timeNow = (time(nullptr));
            string actual = ctime(&timeNow);
            string simulated = ctime(&simulatedTime);

           // string info = "The real time now is: " + actual + "\nThe simulated time is: " + simulated + '\n';
           // cout << info;
            /**
             * add information to be sent -> IP, Port, Sensortype, Produktinfo, Timestamp
             */
            string sensorInfo = " ";
            /**
             * get ip information from ip char array
             */
            string ip = "";
            int i = 0;
            while (ipstr[i] != '\0'){
                ip += ipstr[i];
                i++;
            }
            string idstring = "";
            int j = 0;
            while (sensorID[j] != '\0'){
                idstring += sensorID[j];
                j++;
            }

            sensorInfo = ip + ";" + to_string(port) + ";" + setScannerType() + ";" + scanProduct() + ";" + simulated + ";" + idstring + ";";

            char *message = new char [sensorInfo.length()+1];
            strcpy(message, sensorInfo.c_str());

            //Send message to server
            //     sockFD,   container w/ msg,       bufLength,        flags,     const struct sockaddr *destAddr,    socklen_t addrLength
            sendto(sockFD, (const char *) message, strlen(message), 0, (const struct sockaddr *) &serverAddr,
                   sizeof(serverAddr));
            packetCounterS++;
        //    cout << "Number of sensor messages sent: " << packetCounterS << endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));


}

string amount = to_string(packetCounterS);
string msgAmount = "TOTAL;" + amount;
char *msgAmountStr = new char [msgAmount.length()+1];
strcpy(msgAmountStr, msgAmount.c_str());

// cout << "Sensor msg Amount: " << msgAmountStr << endl;

// SENSOR SENDS LAST MESSAGE WITH HOW MANY MESSAGES IT SENT

sendto(sockFD, (const char *) msgAmountStr, strlen(msgAmountStr), 0, (const struct sockaddr *) &serverAddr,
                   sizeof(serverAddr));

        //Close socket descriptor sockFD and exit
        close(sockFD);

cout << "Sensor shutting down...Ran for " << difftime(time(NULL), startTime) << "seconds.\nSensor message counter: " << packetCounterS << endl;

        return 0;
    }

    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    catch (...) {
        std::cerr << "An unknown error has occured" << '\n';
        return -2;
    }
}