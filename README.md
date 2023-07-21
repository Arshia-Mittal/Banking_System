# Banking_System

## Introduction
This project is a simple banking system that consists of a server and a client application. The server handles multiple client connections over TCP/IT, allowing customers to perform various banking operations concurrently. 
The server stores customer accounts and balance in a baking ledger and processes commands sent by the clients. The clients can connect remotely to the server and perform banking operations.

## Server Program
The server program handles client connections and allows multiple customers to access banking ledger in parallel. 
It uses POSIX socket APIs for TCP/IP communication and pthread library to achieve concurrency.
The server allows followings commands sent by the client process:
•	OPEN ACCOUNT <CustomerName> - Generates a new Account number.
•	CLOSE ACCOUNT <AccountNumber>
•	SHOW ACCOUNT BALANCE <AccountNumber>
•	DEPOSIT <AccountNumber> <Amount in decimal>
•	WITHDRAW <AccountNumber> <Amount in decimal>
•	SHOW MINI STATEMENT <AccountNumber> - Last 10 transactions.
•	SHOW ALL ACCOUNTS - List Name and account number.
•	SHOW BANK BALANCE - Show current bank balance.
•	SHOW ACTIVE CONNECTIONS - Show IP and Port number of all connected clients.
•	SHUTDOWN SERVER

Building the Server
•	To build the server, use the provided Makefile. Simply navigate to the server’s directory and run the following commands: 
•	make  
•	./server_1
•	The make command will compile the server program and generate executable. After building the server, you can run it using the ./server_1 command. 
•	The server will start listening for incoming client connections.

## Client Program
The client program connects to the server and allows customers to perform banking operations by sending commands to the server and receiving response back from it.
Building the Server
•	To build the client, use the provided Makefile. Simply navigate to the client’s directory and run the following commands: 
•	make  (if not done before)
•	./client_1 
•	The make command will compile the server program and generate executable. After building the client, you can run it using the ./client_1 command. 
    The client will prompt you to enter commands, which will be sent to the server for processing. You can also run the following command:
•	./client_1 <test_file>
•	You can give any test_file having commands as command line argument. All these commands will be sent to the server for processing.

