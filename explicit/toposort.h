//
//  toposort.h
//  explicit
//
//  Created by Li Li on 25/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __explicit__toposort__
#define __explicit__toposort__

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "common/exception.h"
#include "common/runtime.h"

namespace emc {
    template<typename _Key>
    class Toposort {
    public:
        class NoneSelection : public com::Exception {};
        class NotSortable : public com::Exception {};
        typedef _Key Key;
        
    protected:
        enum Mark {
            NON_MARK,
            TMP_MARK,
            PER_MARK,
            _END,
        };
        
    protected:
        std::map<Key, std::pair<Mark, std::set<Key>>> _mapping;
        
    public:
        Toposort() {}
        ~Toposort() {}
        
    protected:
        Key select() const {
            for (auto i = _mapping.begin(); i != _mapping.end(); ++i) {
                if (i->second.first == Mark::NON_MARK) {
                    return i->first;
                }
            }
            throw NoneSelection();
        }
        
        void visit(Key index, std::vector<Key> &sorted) {
            if (_mapping.find(index) == _mapping.end()) {
                sorted.push_back(index);
                return;
            }
            
            if (_mapping.at(index).first == Mark::TMP_MARK) {
                throw NotSortable();
            }
            
            std::pair<Mark, std::set<Key>> & node = _mapping.at(index);
            if (node.first == Mark::NON_MARK) {
                node.first = Mark::TMP_MARK;
                for (auto i = node.second.begin(); i != node.second.end(); ++i) {
                    visit(*i, sorted);
                }
                node.first = Mark::PER_MARK;
                sorted.push_back(index);
            }
        }
        
    public:
        void insert(const Key &source, const std::set<Key> &targets) {
            auto ret = _mapping.insert(std::make_pair(source, std::make_pair(Mark::NON_MARK, targets)));
            ASSERT(ret.second); // name already exists
        }
        
        std::pair<bool, std::vector<Key>> sort() {
            std::vector<Key> sorted;
            try {
                while (true) {
                    Key index = select();
                    visit(index, sorted);
                }
            } catch (const NoneSelection &e) {
                return std::make_pair(true, sorted);
            } catch (const NotSortable &e) {
                return std::make_pair(false, std::vector<Key>());
            }
        }
        
        bool sortable() {
            return sort().first;
        }
        
    public:
        std::set<std::set<Key>> coversets() const {
            std::map<Key, std::set<Key>> mapping;
            for (auto i = _mapping.begin(); i != _mapping.end(); ++i) {
                mapping[*i].insert(*i);
                for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                    if (_mapping.find(*j) != _mapping.end()) {
                        mapping[*i].insert(*j);
                    }
                }
            }
            
            std::set<std::set<Key>> coversets;
            for (auto i = mapping.begin(); i != mapping.end(); ++i) {
                std::set<std::set<Key>> coversets_with_i;
                coversets_with_i.insert(i->second);
                for (auto j = coversets.begin(); j != coversets.end(); ++j) {
                    std::set<Key> coverset = *j;
                    coverset.insert(i->second.begin(), i->second.end());
                    coversets_with_i.insert(coverset);
                }
                coversets.insert(coversets_with_i.begin(), coversets_with_i.end());
            }
            return coversets;
        }
    };
}

#endif /* defined(__explicit__toposort__) */
