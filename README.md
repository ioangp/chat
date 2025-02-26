# chat
[Kjungjae's](https://kyungjae.com/projects/tcpip-group-chat-application/) groupchat client but for Windows. Posh terminal interface too.

## Compiling
Run build.bat or
```
gcc .\src\*.c -o client -lws2_32 -lpdcurses
```
(Requires ncurses)

## Usage
Run or know the ip to a [server](https://kyungjae.com/projects/tcpip-group-chat-application/#Server:~:text=Source%20Code-,Server,%7D%20//%20end%20of%20handle_err,-Client), then use:
```
.\client.exe <server ip> <port> <username> <colour>
```
Where ``<colour>`` is the colour of your username and can be:
```
w - white
r - red
g - green
b - blue
y - yellow
c - cyan
m - magenta
```