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

	map<int, thread*> sockThreadMap;
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

	int removeConnection(int);
	int ricerca(int);
	int parseReceivedData(int, void *, size_t);

	double receiveDouble(const char*);
	float receiveFloat(const char*);
	int receiveInt(const char*);
	size_t receiveChar(const char*);
	size_t receiveString(const char*);
	int closeAndRemoveConnection(int, const char*);
	int ParseData(char*);

	static void threadHandler(int, ServerConnectionHandler*, int);
};

#endif /* INCLUDE_SERVERCONNECTIONHANDLER_H_ */
