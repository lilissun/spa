//
//  fact.cpp
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "fact.h"

namespace sym {
    std::set<emc::Name> Fact::get_subnames(unsigned long mask, const Unifier &unifier) const
    {
        std::set<emc::Name> names;
        subnames(mask, unifier, names);
        return names;
    }
    
    std::set<emc::Name> Fact::get_subnames(unsigned long mask) const
    {
        std::set<emc::Name> names;
        subnames(mask, names);
        return names;
    }
}