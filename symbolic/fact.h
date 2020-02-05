//
//  fact.h
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__fact__
#define __symbolic__fact__

#include <set>

#include "model.h"
#include "explicit/name.h"

namespace emc {
    class Renamer;
    class Namer;
    class Indexer;
}

namespace sym {
    const std::string fact_left = "(";
    const std::string fact_signature_separator = ":";
    const std::string fact_right = ")";
    const std::string fact_key = "*";
    const std::string fact_argument_separator = ",";
    const std::string fact_keycount_separator = "/";
    const std::string fact_space = " ";
    const std::string fact_mapsto = "->";
    
    class Unifier;
    class Selector;
    class Pruner;
    class Matcher;
    class Term;
    class Key;
    class Pattern;
    class Guard;
    class Constraint;
    class Event;
    class Key;
    class State;
    
    enum FactType {
        FACT_GUARD,
        FACT_EVENT,
        FACT_KEY,
        FACT_STATE,
        FACT_CONSTRAINT,
        FACT_RELATION,
        FACT_TYPE_END,
    };
    
    class Fact : public Model {
    protected:
        FactType _type;
        
    public:
        Fact(FactType type) : _type(type) {}
        virtual ~Fact() {}
        
    public:
        FactType type() const { return _type; }
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const = 0;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const = 0;
        
    public:
        std::set<emc::Name> get_subnames(unsigned long mask, const Unifier &unifier) const;
        std::set<emc::Name> get_subnames(unsigned long mask) const;
        
    public:
        virtual void info(std::ostream &os) const = 0;
    };
}

#endif /* defined(__symbolic__fact__) */
