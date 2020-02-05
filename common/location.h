//
//  location.h
//  common
//
//  Created by Li Li on 1/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __common__location__
#define __common__location__

#include <string>
#include <iostream>

namespace com {
    class Location {
    protected:
        std::string _filename;
        size_t _first_line;
        size_t _first_column;
        size_t _last_line;
        size_t _last_column;
        
    public:
        Location() : _filename(""), _first_line(0), _first_column(0), _last_line(0), _last_column(0) {}
        Location(std::string filename, size_t first_line, size_t first_column, size_t last_line, size_t last_column)
        : _filename(filename), _first_line(first_line), _first_column(first_column)
        , _last_line(last_line), _last_column(last_column) {}
        Location(const Location &location) : _filename(location._filename)
        , _first_line(location._first_line), _first_column(location._first_column)
        , _last_line(location._last_line), _last_column(location._last_column) {}
        ~Location() {}
        
    public:
        void update(const std::string &filename, size_t first_line, size_t first_column
                    , size_t last_line, size_t last_column);
        const std::string & filename() const { return _filename; }
        size_t first_line() const { return _first_line; }
        size_t first_column() const { return _first_column; }
        size_t last_line() const { return _last_line; }
        size_t last_column() const { return _last_column; }
        
    public:
        Location & operator = (const Location &location) {
            _filename = location._filename;
            _first_line = location._first_line;
            _first_column = location._first_column;
            _last_line = location._last_line;
            _last_column = location._last_column;
            return *this;
        }
        
        Location operator - (const Location &location) {
            return Location(_filename, _first_line, _first_column, location._last_line, location._last_column);
        }
        
    public:
        friend std::ostream & operator << (std::ostream &os, const Location &location);
    };
}

#define YYLTYPE com::Location
#define YYLTYPE_IS_DECLARED

#define YY_USER_ACTION \
    yylloc.update(_filename, _lineno, _column, _lineno, _column + yyleng); \
    _column += yyleng;

#define YYLLOC_DEFAULT(Current, Rhs, N) \
do {\
    if (YYID (N)) { \
        (Current).update(YYRHSLOC(Rhs, 1).filename(), YYRHSLOC(Rhs, 1).first_line(), YYRHSLOC(Rhs, 1).first_column() \
            , YYRHSLOC(Rhs, N).last_line(), YYRHSLOC(Rhs, N).last_column());	\
    } else {  \
        (Current).update(YYRHSLOC(Rhs, 0).filename(), YYRHSLOC(Rhs, 0).first_line(), YYRHSLOC(Rhs, 0).first_column() \
            , YYRHSLOC(Rhs, 0).last_line(), YYRHSLOC(Rhs, 0).last_column()); \
    } \
} while (YYID (0))

#endif /* defined(__common__location__) */
