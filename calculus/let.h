//
//  let.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__let__
#define __calculus__let__

#include "process.h"

namespace cal {
    // let @timestamp _name = _function then _then_process else _else_process
    class Let : public Process {
    protected:
        Expression *_identifier; // owned
        Function *_function; // owned
        Process *_then_process; // owned
        Process *_else_process; // owned
        
    public:
        Let(Expression *identifier, Function *function, Process *then_process, Process *else_process, const com::Location &location)
        : Process(ProcessType::PROCESS_LET, location), _identifier(identifier)
        , _function(function), _then_process(then_process), _else_process(else_process) {}
        virtual ~Let();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__let__) */
