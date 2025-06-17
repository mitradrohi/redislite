#pragma once
#include"KeyValueStore.h"
class CommandHandler
{
KeyValueStore &store;
    public:
    CommandHandler()=default;
    CommandHandler(KeyValueStore &kv):store(kv)
    {
    }
    std::string handle(const std::string &command);
};