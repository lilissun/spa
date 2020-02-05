//
//  name.h
//  explicit
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __explicit__name__
#define __explicit__name__

#include <iostream>
#include <string>

#include "some.h"

namespace emc {
    enum NameType {
        CONSTANT = 0,
        VARIABLE = 1,
        NAME_TYPE_END,
    };
    
    class Name {
    public:
        typedef size_t ID;
        
    protected:
        NameType _type;
        std::string _name;
        ID _id;
        
    public:
        Name(NameType type, const std::string &name, ID id) : _type(type), _name(name), _id(id) {}
        Name(const Name &name) : _type(name._type), _name(name._name), _id(name._id) {}
        ~Name() {}
        
    public:
        const NameType & type() const { return _type; }
        const std::string & name() const { return _name; }
        ID id() const { return _id; }
        std::string string() const { return _name + std::to_string(_id); }
        
    public:
        bool operator == (const Name &name) const { return _id == name._id; }
        bool operator != (const Name &name) const { return _id != name._id; }
        bool operator >  (const Name &name) const { return _id >  name._id; }
        bool operator >= (const Name &name) const { return _id >= name._id; }
        bool operator <  (const Name &name) const { return _id <  name._id; }
        bool operator <= (const Name &name) const { return _id <= name._id; }
        
    public:
        Name & operator = (const Name &name);
        
    public:
        friend std::ostream & operator << (std::ostream &os, const Name &name);
    };
    
    const Name INVALID_NAME(NameType::CONSTANT, "@invalid", -1);
}

#endif /* defined(__explicit__name__) */
