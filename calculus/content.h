//
//  content.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__content__
#define __calculus__content__

#include <iostream>

namespace cal {
    class Context;
    class Content {
    protected:
        Context *_context; // referred
        
    public:
        Content(Context *context) : _context(context) {}
        virtual ~Content() {}
        
    public:
        Context * context() const { return _context; }
    };
}

#endif /* defined(__calculus__content__) */
