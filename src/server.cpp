/*
 * server.cpp
 *
 *  Created on: 18 ott 2018
 *      Author: skafo
 */


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;
#include "constants.h"
#include "server.h"

Server::Server(){
	portNumber  = Constants::DEFAULT_PORT_NUMBER;

    initSockAddr();
	initConnection();
}

Server::Server(int portNumber){
    this->portNumber = portNumber;
    initSockAddr();
	initConnection();
}

void Server::initSockAddr(){
    memset(&serverAddress, '0', sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(portNumber );
}

int Server::initConnection(){
    int opt = 1;

	availableConnections = Constants::MAX_CONNECTIONS_NUMBER;

    // Creating socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, Constants::MAX_CONNECTIONS_NUMBER) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int Server::acceptConnections(int exitCondition){
	int newSocket;
    int addrlen = sizeof(serverAddress);

    int nextConnection = Constants::MAX_CONNECTIONS_NUMBER - availableConnections;

    char *ip = inet_ntoa(serverAddress.sin_addr);
    unsigned int port = ntohs(serverAddress.sin_port);

    cout << "Server, accepting nextConnections at address " << ip << " and port " << port << endl;

    while( !exitCondition ){
    	if(availableConnections == 0)
    		continue;

		if ((newSocket = accept(serverFd, (struct sockaddr *)&serverAddress, (socklen_t*)&addrlen))<0){
			perror("not accept");
			return EXIT_FAILURE;
		}
        cout << "connessione accettata"<< endl;
		clientSockFd[nextConnection] = newSocket;
		availableConnections--;

	}
    return newSocket;
}

