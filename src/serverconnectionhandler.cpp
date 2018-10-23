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


#include <vector>
#include <thread>

#include "constants.h"
#include "serverconnectionhandler.h"

ServerConnectionHandler::ServerConnectionHandler() {
	portNumber = Constants::DEFAULT_PORT_NUMBER;

	initSockAddr();
	initConnection();
}

ServerConnectionHandler::ServerConnectionHandler(int portNumber) {
	this->portNumber = portNumber;
	initSockAddr();
	initConnection();
}

void ServerConnectionHandler::initSockAddr() {
	memset(&serverAddress, '0', sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(portNumber);
}

int ServerConnectionHandler::initConnection() {
	int opt = 1;
	//connessioni disponibili
	availableConnections = Constants::MAX_CONNECTIONS_NUMBER;

	// Creating socket file descriptor
	if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (bind(serverFd, (struct sockaddr *) &serverAddress,
			sizeof(serverAddress)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(serverFd, Constants::MAX_CONNECTIONS_NUMBER) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int ServerConnectionHandler::acceptConnections(int exitCondition) {
	// Stabilisce connessione  con il client
	int newSocket;
	int addrlen = sizeof(serverAddress);

	thread *tmpThread = NULL;

	int nextConnection = Constants::MAX_CONNECTIONS_NUMBER
			- availableConnections;

	char *ip = inet_ntoa(serverAddress.sin_addr);
	unsigned int port = ntohs(serverAddress.sin_port);

	cout << "Server, accepting nextConnections at address " << ip
			<< " and port " << port << endl;

	while (!exitCondition) {
		cout<<"ASPETTO nuova connessione"<<endl;

		if (availableConnections == 0)
			continue;

		if ((newSocket = accept(serverFd, (struct sockaddr *) &serverAddress,
				(socklen_t*) &addrlen)) < 0) {
			// Se la creazione del socket da problemi  ritorna errore
			perror("not accept");
			return EXIT_FAILURE;
		}
		// Se la connessione è andata a buon fine mi crea il threa che gestiche i dati
			cout<<"nuova connessione"<<endl;


		//thread thread1(ServerConnectionHandler::foo, &newSocket);

		tmpThread = new thread(ServerConnectionHandler::foo, &newSocket);

		tidVector.push_back(tmpThread);
//		if(tmpThread == NULL)
//			return 0;

		cout << "connessione accettata" << endl;

		//tidVector.push_back(tmpThread);
		clientSockFd[nextConnection] = newSocket;
		availableConnections--;

		cout << "FINITO" << endl;
	}
	return newSocket;
}

void ServerConnectionHandler::foo(int* arg){

	 while(1){
		cout << "lettura dal client"<<*arg << endl;
		this_thread::sleep_for(1s);
	 }
	return;
}


int ServerConnectionHandler::removeConnection(int idConnection) {

	//ritorna 1 se la connessione  è andata buon fine

//	int serachresult = ricerca(idConnection);
//
//	if (serachresult < 0)
//		return 0;
//
//	//mette 0 nel corrispettivo elemento dell'array dei socket
//
//	clientSockFd[idConnection] = 0;
//	//chiude il tread
//	//mette 0 nell'array dei tread_i
//	thread_id[idConnection] = 0;
//	//incrementa le disponibili
//	availableConnections++;
}

int ServerConnectionHandler::ricerca(int val) {
//rest6ituisce la posizione dell'elemento nell'array -1 altrimenti

	return 1;
}

