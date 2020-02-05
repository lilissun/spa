//
//  query.h
//  calculus
//
//  Created by Li Li on 24/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__query__
#define __calculus__query__

#include <iostream>
#include <vector>

#include "symbolic/model.h"
#include "symbolic/constraint.h"

namespace tim {
    class Correspondence;
}

namespace cal {
    class State;
    class Program;
    class Event;
    class Expression;
    class Correspondence : public sym::Model {
    protected:
        Expression *_identifier;
        Event *_init;
        std::vector<Event *> _joins;
        Event *_accept;
        std::vector<std::tuple<Expression *, sym::ConstraintType, Expression *>> _constraints;
        bool _enforced;
        
    public:
        Correspondence(Expression *identifier) : _identifier(identifier), _init(nullptr), _joins(), _accept(nullptr), _enforced(false) {}
        virtual ~Correspondence();
        
    public:
        Correspondence * add_event(Event *event);
        Correspondence * add_constraint(Expression *left, sym::ConstraintType type, Expression *right) { _constraints.push_back(std::make_tuple(left, type, right)); return this; }
        Correspondence * enforced() { _enforced = true; return this; }
        
    public:
        tim::Correspondence * evaluate(const Program *program, const State *state) const;
        
    public:
        void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__query__) */
