//
//  matcher.cpp
//  symbolic
//
//  Created by Li Li on 21/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "matcher.h"



#include "common/runtime.h"
#include "variable.h"

namespace sym {
    Matcher::~Matcher()
    {
        for (auto i = _patterns.begin(); i != _patterns.end(); ++i) {
            delete *i;
        }
    }
    
    void Matcher::pattern(Pattern *pattern)
    {
        _patterns.push_back(pattern);
    }
    
    Pattern * Matcher::variable(const emc::Name &name)
    {
        auto it = _naming.find(name);
        if (it == _naming.end()) {
            VariablePattern *pattern = new VariablePattern();
            _patterns.push_back(pattern);
            _naming.insert(std::make_pair(name, pattern));
            return pattern;
        } else {
            ASSERT(it->second->type() == TermType::TERM_VARIABLE);
            return it->second;
        }
    }
    
    void Matcher::map(const emc::Name &name, Pattern *pattern)
    {
        _patterns.push_back(pattern);
        
        auto it = _naming.find(name);
        VariablePattern *variable = nullptr;
        if (it == _naming.end()) {
            variable = new VariablePattern();
            auto res = _naming.insert(std::make_pair(name, variable));
            ASSERT(res.second);
        } else {
            ASSERT(it->second->type() == TermType::TERM_VARIABLE);
            variable = (VariablePattern *) it->second;
        }
        ASSERT(variable != nullptr);
        variable->append(pattern);
    }
}