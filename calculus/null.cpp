//
//  null.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "null.h"

#include "common/iostream.h"
#include "state.h"

namespace cal {
    void Null::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        return ;
    }
    
    void Null::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_null << process_endline << std::endl;
    }
}