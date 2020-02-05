//
//  wait.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__wait__
#define __calculus__wait__

#include "process.h"

namespace cal {
    enum WaitType {
        WAIT_EQUAL,
        WAIT_GREATER,
        WAIT_GREATER_EQUAL,
        WAIT_LESS,
        WAIT_LESS_EQUAL,
        WAIT_TYPE_END,
    };
    
    const std::string WaitTypeName[WaitType::WAIT_TYPE_END] = {
        "==",
        ">",
        ">=",
        "<",
        "<=",
    };

    // wait _timestamp : _constraint then _next_process
    class Wait : public Process {
    protected:
        Name *_timestamp; // owned
        std::string _clock;
        Expression *_left; // owned
        WaitType _op;
        Expression *_right; // owned
        Process *_next_process; // owned
        
    public:
        Wait(Name *timestamp, const std::string &clock, Expression *left, WaitType op, Expression *right, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_WAIT, location), _timestamp(timestamp), _clock(clock), _left(left), _op(op), _right(right), _next_process(next_process) {}
        virtual ~Wait();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__wait__) */
