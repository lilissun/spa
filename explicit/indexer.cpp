//
//  indexer.cpp
//  explicit
//
//  Created by Li Li on 2/6/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "indexer.h"

#include "namer.h"
namespace emc {
    Name Indexer::variable(const std::string &name, Namer &namer)
    {
        ASSERT(namer.exist(name) == false);
        auto it = _variables.find(name);
        if (it == _variables.end()) {
            auto ret = _variables.insert(std::pair<std::string, Name>(name, namer.variable(name)));
            ASSERT(ret.second);
            return ret.first->second;
        } else {
            return it->second;
        }
    }
    
    Name Indexer::constant(const std::string &name, Namer &namer)
    {
        ASSERT(_variables.find(name) == _variables.end());
        return namer.constant(name);
    }
    
    bool Indexer::exist(const std::string &name, Namer &namer) const
    {
        if (namer.exist(name)) {
            return true;
        } else if (_variables.find(name) != _variables.end()) {
            return true;
        } else {
            return false;
        }
    }

    Name Indexer::name(const std::string &name, Namer &namer) const
    {
        if (namer.exist(name)) {
            return namer.name(name);
        } else {
            auto it = _variables.find(name);
            ASSERT(it != _variables.end());
            return it->second;
        }
    }
}