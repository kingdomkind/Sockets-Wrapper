// LICENSE GPL-3.0-only AUTHOR: 

#include "networker.h"
#include "logger.h"

/*
networkedClient::networkedClient()
{
    ServerSocket = -1;
    ClientSocket = -1;
    Port = -1;
}
*/

void setup_winsock()
{
    WSADATA Data;
    WORD DllVersionRequest = MAKEWORD(2, 2);

    if (WSAStartup(DllVersionRequest, &Data))
    {
        append_log_message("Failed to initialise the winsock2 dll.", 1);
    }
}

void networkedClient::connect_tcp(int DesiredPort, std::string IP)
{
    Port = DesiredPort;
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(Port);
    //ServerAddress.sin_addr.s_addr = inet_addr(IP.c_str()); > Deprecated usage for windows, updated to inet_pton, as shown below in the else

    if (IP == "-1")
    {
        // Binds to all interfaces
        ServerAddress.sin_addr.s_addr = INADDR_ANY;

        int ReturnValue = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
        append_log_message("Server is listening");

        ReturnValue = listen(ServerSocket, MaxClients);
        if (ReturnValue == -1) {
            append_log_message("Server Socket failed to initialise - perhaps try changing ports / restarting?", 1); // Port could possibly already be used
            perror("bind"); closesocket(ServerSocket);
        }

        while (true) {
            ClientSocket = accept(ServerSocket, nullptr, nullptr);
            if (ClientSocket == -1) {
                append_log_message("Client Socket failed to initialise - perhaps try changing ports / restarting?", 1);
                perror("accept"); closesocket(ClientSocket); closesocket(ServerSocket);
            }
            append_log_message("Client has connected", 1);
            break;
        }
    }
    else 
    {
        inet_pton(AF_INET, (PCSTR)IP.c_str(), &ServerAddress.sin_addr.s_addr);

        if (connect(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1) {
            perror("connect");
            closesocket(ClientSocket);
        }
    }
}

void networkedClient::send_tcp(std::string Buffer)
{
    std::string DataToSend = Buffer + Delimiter;
    append_log_message("Added Buffer and Delimiter, sending Message", 15);

    if (ServerSocket == INVALID_SOCKET) {
        append_log_message("Invalid socket.", 1);
        return;
    }

    SSIZE_T SentData = send(ServerSocket, DataToSend.c_str(), DataToSend.size(), 0);

    if (SentData == -1) {
        perror("send");
        append_log_message("Failed to send " + std::to_string(DataToSend.size()) + " bytes to the client.", 1);
    }
    else {
        append_log_message("Sent data to the client, bytes: " + std::to_string(DataToSend.size()));
    }
}

std::vector<char> networkedClient::listen_tcp()
{
    std::vector<char> Buffer;
    Buffer.resize(MaxPacketSize); // Max TCP packet size
    int TotalReceived = 0;

    while (true) {
        int RemainingSpace = Buffer.size() - TotalReceived;
        SSIZE_T ReceiveSize = recv(ClientSocket, Buffer.data() + TotalReceived, RemainingSpace, 0);

        if (ReceiveSize == -1) {
            perror("recv");
            append_log_message("Recieved data was -1, which indicates that a bad packet was recieved.", 1);
        }

        TotalReceived += ReceiveSize;
        size_t DelimiterPos = std::string(Buffer.data(), TotalReceived).find(Delimiter);

        if (DelimiterPos != std::string::npos)
        {
            Buffer.resize(DelimiterPos);
            break;
        }
        else {
            Buffer.resize(TotalReceived + MaxPacketSize);
        }
    }
    return Buffer;
}

void networkedClient::disconnect_tcp()
{
    append_log_message("Disconnecting communication.", 1);
    closesocket(ClientSocket);
    closesocket(ServerSocket);
}