//
//  helper.cpp
//  time
//
//  Created by Li Li on 16/10/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "helper.h"

#include "common/runtime.h"
#include "explicit/namer.h"
#include "builder.h"
#include "context.h"
#include "rule.h"
#include "solver.h"

extern void scan_file(const std::string &filename);
extern int yyparse();

extern tim::Context *context;
extern int yydebug;

namespace tim {
    void Helper::debug() const
    {
        yydebug = 1;
    }
    
    void Helper::init() const
    {
        if (context == nullptr) {
            context = new Context();
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
        return ret;
    }
}