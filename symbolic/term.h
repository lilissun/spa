//
//  term.h
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__term__
#define __symbolic__term__

#include <set>

#include "common/runtime.h"
#include "model.h"

namespace emc {
    class Valuator;
    class Renamer;
    class Namer;
    class Indexer;
    class Value;
    class Name;
}

namespace sym {
    class Unifier;
    class Matcher;
    class Pattern;
    // the ordering is used in the relationship processing,
    // do not change it if you do not know what it is for.
    enum TermType {
        TERM_VARIABLE   = 0x001, // Term        // symbolic variable
        TERM_NUMBER     = 0x002, // Term & Math // numeric symbolic variable
        TERM_CONSTANT   = 0x004, //        Math // numeric symbolic constant
        TERM_NONCE      = 0x008, // Term        // symbolic nonce
        TERM_FUNCTION   = 0x010, // Term        // symbolic function
        TERM_NAME       = 0x020, // Term        // symbolic name
        TERM_VALUE      = 0x040, //        Math // numeric value
        TERM_LIST       = 0x080, // Term        // tuple with unfixed length (w./w.o. max length)
        TERM_PAIR       = 0x100, // Term        // special function without name of size = 2
        TERM_TUPLE      = 0x200, // Term        // special function without name of size = n
        TERM_REFERENCE  = 0x400, // Term        // reference of events and states
        TERM_TYPE_END   = 0x000,
    };
    
    enum TermSort {
        TERM_SORT_VALUE,
        TERM_SORT_NUMBER,
        TERM_SORT_USER_DEFINED,
        TERM_SORT_LINEAR_EXPRESSION,
        TERM_SORT_COMPLEX,
        TERM_SORT_END,
    };
    
    const std::string term_name_mark = "[]";
    const std::string term_function_mark_left = "(";
    const std::string term_function_mark_right = ")";
    const std::string term_tuple_mark_left = "(";
    const std::string term_tuple_mark_right = ")";
    const std::string term_pair_mark_left = "(";
    const std::string term_pair_mark_right = ")";
    const std::string term_list_mark_left = "{";
    const std::string term_list_mark_right = "}";
    const std::string term_nonce_mark_left = "[";
    const std::string term_nonce_mark_right = "]";
    const std::string term_number_mark = "#";
    const std::string term_constant_mark = "$";
    const std::string term_math_mark_left = "(";
    const std::string term_math_mark_right = ")";
    const std::string term_value_mark = "%";
    const std::string term_reference_mark = "&";
    const std::string term_remaining = "@more";
    
    const std::string term_signature_separator = ":";
    const std::string term_argument_separator = ",";
    const std::string term_space = " ";
    const std::string term_key_mark = "*";
    const std::string term_any_mark = "-";

    // Pattern matching should be called like term->match(pattern) from outside. 
    class Term : public Model {
    protected:
        TermType _type;
        
    public:
        Term(TermType type) : _type(type) {}
        virtual ~Term() {}
        
    protected:
        friend class Function;
        virtual TermSort _sort() const = 0;
        
    public:
        TermType type() const { return _type; }
        TermSort sort() const;
        
    public:
        Term(const Term &term) = delete;
        Term & operator = (const Term &term) = delete;
        
    public:
        virtual Term * clone() const = 0;
        virtual Term * map(const Unifier &unifier) const = 0;
        virtual Term * rename(emc::Renamer &renamer, emc::Namer &namer) = 0;
        
    public:
        bool equal(const Term *term, const Unifier &unifier) const;
        bool equal(const Term *term) const;
        
    public:
        virtual bool same(const emc::Name &name) const = 0;
        virtual const emc::Name & name() const = 0;
        virtual void unify(const Term *term, Unifier &unifier) const = 0;
        virtual void unify_to(const Term *term, Unifier &unifier) const = 0;
        virtual bool match(const Pattern *pattern, const Unifier &unifier) const = 0;
        virtual size_t depth(const emc::Name &name, const Unifier &unifier) const = 0;
        virtual size_t weight() const = 0;
       
    public:
        bool can_unify(const Term *term, Unifier &unifier) const;
        bool can_unify_to(const Term *term, Unifier &unifier) const;
        bool can_match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const = 0;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const = 0;
        virtual void signature(const Unifier &unifier, std::ostream &os) const = 0;
        virtual void signature(std::ostream &os) const = 0;
        
    public:
        virtual emc::Value evaluate(const emc::Valuator &valuator) const = 0;
    };
    
    class Pattern : public Model {
    protected:
        TermType _type;
        
    public:
        Pattern(TermType type) : _type(type) {}
        virtual ~Pattern() {}
        
    public:
        Pattern(const Pattern &) = delete;
        Pattern & operator = (const Pattern &) = delete;
        
    public:
        TermType type() const { return _type; }
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const = 0;
    };

    const unsigned long mask_symbol_term_variable = TermType::TERM_VARIABLE | TermType::TERM_NONCE | TermType::TERM_NUMBER | TermType::TERM_REFERENCE;
    const unsigned long mask_symbol_term_constant = TermType::TERM_FUNCTION | TermType::TERM_VALUE | TermType::TERM_CONSTANT;
}

#endif /* defined(__symbolic__term__) */
