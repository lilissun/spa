//
//  builder.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__builder__
#define __calculus__builder__

#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <set>

#include "content.h"
#include "event.h"

namespace com {
    class Location;
}

namespace emc {
    class Name;
    class Namer;
    class Indexer;
}

namespace sym {
    class Term;
    class Nonce;
    class Variable;
    class Number;
    class Constant;
    class Function;
    class Value;
    class Binary;
    class Event;
    class Guard;
    class Constraint;
}

namespace cal {
    class Expression;
    class Function;
    class Tuple;
    class Name;
    class Value;
    class Identifier;
    
    class Process;
    class Null;
    class Parallel;
    class Replication;
    class Nonce;
    class Timestamp;
    class Unique;
    class If;
    class Wait;
    class Let;
    class Input;
    class Output;
    class Init;
    class Join;
    class Accept;
    class Secrecy;
    class Open;
    class Call;
    class Context;
    
    class Event;
    class Correspondence;
    class Constraint;
    
    class Builder : public Content {
    protected:
        std::map<std::string, EventType> _events;
        
    public:
        Builder(Context *context) : Content(context) {}
        virtual ~Builder();
        
    public:
        Builder(const Builder &builder) = delete;
        Builder & operator = (const Builder &builder) = delete;
        
    public:
        void declare_init_event(const std::string &name);
        void declare_join_event(const std::string &name);
        void declare_accept_event(const std::string &name);
        
    public:
        Name * name(const std::string &name, const com::Location &location);
        Value * value(int value, const com::Location &location);
        Identifier * identifier(bool variable, const std::string &name, const com::Location &location);
        Tuple * tuple(std::vector<Expression *> arguments, const com::Location &location);
        Function * function(const std::string &name, Tuple *arguments, const com::Location &location);
        Function * plus(Expression *left, Expression *right, const com::Location &location);
        Function * minus(Expression *left, Expression *right, const com::Location &location);
        Function * multiply(Expression *left, Expression *right, const com::Location &location);
        Constraint * equal(Expression *left, Expression *right, const com::Location &location);
        Constraint * less(Expression *left, Expression *right, const com::Location &location);
        Constraint * less_equal(Expression *left, Expression *right, const com::Location &location);
        Constraint * greater(Expression *left, Expression *right, const com::Location &location);
        Constraint * greater_equal(Expression *left, Expression *right, const com::Location &location);
        
    public:
        Null * null_process(const com::Location &location);
        Parallel * parallel_process(std::vector<Process *> processes, const com::Location &location);
        Replication * replication_process(Process *next_process, const com::Location &location);
        Nonce * nonce_process(Name *nonce, Process *next_process, const com::Location &location);
        Unique * unique_process(Expression *expression, Name *database, Process *next_process, const com::Location &location);
        Timestamp * timestamp_process(Name *timestamp, const std::string &clock, Process *next_process, const com::Location &location);
        If * if_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location);
        If * if_not_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location);
        If * if_less_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location);
        If * if_less_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location);
        If * if_greater_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location);
        If * if_greater_equal_process(Expression *left, Expression *right, Process *then_process, Process *else_process, const com::Location &location);
        Wait * wait_equal_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location);
        Wait * wait_less_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location);
        Wait * wait_less_equal_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location);
        Wait * wait_greater_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location);
        Wait * wait_greater_equal_process(Name *timestamp, const std::string &clock, Expression *left, Expression *right, Process *next_process, const com::Location &location);
        Let * let_process(Expression *expression, Function *function, Process *then_process, Process *else_process, const com::Location &location);
        Input * input_process(Expression *expression, Process *next_process, const com::Location &location);
        Output * output_process(Expression *term, Process *next_process, const com::Location &location);
        Process * event_process(Name *name, Tuple *arguments, Name *timestamp, Process *next_process, const com::Location &location);
        Secrecy * secrecy_process(Expression *term, Process *next_process, const com::Location &location);
        Open * open_process(Expression *term, Process *next_process, const com::Location &location);
        Call * call_process(Name *name, Tuple *arguments, const com::Location &location);
        
    public:
        Event * event(Name *name, Tuple *arguments, Name *timestamp, const com::Location &location);
        Correspondence * correspondence(Expression *identifier, const com::Location &location);
        
    public:
//        void info(std::ostream &os);
    };
}

#endif /* defined(__calculus__builder__) */
