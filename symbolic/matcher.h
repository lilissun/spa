//
//  matcher.h
//  symbolic
//
//  Created by Li Li on 21/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __symbolic__matcher__
#define __symbolic__matcher__

#include <iostream>
#include <map>
#include <vector>

#include "term.h"
#include "explicit/name.h"

namespace sym {
    class Pattern;
    class VariablePattern;
    class Matcher {
    protected:
        std::vector<Pattern *> _patterns; // owned
        std::map<emc::Name, Pattern *> _naming; // referred
        
    public:
        Matcher() {}
        ~Matcher();
        
    public:
        void pattern(Pattern *pattern); 
        Pattern * variable(const emc::Name &name);
        void map(const emc::Name &name, Pattern *pattern);
    };
}

#endif /* defined(__symbolic__matcher__) */
