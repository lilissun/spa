//
//  namer.h
//  explicit
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __explicit__namer__
#define __explicit__namer__

#include <iostream>
#include <map>

#include "common/exception.h"
#include "name.h"
#include "tick.h"

namespace emc {
    // Act as a Global Namer
    class Namer {
    protected:
        Tick _tick;
        std::map<std::string, Name> _constants;
        
    public:
        Namer(Name::ID init) : _tick(init) {}
        ~Namer() {}
        
    public:
        Namer(const Namer &namer) = delete;
        Namer & operator = (const Namer &namer) = delete;
        
    public:
        bool passed(Name::ID id) const;
        size_t current() const { return _tick.current(); }
        
    protected:
        Name generate(const NameType &type, const std::string &name);
        
    public:
        Name variable(const std::string &name);
        Name variable(const std::string &name, Name::ID id);
        Name constant(const std::string &name);
        Name constant(const std::string &name, Name::ID id) = delete;
        bool exist(const std::string &name) const;
        Name name(const std::string &name) const;
        
    public:
        bool jump(Name::ID id) { return _tick.jump(id); }
    };
}

#endif /* defined(__explicit__namer__) */
