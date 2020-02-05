//
//  helper.h
//  calculus
//
//  Created by Li Li on 24/5/15.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __calculus__helper__
#define __calculus__helper__

#include <iostream>
#include <list>

namespace cal {
    class Context;
    class Helper {
    public:
        Helper() {}
        ~Helper() {}
        
    public:
        void debug() const;
        void init() const;
        void scan(const std::string &filename) const;
        Context * get() const;
    };
}

#endif /* defined(__calculus__helper__) */
