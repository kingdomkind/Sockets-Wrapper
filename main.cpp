#include <iostream>
#include <vector>
#include <winsock2.h>
#include <windows.h>
#include "networker.h"
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    setup_winsock();

    networkedClient TestClient;

    std::cout << "Server (1), Client (2)" << std::endl;
    std::string Input;
    std::cin >> Input;

    if (Input == "1") {
       TestClient.connect_tcp(123243, "-1");
       std::vector<char> Listening = TestClient.listen_tcp();

       for (char Character : Listening) {
           std::cout << Character;
       }
       std::cout << std::endl;
    }
    else {
        TestClient.connect_tcp(123243, "127.0.0.1");
        TestClient.send_tcp("Banana and eggs");
    }
}
