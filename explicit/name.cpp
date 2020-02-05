//
//  name.cpp
//  explicit
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "name.h"

namespace emc {
    Name & Name::operator = (const Name &name)
    {
        _type = name._type;
        _name = name._name;
        _id = name._id;
        return *this;
    }
    
    std::ostream & operator << (std::ostream &os, const Name &name)
    {
        if (name.type() == NameType::CONSTANT) {
            os << name._name;
        } else {
            os << name._name << "@" << name._id;
        }
        return os;
    }
}