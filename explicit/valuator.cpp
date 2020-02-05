//
//  valuator.cpp
//  explicit
//
//  Created by Li Li on 20/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "valuator.h"

#include <sstream>

#include "common/runtime.h"

namespace emc {    
    Value Valuator::evaluate(const Name &name) const
    {
        auto it = _map.find(name);
        ASSERT(it != _map.end());
        return it->second;
    }
    
    void Valuator::assign(const Name &name, const Value &value)
    {
        auto res = _map.insert(std::pair<Name, Value>(name, value));
        if (res.second == false) {
            res.first->second = value;
        }
    }
    
    void Valuator::erase(const Name &name)
    {
        auto it = _map.find(name);
        ASSERT(it != _map.end());
        _map.erase(it);
    }
}