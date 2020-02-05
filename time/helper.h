//
//  helper.h
//  time
//
//  Created by Li Li on 16/10/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __time__helper__
#define __time__helper__

#include <iostream>
#include <list>
#include <string>

namespace tim {

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

#endif /* defined(__time__helper__) */
