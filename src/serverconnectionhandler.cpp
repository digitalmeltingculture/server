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
#include <math.h>
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
		cout << "ASPETTO nuova connessione" << endl;

		if (availableConnections == 0)
			continue;

		if ((newSocket = accept(serverFd, (struct sockaddr *) &serverAddress,
				(socklen_t*) &addrlen)) < 0) {
			// Se la creazione del socket da problemi  ritorna errore
			perror("not accept");
			return EXIT_FAILURE;
		}
		// Se la connessione è andata a buon fine mi crea il threa che gestiche i dati
		cout << "nuova connessione" << endl;

		if ((tmpThread = new thread(ServerConnectionHandler::foo, newSocket, this))
				== NULL)
			return 0;

		tidVector.push_back(tmpThread);

		cout << "connessione accettata" << endl;

		//tidVector.push_back(tmpThread);
		clientSockFd[nextConnection] = newSocket;
		availableConnections--;

		cout << "FINITO" << endl;
	}
	return newSocket;
}

void ServerConnectionHandler::foo(int arg, ServerConnectionHandler* instance) {
	int size = 512;
	int arg2 = arg;
	int prova = -1;
	while (prova == -1) {
		cout << "lettura dal client" << arg << endl;

		prova = receive(size, arg);
		this_thread::sleep_for(5s);
		cout << "la stringa ricevuta" << prova << endl;
	}
	return;
}

int ServerConnectionHandler::removeConnection(int idConnection) {
	//ritorna 1 se la rimozione è andata buon fine

	int serachresult = ricerca(idConnection);

	if (serachresult < 0)
		return 0;

//	//mette 0 nel corrispettivo elemento dell'array dei socket

	clientSockFd[idConnection] = 0;
//	//chiude il tread
//	//mette 0 nell'array dei tread_i
	tidVector[idConnection] = 0;
//	//incrementa le disponibili
	availableConnections++;
	return 1;
}

int ServerConnectionHandler::ricerca(int val) {
//rest6ituisce la posizione dell'elemento nell'array -1 altrimenti
	int i;

	for (i = 0; i < Constants::MAX_CONNECTIONS_NUMBER; i++) {

		if (clientSockFd[i] == val) {
			return 1;

		}

	}
	return 0;
}

/**
 Receive data from the connected host
 */

int ServerConnectionHandler::receive(int size, int sock) {
	string reply;
	char buffer[size];
	int res = 0;
	string temp;
	size_t byteSent;

	//Receive a reply from the server
		if (recv(sock, buffer, size, 0) < 0) {
			perror("Receive failed : ");
			res = -1;
		}
	temp.assign(buffer,1);
	res = atoi(temp.c_str());
	string delimiter = "|";
	char* token = strtok (buffer,"|");
	cout<<token<<endl;
	switch (res) {

	case 0:
		//end of connection
		if ((byteSent = this->receiveClose(token,sock)) == -1)
			cout << Constants::ERROR_RECEIVE_END_CONNECTION_MSG << endl;

		break;

	case 1:
		// byte
//		if ((byteSent = this->sendByte(buffer)) == -1)
//				cout << Constants::ERROR_RECEIVE_INTEGER << endl;

		break;

	case 2:
		//boolean

		break;

	case 3:
		//Carattere

		if ((byteSent = this->receiveChar(token)) == -1)
			cout << Constants::ERROR_RECEIVE_CHAR_MSG << endl;

		break;
	case 4:
		//int

		if ((byteSent = this->receiveInt(token)) == -1)
			cout << Constants::ERROR_RECEIVE_INTEGER_MSG << endl;

		break;
	case 5:
		//float

		if ((byteSent = this->receiveFloat(token)) == -1)
			cout << Constants::ERROR_RECEIVE_FLOAT_MSG << endl;

		break;
	case 8:
		//double

		if ((byteSent = this->receiveDouble(token)) == -1)
			cout << Constants::ERROR_RECEIVE_DOUBLE_MSG << endl;

		break;
	case 9:
		//Stringa
		if ((byteSent = this->receiveString(token)) == -1)
			cout << Constants::ERROR_RECEIVE_STRING_MSG << endl;

		break;
		//Caso default
	default:
		cout << "Tipo non ammesso" << endl;
		res = -1;
		break;

	}

	cout << "Il dato inviato è " << byteSent << endl;

	return res;
}

size_t ServerConnectionHandler::receiveChar(const char* arg){
	size_t ris;
ris =0;
return ris;
}
//Function receive Close

size_t ServerConnectionHandler::receiveClose(const char* arg,int sock){

	size_t res =0;

	if(close(sock)!=0){

		res =-1;
	}
	pthread_exit(&tidVector);
	availableConnections++;


return res;
}

//Function receive Double

size_t ServerConnectionHandler::receiveDouble(const char* arg){

	size_t ris ;
ris  =atof(arg);

return ris;
}

// Function receive Float

size_t ServerConnectionHandler::receiveFloat(const char* arg){
	size_t ris ;

ris  =atof(arg);

return ris;
}
//  Function receive Integer

size_t ServerConnectionHandler::receiveInt(const char* arg){

	size_t ris ;
	ris =atoi(arg);

return ris;
}
size_t ServerConnectionHandler::receiveString(const char* arg){

	size_t ris ;


	ris =0;
	return ris;
}
