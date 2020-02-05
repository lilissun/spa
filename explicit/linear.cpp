//
//  linear.cpp
//  explicit
//
//  Created by Li Li on 5/5/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#include "linear.h"

namespace emc {
    void LinearExpression::update_value(const Name &name, const Value &value)
    {
        if (value.is_zero()) {
            _expr.erase(name);
        } else {
            auto res = _expr.insert(std::make_pair(name, value));
            if (res.second == false) {
                res.first->second = value;
            }
        }
    }
    
    LinearExpression operator + (LinearExpression expr, const Value &value)
    {
        expr += value;
        return expr;
    }
    
    LinearExpression operator + (LinearExpression expr, const Name &name)
    {
        expr += name;
        return expr;
    }
    
    LinearExpression operator + (const Name &name, const LinearExpression &expr)
    {
        return expr + name;
    }
    
    LinearExpression operator + (const Value &value, const LinearExpression &expr)
    {
        return expr + value;
    }
    
    LinearExpression operator + (const Name &name, const Value &value)
    {
        LinearExpression expr(value);
        return expr + name;
    }
    
    LinearExpression operator + (const Value &value, const Name &name)
    {
        return name + value;
    }
    
    LinearExpression operator + (const Name &first, const Name &second)
    {
        LinearExpression expr;
        expr += first;
        expr += second;
        return expr;
    }
    
    LinearExpression operator + (LinearExpression first, const LinearExpression &second)
    {
        first += second;
        return first;
    }
    
    LinearExpression operator * (const Value &value, const Name &name)
    {
        LinearExpression expr;
        expr += name;
        expr *= value;
        return expr;
    }
    
    LinearExpression operator * (const Name &name, const Value &value)
    {
        return value * name;
    }
    
    LinearExpression operator * (LinearExpression expr, const Value &value)
    {
        expr *= value;
        return expr;
    }
    
    LinearExpression operator * (const Value &value, const LinearExpression &expr)
    {
        return expr * value;
    }
    
    LinearExpression operator - (LinearExpression expr, const Value &value)
    {
        return expr + value * Value(-1L);
    }
    
    LinearExpression operator - (LinearExpression expr, const Name &name)
    {
        return expr + name * Value(-1L);
    }
    
    LinearExpression operator - (const Name &name, const LinearExpression &expr)
    {
        return name + expr * Value(-1L);
    }
    
    LinearExpression operator - (const Value &value, const LinearExpression &expr)
    {
        return value + expr * Value(-1L);
    }
    
    LinearExpression operator - (const Value &value, const Name &name)
    {
        return value + name * Value(-1L);
    }
    
    LinearExpression operator - (const Name &name, const Value &value)
    {
        return name + value * Value(-1L);
    }
    
    LinearExpression operator - (const Name &first, const Name &second)
    {
        return first + second * Value(-1L);
    }
    
    LinearExpression operator - (LinearExpression first, const LinearExpression &second)
    {
        return first + second * Value(-1L);
    }
    
    std::ostream & operator << (std::ostream &os, const LinearConstraint &constraint)
    {
        const LinearExpression &expr = constraint.expr();
        bool empty_expr = true;
        for (auto i = expr.begin(); i != expr.end(); ++i) {
            if (i->second.is_positive()) {
                if (empty_expr == false) {
                    os << linear_space << linear_plus << linear_space;
                }
                os << i->second << linear_space << linear_multiply << linear_space << i->first;
                empty_expr = false;
            }
        }
        if (expr.value().is_positive()) {
            if (empty_expr == false) {
                os << linear_space << linear_plus << linear_space;
            }
            os << expr.value();
        }
        
        if (constraint.type() == LinearConstraintType::LINEAR_CONSTRAINT_LESS) {
            os << linear_space << linear_less << linear_space;
        } else if (constraint.type() == LinearConstraintType::LINEAR_CONSTRAINT_LESS_EQUAL) {
            os << linear_space << linear_less_equal << linear_space;
        } else {
            ASSERT(constraint.type() == LinearConstraintType::LINEAR_CONSTRAINT_EQUAL);
            os << linear_space << linear_equal << linear_space;
        }
        
        empty_expr = true;
        for (auto i = expr.begin(); i != expr.end(); ++i) {
            if (i->second.is_negative()) {
                if (empty_expr == false) {
                    os << linear_space << linear_plus << linear_space;
                }
                os << i->second * Value(-1L) << linear_space << linear_multiply << linear_space << i->first;
                empty_expr = false;
            }
        }
        if (expr.value().is_negative()) {
            if (empty_expr == false) {
                os << linear_space << linear_plus << linear_space;
            }
            os << expr.value() * Value(-1L);
        }
        
        return os;
    }
    
    LinearConstraint operator  < (const LinearExpression &left, const LinearExpression &right)
    {
        return LinearConstraint(left - right, LinearConstraintType::LINEAR_CONSTRAINT_LESS);
    }
    
    LinearConstraint operator <= (const LinearExpression &left, const LinearExpression &right)
    {
        return LinearConstraint(left - right, LinearConstraintType::LINEAR_CONSTRAINT_LESS_EQUAL);
    }
    
    LinearConstraint operator == (const LinearExpression &left, const LinearExpression &right)
    {
        return LinearConstraint(left - right, LinearConstraintType::LINEAR_CONSTRAINT_EQUAL);
    }
}