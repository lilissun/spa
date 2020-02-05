//
//  iostream.cpp
//  common
//
//  Created by Li Li on 25/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "iostream.h"

namespace com {
    std::ostream & operator << (std::ostream &os, const tabs &t)
    {
        os << std::left << std::setw((int)t._length) << "";
        return os;
    }
}