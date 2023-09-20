# 42KL-Exam-Rank06

mini version of the 42 project 'webserv'.

the main focus of this exam is to test your knowledge of sockets and how you communicate between them.

# Expected File

mini_serv.c

# Allowed Functions

<unistd.h>

```
write, close and select
```

<sys/socket.h>

```
socket, accept, listen, send, recv and bind
```

<string.h>

```
strstr, strlen, strcpy, strcat, memset and bzero
```

<stdlib.h>

```
malloc, realloc, free, calloc, atoi and exit
```

<stdio.h>

```
- sprintf
```

# Subject Text

Write a program that will listen for client to connect on a certain port on 127.0.0.1 and will let clients to speak with each other. This program will take as first argument the port to bind to.

- If no argument is given, it should write in stderr "Wrong number of arguments" followed by a \n and exit with status 1
- If a System Calls returns an error before the program start accepting connection, it should write in stderr "Fatal error" followed by a \n and exit with status 1
- If you cant allocate memory it should write in stderr "Fatal error" followed by a \n and exit with status 1

### Your Program

- Your program must be non-blocking but client can be lazy and if they don't read your message you must NOT disconnect them...
- Your program must not contains #define preproc
- Your program must only listen to 127.0.0.1

The fd that you will receive will already be set to make 'recv' or 'send' to block if select hasn't be called before calling them, but will not block otherwise.

### When a client connect to the server:

- the client will be given an id. the first client will receive the id 0 and each new client will received the last client id + 1
- %d will be replace by this number
- a message is sent to all the client that was connected to the server: "server: client %d just arrived\n"

### Clients must be able to send messages to your program.

- message will only be printable characters, no need to check
- a single message can contains multiple \n
- when the server receive a message, it must resend it to all the other client with "client %d: " before every line!

### When a client disconnect from the server:

- a message is sent to all the client that was connected to the server: "server: client %d just left\n"

```
Memory or fd leaks are forbidden
```
