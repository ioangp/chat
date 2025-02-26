#define _WIN32_WINNT 0x0501

#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string.h>
#include "colours.h"

#define MSG_LEN     1024
#define NAME_LEN    32

void check(int, char*);
void print_coloured(char*, char*);
DWORD WINAPI send_msg(void*);
DWORD WINAPI recv_msg(void*);

char name[NAME_LEN] = "noname";
char msg[MSG_LEN];
char info_msg[MSG_LEN];

int main(int argc, char *argv[]){
	// Check for correct number of arguments
	if(!(argc == 4 || argc == 5)){
		printf("Usage: .\\client.exe <serverIP> <port> <name> <colour>");
		exit(1);
	}

	// Windows sockets init
	WSADATA wsaData;

	check(WSAStartup(MAKEWORD(2,2), &wsaData),
		"WSAStartup failed");
	
	// Set up user name
	char *colour;
	if(argc == 4){
		colour = WHT;
	}else{
		colour = string_to_colour(argv[4]);
	}
	sprintf(name, "%s%s%s", colour, argv[3], RESET);
	
	// Create socket
	SOCKET sock = INVALID_SOCKET;
	
	// Address
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	// Resolve the server address and port
	int iResult = 0;
	iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed | %d\n", iResult);
		WSACleanup();
		exit(1);
	}
	
	// Attempt to connect to the first address returned by the call to getaddrinfo
	ptr=result;

	// Create a SOCKET for connecting to server
	sock = socket(ptr->ai_family, ptr->ai_socktype, 
		ptr->ai_protocol);
		
	if (sock == INVALID_SOCKET) {
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	// Connect to server.
	iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1);
	}

	// clear the screen
	system("cls");

    // print the welcome message to the screen
	printf(BOLD "Welcome to group chat! (Q/q to quit)\n\n" RESET);
	
	// Send join message
	sprintf(msg, "== %s has joined ==\n", name);
	send(sock, msg, strlen(msg), 0);

	// It's thread time
	// create two threads to carry out send and receive operations, respectively
	HANDLE send_thread = CreateThread(NULL, 0, send_msg, &sock, 0, NULL);
	HANDLE recv_thread = CreateThread(NULL, 0, recv_msg, &sock, 0, NULL);
	HANDLE threads[2] = {send_thread, recv_thread};
	
	WaitForMultipleObjects(2, threads, FALSE, INFINITE);
	
	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(sock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed | %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	
	// cleanup
	closesocket(sock);
	WSACleanup();

	return 0;
}

DWORD WINAPI send_msg(void* data) {
	SOCKET sock = *((SOCKET*)data);
	char name_msg[NAME_LEN + MSG_LEN + 2];  // add '2' for brackets around the name

	while (1)
	{
		fgets(msg, MSG_LEN, stdin);

		// terminate the connection upon user input Q/q
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			// print the leave message to the screen
			sprintf(msg, "== %s has left! ==\n", name);
			send(sock, msg, strlen(msg), 0);

			// terminate the connection
			closesocket(sock);
			WSACleanup();
			exit(0);
		}

		// construct the message and send it to the server
		sprintf(name_msg, "[%s] %s", name, msg);
		send(sock, name_msg, strlen(name_msg), 0);
	}
	return 0;
}

DWORD WINAPI recv_msg(void* data) {
	SOCKET sock = *((SOCKET*)data);
	char name_msg[NAME_LEN + MSG_LEN + 2];
	int str_len;
    
	char nym[NAME_LEN + 2];
	sprintf(nym, "[%s]", name);

	while(1)
	{
		// receive the message broadcasted by the server
		str_len = recv(sock, name_msg, NAME_LEN + MSG_LEN + 2 - 1, 0); 
			// add '2' for brackets around the name

		// terminate the thread if the client has been disconnected
		if (str_len == -1)
			return 0;

		name_msg[str_len] = 0;

		// check if this is ourselves (TODO: what about duplicate usernames?)
		/*
		int cmp = strncmp(nym, name_msg, strlen(nym));		
		if(cmp == 0){
			// make our own text dim
			print_coloured(name_msg, DIM);
		} else{
			// print the received message to the screen
		    name_msg[str_len] = 0;
		    print_coloured(name_msg, WHT);	
		}
		*/
		printf(name_msg);
     
	}
	return 0;
}

void print_coloured(char *msg, char *colour){
	printf(colour);
	printf(msg);
	printf(RESET);
}

void check(int result, char *message){
	if(result != 0){
		printf("%s | code %d\n", message, result);
		exit(1);
	}
}
