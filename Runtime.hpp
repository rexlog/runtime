//-------------------------------------------------------
// Dependency Injection for Modern C++
// Runtime.hpp
// Version 1.0.4
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2019 Lei Peng <http://www.leiex.com>.
//-------------------------------------------------------
/*
 Permission is hereby  granted, free of charge, to any  person obtaining a copy
 of this software and associated  documentation files (the "Software"), to deal
 in the Software  without restriction, including without  limitation the rights
 to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef SRC_INCLUDE_RUNTIME_H_
#define SRC_INCLUDE_RUNTIME_H_

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

namespace rexlog {
    class RuntimeStorage {
        public:
        //-------------------------------------------------------
        /// Method: Custom delete method for data when store pointers;
        /// Parameters:
        /// self - this pointer of the class;
        /// Example:
        /// void freeObject(RuntimeStorage *self) {
        ///		// 1, cast runtime type;
        ///		RealRuntimeStorage<MyType*> *storage=static_cast<RealRuntimeStorage<MyType*>*>(self);
        ///		// 2, Delete stored data;
        /// 	delete storage->data;
        /// }
        //-------------------------------------------------------
        void (*freeMethod)(RuntimeStorage *self);
        public:
        //-------------------------------------------------------
        /// Noticed that this class ensure all data stored can be safety released;
        //-------------------------------------------------------
        RuntimeStorage() {
            this->freeMethod = nullptr;
        }
    };
//-------------------------------------------------------
/// The 'real' storage; can store any type of data;
//-------------------------------------------------------
    template<typename T>
    class RealRuntimeStorage: public RuntimeStorage {
        public:
        T data;
    };

    class Runtime {
        public:
        //-------------------------------------------------------
        /// [Static] Get the global instance(singleton);
        //-------------------------------------------------------
        static Runtime* getInstance() {
            if (Runtime::s_instance == nullptr) {
                Runtime::s_instance = new Runtime();
            }
            return Runtime::s_instance;
        }
        //-------------------------------------------------------
        /// [Static] Release the global instance(singleton);
        /// This method will clear all stored data;
        //-------------------------------------------------------
        static void freeInstance() {
            if (Runtime::s_instance != nullptr) {
                Runtime::s_instance->clearAll();
                delete Runtime::s_instance;
            }
        }
        private:
        Runtime() {
            this->m_objMap.clear();
        }
        protected:
        static Runtime *s_instance;
        std::map<std::string, RuntimeStorage*> m_objMap;
        public:
        //-------------------------------------------------------
        /// Setter: template method, store any type of data with a key;
        /// Parameters:
        /// name - the 'key' of data;
        /// data - the real data;
        //-------------------------------------------------------
        template<typename T>
        void set(std::string name, T data,
                 void (*freeMethod)(RuntimeStorage *self)=nullptr) {
            RealRuntimeStorage<T> *storage = nullptr;
            auto it = this->m_objMap.find(name);
            if (it == this->m_objMap.end()) {
                storage = new RealRuntimeStorage<T>();
                if (freeMethod != nullptr) {
                    storage->freeMethod = freeMethod;
                }
                this->m_objMap[name] = storage;
            }
            storage->data = data;
        }
        //-------------------------------------------------------
        /// Getter: template method, get any type of data by key;
        /// Parameters:
        /// name - the 'key' of data;
        /// Return:
        /// data stored, nullptr when not found;
        //-------------------------------------------------------
        template<typename T>
        T get(std::string name) {
            RealRuntimeStorage<T> *storage = nullptr;
            auto it = this->m_objMap.find(name);
            if (it != this->m_objMap.end()) {
                storage = static_cast<RealRuntimeStorage<T>*>(it->second);
                return storage->data;
            }
            std::string whatStr = name;
            whatStr += " is not set";
            throw std::out_of_range(whatStr);
        }
        //-------------------------------------------------------
        /// Delete a key, the data will also be free;
        /// Parameters:
        /// name - the 'key' of data;
        //-------------------------------------------------------
        void unset(std::string name) {
            RuntimeStorage *storage = nullptr;
            auto it = this->m_objMap.find(name);
            if (it != this->m_objMap.end()) {
                storage = it->second;
                if (storage->freeMethod != nullptr) {
                    storage->freeMethod(storage);
                }
                delete storage;
                this->m_objMap.erase(it);
            }
        }
        //-------------------------------------------------------
        /// Clear and free all data;
        //-------------------------------------------------------
        void clearAll() {
            auto it = this->m_objMap.begin();
            while(it!=this->m_objMap.end()) {
                RuntimeStorage *storage = it->second;
                if (storage->freeMethod != nullptr) {
                    storage->freeMethod(storage);
                }
                delete storage;
                it=this->m_objMap.erase(it);
            }
        }
    };
    //Runtime* Runtime::s_instance = nullptr;
}

#define REXLOG_RUNTIME_IMPLEMENTION rexlog::Runtime *rexlog::Runtime::s_instance = nullptr

#endif /* SRC_INCLUDE_RUNTIME_H_ */
