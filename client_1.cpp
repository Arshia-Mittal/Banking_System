// client side (customer)
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
using namespace std;

const int PORT = 8080;

// function to send commands to server
void sendRequest(int socket, const string &request)
{
    send(socket, request.c_str(), request.size(), 0);
}

// function to receive response from server
string receiveResponse(int socket)
{
    const int bufferSize = 1024;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);
    recv(socket, buffer, bufferSize - 1, 0);
    return string(buffer);
}

int main(int argc, char *argv[])
{
    // customer giving test file having commands
    if (argc > 1)
    {
        // create the client socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1)
        {
            cerr << "Error creating the socket." << endl;
            return 1;
        }
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);

        if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
        {
            cerr << "Failed to set server address" << endl;
            return 1;
        }

        // forming connection between server and client
        if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        {
            cerr << "connection failed" << endl;
            return 1;
        }

        string request;
        string response;
        ifstream inputFile(argv[1]);
        if (!inputFile.is_open())
        {
            cout << "Failed to open the file. " << endl;
            return 1;
        }

        while (getline(inputFile, request))
        {
            sendRequest(clientSocket, request);
            response = receiveResponse(clientSocket);
            cout << "Response: " << response << endl;
        }
        inputFile.close();
        close(clientSocket);
    }

    else
    {
        // create the client socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1)
        {
            cerr << "Error creating the socket." << endl;
            return 1;
        }
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);

        if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
        {
            cerr << "Failed to set server address" << endl;
            return 1;
        }

        // forming connection between server and client
        if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        {
            cerr << "connection failed" << endl;
            return 1;
        }

        string request;
        string response;
        while (true)
        {
            cout << "Enter a command or 'q' to quit: ";
            getline(cin, request);
            if (request == "q")
            {
                close(clientSocket);
                return 0;
            }
            sendRequest(clientSocket, request);
            response = receiveResponse(clientSocket);
            cout << "Response: " << response << endl;
        }
        close(clientSocket);
    }

    return 0;
}