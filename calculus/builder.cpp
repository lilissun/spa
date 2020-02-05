//
//  builder.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "builder.h"

#include <sstream>

#include "common/runtime.h"
#include "symbolic/term.h"
#include "symbolic/event.h"

#include "process.h"
#include "null.h"
#include "parallel.h"
#include "replication.h"
#include "if.h"
#include "nonce.h"
#include "timestamp.h"
#include "unique.h"
#include "wait.h"
#include "let.h"
#include "input.h"
#include "output.h"
#include "init.h"
#include "join.h"
#include "accept.h"
#include "secrecy.h"
#include "open.h"
#include "call.h"

#include "expression.h"
#include "function.h"
#include "tuple.h"
#include "name.h"
#include "value.h"
#include "identifier.h"

#include "event.h"
#include "query.h"
#include "constraint.h"

namespace cal {
    Builder::~Builder()
    {
        
    }
    
    void Builder::declare_init_event(const std::string &name)
    {
        auto res = _events.insert(std::make_pair(name, EventType::EVENT_INIT));
        if (res.second == false) {
            std::stringstream ss;
            ss << "event " << name << " has already been declared";
            USER_ERROR(ss.str());
        }
    }

    void Builder::declare_join_event(const std::string &name)
    {
        auto res = _events.insert(std::make_pair(name, EventType::EVENT_JOIN));
        if (res.second == false) {
            std::stringstream ss;
            ss << "event " << name << " has already been declared";
            USER_ERROR(ss.str());
        }
    }

    void Builder::declare_accept_event(const std::string &name)
    {
        auto res = _events.insert(std::make_pair(name, EventType::EVENT_ACCEPT));
        if (res.second == false) {
            std::stringstream ss;
            ss << "event " << name << " has already been declared";
            USER_ERROR(ss.str());
        }
    }

    Name * Builder::name(const std::string &name, const com::Location &location)
    {
        return new Name(name, location);
    }
    
    Value * Builder::value(int value, const com::Location &location)
    {
        return new Value(value, location);
    }

    Identifier * Builder::identifier(bool variable, const std::string &name, const com::Location &location)
    {
        return new Identifier(variable, name, location);
    }

    Tuple * Builder::tuple(std::vector<Expression *> arguments, const com::Location &location)
    {
        return new Tuple(arguments, location);
    }

    Function * Builder::function(const std::string &name, Tuple *arguments, const com::Location &location)
    {
        return new Function(name, arguments, location);
    }

    Function * Builder::plus(Expression *left, Expression *right, const com::Location &location)
    {
        return new Function(sym::FunctionType::FUNCTION_PLUS, new Tuple({left, right}, location), location);
    }

    Function * Builder::minus(Expression *left, Expression *right, const com::Location &location)
    {
        return new Function(sym::FunctionType::FUNCTION_MINUS, new Tuple({left, right}, location), location);
    }

    Function * Builder::multiply(Expression *left, Expression *right, const com::Location &location)
    {
        return new Function(sym::FunctionType::FUNCTION_MULTIPLY, new Tuple({left, right}, location), location);
    }
    
    Constraint * Builder::equal(Expression *left, Expression *right, const com::Location &location)
    {
        return new Constraint(left, sym::ConstraintType::CONSTRAINT_EQUAL, right);
    }
    
    Constraint * Builder::less(Expression *left, Expression *right, const com::Location &location)
    {
        return new Constraint(left, sym::ConstraintType::CONSTRAINT_LESS, right);
    }
    
    Constraint * Builder::less_equal(Expression *left, Expression *right, const com::Location &location)
    {
        return new Constraint(left, sym::ConstraintType::CONSTRAINT_LESS_EQUAL, right);
    }
    
    Constraint * Builder::greater(Expression *left, Expression *right, const com::Location &location)
    {
        return new Constraint(left, sym::ConstraintType::CONSTRAINT_GREATER, right);
    }
    
    Constraint * Builder::greater_equal(Expression *left, Expression *right, const com::Location &location)
    {
        return new Constraint(left, sym::ConstraintType::CONSTRAINT_GREATER_EQUAL, right);
    }
    
    Null * Builder::null_process(const com::Location &location)
    {
        return new Null(location);
    }
    
    Parallel * Builder::parallel_process(std::vector<Process *> processes, const com::Location &location)
    {
        return new Parallel(processes, location);
    }
    
    Replication * Builder::replication_process(Process *next_process, const com::Location &location)
    {
        return new Replication(next_process, location);
    }
    
    Nonce * Builder::nonce_process(Name *nonce, Process *next_process, const com::Location &location)
    {
        return new Nonce(nonce, next_process, location);
    }
    
    Timestamp * Builder::timestamp_process(Name *timestamp, const std::string &clock, Process *next_process, const com::Location &location)
    {
        return new Timestamp(timestamp, clock, next_process, location);
    }
    
    Unique * Builder::unique_process(Expression *expression, Name *database, Process *next_process, const com::Location &location)
    {
        return new Unique(expression, database, next_process, location);
    }
    
    If * Builder::if_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new If(left, IfType::IF_EQUAL, right, then_process, else_process, location);
    }
    
    If * Builder::if_not_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new If(left, IfType::IF_NOT_EQUAL, right, then_process, else_process, location);
    }
    
    If * Builder::if_less_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new If(left, IfType::IF_LESS, right, then_process, else_process, location);
    }
    
    If * Builder::if_less_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new If(left, IfType::IF_LESS_EQUAL, right, then_process, else_process, location);
    }
    
    If * Builder::if_greater_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new If(left, IfType::IF_GREATER, right, then_process, else_process, location);
    }
    
    If * Builder::if_greater_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new If(left, IfType::IF_GREATER_EQUAL, right, then_process, else_process, location);
    }
    
    Wait * Builder::wait_equal_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location)
    {
        return new Wait(timestamp, clock, left, WaitType::WAIT_EQUAL, right, next_process, location);
    }
    
    Wait * Builder::wait_less_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location)
    {
        return new Wait(timestamp, clock, left, WaitType::WAIT_LESS, right, next_process, location);
    }
    
    Wait * Builder::wait_less_equal_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location)
    {
        return new Wait(timestamp, clock, left, WaitType::WAIT_LESS_EQUAL, right, next_process, location);
    }
    
    Wait * Builder::wait_greater_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location)
    {
        return new Wait(timestamp, clock, left, WaitType::WAIT_GREATER, right, next_process, location);
    }
    
    Wait * Builder::wait_greater_equal_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location)
    {
        return new Wait(timestamp, clock, left, WaitType::WAIT_GREATER_EQUAL, right, next_process, location);
    }
    
    Let * Builder::let_process(Expression *expression, Function *function, Process *then_process, Process *else_process, const com::Location &location)
    {
        return new Let(expression, function, then_process, else_process, location);
    }

    Input * Builder::input_process(Expression *expression, Process *next_process, const com::Location &location)
    {
        return new Input(expression, next_process, location);
    }

    Output * Builder::output_process(Expression *term, Process *next_process, const com::Location &location)
    {
        return new Output(term, next_process, location);
    }

    Process * Builder::event_process(Name *name, Tuple *arguments, Name *timestamp, Process *next_process, const com::Location &location)
    {
        auto it = _events.find(name->name());
        if (it == _events.end()) {
            std::stringstream ss;
            ss << "event " << name->name() << " has not been declared";
            USER_ERROR(ss.str());
        } else if (it->second == EventType::EVENT_INIT) {
            return new Init(name, arguments, timestamp, next_process, location);
        } else if (it->second == EventType::EVENT_JOIN) {
            return new Join(name, arguments, timestamp, next_process, location);
        } else {
            ASSERT(it->second == EventType::EVENT_ACCEPT);
            return new Accept(name, arguments, timestamp, next_process, location);
        }
    }

    Secrecy * Builder::secrecy_process(Expression *term, Process *next_process, const com::Location &location)
    {
        return new Secrecy(term, next_process, location);
    }

    Open * Builder::open_process(Expression *term, Process *next_process, const com::Location &location)
    {
        return new Open(term, next_process, location);
    }

    Call * Builder::call_process(Name *name, Tuple *arguments, const com::Location &location)
    {
        return new Call(name, arguments, location);
    }
    
    Event * Builder::event(Name *name, Tuple *arguments, Name *timestamp, const com::Location &location)
    {
        auto it = _events.find(name->name());
        if (it == _events.end()) {
            std::stringstream ss;
            ss << "event " << name->name() << " has not been declared";
            USER_ERROR(ss.str());
        } else {
            return new Event(it->second, name, arguments, timestamp);
        }
    }
    
    Correspondence * Builder::correspondence(Expression *identifier, const com::Location &location)
    {
        return new Correspondence(identifier);
    }
}