//
//  context.h
//  time
//
//  Created by Li Li on 19/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __time__context__
#define __time__context__

#include "explicit/namer.h"
#include "symbolic/selector.h"
#include "symbolic/pruner.h"
#include "builder.h"
#include "solver.h"

namespace tim {
    class Context {
    protected:
        emc::Namer _namer;
        sym::Selector _selector;
        sym::Pruner _pruner;
        Builder _builder;
        Solver _solver;
        
    public:
        Context() : _namer(0), _selector(), _pruner(), _builder(this), _solver(this) {}
        virtual ~Context() {}
        
    public:
        Context(const Context &) = delete;
        Context & operator = (const Context &) = delete;
        
    public:
        emc::Namer & namer() { return _namer; }
        sym::Selector & selector() { return _selector; }
        sym::Pruner & pruner() { return _pruner; }
        Builder & builder() { return _builder; }
        Solver & solver() { return _solver; }
    };
}

#endif /* defined(__time__context__) */
