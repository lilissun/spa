//
//  renamer.h
//  explicit
//
//  Created by Li Li on 15/10/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __explicit__renamer__
#define __explicit__renamer__

#include <iostream>
#include <map>

#include "common/exception.h"
#include "name.h"

namespace emc {
    class Namer;
    class Renamer {
    protected:
        std::map<Name, Name> _mapping;
        
    public:
        Renamer() {}
        Renamer(const Renamer &renamer) : _mapping(renamer._mapping) {}
        ~Renamer() {}
        
    public:
        Renamer & operator = (const Renamer &renamer) = delete;
        
    public:
        Name rename(const Name &name, Namer &namer);
        Name name(const Name &name) const;
        bool exist(const Name &name) const { return _mapping.find(name) != _mapping.end(); }
        const std::map<Name, Name> & map() const { return _mapping; }
    };
}

#endif /* defined(__explicit__renamer__) */
