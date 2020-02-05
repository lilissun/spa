//
//  if.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__if__
#define __calculus__if__

#include "process.h"

namespace cal {
    enum IfType {
        IF_EQUAL,
        IF_NOT_EQUAL,
        IF_GREATER,
        IF_GREATER_EQUAL,
        IF_LESS,
        IF_LESS_EQUAL,
        IF_TYPE_END,
    };
    
    const std::string IfTypeName[IfType::IF_TYPE_END] = {
        "==",
        "!=",
        ">",
        ">=",
        "<",
        "<=",
    };
    
    // if _condition then _then_process else _else_process
    class If : public Process {
    protected:
        Expression *_left; // owned
        IfType _op;
        Expression *_right; // owned
        Process *_then_process; // owned
        Process *_else_process; // owned
        
    public:
        If(Expression *left, IfType op, Expression *right, Process *then_process, Process *else_process, const com::Location &location) : Process(ProcessType::PROCESS_IF, location), _left(left), _op(op), _right(right), _then_process(then_process), _else_process(else_process) {}
        virtual ~If();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__if__) */
