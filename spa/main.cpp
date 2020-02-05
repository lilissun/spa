//
//  main.cpp
//  spa
//
//  Created by Li Li on 16/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

#include "common/exception.h"
#include "common/runtime.h"
#include "common/debug.h"
#include "common/timer.h"

#include "time/context.h"
#include "time/rule.h"
#include "time/helper.h"
#include "time/solver.h"

#include "calculus/helper.h"
#include "calculus/context.h"

int main(int argc, const char * argv[])
{
    try {
        std::string filename;
        if (argc == 2) {
            filename = argv[1];
        } else {
            std::cout << "Usage: cert filename" << std::endl;
            return -1;
        }
        
        if (filename.substr(filename.find_last_of(".")) == ".cal") {
            cal::Helper helper;
            helper.init();
            helper.scan(filename);
            auto context = helper.get();
            context->program()->execute();
            
            auto new_filename = filename.substr(0, filename.find_last_of(".")) + ".tim";
            std::ofstream ofstream;
            ofstream.open(new_filename);
            context->program()->output(ofstream);
            ofstream.close();
            filename = new_filename;
            delete context;
        }
        
        if (filename.substr(filename.find_last_of(".")) == ".tim") {
            tim::Helper helper;
            helper.init();
            helper.scan(filename);
            auto context = helper.get();
            
            
            com::Timer timer;
            timer.start();
            auto result = context->solver().solve();
            timer.pause();
            
            std::cout << std::endl << std::endl;
            
            std::cout << "Overall generated rule count : " << context->solver().rule_count() << std::endl;
            std::cout << "Basis size : " << context->solver().size() << std::endl;
            std::cout << "Verification time: " << timer << std::endl << std::endl;
            
            if (result.first == tim::SolverResult::ATTACK_FOUND) {
                std::cout << "An attack exists: " << result.second << std::endl << std::endl;
            } else if (result.first == tim::SolverResult::SECURE) {
                std::cout << "The protocol can be configured as secure." << std::endl << std::endl;
            } else {
                ASSERT(result.first == tim::SolverResult::NO_QUERY);
                std::cout << "No query." << std::endl << std::endl;
            }
            
            std::cout << "Details:" << std::endl;
            context->solver().output_zones();
            std::cout << std::endl;
            context->solver().output_finals(std::cout);
            std::cout << std::endl;
//            context->solver().output_rules(std::cout);
//            std::cout << std::endl;
            
            delete context;
        } else {
            std::cout << "filename extension should match .tim or .cal" << std::endl;
            return -1;
        }
        
        return 0;
    } catch (const com::UserError &e) {
        std::cout << e.string() << std::endl;
        return -1;
#ifndef DEBUG
    } catch (const com::Exception &e) {
        std::cout << e.string() << std::endl;
        return -1;
#endif
    }
}
