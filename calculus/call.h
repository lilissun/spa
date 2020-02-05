//
//  call.h
//  calculus
//
//  Created by Li Li on 27/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__call__
#define __calculus__call__

#include "process.h"

#include "explicit/name.h"

namespace cal {
    class Tuple;
    // @envoke _name(_arguments)
    class Call : public Process {
    protected:
        Name *_name; // owned
        Tuple *_arguments; // owned
        
    public:
        Call(Name *name, Tuple *arguments, const com::Location &location) : Process(ProcessType::PROCESS_CALL, location), _name(name), _arguments(arguments) {}
        virtual ~Call();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__call__) */
