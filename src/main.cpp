/*
 * main.cpp
 *
 *  Created on: 18 ott 2018
 *      Author: skafo
 */

#include <iostream>
using  namespace std;

#include <netinet/in.h>

#include "constants.h"
#include "server.h"


int foo( const void* tmp){
	int *a;
	a = (int*)tmp;
	printf("\n%d\n",*a);
}

int main (int argc, char** argv){
	if(argc < 2){
		cout << "Not ehough argument! Read the man for details" << endl;
		//man();
		return 0;
	}

	int n = 23;
	foo(&n);
	return 0;
	
	int portNumber = atoi(argv[1]);
	Server *server1= new Server(portNumber);
	server1->acceptConnections(0);
}
