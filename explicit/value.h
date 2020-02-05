//
//  value.h
//  explicit
//
//  Created by Li Li on 14/7/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __explicit__value__
#define __explicit__value__

#include <iostream>
#include <string>

namespace emc {
    enum ValueType { // ANY = {BOOLEAN, INTEGER, REAL} != NONE
        BOOLEAN,
        INTEGER,
        REAL,
        NONE,
        ANY,
        _VALUE_TYPE_END,
    };
    const std::string ValueName[ValueType::_VALUE_TYPE_END] = {
        "@bool",
        "@int",
        "@real",
        "@none",
        "@any",
    };
    const std::string value_true = "@true";
    const std::string value_false = "@false";

    class Value
    {
    protected:
        union RawValue
        {
            bool b_value;
            long i_value;
            double r_value;
        } _value;
        ValueType _type;
        
    public:
        Value(const std::string &value);
        Value(long value) :_type(ValueType::INTEGER) { _value.i_value = value; }
        Value(double value) : _type(ValueType::REAL) { _value.r_value = value; }
        Value(bool value) : _type(ValueType::BOOLEAN) { _value.b_value = value; }
        Value(ValueType type = ValueType::NONE) : _type(type) {}
        Value(const Value &value): _value(value._value), _type(value._type) {}
        virtual ~Value() {}
        
    public:
        ValueType type() const { return _type; }
        Value convert(ValueType type) const;
        void assign(long value) { _type = ValueType::INTEGER; _value.i_value = value; }
        void assign(double value) { _type = ValueType::REAL; _value.r_value = value; }
        void assign(bool value) { _type = ValueType::BOOLEAN; _value.b_value = value; }
        
    public:
        bool is_int() const { return _type == ValueType::INTEGER; }
        bool is_real() const { return _type == ValueType::REAL; }
        bool is_bool() const { return _type == ValueType::BOOLEAN; }
        bool is_none() const { return _type == ValueType::NONE; }
        bool is_any() const { return _type == ValueType::ANY; }
        
    public:
        bool is_numeric() const { return is_int() || is_real(); }
        bool is_positive() const { return is_int() ? get_int() > 0 : (is_real() ? get_real() > 0 : false); }
        bool is_negative() const { return is_int() ? get_int() < 0 : (is_real() ? get_real() < 0 : false); }
        bool is_zero() const { return is_int() ? get_int() == 0 : (is_real() ? get_real() == 0 : false); }
        
    public:
        long get_int() const;
        double get_real() const;
        bool get_bool() const;
        
    public:
        bool operator == (const Value &value) const;
        inline bool operator != (const Value &value) const { return !(*this == value); }
        bool operator < (const Value &value) const;
        inline bool operator > (const Value &value) const { return (value < (*this)); }
        inline bool operator <= (const Value &value) const { return !(*this > value); }
        inline bool operator >= (const Value &value) const { return !(*this < value); }
        
    public:
        Value & operator = (Value value);
        
    public:
        Value & operator ++ ();
        inline Value operator ++ (int);
        Value & operator -- ();
        inline Value operator -- (int);
        Value operator ! ();
        
        Value & operator += (const Value &rhs);
        friend inline Value operator + (Value lhs, const Value &rhs) { lhs += rhs; return lhs; }
        Value & operator -= (const Value &rhs);
        friend inline Value operator - (Value lhs, const Value &rhs) { lhs -= rhs; return lhs; }
        Value & operator *= (const Value &rhs);
        friend inline Value operator * (Value lhs, const Value &rhs) { lhs *= rhs; return lhs; }
        Value & operator /= (const Value &rhs);
        friend inline Value operator / (Value lhs, const Value &rhs) { lhs /= rhs; return lhs; }
        Value operator && (const Value &rhs);
        Value operator || (const Value &rhs);
        
        friend std::ostream & operator << (std::ostream &os, const Value &value);
    };
    
}

#endif /* defined(__explicit__value__) */
