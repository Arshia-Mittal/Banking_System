# Banking_System

## Introduction
This project is a simple banking system that consists of a server and a client application. The server handles multiple client connections over TCP/IT, allowing customers to perform various banking operations concurrently. 
The server stores customer accounts and balance in a baking ledger and processes commands sent by the clients. The clients can connect remotely to the server and perform banking operations.

## Server Program
The server program handles client connections and allows multiple customers to access banking ledger in parallel. 
It uses POSIX socket APIs for TCP/IP communication and pthread library to achieve concurrency.  <br />
The server allows followings commands sent by the client process:  <br />
•	OPEN ACCOUNT <CustomerName> - Generates a new Account number.  <br />
•	CLOSE ACCOUNT <AccountNumber>  <br />
•	SHOW ACCOUNT BALANCE <AccountNumber>  <br />
•	DEPOSIT <AccountNumber> <Amount in decimal>  <br />
•	WITHDRAW <AccountNumber> <Amount in decimal>  <br />
•	SHOW MINI STATEMENT <AccountNumber> - Last 10 transactions.  <br />
•	SHOW ALL ACCOUNTS - List Name and account number.  <br />
•	SHOW BANK BALANCE - Show current bank balance.  <br />
•	SHOW ACTIVE CONNECTIONS - Show IP and Port number of all connected clients.  <br />
•	SHUTDOWN SERVER  <br />

### Building the Server
•	To build the server, use the provided Makefile. Simply navigate to the server’s directory and run the following commands:  <br />
•	make  <br />
•	./server_1  <br />
•	The make command will compile the server program and generate executable. After building the server, you can run it using the ./server_1 command.  <br />
•	The server will start listening for incoming client connections.

## Client Program
The client program connects to the server and allows customers to perform banking operations by sending commands to the server and receiving response back from it.
### Building the Client
•	To build the client, use the provided Makefile. Simply navigate to the client’s directory and run the following commands:  <br />
•	make  (if not done before)  <br />
•	./client_1  <br />
•	The make command will compile the server program and generate executable. After building the client, you can run it using the ./client_1 command. 
    The client will prompt you to enter commands, which will be sent to the server for processing. You can also run the following command:  <br />
•	./client_1 <test_file>  <br />
•	You can give any test_file having commands as command line argument. All these commands will be sent to the server for processing.

