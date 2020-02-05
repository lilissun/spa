//
//  helper.cpp
//  calculus
//
//  Created by Li Li on 24/5/15.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "helper.h"

#include "common/runtime.h"
#include "explicit/namer.h"
#include "context.h"

extern void scan_file(const std::string &filename);
extern int yyparse();

extern cal::Context *context;
extern cal::Program *program;
extern cal::Builder *builder;
extern cal::State *state;
extern int yydebug;

namespace cal {
    void Helper::debug() const
    {
        yydebug = 1;
    }
    
    void Helper::init() const
    {
        if (context == nullptr) {
            context = new Context();
            builder = context->builder();
            program = context->program();
            state = program->init_state();
        }
    }
    
    void Helper::scan(const std::string &filename) const
    {
        scan_file(filename.c_str());
        yyparse();
    }
    
    Context * Helper::get() const
    {
        auto ret = context;
        context = nullptr;
        builder = nullptr;
        program = nullptr;
        return ret;
    }
}