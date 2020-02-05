//
//  query.cpp
//  calculus
//
//  Created by Li Li on 24/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "query.h"

#include "common/iostream.h"
#include "common/runtime.h"
#include "symbolic/term.h"
#include "symbolic/function.h"
#include "time/query.h"
#include "state.h"
#include "program.h"
#include "identifier.h"
#include "expression.h"

namespace cal {
    Correspondence::~Correspondence()
    {
        delete _init;
        for (auto i = _joins.begin(); i != _joins.end(); ++i) {
            delete *i;
        }
        delete _accept;
    }
    
    Correspondence * Correspondence::add_event(Event *event)
    {
        if (event->type() == EventType::EVENT_INIT) {
            ASSERT(_init == nullptr);
            _init = event;
        } else if (event->type() == EventType::EVENT_JOIN) {
            _joins.push_back(event);
        } else {
            ASSERT(event->type() == EventType::EVENT_ACCEPT);
            _accept = event;
        }
        return this;
    }
    
    tim::Correspondence * Correspondence::evaluate(const Program *program, const State *state) const
    {
        auto new_state = state->clone();
        delete (_identifier->evaluate(program, new_state));
        ASSERT(_accept != nullptr);
        sym::Event *init = _init == nullptr ? nullptr : _init->evaluate(program, new_state);
        std::vector<sym::Event *> joins;
        for (auto i = _joins.begin(); i != _joins.end(); ++i) {
            joins.push_back((*i)->evaluate(program, new_state));
        }
        sym::Event *accept = _accept->evaluate(program, new_state);
        auto query = new tim::Correspondence(init, joins, accept, _enforced);
        for (auto i = _constraints.begin(); i != _constraints.end(); ++i) {
            auto left = std::get<0>(*i)->evaluate(program, new_state);
            auto ope = std::get<1>(*i);
            auto right = std::get<2>(*i)->evaluate(program, new_state);
            auto constraint = sym::make_constraint(left, ope, right);
            query->add_constraint(constraint);
        }
        if (_enforced) {
            query->enforced();
        }
        return query;
    }
    
    void Correspondence::info(std::ostream &os) const
    {
        os << "@correspondence ";
        _identifier->info(os);
        os << " {" << std::endl;
        if (_init != nullptr) {
            os << com::tabs(4, 1) << "@event ";
            _init->info(os);
            os << ";" << std::endl;
        }
        for (auto i = _joins.begin(); i != _joins.end(); ++i) {
            os << com::tabs(4, 1) << "@event ";
            (*i)->info(os);
            os << ";" << std::endl;
        }
        ASSERT(_accept != nullptr);
        os << com::tabs(4, 1) << "@event ";
        _accept->info(os);
        os << ";" << std::endl;
        for (auto i = _constraints.begin(); i != _constraints.end(); ++i) {
            os << com::tabs(4, 1) << "@constraint ";
            std::get<0>(*i)->info(os);
            os << expression_space << sym::ConstraintTypeName[std::get<1>(*i)] << expression_space;
            std::get<2>(*i)->info(os);
            os << ";" << std::endl;
        }
        if (_enforced) {
            os << com::tabs(4, 1) << "@enforced;" << std::endl;
        }
        os << "}" << std::endl;
    }
}