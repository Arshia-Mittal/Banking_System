// server side (Bank)
#include<iostream>
#include<string>
#include<stack>
#include<pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sstream>
#include"linkedList.h"
//#include"bison_parser.tab.h"
//extern int yyparse();
//extern FILE* yyin;
using namespace std;
#define max_client 10

// maintain active client information
struct clientInfo{
  int socket;
  struct sockaddr_in address;
};
int clientCount = 0;
clientInfo clients[max_client];
bool active_server_flag = true;


bank b;
/*string process_command(int command_type, YYSTYPE* command_data)
{
  string response = "";
  switch (command_type)
  {
  case OPEN_ACCOUNT:
    int an = b.create_account(command_data->strval);
    response = "Your account has been created. Account number is: " + to_string(an) + "\n";
    break;
  
  default:
    response = "Wrong command. \n";
    cerr << "Error: Invalid command" << endl;
    break;
  }

  return response;
}*/

// client handler function for receiving commands, processing them and sending reply
void *clientHandler(void *arg)
{
  int clientSocket = *((int *)arg);
  char buffer[1024];

  while (true)
  {
    memset(buffer, 0, sizeof(buffer));
    int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (valread <= 0)
    {
      for (int i = 0; i < clientCount; i++)
      {
        if (clients[i].socket == clientSocket)
        {
          // remove client from the list
          memmove(&clients[i], &clients[i + 1], (clientCount - i - 1) * sizeof(clientInfo));
          clientCount--;
          break;
        }
      }
      break;
    }

    string response;
    try
    {
      string request(buffer);
      istringstream iss(request);
      string command, arg1, arg2;
      iss >> command >> arg1 >> arg2;

      if (command == "OPEN" && arg1 == "ACCOUNT")
      {
        // int an = btree.insert(arg2);
        int an = b.create_account(arg2);
        response = "Your account has been created. Account number is: " + to_string(an) + "\n";
      }

      else if (command == "CLOSE" && arg1 == "ACCOUNT")
      {
        int ac2 = stoi(arg2);
        if (b.remove_account(ac2)) // btree.remove_acc(ac2)
        {
          response = "Account number " + arg2 + " has been deleted \n";
        }
        else
        {
          response = "No such account exists. \n";
        }
      }

      else if (command == "SHOW" && arg1 == "ACCOUNT_BALANCE")
      {
        int account_num = stoi(arg2);
        float bal = b.balance(account_num);
        if (bal != -1)
        {
          response = "Account Number: " + to_string(account_num) + " has a balance of " + to_string(bal) + "\n";
        }
        else
        {
          response = "Account not found. \n";
        }
      }

      else if (command == "DEPOSIT")
      {
        int an3 = stoi(arg1);
        float amount = stof(arg2);

        if (b.deposit(an3, amount))
        {
          response = "Money has been added. Account Number: " + to_string(an3) + "\n";
        }
        else
        {
          response = "Account not found. \n";
        }
      }

      else if (command == "WITHDRAW")
      {
        int an4 = stoi(arg1);
        float amount1 = stof(arg2);

        if (b.withdraw(an4, amount1))
        {
          response = "Money has been withdrawn. Account Number: " + to_string(an4) + "\n";
        }
        else
        {
          response = " Either account not found or balance is not sufficient. \n";
        }
      }

      else if (command == "RESET_ACCOUNT")
      {
        int an5 = stoi(arg1);
        if (b.resetAccount(an5))
        {
          response = "Reset account Number: " + to_string(an5) + "\n";
        }
        else
        {
          response = " Error reseting account Number: " + to_string(an5) + "\n";
        }
      }

      else if (command == "SHOW" && arg1 == "MINI_STATEMENT")
      {
        int an5 = stoi(arg2);
        vector<double> v;
        if (b.min_statement(an5, v))
        {
          response = "Mini statement of account number " + arg2 + " is: \n";
          for (int i = 0; i < v.size(); i++)
          {
            response += to_string(v[i]) + "\n";
          }
          response += "\n";
        }
        else
        {
          response = "Account not found. \n";
        }
      }

      else if (command == "SHOW" && arg1 == "ACTIVE" && arg2 == "CONNECTIONS")
      {
        // struct sockaddr_in clientAddress;
        // socklen_t clientAddressLength = sizeof(clientAddress);
        // getpeername(clientSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        // char clientIP[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        // int clientPort = ntohs(clientAddress.sin_port);
        // cout << "New connection form " << clientIP << " : " << clientPort << endl;
        // string ipString(clientIP);
        // response = "Port number of client is : " + to_string(clientPort) + "\n";

        response = "Active connections: \n";
        cout << "Active connections: \n";
        for (int i = 0; i < clientCount; i++)
        {
          char clientAddr[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &(clients[i].address.sin_addr), clientAddr, INET_ADDRSTRLEN);
          int clientPort = ntohs(clients[i].address.sin_port);
          response += "Client " + to_string(i + 1) + ": " + clientAddr + ": " + to_string(clientPort) + "\n";
          cout << "Client " << i + 1 << ": " << clientAddr << ": " << clientPort << endl;
        }
      }

      else if (command == "SHOW" && arg1 == "BANK" && arg2 == "BALANCE")
      {
        double bankBalance = b.show_bal();
        response = "Total balance of bank is " + to_string(bankBalance) + "\n";
      }

      else if (command == "SHOW" && arg1 == "ALL" && arg2 == "ACCOUNTS")
      {
        string ans = b.details();
        if (ans != "")
        {
          response = ans;
        }
        else
        {
          response = "No account found. \n";
        }
      }

      else if (command == "SHUTDOWN" && arg1 == "SERVER" && arg2 == ".")
      {
        /*vector<int>::iterator it = find(connected_clients.begin(), connected_clients.end(), clientSocket);
        if(it != connected_clients.end())
        {
          connected_clients.erase(it);
          active_connections--;

          if (active_connections == 0 && connected_clients.size() == 1)
          {
            response = "server shutting down \n";
            send(clientSocket, response.c_str(), response.length(), 0);
            break;
          }
        }*/
        if (clientCount <= 1)
        {
          active_server_flag = false;
          response = "server shutting down \n";
          send(clientSocket, response.c_str(), response.length(), 0);
          break;
        }
        else
        {
          response = "Can't shutdown server as active clients are detected. \n";
        }
      }

      else
      {
        response = "Wrong command. \n";
      }
    }

    catch (const invalid_argument &e)
    {
      response = "Wrong command. \n";
      cerr << "Error: Invalid command" << endl;
    }

    send(clientSocket, response.c_str(), response.length(), 0);
  }

  close(clientSocket);
  delete (int *)arg;
  pthread_exit(NULL);
}

int main()
{
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t addrSize;
  pthread_t thread[max_client];

  // create the server socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
  {
    cerr << "Error creating the socket" << endl;
    return -1;
  }

  // set up server address
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  // binding server socket
  bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr));

  // listen for client connections
  if (listen(serverSocket, 10) < 0 && !active_server_flag)
  {
    cerr << "Failed to listen for connections." << endl;
    return -1;
  }

  cout << "Server started. Listening for connections." << endl;
  // Accepting incoming client connections
  while (active_server_flag)
  {
    addrSize = sizeof(clientAddr);

    // accepting a new client connection
    newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
    if (newSocket < 0)
    {
      cerr << "Failed to accept connection." << endl;
      continue;
    }

    // adding new client information into clients vector
    clients[clientCount].socket = newSocket;
    clients[clientCount].address = clientAddr;
    clientCount++;

    // creating a new thread to handle the client request
    int *socketPtr = new int;
    *socketPtr = newSocket;
    if (pthread_create(&thread[max_client], NULL, clientHandler, (void *)socketPtr) != 0)
    {
      cerr << "Failed to create thread." << endl;
      delete socketPtr;
      continue;
    }

    if (!active_server_flag)
    {
      break;
    }
  }

  close(serverSocket);
  return 0;
}
