#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include<arpa/inet.h>

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // sending connection request
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    perror("Error connecting socket");
    exit(1);
}

    // sending data
    // const char* message = "Hello, server!";
    std::string line;
    char recbuffer[1024];
    while (true)
    {
    line="";
    std::getline(std::cin,line);
    
    if(line=="exit")
    {
        std::cout<<"exiting\n";
        break;
        
    }
    if(send(clientSocket, line.c_str(), line.size(), 0)>0)
    {
        std::cout<<"Data sent successfuly\n";
    }
    else
    {
        std::cout<<"error in sending\n";
    }
    memset(recbuffer, 0, sizeof(recbuffer));
    int bytesread=read(clientSocket,recbuffer,sizeof(recbuffer)-1);
    if(bytesread<=0)
        {
            break;
        }
        recbuffer[bytesread]='\0';
        std::cout<<"RECIEVIED:"<<recbuffer;
        memset(recbuffer,0,1023);
    }
    // closing socket
    close(clientSocket);
    
    

    return 0;
}