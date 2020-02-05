//
//  runtime.h
//  common
//
//  Created by Li Li on 9/5/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __common__runtime__
#define __common__runtime__

#include "exception.h"

#include <string>

#define USER_ERROR(msg) throw com::UserError(__func__, (msg))
#define SYSTEM_ERROR(msg) throw com::SystemError(__func__, (msg))

#define ASSERT(cond) if (!(cond)) SYSTEM_ERROR("assertion is invalid")
#define ASSERT1(cond, msg) if (!(cond)) SYSTEM_ERROR(msg)

namespace com {
    // RuntimeError is error that is not allowed.
    // We check the RuntimeError in the program and report them to the end-users
    class RuntimeError : public Error
    {
    protected:
        std::string _detail;
        
    public:
        RuntimeError(std::string function, std::string detail) : Error(function), _detail(detail) {}
        std::string detail() { return _detail; }
    };
    
    // User Error comes from the input
    class UserError : public RuntimeError
    {
    public:
        UserError(std::string function, std::string detail) : RuntimeError(function, detail) {}
        
    public:
        virtual std::string string() const { return "Error: " + _detail;  }
        virtual std::string internal_string() const { return "Error: " + _detail + " at function " + _function; }
        
    };

    // System Error currently is not used but replaced with c-style assertions
    class SystemError : public RuntimeError
    {
    public:
        SystemError(std::string function, std::string detail) : RuntimeError(function, detail) {}
        
    public:
        virtual std::string string() const { return "System error: " + _contact;  }
        virtual std::string internal_string() const { return "System Error: " + _detail + " at function " + _function; }
    };
}

#endif /* defined(__common__runtime__) */
