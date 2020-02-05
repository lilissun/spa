//
//  value.cpp
//  explicit
//
//  Created by Li Li on 14/7/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "value.h"

#include <sstream>
#include <cmath>

#include "common/runtime.h"

namespace emc {
    const double _double_fineness = 0.000001;

    Value::Value(const std::string &value)
    {
        if (value == value_true) {
            assign(true);
            return ;
        } else if (value == value_false) {
            assign(false);
            return ;
        }

        if (value[0] < '0' || value[0] > '9')
        {
            assign(false);
            SYSTEM_ERROR("unknown value");
            return ;
        }

        long i_value;
        std::stringstream iss(value);
        iss >> i_value;
        if (iss.get() == EOF) {
            assign(i_value);
            return ;
        }

        double r_value;
        std::stringstream rss(value);
        rss >> r_value;
        if (rss.get() == EOF) {
            assign(r_value);
            return ;
        }
        
        assign(false);
        SYSTEM_ERROR("unknown value");
        return ;
    }
    
    Value Value::convert(ValueType type) const
    {
        if (type == _type) {
            return *this;
        } else if (type == ValueType::REAL && _type == ValueType::INTEGER) {
            return Value((double)_value.i_value);
        } else {
            throw com::UserError(__func__, "Convertion Not Allowed");
        }
    }
    
    long Value::get_int() const
    {
        ASSERT(is_int());
        return _value.i_value;
    }
    
    double Value::get_real() const
    {
        ASSERT(is_real());
        return _value.r_value;
    }
    
    bool Value::get_bool() const
    {
        ASSERT(is_bool());
        return _value.b_value;
    }
        
    bool Value::operator == (const Value &value) const
    {
        if (_type == ValueType::ANY || value._type == ValueType::ANY) {
            return true;
        }
        
        if (_type == ValueType::NONE || value._type == ValueType::NONE) {
            if (_type == value._type) {
                return true;
            } else {
                return false;
            }
        }
        
        ASSERT(_type == value._type);
        switch(_type)
        {
            case ValueType::BOOLEAN:
                return _value.b_value == value._value.b_value;
            case ValueType::INTEGER:
                return _value.i_value == value._value.i_value;
            case ValueType::REAL:
                return std::abs(_value.r_value - value._value.r_value) < _double_fineness;
            default:
                SYSTEM_ERROR("unsupported value type");
        }
    }
    
    bool Value::operator < (const Value &value) const
    {
        ASSERT(_type == value._type);
        switch(_type)
        {
            case ValueType::INTEGER:
                return _value.i_value < value._value.i_value;
            case ValueType::REAL:
                return _value.r_value < value._value.r_value;
            default:
                SYSTEM_ERROR("unsupported value type");
        }
    }
    
    Value & Value::operator = (Value rhs)
    {
        std::swap(*this, rhs);
        return *this;
    }
    
    Value & Value::operator ++ ()
    {
        _value.i_value = get_int() + 1;
        return *this;
    }
    
    Value Value::operator ++ (int)
    {
        Value tmp(*this);
        operator++();
        return tmp;
    }
    
    Value & Value::operator -- ()
    {
        _value.i_value = get_int() - 1;
        return *this;
    }
    
    Value Value::operator -- (int)
    {
        Value tmp(*this);
        operator--();
        return tmp;
    }
    
    Value Value::operator ! ()
    {
        Value tmp(!get_bool());
        return tmp;
    }
    
    Value & Value::operator += (const Value &rhs)
    {
        ASSERT(_type == rhs._type);
        switch(_type)
        {
            case ValueType::INTEGER:
                _value.i_value += rhs.get_int();
            case ValueType::REAL:
                _value.r_value += rhs.get_real();
            default:
                SYSTEM_ERROR("unsupported value type");
        }
        return *this;
    }
    
    Value & Value::operator -= (const Value &rhs)
    {
        ASSERT(_type == rhs._type);
        switch(_type)
        {
            case ValueType::INTEGER:
                _value.i_value -= rhs.get_int();
            case ValueType::REAL:
                _value.r_value -= rhs.get_real();
            default:
                SYSTEM_ERROR("unsupported value type");
        }
        return *this;
    }
    
    Value & Value::operator *= (const Value &rhs)
    {
        ASSERT(_type == rhs._type);
        switch(_type)
        {
            case ValueType::INTEGER:
                _value.i_value *= rhs.get_int();
            case ValueType::REAL:
                _value.r_value *= rhs.get_real();
            default:
                SYSTEM_ERROR("unsupported value type");
        }
        return *this;
    }
    
    Value & Value::operator /= (const Value &rhs)
    {
        ASSERT(_type == rhs._type);
        switch(_type)
        {
            case ValueType::INTEGER:
                _value.i_value /= rhs.get_int();
            case ValueType::REAL:
                _value.r_value /= rhs.get_real();
            default:
                SYSTEM_ERROR("unsupported value type");
        }
        return *this;
    }
    
    Value Value::operator && (const Value &rhs)
    {
        return Value(get_bool() && rhs.get_bool());
    }
    
    Value Value::operator || (const Value &rhs)
    {
        return Value(get_bool() || rhs.get_bool());
    }
    
    std::ostream & operator << (std::ostream &os, const Value &value)
    {
        switch(value._type)
        {
            case ValueType::BOOLEAN:
                if (value._value.b_value) {
                    os << value_true;
                } else {
                    os << value_false;
                }
                break;
            case ValueType::INTEGER:
                os << value._value.i_value;
                break;
            case ValueType::REAL:
                os << value._value.r_value;
                break;
            case ValueType::NONE:
            case ValueType::ANY:
                os << ValueName[value._type];
                break;
            default:
                SYSTEM_ERROR("unsupported value type");
        }
        return os;
    }
}
