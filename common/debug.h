//
//  debug.h
//  common
//
//  Created by Li Li on 9/5/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __common__debug__
#define __common__debug__

#include "exception.h"

#define NOT_IMPL throw com::NotImplemented(__func__)
#define NEED_FIX throw com::NeedFix(__func__)

namespace com {
    // DebugError is we know it is there but we haven't fix yet
    class DebugError : public Error
    {
    public:
        DebugError(std::string function) : Error(function) {}
        
    public:
        virtual std::string string() const { return "Debug Error: " + _contact; }
    };

    class NotImplemented : public DebugError
    {
    public:
        NotImplemented(std::string function) :DebugError(function) {}
        virtual std::string internal_string() const { return "Debug Error: function " + _function + " is not implemented"; }
    };
    
    class NeedFix : public DebugError
    {
    public:
        NeedFix(std::string function) :DebugError(function) {}
        virtual std::string internal_string() const { return "Debug Error: function " + _function + " needs fix"; }
    };
}

#endif /* defined(__common__debug__) */
