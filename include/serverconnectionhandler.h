/*
 * server.h
 *
 *  Created on: 18 ott 2018
 *      Author: skafo
 */

#ifndef INCLUDE_SERVERCONNECTIONHANDLER_H_
#define INCLUDE_SERVERCONNECTIONHANDLER_H_

class Constants;

class ServerConnectionHandler {

private:

	int serverFd;
	int portNumber;

	struct sockaddr_in serverAddress;

	int clientSockFd[Constants::MAX_CONNECTIONS_NUMBER];
	vector<thread*> tidVector;
	int availableConnections;

public:
	ServerConnectionHandler();
	ServerConnectionHandler(int);

	void initSockAddr();
	int initConnection();
	int removeSockFd(int);

	int getPortNumber();
	void setPortNumber(int);
	int acceptConnections(int);
	static void* threadHandler(void*);
	int removeConnection(int);
	int ricerca(int val);

	static void foo(int*);
};

#endif /* INCLUDE_SERVERCONNECTIONHANDLER_H_ */
