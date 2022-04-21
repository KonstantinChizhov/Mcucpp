
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
#define NOALLOC_FUNCTION_STORAGE_SIZE 4
#endif

namespace Mcucppp
{

    struct noalloc_function_storage
    {
        union{
            void *data[NOALLOC_FUNCTION_STORAGE_SIZE]; // void * should satisfy most of alligment requirements in embedded systems
            void *object;
        };
    };

    template <class>
    class noalloc_function;

    template <class R, class... Args>
    class noalloc_function<R(Args...)>
    {
        using invoker_type = R(Args..., noalloc_function_storage&);

    public:
        using result_type = R;
        using target_type = R(Args...);
        noalloc_function() = default;
        
        noalloc_function(target_type func)
        {
            target = func;
        }

        noalloc_function& operator=(target_type func)
        {
            target = func;
            return *this;
        }

        template<class CallableT>
        noalloc_function(CallableT &functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
        }


        template<class CallableT>
        noalloc_function& operator=(CallableT &functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
            return *this;
        }

        template<class CallableT>
        noalloc_function& operator=(CallableT &&functor)
        {
            static_assert(sizeof(functor) <= sizeof(storage), "Storage is too small to hold functor");
            memcpy(storage.data, &functor, sizeof(functor));
            invoker = invoke_functor<CallableT>;
            return *this;
        }

        inline result_type operator()(Args...args)
		{
            if(!target)
            {
                return result_type();
            }
            if(storage.object)
            {
                return invoker(args..., storage);
            }
            return target(args...);
		}

        operator bool() { return (bool)target; }

    private:
        noalloc_function_storage storage = {0};
        union
        {
            invoker_type *invoker = nullptr;
            target_type * target;
        };

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
    };

}