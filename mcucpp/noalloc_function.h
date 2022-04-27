
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2022
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#pragma once

#if !defined(NOALLOC_FUNCTION_STORAGE_SIZE)
#define NOALLOC_FUNCTION_STORAGE_SIZE 3
#endif

#include <string.h>

namespace Mcucpp
{

    struct noalloc_function_storage
    {
        union
        {
            void *data[NOALLOC_FUNCTION_STORAGE_SIZE]; // void * should satisfy most of alligment requirements in embedded systems
            void *object;
        };
    };

    template <class>
    class noalloc_function;

    template <class R, class... Args>
    class noalloc_function<R(Args...)>
    {

    public:
        using result_type = R;
        using target_type = R(Args...);
        using invoker_type = R(Args..., noalloc_function_storage &);

        noalloc_function() = default;
        noalloc_function &operator=(noalloc_function &rhs) = default;
        noalloc_function &operator=(noalloc_function &&rhs) = default;
        noalloc_function &operator=(const noalloc_function &rhs) = default;
        noalloc_function (noalloc_function &rhs) = default;
        noalloc_function (noalloc_function &&rhs) = default;
        noalloc_function (const noalloc_function &rhs) = default;

        noalloc_function(target_type func)
            : invoker(invoke_function)
        {
            storage.object = reinterpret_cast<void *>(func);
        }

        noalloc_function &operator=(target_type func)
        {
            invoker = invoke_function;
            storage.object = reinterpret_cast<void *>(func);
            return *this;
        }

        noalloc_function(decltype(nullptr))
        {
        }

        noalloc_function &operator=(decltype(nullptr))
        {
            invoker = nullptr;
            return *this;
        }

        template <class CallableT>
        noalloc_function(CallableT &functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
        }

        template <class CallableT>
        noalloc_function(CallableT &&functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
        }

        template <class CallableT>
        noalloc_function &operator=(CallableT &functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
            return *this;
        }

        template <class CallableT>
        noalloc_function &operator=(CallableT &&functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
            return *this;
        }

        template <class ObjectT>
        noalloc_function(ObjectT &object, invoker_type invokerFunc)
        {
            storage.object = &object;
            invoker = invokerFunc;
        }

        inline result_type operator()(Args... args)
        {
            if (!invoker)
            {
                return result_type();
            }
            return invoker(args..., storage);
        }

        operator bool() { return (bool)invoker; }

    private:
        noalloc_function_storage storage = {0};
        invoker_type *invoker = nullptr;

    private:
        template <class ObjectT, result_type (ObjectT::*Func)(Args...)>
        static result_type invoke_class_member(Args... args, noalloc_function_storage &storage)
        {
            return (static_cast<ObjectT *>(storage.object)->*Func)(args...);
        }

        template <class CallableT>
        static result_type invoke_functor(Args... args, noalloc_function_storage &storage)
        {
            return (*reinterpret_cast<CallableT *>(&storage.data[0]))(args...);
        }

        static result_type invoke_function(Args... args, noalloc_function_storage &storage)
        {
            return (reinterpret_cast<target_type*>(storage.object))(args...);
        }
    };

    template <class ObjectT, class R, class... Args>
    auto bind_s(R (ObjectT::*memberFunc)(Args...) const, const ObjectT &object)
    {
        using function = noalloc_function<R(Args...)>;
        return function([memberFunc, object](Args... args)
                        { (object.*memberFunc)(args...); });
    }

    template <class ObjectT, class R, class... Args>
    auto bind_s(R (ObjectT::*memberFunc)(Args...), ObjectT &object)
    {
        using function = noalloc_function<R(Args...)>;
        return function([memberFunc, &object](Args... args) mutable
                        { (object.*memberFunc)(args...); });
    }

}