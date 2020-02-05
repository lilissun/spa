//
//  namer.cpp
//  explicit
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "namer.h"

#include <sstream>

#include "common/runtime.h"
#include "name.h"

namespace emc {
    bool Namer::passed(Name::ID id) const
    {
        return _tick.current() > id;
    }
    
    Name Namer::generate(const NameType &type, const std::string &name)
    {
        return Name(type, name, _tick.next());
    }
    
    Name Namer::variable(const std::string &name)
    {
        return generate(NameType::VARIABLE, name);
    }
    
    Name Namer::variable(const std::string &name, Name::ID id)
    {
        ASSERT(passed(id));
        return Name(NameType::VARIABLE, name, id);
    }
    
    Name Namer::constant(const std::string &name)
    {
        auto it = _constants.find(name);
        if (it == _constants.end()) {
            auto ret = _constants.insert(std::pair<std::string, Name>(name, generate(NameType::CONSTANT, name)));
            ASSERT(ret.second);
            return ret.first->second;
        } else {
            return it->second;
        }
    }
   
    bool Namer::exist(const std::string &name) const
    {
        return _constants.find(name) != _constants.end();
    }
    
    Name Namer::name(const std::string &name) const
    {
        auto it = _constants.find(name);
        ASSERT(it != _constants.end());
        return it->second;
    }
}