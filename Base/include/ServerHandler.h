#pragma once
#include"CommandHandler.h"
#include<atomic>

class ServerHandler
{
CommandHandler cmdhandler;
int serverfd;
int port;
void handleClient(int client_socket);
public:
    
    ServerHandler(int serverport,KeyValueStore &kv):port(serverport),cmdhandler(kv)
    {
    };
    void runServerHandler();
    
};