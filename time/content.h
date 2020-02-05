//
//  content.h
//  time
//
//  Created by Li Li on 19/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __time__content__
#define __time__content__

#include <iostream>

namespace tim {
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

#endif /* defined(__time__content__) */
