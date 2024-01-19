#pragma once
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

// Networking header files + libs
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

// Sets up the winsock dll, and requests the required version
void setup_winsock();

// Class that manages a TCP connection to another TCP body.
class networkedClient
{
public:
	int ClientSocket = -1;
	int ServerSocket = -1;;
	int Port = -1;

	// Maximum number of clients that can be concurrently connected
	int MaxClients = 5;
	// Maximum TCP Packet size, set to nominal value.
	int MaxPacketSize = 500; //65536;
	// Delimiter which ends a TCP packet, to allow knowledge of when 1 sent packet begins and ends, even if it is further broken up.
	std::string Delimiter = "VtK9A3&YDuQ%uLNEe2$RARt3w%Z^RztUL%vBT*%GxB#9HLbfiL5X6LR35Sm$7G*c";
	struct sockaddr_in ServerAddress;

	// Initialiser function, which sets the value of certain variables.
	//networkedClient();

	// Allows the server to setup a TCP connection, and listen for incoming clients, or connect to another server by not specifying an IP.
	void connect_tcp(int DesiredPort, std::string IP = "-1");

	// Sends a TCP message
	void send_tcp(std::string Buffer);

	// Recieves a TCP message, and yields until it has been recieved, including the delimiter
	std::vector<char> listen_tcp();

	// Disconnects the server ports, and any client ports
	void disconnect_tcp();
};