//
//  indexer.h
//  explicit
//
//  Created by Li Li on 2/6/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __explicit__indexer__
#define __explicit__indexer__

#include <iostream>
#include <map>

#include "name.h"

namespace emc {
    // Act as a Local Namer
    // Currently, we not allow sub local domain, which means:
    // { int n = 1; { int n = 2; } } is not allowed.
    class Namer;
    class Indexer {
    protected:
        std::map<std::string, Name> _variables;
        
    public:
        Indexer() : _variables() {}
        ~Indexer() {}
        
    public:
        Indexer(const Indexer &indexer) = delete;
        Indexer & operator = (const Indexer &indexer) = delete;
        
    public:
        Name variable(const std::string &name, Namer &namer);
        Name constant(const std::string &name, Namer &namer);
        bool exist(const std::string &name, Namer &namer) const;
        Name name(const std::string &name, Namer &namer) const;
        void clear() { _variables.clear(); }
    };
}

#endif /* defined(__explicit__indexer__) */
