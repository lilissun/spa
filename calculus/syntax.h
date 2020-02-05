//
//  syntax.h
//  calculus
//
//  Created by Li Li on 14/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__syntax__
#define __calculus__syntax__

#include <iostream>

namespace com {
    class Node;
}

#define YYSTYPE cal::Syntax

namespace cal {
    struct Syntax {
        int value;
        std::string name;
        com::Node *node;
    };
}

#endif /* defined(__calculus__syntax__) */
