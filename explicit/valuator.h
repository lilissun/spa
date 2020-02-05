//
//  valuator.h
//  explicit
//
//  Created by Li Li on 20/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __explicit__valuator__
#define __explicit__valuator__

#include <map>

#include "value.h"
#include "name.h"
#include "common/exception.h"

namespace emc {
    class Valuator
    {
    protected:
        std::map<Name, Value> _map;
        
    public:
        Valuator() {}
        Valuator(const Valuator &valuator) : _map(valuator._map) {}
        ~Valuator() {}
        
    public:
        Value evaluate(const Name &name) const;
        void assign(const Name &name, const Value &value);
        void erase(const Name &name);
    };
}

#endif /* defined(__explicit__valuator__) */
