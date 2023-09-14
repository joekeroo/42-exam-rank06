#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 128
#define BUFFER_SIZE 20000

void fatal(int fd)
{
	write(2, "Fatal error\n", 12);
	close(fd);
	exit(1);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	int next_id = 0;
	int clientSockets[MAX_CLIENTS];

	char buffer[BUFFER_SIZE];
	fd_set activeSockets, readySockets;

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		fatal(serverSocket);

	struct sockaddr_in serverAddress = {0};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serverAddress.sin_port = htons(atoi(argv[1]));

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		fatal(serverSocket);

	FD_ZERO(&activeSockets);
	FD_SET(serverSocket, &activeSockets);
	int maxSocket = serverSocket;

	while (1)
	{
		readySockets = activeSockets;
		if (select(maxSocket + 1, &readySockets, NULL, NULL, NULL) < 0)
			fatal(serverSocket);

		for (int socketId = 0; socketId <= maxSocket; socketId++)
		{
			if (FD_ISSET(socketId, &readySockets))
			{
				int clientSocket = accept(serverSocket, NULL, NULL);
				if (clientSocket < 0)
					fatal(serverSocket);

				FD_SET(clientSocket, &activeSockets);
				maxSocket = (clientSocket > maxSocket) ? clientSocket : maxSocket;

				sprintf(buffer, "server: client %d just arrived\n", next_id);
				if (send(clientSocket, buffer, strlen(buffer), 0) < 0)
					fatal(serverSocket);
				clientSockets[next_id++] = clientSocket;
			}
			else
			{
				int bytesRead = recv(socketId, buffer, sizeof(buffer) - 1, 0);
				if (bytesRead <= 0)
				{
					sprintf(buffer, "server: client %d just left\n", socketId);
					for (int i = 0; i < next_id; i++)
					{
						if (clientSockets[i] != socketId)
						{
							if (send(clientSockets[i], buffer, strlen(buffer), 0) < 0)
								fatal(serverSocket);
						}
					}

					close(socketId);
					FD_CLR(socketId, &activeSockets);
				}
				else
				{
					char temp[20050];
					buffer[bytesRead] = '\0';
					sprintf(temp, "client %d: %s\n", socketId, buffer);
					for (int i = 0; i < next_id; i++)
					{
						if (clientSockets[i] != socketId)
							send(clientSockets[i], temp, strlen(temp), 0);
					}
				}
			}
		}
	}
	return (0);
}
