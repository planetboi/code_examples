//
// Created by Cheryl on 22.11.2021.
//

#include "Webserver.h"

int Webserver::runServer() {

    /**
     * create socket
     * int sockfd = socket(domain, type, protocol)
     * https://www.geeksforgeeks.org/socket-programming-cc/
     */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("server: can't open stream socket");


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_TCP_PORT); //PORT

    /**
     * int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
     */
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        perror("server: can't bind local address");

    /**
     * int listen(int sockfd, int backlog);
     * backlog: maximum length to which the queue of pending connections for sockfd may grow
     */
    listen(sockfd, 5);

    cout << "[TCP Server] Online! warte auf Browseranfragen...." << endl;

    // Wait for a connection from a client process

    clilen = sizeof(cli_addr);

    while (true) {

        //creates new connected socket
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        if (newsockfd < 0)
            perror("server: accept error");
        if ((childpid = fork()) < 0)
            perror("server: fork error");
        else if (childpid == 0) { // child process
            close(sockfd); // close original socket
            cout << "[TCP Server] New connection request....." << endl;
            processRequest(newsockfd); // process the request
            exit(0);
        }
        close(newsockfd); // parent process

    }
                return 0;

}

int Webserver::processRequest(int socket) {

    char buffer[BUFFERSIZE];
    string output = "";
    int size;
    HTTPHandler handler;

    while (true) {
        int bytesReceived = recv(socket, buffer, sizeof(buffer), 0);

        if (bytesReceived == -1) {
            cerr << "Error in recv(). Quitting" << endl;
            exit(EXIT_FAILURE);
        }

        if (bytesReceived == 0) {
            cout << "Client disconnected " << endl;
            return -1;
        } else {

            //full end of message \r\n\r\n
            handler.parseHTTPRequest(buffer);
            handler.generateHTTPResponse();
            output = handler.response;


            size = output.size() + 1;
            send(socket, output.c_str(), size, 0);

            fprintf(stderr, "Server: got message: `%s'\n", buffer);
            return 0;

        }

        // cout << string(buffer, 0, bytesReceived) << endl;
    }

}