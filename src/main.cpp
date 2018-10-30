/*
 * main.cpp
 *
 *  Created on: 18 ott 2018
 *      Author: skafo
 */

#include <iostream>
#include <vector>
#include <thread>

using namespace std;

#include <netinet/in.h>

#include "constants.h"
#include "serverconnectionhandler.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Not ehough argument! Read the man for details" << endl;
		//man();
		return 0;
	}

	int portNumber = atoi(argv[1]);
	ServerConnectionHandler *server1 = new ServerConnectionHandler(portNumber);
	server1->acceptConnections(0);
}
