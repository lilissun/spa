//
//  syntax.h
//  time
//
//  Created by Li Li on 14/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __time__syntax__
#define __time__syntax__

#include <iostream>

namespace com {
    class Node;
}

#define YYSTYPE tim::Syntax

namespace tim {
    struct Syntax {
        int value;
        std::string name;
        com::Node *node;
    };
}

#endif /* defined(__time__syntax__) */
