#include "KeyValueStore.h"
#include <optional>
 
void KeyValueStore::setKeyValueStore(const std::string& key, const std::string& value) 
{
        m_keyvaluestore[key]=value;
        std::cout<<"\nkey value is stored:"<< key<<","<<value<< "\n";
}

std::optional<std::string>KeyValueStore::getKeyValueStore(const std::string& key)
{
    auto it = m_keyvaluestore.find(key);

    if(it!=m_keyvaluestore.end())
    {
        std::cout<<"\nkey retrieved:\n";
        return it->second;
    }
    else
    {
        std::cout<<"\nkey not found to retrieve\n";
        return std::nullopt;
    }
}

bool KeyValueStore::delKeyValueStore(const std::string& key)
{
    bool status=false;
    if(m_keyvaluestore.erase(key)!=0)
    {
        std::cout<<"\nkey deleted\n";
        status=true;
    }
    else{
        std::cout<<"\nkey not found to delete\n";
        status=false;
    }
    return status;
}