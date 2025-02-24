# chat
As of 24/02/2025, simply [Kjungjae's](https://kyungjae.com/projects/tcpip-group-chat-application/) groupchat client but for Windows. More features are planned for the future.

## Compiling
```
gcc client.c -o client -lws2_32 
```

## Usage
Run or know the ip to a [server](https://kyungjae.com/projects/tcpip-group-chat-application/#Server:~:text=Source%20Code-,Server,%7D%20//%20end%20of%20handle_err,-Client), then use:
```
.\client.exe <server ip> <port> <username>
```
