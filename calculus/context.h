//
//  context.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__context__
#define __calculus__context__

#include "explicit/namer.h"
#include "builder.h"
#include "program.h"

namespace cal {
    class Context {
    protected:
        emc::Namer _namer;
        Builder _builder;
        Program _program;
        
    public:
        Context() : _namer(0), _builder(this), _program(this) {}
        virtual ~Context() {}
        
    public:
        Context(const Context &) = delete;
        Context & operator = (const Context &) = delete;
        
    public:
        emc::Namer & namer() { return _namer; }
        Builder * builder() { return &_builder; }
        Program * program() { return &_program; }
    };
}

#endif /* defined(__calculus__context__) */
