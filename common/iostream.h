//
//  iostream.h
//  common
//
//  Created by Li Li on 25/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __common__iostream__
#define __common__iostream__

#include <iostream>
#include <iomanip>

namespace com {
    class tabs {
    protected:
        size_t _length;
        
    public:
        tabs(size_t size, size_t times) : _length(size * times) {}
        
    public:
        friend std::ostream & operator << (std::ostream &os, const tabs &t);
    };
}

#endif /* defined(__common__iostream__) */
