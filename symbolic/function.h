//
//  function.h
//  symbolic
//
//  Created by Li Li on 22/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __symbolic__function__
#define __symbolic__function__

#include <iostream>
#include <vector>

#include "common/debug.h"
#include "explicit/name.h"
#include "term.h"

namespace sym {
    enum FunctionType {
        FUNCTION_NEGATIVE,

        FUNCTION_PLUS,
        FUNCTION_MINUS,
        FUNCTION_MULTIPLY,
        FUNCTION_DIVIDE,
        
        FUNCTION_USER_DEFINED,
    };
    
    enum FunctionPriority {
        FUNCTION_PRIORITY_PLUS,
        FUNCTION_PRIORITY_MULTIPLY,
        FUNCTION_PRIORITY_COMPLEX,
        FUNCTION_PRIORITY_USER_DEFINED,
    };
    bool term_has_higher_function_priority(const Term *term, FunctionPriority priority);
    bool term_has_lower_function_priority(const Term *term, FunctionPriority priority);
    
    class FunctionInfo {
    protected:
        std::string _symbol;
        size_t _size;
        FunctionPriority _priority;
        bool _surround;
        
    public:
        FunctionInfo(std::string symbol, size_t size, FunctionPriority priority, bool surround)
        : _symbol(symbol), _size(size), _priority(priority), _surround(surround) {}
        
    public:
        const std::string & symbol() const { return _symbol; }
        size_t size() const { return _size; }
        FunctionPriority priority() const { return _priority; }
        bool surround() const { return _surround; }
    };
    
    const FunctionInfo FunctionTypeInfo[FUNCTION_USER_DEFINED + 1] = {
        FunctionInfo("-", 1, FunctionPriority::FUNCTION_PRIORITY_PLUS, false),
        
        FunctionInfo("+", 2, FunctionPriority::FUNCTION_PRIORITY_PLUS, false),
        FunctionInfo("-", 2, FunctionPriority::FUNCTION_PRIORITY_PLUS, false),
        FunctionInfo("*", 2, FunctionPriority::FUNCTION_PRIORITY_MULTIPLY, false),
        FunctionInfo("/", 2, FunctionPriority::FUNCTION_PRIORITY_MULTIPLY, false),
        
        FunctionInfo("f", 1, FunctionPriority::FUNCTION_PRIORITY_USER_DEFINED, true),
    };
    
    class TuplePattern;
    class FunctionPattern : public Pattern {
        
        friend FunctionPattern * make_function_pattern(const std::string &name, TuplePattern *arguments, emc::Namer &namer);
        friend FunctionPattern * make_negative_pattern(Pattern *term);
        friend FunctionPattern * make_plus_pattern(Pattern *left, Pattern *right);
        friend FunctionPattern * make_minus_pattern(Pattern *left, Pattern *right);
        friend FunctionPattern * make_multiply_pattern(Pattern *left, Pattern *right);
        friend FunctionPattern * make_divide_pattern(Pattern *left, Pattern *right);
        
    protected:
        FunctionType _op;
        emc::Name _name;
        TuplePattern *_arguments; // refered
        
    protected:
        FunctionPattern(const emc::Name &name, TuplePattern *arguments)
        : Pattern(TermType::TERM_FUNCTION), _op(FunctionType::FUNCTION_USER_DEFINED), _name(name), _arguments(arguments) { ASSERT(name.id() != -1); }
        FunctionPattern(FunctionType op, TuplePattern *arguments)
        : Pattern(TermType::TERM_FUNCTION), _op(op), _name(emc::INVALID_NAME), _arguments(arguments)
        { ASSERT(_op != FunctionType::FUNCTION_USER_DEFINED); }
        
    public:
        virtual ~FunctionPattern() {}
        
    public:
        FunctionType op() const { return _op; }
        const emc::Name & name() const { return _name; }
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    FunctionPattern * make_function_pattern(const std::string &name, TuplePattern *arguments, emc::Namer &namer);
    FunctionPattern * make_negative_pattern(Pattern *term);
    FunctionPattern * make_plus_pattern(Pattern *left, Pattern *right);
    FunctionPattern * make_minus_pattern(Pattern *left, Pattern *right);
    FunctionPattern * make_multiply_pattern(Pattern *left, Pattern *right);
    FunctionPattern * make_divide_pattern(Pattern *left, Pattern *right);
    
    class Tuple;
    class Function : public Term {
        
        friend Function * make_function(const std::string &name, Tuple *arguments, emc::Namer &namer);
        friend Function * make_negative(Term *term);
        friend Function * make_plus(Term *left, Term *right);
        friend Function * make_minus(Term *left, Term *right);
        friend Function * make_multiply(Term *left, Term *right);
        friend Function * make_divide(Term *left, Term *right);

    protected:
        FunctionType _op;
        emc::Name _name;
        Tuple *_arguments; // owned
        
    protected:
        Function(const emc::Name &name, Tuple *arguments);
        Function(FunctionType op, Tuple *arguments);
        
    public:
        virtual ~Function();
        
    public:
        virtual bool same(const emc::Name &name) const { return _name == name; }
        virtual const emc::Name & name() const { return _name; }
        
    public:
        FunctionType op() const { return _op; }
        const Tuple * arguments() const { return _arguments; }
        std::vector<Function *> reverse(TermSort sort) const;
        
    protected:
        virtual TermSort _sort() const;
        
    public:
        virtual Term * clone() const;
        virtual Term * map(const Unifier &unifier) const;
        virtual Term * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        virtual void unify(const Term *term, Unifier &unifier) const;
        virtual void unify_to(const Term *term, Unifier &unifier) const;
        virtual bool match(const Pattern *pattern, const Unifier &unifier) const;
        virtual size_t depth(const emc::Name &name, const Unifier &unifier) const;
        virtual size_t weight() const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        virtual void signature(const Unifier &unifier, std::ostream &os) const;
        virtual void signature(std::ostream &os) const;
        
    public:
        virtual emc::Value evaluate(const emc::Valuator &valuator) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    Function * make_function(const std::string &name, Tuple *arguments, emc::Namer &namer);
    Function * make_negative(Term *term);
    Function * make_plus(Term *left, Term *right);
    Function * make_minus(Term *left, Term *right);
    Function * make_multiply(Term *left, Term *right);
    Function * make_divide(Term *left, Term *right);
}

#endif /* defined(__symbolic__function__) */
