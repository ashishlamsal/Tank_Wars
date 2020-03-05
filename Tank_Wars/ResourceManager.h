#pragma once
#include <memory>
#include <map>
#include <cassert>
#include <string>
#include "ResourceIdentifiers.h"

template <class Resource, class Identifier>
class ResourceManager
{
    public:
        ResourceManager() = default;
        bool load(Identifier id, const std::string& file_path)
        {
            std::unique_ptr<Resource> a(new Resource());
            bool status = a->loadFromFile(file_path);
            auto x = resMap.insert(std::make_pair(id,std::move(a)));
            assert(x.second);
            return status;
        }
        template<typename Parameter>
        bool load(Identifier id, const std::string& file_path,Parameter secParam)
        {
            std::unique_ptr<Resource> a(new Resource());
            bool status = a->loadFromFile(file_path,secParam);
            resMap.insert(std::make_pair(id,std::move(a)));
            return status;
        }
        Resource &get(Identifier id)
        {
            auto res = resMap.find(id);
            assert(res != resMap.end());
            return *(res->second);
        }

private:
        std::map<Identifier, std::unique_ptr<Resource> > resMap;
};



class musicManager {
private:
    std::map<musicIdentifier, std::unique_ptr<sf::Music> > resMap;

public:
    musicManager() = default;
    bool load(musicIdentifier id, const std::string& file_path) {
        std::unique_ptr<sf::Music> a(new sf::Music());
        bool status = a->openFromFile(file_path);
        auto x = resMap.insert(std::make_pair(id, std::move(a)));
        assert(x.second);
        return status;
    }

    sf::Music& get(musicIdentifier id) {
        auto res = resMap.find(id);
        assert(res != resMap.end());
        return *(res->second);
    }

};



