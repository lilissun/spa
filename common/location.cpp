//
//  location.cpp
//  common
//
//  Created by Li Li on 1/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "location.h"

namespace com {
    void Location::update(const std::string &filename, size_t first_line, size_t first_column
                          , size_t last_line, size_t last_column) {
        _filename = filename;
        _first_line = first_line;
        _first_column = first_column;
        _last_line = last_line;
        _last_column = last_column;
    }
    
    std::ostream & operator << (std::ostream &os, const Location &location) {
        os << "[" << location._filename << "@" << location._first_line << "-" << location._first_column << "]";
        return os;
    }
}