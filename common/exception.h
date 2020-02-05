//
//  Exception.h
//  Common
//
//  Created by Li Li on 8/12/12.
//  Copyright (c) 2012 Lilissun. All rights reserved.
//

#ifndef __Common__Exception__
#define __Common__Exception__

#include <string>
#include <exception>

namespace com {
    // All functional Exception should be derived from Exception class
    const std::string _contact = "Please report the bug to Li Li<lilissun@gmail.com>. Thank you very much!";
    
    class _Exception : public std::exception
    {
    public:
        _Exception() {}
        virtual ~_Exception() {}
        
    public:
        virtual std::string string() const = 0;
        virtual std::string internal_string() const = 0;
    };
    
    class Exception : public _Exception
    {
    public:
        Exception() {}
        
    public:
        virtual std::string string() const { return "Unexpected Exception: " + _contact; }
        virtual std::string internal_string() const { return "Unexpected exception"; }
    };
    
    // Error is build in interface for error
    class Error : public _Exception
    {
    protected:
        std::string _function;
        
    public:
        Error(std::string function) : _function(function) {}
        std::string function() const { return _function; }
    };
    
    // Warning is build in interface for warning
    class Warning : public _Exception
    {
    protected:
        std::string _function;
        std::string _detail;
        
    public:
        Warning(std::string function, std::string detail) : _function(function), _detail(detail) {}
        std::string function() const { return _function; }
        std::string detail() const { return _detail; }
        
    public:
        virtual std::string string() const { return "Warning: " + _detail;  }
        virtual std::string internal_string() const { return "Warning: " + _detail + " at function " + _function; }
    };
}

#endif /* defined(__Common__Exception__) */
