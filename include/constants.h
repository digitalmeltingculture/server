/*
 * constants.h
 *
 *  Created on: 18 ott 2018
 *      Author: skafo
 */

#ifndef INCLUDE_CONSTANTS_H_
#define INCLUDE_CONSTANTS_H_

class Constants {
public:
	const static int MAX_BUFFER_SIZE = 1024;
	const static int DEFAULT_PORT_NUMBER = 8080;
	const static int MAX_CONNECTIONS_NUMBER = 5;
	static const constexpr char* DELIMITER = "|";
	static const constexpr char* ERROR_RECEIVE_INTEGER_MSG =
			"Error receiveIntenger function: integer data received failed.";
	static const constexpr char* ERROR_RECEIVE_FLOAT_MSG =
			"Error receiveFloat function: float data received failed.";
	static const constexpr char* ERROR_RECEIVE_DOUBLE_MSG =
			"Error receiveDouble function: double data received failed.";
	static constexpr const char* ERROR_RECEIVE_CHAR_MSG =
			"Error receiveChar function: char data received failed.";
	static constexpr const char* ERROR_RECEIVE_STRING_MSG =
			"Error receiveString function: string data received failed.";
	static constexpr const char* ERROR_RECEIVE_END_CONNECTION_MSG =
			"Error receiveEndConnection  function: end connection  failed.";

	static constexpr const char* ERROR_CREATE_SOCKET_MSG = "Create socket failed";
	static constexpr const char* ERROR_BIND_FAILED_MSG = "Error bind failed";
	static constexpr const char* ERROR_LISTEN_FAILURE = "Error listen failure";
	static constexpr const char* SERVER_ACCEPT =
			"Server, accepting nextConnections at address ";
	static constexpr const char* SERVER_NOT_ACCEPT =
			"Error server  not accept connection";
	static constexpr const char* ERROR_FAILED_RECEIVE = "Receive failed : ";

};

#endif /* INCLUDE_CONSTANTS_H_ */
