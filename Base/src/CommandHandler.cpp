#include<CommandHandler.h>
#include<sstream>
#include<iostream>
#include<string>
#include<optional>

std::string CommandHandler::handle(const std::string &command)
{
   std::istringstream iss(command);
   std::string operation,key,value;
   iss >> operation >> key >> value;
   std::string returnvalue;
   if (operation == "SET") {
    store.setKeyValueStore(key, value);
    returnvalue = "OK\n";
    }
    else if (operation == "GET") {
        std::optional<std::string> getval = store.getKeyValueStore(key);
        if (getval.has_value()) {
            returnvalue=*getval+"\n";
        } else {
            returnvalue = "NOT FOUND\n";
        }
    }
    else if (operation == "DEL") {
        if (store.delKeyValueStore(key)) {
            returnvalue = "DELETED\n";
        } else {
            returnvalue = "NOT FOUND\n";
        }
    }
    else {
        returnvalue = "ERROR: Unknown Command\n";
    }
    return returnvalue;
}