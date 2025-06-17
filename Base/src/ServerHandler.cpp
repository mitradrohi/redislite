  #include"ServerHandler.h"
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <unistd.h>
  #include <thread>
  #include <cstring>
  #include "SyncSignal.h"
  #include <arpa/inet.h> 
    
  void ServerHandler::runServerHandler()
    {
        int clientfd=-1;
        socklen_t serveraddresslen;
        sockaddr_in serveraddress;
        serveraddresslen= sizeof(serveraddress);
        
        serveraddress.sin_addr.s_addr=INADDR_ANY;
        serveraddress.sin_port=htons(port);
        serveraddress.sin_family=AF_INET;

        serverfd=socket(AF_INET,SOCK_STREAM,0);
        bind(serverfd,(const struct sockaddr *)&serveraddress,serveraddresslen);
        listen(serverfd,10);
        char client_ip[INET_ADDRSTRLEN];
        std::cout<<"socket created and waiting for connection\n";
        while(true)
        {

          sockaddr_in client_addr;
          socklen_t client_len = sizeof(client_addr);
          clientfd=accept(serverfd,(struct sockaddr *)&client_addr,&client_len);
          if(clientfd<0){
          perror("accept failed");
          continue;
          }

          inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
          std::cout << "Connection from: " << client_ip << ":" << ntohs(client_addr.sin_port) << "\n";
          std::thread(&ServerHandler::handleClient,this,clientfd).detach();
        }

        std::cout<<"closing server"<<std::endl;
        close(serverfd);
    }

    void ServerHandler::handleClient(int client_socket)
    {

      char buffer[1024];
      
      while(true)//change here to handle any interupts
      {
        {
        std::unique_lock<std::mutex> lock(global_mtx);
        if (stop_flag) 
            break;
        }

        memset(buffer,0,1024);
        size_t recsize = read(client_socket,buffer,sizeof(buffer)-1);

      if(recsize<=0)
        {
          break;
        }

        buffer[recsize]='\0';
        std::cout<<"Data recieved:"<<buffer;
        std::string command(buffer);
        std::string response="";
        response = cmdhandler.handle(command);
        send(client_socket,response.c_str(),response.length(),0);
      }
      std::cout<<"Closing connection with client\n"<<std::endl;
      close(client_socket);
    }