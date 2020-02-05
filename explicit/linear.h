//
//  linear.h
//  explicit
//
//  Created by Li Li on 5/5/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#ifndef __explicit__linear__
#define __explicit__linear__

#include <iostream>
#include <map>
#include <vector>

#include "value.h"
#include "name.h"

namespace emc {
    const std::string linear_plus = "+";
    const std::string linear_minus = "-";
    const std::string linear_multiply = "*";
    const std::string linear_equal = "==";
    const std::string linear_not_equal = "!=";
    const std::string linear_less = "<";
    const std::string linear_less_equal = "<=";
    const std::string linear_greater = ">";
    const std::string linear_greater_equal = ">=";
    const std::string linear_space = " ";
    
    class LinearExpression {
    protected:
        std::map<Name, Value> _expr;
        Value _value;
        
    public:
        LinearExpression() : _expr(), _value(0L) {}
        LinearExpression(const Value &value) : _expr(), _value(value) {}
        LinearExpression(const Name &name, const Value &value = Value(1L)) : _expr({std::make_pair(name, value)}), _value(0L) {}
        LinearExpression(const LinearExpression &expr) : _expr(expr._expr), _value(expr._value) {}
        ~LinearExpression() {}
        
    public:
        const Value & value(const Name &name) const { return _expr.at(name); }
        bool exist(const Name &name) const { return _expr.find(name) != _expr.end(); }
        std::map<Name, Value>::const_iterator begin() const { return _expr.begin(); }
        std::map<Name, Value>::const_iterator end() const { return _expr.end(); }
        size_t size() const { return _expr.size(); }
        const Value & value() const { return _value; }
        
    public:
        void update_value(const Name &name, const Value &value);
        void update_value(const Value &value) { _value = value; }
        
    public:
        LinearExpression & operator = (const LinearExpression &expr) {
            _expr = expr._expr;
            _value = expr._value;
            return *this;
        }
        
        LinearExpression & operator *= (const Value &value) {
            if (value.is_zero()) {
                _expr.clear();
                _value.assign(0L);
            } else {
                for (auto i = _expr.begin(); i != _expr.end(); ++i) {
                    i->second *= value;
                }
                _value *= value;
            }
            return *this;
        }

        LinearExpression & operator += (const Name &name) {
            auto res = _expr.insert(std::make_pair(name, Value(1L)));
            if (res.second == false) {
                res.first->second += Value(1L);
                if (res.first->second.is_zero()) {
                    _expr.erase(res.first);
                }
            }
            return *this;
        }
        
        LinearExpression & operator += (const Value &value) {
            _value += value;
            return *this;
        }
        
        LinearExpression & operator += (const LinearExpression &expr) {
            for (auto i = expr._expr.begin(); i != expr._expr.end(); ++i) {
                auto res = _expr.insert(std::make_pair(i->first, i->second));
                if (res.second == false) {
                    res.first->second += i->second;
                    if (res.first->second.is_zero()) {
                        _expr.erase(res.first);
                    }
                }
            }
            _value += expr._value;
            return *this;
        }
        
    public:
        
    };
    
    LinearExpression operator * (const Value &value, const Name &name);
    LinearExpression operator * (const Name &name, const Value &value);
    LinearExpression operator * (LinearExpression expr, const Value &value);
    LinearExpression operator * (const Value &value, const LinearExpression &expr);
    
    LinearExpression operator + (LinearExpression expr, const Value &value);
    LinearExpression operator + (LinearExpression expr, const Name &name);
    LinearExpression operator + (const Name &name, const LinearExpression &expr);
    LinearExpression operator + (const Value &value, const LinearExpression &expr);
    LinearExpression operator + (const Value &value, const Name &name);
    LinearExpression operator + (const Name &name, const Value &value);
    LinearExpression operator + (const Name &first, const Name &second);
    LinearExpression operator + (LinearExpression first, const LinearExpression &second);

    LinearExpression operator - (LinearExpression expr, const Value &value);
    LinearExpression operator - (LinearExpression expr, const Name &name);
    LinearExpression operator - (const Name &name, const LinearExpression &expr);
    LinearExpression operator - (const Value &value, const LinearExpression &expr);
    LinearExpression operator - (const Value &value, const Name &name);
    LinearExpression operator - (const Name &name, const Value &value);
    LinearExpression operator - (const Name &first, const Name &second);
    LinearExpression operator - (LinearExpression first, const LinearExpression &second);
    
    enum LinearConstraintType {
        LINEAR_CONSTRAINT_LESS,
        LINEAR_CONSTRAINT_LESS_EQUAL,
        LINEAR_CONSTRAINT_EQUAL,
        LINEAR_CONSTRAINT_END, 
    };
    
    class LinearConstraint {
    protected:
        LinearExpression _expr;
        LinearConstraintType _type;
        
    public:
        LinearConstraint(LinearExpression expr, LinearConstraintType type) : _expr(expr), _type(type) {}
        ~LinearConstraint() {}
        
    public:
        const LinearExpression & expr() const { return _expr; }
        LinearConstraintType type() const { return _type; }
        
    public:
        friend std::ostream & operator << (std::ostream &os, const LinearConstraint &constraint);
    };
    
    LinearConstraint operator  < (const LinearExpression &left, const LinearExpression &right);
    LinearConstraint operator <= (const LinearExpression &left, const LinearExpression &right);
    LinearConstraint operator == (const LinearExpression &left, const LinearExpression &right);
}


#endif /* __explicit__linear__ */
