/*
 * server.h
 *
 *  Created on: 18 ott 2018
 *      Author: skafo
 */

#ifndef INCLUDE_SERVER_H_
#define INCLUDE_SERVER_H_

class Constants;

class Server{

private:

	int serverFd;
	int portNumber;

    struct sockaddr_in serverAddress;

	int clientSockFd[Constants::MAX_CONNECTIONS_NUMBER];
	int availableConnections;

public:
	Server();
	Server(int);

	void initSockAddr();
	int initConnection();
	int removeSockFd(int);

	int getPortNumber();
	void setPortNumber(int);
	int acceptConnections(int);
};


#endif /* INCLUDE_SERVER_H_ */
