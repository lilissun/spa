//
//  program.cpp
//  calculus
//
//  Created by Li Li on 22/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "program.h"

#include <sstream>

#include "common/iostream.h"
#include "common/debug.h"

#include "explicit/indexer.h"
#include "explicit/namer.h"

#include "symbolic/name.h"
#include "symbolic/tuple.h"
#include "symbolic/function.h"
#include "symbolic/nonce.h"
#include "symbolic/number.h"
#include "symbolic/constant.h"
#include "symbolic/value.h"
#include "symbolic/variable.h"
#include "symbolic/event.h"
#include "symbolic/constraint.h"
#include "symbolic/ppl.h"

#include "time/rule.h"
#include "time/query.h"

#include "context.h"
#include "state.h"
#include "expression.h"
#include "function.h"
#include "tuple.h"
#include "name.h"
#include "value.h"
#include "process.h"
#include "identifier.h"
#include "query.h"
#include "event.h"
#include "constraint.h"

namespace cal {
    Program::Program(Context *context)
    : Content(context), _init_state(new State(_context)), _location(0), _shared_clock_rate(false)
    {
        register_parameter("pn");
        _init_state->declare_parameter("pn", com::Location());
    }
    
    Program::~Program()
    {
        for (auto i = _rules.begin(); i != _rules.end(); ++i) {
            delete *i;
        }
        for (auto i = _queries.begin(); i != _queries.end(); ++i) {
            delete *i;
        }
    }
    
    void Program::init(Process *init_process)
    {
        _init_process = init_process;
        
        _init_state->add_constraint(sym::make_constraint_greater(_context->program()->network_delay(), sym::make_value(0)));
        _init_state->init_history();
        _init_state->declare_clock(default_global_clock, com::Location());
        
        for (auto i = _registered_clocks.begin(); i != _registered_clocks.end(); ++i) {
            if (i->second.second == nullptr && _shared_clock_rate) {
                USER_ERROR("Inconsistent Shared Clock Rate or Variable Clock Rate");
            } else if (i->second.second != nullptr) {
                _constraints.push_back(sym::make_constraint_greater_equal(i->second.first->clone(), i->second.second->clone()));
                _constraints.push_back(sym::make_constraint_greater_equal(sym::make_plus(i->second.first->clone(), i->second.second->clone()), sym::make_value(0)));
            }
        }
    }
    
    void Program::execute()
    {
        ASSERT(_init_state != nullptr);
        ASSERT(_init_process != nullptr);
        _init_process->execute(this, _init_state, _rules);
        
        for (auto i = _registered_queries.begin(); i != _registered_queries.end(); ++i) {
            _queries.push_back((*i)->evaluate(this, _init_state));
        }
        
        for (auto i = _registered_constraints.begin(); i != _registered_constraints.end(); ++i) {
            _constraints.push_back((*i)->evaluate(_init_state));
        }
    }
    
    void Program::register_init_event(const std::string &name, int upperbound)
    {
        _context->builder()->declare_init_event(name);
        auto res = _registered_events.insert(std::make_pair(name, std::make_pair(EventType::EVENT_INIT, upperbound)));
        ASSERT(res.second);
    }
    
    void Program::register_join_event(const std::string &name, int upperbound)
    {
        _context->builder()->declare_join_event(name);
        auto res = _registered_events.insert(std::make_pair(name, std::make_pair(EventType::EVENT_JOIN, upperbound)));
        ASSERT(res.second);
    }
    
    void Program::register_accept_event(const std::string &name)
    {
        _context->builder()->declare_accept_event(name);
        auto res = _registered_events.insert(std::make_pair(name, std::make_pair(EventType::EVENT_ACCEPT, -1)));
        ASSERT(res.second);
    }
    
    void Program::register_function(Tuple *identifiers, Function *function, Expression *result, Expression *time, bool open)
    {
        auto res = _registered_functions.insert(std::make_pair(function->name(), std::make_tuple(identifiers, function, result, time, open)));
        ASSERT(res.second);
    }
    
    void Program::register_process(const std::string &name, Tuple *identifiers, Process *process)
    {
        auto res = _registered_process.insert(std::make_pair(name, std::make_pair(identifiers, process)));
        ASSERT(res.second);
    }
    
    void Program::register_query(Correspondence *correspondence)
    {
        _registered_queries.push_back(correspondence);
    }
    
    void Program::register_string(const std::string &name)
    {
        _registered_strings.push_back(name);
    }
    
    void Program::register_parameter(const std::string &name)
    {
        _registered_parameters.push_back(name);
    }
    
    void Program::register_constraint(Constraint *constraint)
    {
        _registered_constraints.push_back(constraint);
    }
    
    void Program::register_clock(const std::string &name, const std::string &maximum_drift)
    {
        _registered_clocks.insert(std::make_pair(name, std::make_pair(sym::make_constant(maximum_drift, _context->namer()), nullptr)));
    }
    
    void Program::register_clock(const std::string &name, const std::string &maximum_drift, const std::string &shared_drift)
    {
        _shared_clock_rate = true;
        _registered_clocks.insert(std::make_pair(name, std::make_pair(sym::make_constant(maximum_drift, _context->namer()), sym::make_constant(shared_drift, _context->namer()))));
    }

    
    std::pair<sym::Function *, sym::Term *> Program::function(const std::string &name) const
    {
        auto it = _registered_functions.find(name);
        ASSERT(it != _registered_functions.end());
        
        auto identifiers = std::get<0>(it->second);
        auto function = std::get<1>(it->second);
        auto result = std::get<2>(it->second);
        
        auto state = _init_state->clone();
        delete (identifiers->evaluate(this, state));
        auto ret = std::make_pair(function->evaluate_function(state), result == nullptr ? nullptr : result->evaluate(state));
        delete state;
        return ret;
    }
    
    std::pair<Tuple *, Process *> Program::process(const std::string &name) const
    {
        return _registered_process.at(name);
    }
    
    sym::Name * Program::unique_location()
    {
        std::stringstream ss;
        ss << "l" << _location++;
        return sym::make_name(ss.str(), _context->namer());
    }
    
    sym::Number * Program::new_timestamp() const
    {
        return sym::make_number("t", _context->namer());
    }
    
    sym::Constant * Program::network_delay() const
    {
        return sym::make_constant("pn", _context->namer());
    }
    
    sym::Nonce * Program::fresh_nonce() const
    {
        return sym::make_nonce("n", _context->namer());
    }
    
    sym::Variable * Program::new_variable() const
    {
        return sym::make_variable("v", _context->namer());
    }
    
    sym::Event * Program::know_event(sym::Term *message, sym::Term *timestamp) const
    {
        ASSERT(timestamp->type() == sym::TermType::TERM_NUMBER);
        return sym::make_event("know", 1, sym::make_tuple({message, timestamp}), _context->namer());
    }
    
    sym::Event * Program::new_event(sym::Term *nonce, sym::Term *location) const
    {
        ASSERT(nonce->type() == sym::TermType::TERM_NONCE);
        ASSERT(location->type() == sym::TermType::TERM_NAME);
        return sym::make_event("new", 1, sym::make_tuple({nonce, location}), _context->namer());
    }
    
    sym::Event * Program::unique_event(sym::Term *term, sym::Term *location, sym::Term *arguments) const
    {
        ASSERT(location->type() == sym::TermType::TERM_NAME);
        ASSERT(arguments->type() == sym::TermType::TERM_LIST);
        return sym::make_event("unique", 2, sym::make_tuple({term, location, arguments}), _context->namer());
    }
    
    sym::Event * Program::init_event(const std::string &name, sym::Term *term, sym::Term *arguments, sym::Term *timestamp) const
    {
        ASSERT(arguments->type() == sym::TermType::TERM_TUPLE);
        assert(timestamp->type() == sym::TermType::TERM_NUMBER);
        return sym::make_event(name, 1, sym::make_tuple({term, arguments, timestamp}), _context->namer());
    }
    
    sym::Event * Program::accept_event(const std::string &name, sym::Term *term, sym::Term *arguments, sym::Term *timestamp) const
    {
        ASSERT(arguments->type() == sym::TermType::TERM_TUPLE);
        assert(timestamp->type() == sym::TermType::TERM_NUMBER);
        return sym::make_event(name, 1, sym::make_tuple({term, arguments, timestamp}), _context->namer());
    }
    
    sym::Event * Program::join_event(const std::string &name, sym::Term *term, sym::Term *arguments, sym::Term *timestamp) const
    {
        ASSERT(arguments->type() == sym::TermType::TERM_TUPLE);
        assert(timestamp->type() == sym::TermType::TERM_NUMBER);
        return sym::make_event(name, 1, sym::make_tuple({term, arguments, timestamp}), _context->namer());
    }
    
    sym::Event * Program::open_event(sym::Term *message) const
    {
        return sym::make_event("open", 1, sym::make_tuple({message}), _context->namer());
    }
    
    sym::Event * Program::leak_event(sym::Term *message) const
    {
        return sym::make_event("leak", 1, sym::make_tuple({message}), _context->namer());
    }

    void Program::output(std::ostream &os) const
    {
        for (auto rule = _rules.begin(); rule != _rules.end(); ++rule) {
            (*rule)->rename(_context->namer());
        }
        
        std::vector<std::tuple<sym::Function *, sym::Term *, sym::Term *>> _functions;
        for (auto i = _registered_functions.begin(); i != _registered_functions.end(); ++i) {
            if (std::get<4>(i->second)) {
                auto application = function(i->first);
                auto delay = std::get<3>(i->second);
                _functions.push_back(std::make_tuple(application.first, application.second, delay == nullptr ? nullptr : delay->evaluate(_init_state)));
            }
        }
        
        os << "@set @index " << _context->namer().current() << ";" << std::endl;
        os << std::endl;
        os << "@declare @event new(1, 2);" << std::endl;
        os << "@provide @event new @removable @false;" << std::endl;
        os << std::endl;
        os << "@declare @event unique(2, 3);" << std::endl;
        os << "@provide @event unique @removable @false;" << std::endl;
        os << std::endl;
        for (auto i = _registered_events.begin(); i != _registered_events.end(); ++i) {
            if (i->second.first == EventType::EVENT_INIT || i->second.first == EventType::EVENT_ACCEPT) {
                os << "@declare @event " << i->first << "(1, 3);" << std::endl;
                os << "@provide @event " << i->first << " @removable @false;" << std::endl;
            } else {
                ASSERT(i->second.first == EventType::EVENT_JOIN);
                os << "@declare @event " << i->first << "(1, 3);" << std::endl;
                os << "@provide @event " << i->first << " @removable @false;" << std::endl;
            }
            
            if (i->second.second != -1) {
                os << "@set @event " << i->first << " @upperbound " << i->second.second << ";" << std::endl;
            }
            os << std::endl;
        }
        os << "@declare @event open(1, 1);" << std::endl;
        os << "@provide @event open @removable @false;" << std::endl;
        os << std::endl;
        os << "@declare @event know(1, 2);" << std::endl;
        os << "@provide @event know( m, #t) @removable @true;" << std::endl;
        os << "@provide @event know(#c, #t) @removable @true;" << std::endl;
        os << "@provide @event know(#t, #t) @removable @true;" << std::endl;
        os << std::endl;
        os << "@declare @event leak(1, 1);" << std::endl;
        os << "@provide @event leak @removable @false;" << std::endl;
        os << std::endl;
        for (auto i = _registered_tuples.begin(); i != _registered_tuples.end(); ++i) {
            // tuple -> x_i
            size_t size = *i;
            if (size == 0) {
                continue;
            }

            std::stringstream ss_tuple;
            std::vector<std::string> arguments;
            std::vector<std::string> differences;
            ss_tuple << "know((";
            for (auto i = 0; i != size; ++i) {
                if (i != 0) {
                    ss_tuple << ", ";
                }
                ss_tuple << "x" << i;
                
                std::stringstream ss_argument;
                ss_argument << "know(x" << i << ", #t" << i << ");";
                arguments.push_back(ss_argument.str());
                
                std::stringstream ss_difference;
                ss_difference << "#t" << i << " - #t ";
                differences.push_back(ss_difference.str());
            }
            ss_tuple << "), #t);";
            auto tuple = ss_tuple.str();
            
            os << "@rule {" << std::endl;
            for (auto i = 0; i != size; ++i) {
                os << com::tabs(4, 1) << "@premise " << arguments.at(i) << std::endl;
                os << com::tabs(4, 1) << "@constraint " << differences.at(i) << "<= 0;" << std::endl;
            }
            os << com::tabs(4, 1) << "@conclusion " << tuple << std::endl;
            os << "};" << std::endl;
            os << std::endl;
            
            for (auto i = 0; i != size; ++i) {
                os << "@rule {" << std::endl;
                os << com::tabs(4, 1) << "@premise " << tuple << std::endl;
                os << com::tabs(4, 1) << "@conclusion " << arguments.at(i) << std::endl;
                os << com::tabs(4, 1) << "@constraint " << differences.at(i) << ">= 0;" << std::endl;
                os << "};" << std::endl;
                os << std::endl;
            }
        }
        os << "@set @barrier;" << std::endl;
        os << std::endl;
        for (auto i = 0; i != _location; ++i) {
            os << "@declare @string l" << i << "[];" << std::endl;
        }
        os << std::endl;
        
        for (auto i = 0; i != _registered_strings.size(); ++i) {
            os << "@declare @string " << _registered_strings.at(i) << "[];" << std::endl;
        }
        os << std::endl;
        for (auto i = 0; i != _registered_parameters.size(); ++i) {
            os << "@declare @parameter $" << _registered_parameters.at(i) << ";" << std::endl;
        }
        os << "@set @constraint $pn > 0;" << std::endl;
        ppl::Zone zone(true);
        for (auto i = 0; i != _constraints.size(); ++i) {
            zone.update(_constraints.at(i));
        }
        auto constraints = zone.info();
        for (auto constraint = constraints.begin(); constraint != constraints.end(); ++constraint) {
            os << "@set @constraint " << *constraint << ";" << std::endl;
        }
        os << std::endl;
        
        for (auto function = _registered_functions.begin(); function != _registered_functions.end(); ++function) {
            auto left = std::get<1>(function->second);
            auto right = std::get<2>(function->second);

            if (right == nullptr) {
                os << "@declare @function " << function->first << "/" << left->arguments()->size() << ";" << std::endl;
            }
        }
        os << std::endl;
        
        for (auto function = _functions.begin(); function != _functions.end(); ++function) {
            auto left = std::get<0>(*function);
            auto right = std::get<1>(*function);
            auto delay = std::get<2>(*function);
            
            os << "@rule {" << std::endl;
            os << com::tabs(4, 1) << "@premise know(";
            left->arguments()->info(os);
            os << ", #t);" << std::endl;
            os << com::tabs(4, 1) << "@conclusion know(";
            if (right == nullptr) {
                left->info(os);
            } else {
                right->info(os);
            }
            os << ", #t1);" << std::endl;
            if (delay == nullptr) {
                os << com::tabs(4, 1) << "@constraint #t1 - #t >= 0;" << std::endl;
            } else {
                os << com::tabs(4, 1) << "@constraint #t1 - #t >= ";
                delay->info(os);
                os << ";" << std::endl;
            }
            os << "};" << std::endl;
            os << std::endl;
            
            delete left;
            if (right) {
                delete right;
            }
            if (delay) {
                delete delay;
            }
        }
        
        for (auto rule = _rules.begin(); rule != _rules.end(); ++rule) {
            (*rule)->info(os);
            os << std::endl;
        }
        
        for (auto query = _queries.begin(); query != _queries.end(); ++query) {
            (*query)->info(os);
            os << std::endl;
        }
        
        os << "@correspondence {" << std::endl;
        os << "    @conclusion leak(x);" << std::endl;
        os << "    @premise open(x);" << std::endl;
        os << "};" << std::endl;
    }

    void Program::info(std::ostream &os) const
    {
        for (auto function = _registered_functions.begin(); function != _registered_functions.end(); ++function) {
            if (std::get<4>(function->second) == false) {
                os << process_private << process_space;
            }
            os << process_function << process_space;
            std::get<1>(function->second)->info(os);
            if (std::get<2>(function->second) != nullptr) {
                os << process_space << process_equal << process_space;
                std::get<2>(function->second)->info(os);
            }
            if (std::get<3>(function->second) != nullptr) {
                os << process_space << process_require;
                std::get<3>(function->second)->info(os);
            }
            os << std::endl;
        }
        os << std::endl;
        
        _init_state->info(os);
        os << std::endl;
        
        for (auto process = _registered_process.begin(); process != _registered_process.end(); ++process) {
            os << process_process << process_space << process->first;
            process->second.first->info(os);
            os << process_space << process_equal << process_space << process_left_bracket << std::endl;
            process->second.second->info(1, os);
            os << process_right_bracket << std::endl << std::endl;
        }
        os << process_system << process_space << process_equal << process_space << process_left_bracket << std::endl;
        _init_process->info(1, os);
        os << process_right_bracket << std::endl;
        os << std::endl;
        
        for (auto query = _registered_queries.begin(); query != _registered_queries.end(); ++query) {
            (*query)->info(os);
            os << std::endl;
        }
    }
}