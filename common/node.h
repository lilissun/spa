//
//  node.h
//  common
//
//  Created by Li Li on 3/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __common__node__
#define __common__node__

#include <iostream>
#include <string>
#include <vector>

#include "runtime.h"

namespace com {
    class Node {
    protected:
        void *_ptr;
        Node *_next;
        
    protected:
        Node(void *ptr) : _ptr(ptr), _next(nullptr) {}
        Node(void *ptr, Node *next) : _ptr(ptr), _next(next) {}
        ~Node() {}
        
    public:
        friend Node * encode(void *ptr);
        friend Node * encode(void *ptr, Node *next);
        friend Node * append(Node *first, Node *last);
        template<typename T> friend T * decode_single(Node *node);
        template<typename T> friend std::vector<T *> decode_vector(Node *node);
    };
    
    Node * encode(void *ptr);
    Node * append(Node *first, Node *last);
    
    template<typename T>
    T * decode_single(Node *node) {
        ASSERT(node != nullptr);
        ASSERT(node->_next == nullptr);
        T *ptr = (T *)node->_ptr;
        delete node;
        return ptr;
    }
    
    template<typename T>
    std::vector<T *> decode_vector(Node *node) {
        std::vector<T *> ret;
        while (node != nullptr) {
            ret.push_back((T *)node->_ptr);
            Node *current = node;
            node = node->_next;
            delete current;
        }
        return ret;
    }
}

#endif /* defined(__common__node__) */
