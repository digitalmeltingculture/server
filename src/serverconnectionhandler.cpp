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
#include <map>
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
		perror(Constants::ERROR_CREATE_SOCKET_MSG);
		exit(EXIT_FAILURE);
	}

	if (bind(serverFd, (struct sockaddr *) &serverAddress,
			sizeof(serverAddress)) < 0) {
		perror(Constants::ERROR_BIND_FAILED_MSG);
		exit(EXIT_FAILURE);
	}

	if (listen(serverFd, Constants::MAX_CONNECTIONS_NUMBER) < 0) {
		perror(Constants::ERROR_LISTEN_FAILURE);
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

	cout << Constants::SERVER_ACCEPT<< ip
			<< " and port " << port << endl;

	while (!exitCondition) {

		if (availableConnections == 0)
			continue;

		if ((newSocket = accept(serverFd, (struct sockaddr *) &serverAddress,
				(socklen_t*) &addrlen)) < 0) {
			// Se la creazione del socket da problemi  ritorna errore
			perror(Constants::SERVER_NOT_ACCEPT);
			return EXIT_FAILURE;
		}
		// Se la connessione è andata a buon fine mi crea il thread che gestiche i dati
		cout << "nuova connessione" << endl;

		if ((tmpThread = new thread(ServerConnectionHandler::threadHandler,
				newSocket, this, exitCondition)) == NULL)
			return 0;

		cout << "connessione accettata" << endl;

		tidVector.push_back(tmpThread);
		sockThreadMap.insert(std::pair<int, thread*>(newSocket, tmpThread));

		clientSockFd[nextConnection] = newSocket;
		availableConnections--;
	}
	return newSocket;
}

void ServerConnectionHandler::threadHandler(int sockFd,
		ServerConnectionHandler* instance, int exitCondition) {
	void* buffer[Constants::MAX_BUFFER_SIZE];
	ssize_t byteReceived = -1;
	int gotError = 0;

	while (!exitCondition && !gotError) {
		cout<<"arrivo nel threadhandler\n" <<endl;

		if ((byteReceived = recv(sockFd, buffer, 18, 0)) < 0) {
			perror(Constants::ERROR_FAILED_RECEIVE);

			//PENSARE COSA FARE QUANDO C'È un errore in lettura
			gotError = 1;
		}
		cout<<byteReceived <<endl;
		if (!gotError) {
			cout<<"Received ok" <<endl;

			instance->parseReceivedData(sockFd, buffer, byteReceived);
		}
		this_thread::sleep_for(3s);
	}

	//STAMPA PRIMA DI USCIRE
	return;
}

/**
 Receive data from the connected host
 */

int ServerConnectionHandler::parseReceivedData(int sock, void * inputBuffer, size_t size) {

	string tmpPrefix;

	size_t byteSent,posTrovata;

	int intPrefix = -1;
	string buffer;
	const char * delimiter = Constants::DELIMITER;

	//Receive a reply from the server
	if(inputBuffer==NULL){
		cout<<"Buffer null" <<endl;
		return -1;
	}
	buffer = (char *) inputBuffer;

	tmpPrefix.assign(buffer, 1);
	if (!isdigit(buffer[0])) {
		cout << "Error parse " << endl;
		return -1;
	}

	intPrefix = atoi(tmpPrefix.c_str());

	//usare la funzione substr per dividere
	posTrovata=buffer.find(delimiter);
	const char* token = buffer.substr(posTrovata).c_str();
	cout << token<< endl;

	switch (intPrefix) {

	case 0:
		//end of connection
		if ((byteSent = this->closeAndRemoveConnection(sock, token)) == -1)
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
		intPrefix = -1;
		break;

	}

	cout << "Il dato inviato è " << byteSent << endl;

	return intPrefix;
}

size_t ServerConnectionHandler::receiveChar(const char* arg) {
	size_t ris;
	ris = 0;
	return ris;
}
//Function receive Close

int ServerConnectionHandler::closeAndRemoveConnection(int sock,
		const char* arg) {
	map<int, thread*>::iterator it;

	if (close(sock) != 0) {
		//stampa errore
		return -1;
	}

	it = sockThreadMap.find(sock);

	if (it == sockThreadMap.end()) {
		//STAMPA ERRORE
		return -1;
	}

	sockThreadMap.erase(it);
	//DA SINCRONIZZARE SEZIONE CRITICA
	availableConnections++;

	return 1;
}

//Function receive Double

double ServerConnectionHandler::receiveDouble(const char* arg) {

	double value;

	if (arg == NULL)
		return -1;
	if (isdigit(arg[0]))
		return -1;

	value = atof(arg);

	return value;
}

// Function receive Float

float ServerConnectionHandler::receiveFloat(const char* arg) {

	float value;

	if (arg == NULL)
		return -1;
	if (isdigit(arg[0]))
		return -1;

	value = atof(arg);

	return value;
}
//  Function receive Integer

int ServerConnectionHandler::receiveInt(const char* arg) {

	int value;

	if (arg == NULL)
		return -1;
	if (isdigit(arg[0]))
		return -1;

	value = atoi(arg);

	return value;

}
size_t ServerConnectionHandler::receiveString(const char* arg) {

	size_t ris;

	ris = 0;
	return ris;
}
