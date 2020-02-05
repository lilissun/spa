//
//  renamer.cpp
//  explicit
//
//  Created by Li Li on 15/10/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "renamer.h"

#include "common/runtime.h"
#include "namer.h"
#include "name.h"

namespace emc {
    Name Renamer::rename(const Name &name, Namer &namer)
    {
        if (name.type() == NameType::VARIABLE) {
            auto it = _mapping.find(name);
            if (it == _mapping.end()) {
                Name new_name(namer.variable(name.name()));
                _mapping.insert(std::make_pair(name, new_name));
                return new_name;
            } else {
                return it->second;
            }
        } else {
            ASSERT(name.type() == NameType::CONSTANT);
            return name;
        }
    }
    
    Name Renamer::name(const Name &name) const
    {
        if (name.type() == NameType::VARIABLE) {
            auto it = _mapping.find(name);
            ASSERT(it != _mapping.end());
            return it->second;
        } else {
            ASSERT(name.type() == NameType::CONSTANT);
            return name;
        }
    }
}