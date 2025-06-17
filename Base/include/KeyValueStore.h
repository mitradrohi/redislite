#pragma once

#include<iostream>
#include<unordered_map>
#include<optional>

class KeyValueStore
{
    std::unordered_map<std::string,std::string> m_keyvaluestore;
    public:
        KeyValueStore() = default;
        ~KeyValueStore() = default;

        KeyValueStore(const KeyValueStore&) = default; //copy constructor
        KeyValueStore& operator=(const KeyValueStore&) = default; //copy assignment

        KeyValueStore(KeyValueStore&&) = default; //move constructor
        KeyValueStore& operator=(KeyValueStore&&) = default; //move assignment


        void setKeyValueStore(const std::string& key, const std::string& value);
        std::optional<std::string> getKeyValueStore(const std::string& key);
        bool delKeyValueStore(const std::string& key);


};

