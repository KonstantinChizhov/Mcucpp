//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

namespace Mcucpp
{
	struct GenericClass{};

	template<class Res>
	class Delegate
	{
		typedef Res (GenericClass::*ClassFuncPtr)();
		typedef Res (*FreeFuncPtr)();
		static Res VoidFunc(){return Res();}
	public:
		template<class ObjT>
		explicit Delegate(ObjT &obj, Res (ObjT::*func)())
			:_classFunc(reinterpret_cast<ClassFuncPtr>(func)),
			_pThis(reinterpret_cast<GenericClass*>(&obj))
		{}

		explicit Delegate(FreeFuncPtr func)
			:_freeFunc(func),
			_pThis(0)
		{}

		Delegate()
			:_freeFunc(VoidFunc),
			_pThis(0)
		{}

		Delegate& operator=(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
			return *this;
		}

		Res operator ()()const
		{
			if(_pThis)
				return (_pThis->*_classFunc)();
			return _freeFunc();
		}
	private:
		union
		{
			ClassFuncPtr _classFunc;
			FreeFuncPtr _freeFunc;
		};
		GenericClass *_pThis;
	};

	template<class Res, class Arg0>
	class Delegate1
	{
		typedef Res (GenericClass::*ClassFuncPtr)(Arg0);
		typedef Res (*FreeFuncPtr)(Arg0);
		static Res VoidFunc(Arg0){return Res();}
	public:
		template<class ObjT>
		explicit Delegate1(ObjT &obj, Res (ObjT::*func)(Arg0))
			:_classFunc(reinterpret_cast<ClassFuncPtr>(func)),
			_pThis(reinterpret_cast<GenericClass*>(&obj))
		{}

		explicit Delegate1(FreeFuncPtr func)
			:_freeFunc(func),
			_pThis(0)
		{}

		Delegate1()
			:_freeFunc(VoidFunc),
			_pThis(0)
		{}

		Delegate1& operator=(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
			return *this;
		}

		Res operator ()(Arg0 arg0)const
		{
			if(_pThis)
				return (_pThis->*_classFunc)(arg0);
			return _freeFunc(arg0);
		}
	private:
		union
		{
			ClassFuncPtr _classFunc;
			FreeFuncPtr _freeFunc;
		};
		GenericClass *_pThis;
	};


	template<class Res, class Arg0, class Arg1>
	class Delegate2
	{
		typedef Res (GenericClass::*ClassFuncPtr)(Arg0, Arg1);
		typedef Res (*FreeFuncPtr)(Arg0, Arg1);
		static Res VoidFunc(Arg0, Arg1){return Res();}
	public:
		template<class ObjT>
		explicit Delegate2(ObjT &obj, Res (ObjT::*func)(Arg0, Arg1))
			:_classFunc(reinterpret_cast<ClassFuncPtr>(func)),
			_pThis(reinterpret_cast<GenericClass*>(&obj))
		{}

		explicit Delegate2(FreeFuncPtr func)
			:_freeFunc(func),
			_pThis(0)
		{}

		Delegate2()
			:_freeFunc(VoidFunc),
			_pThis(0)
		{}

		Delegate2& operator=(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
			return *this;
		}

		Res operator ()(Arg0 arg0, Arg1 arg1)const
		{
			if(_pThis)
				return (_pThis->*_classFunc)(arg0, arg1);
			return _freeFunc(arg0, arg1);
		}
	private:
		union
		{
			ClassFuncPtr _classFunc;
			FreeFuncPtr _freeFunc;
		};
		GenericClass *_pThis;
	};

	template<class Res, class Arg0, class Arg1, class Arg2>
	class Delegate3
	{
		typedef Res (GenericClass::*ClassFuncPtr)(Arg0, Arg1, Arg2);
		typedef Res (*FreeFuncPtr)(Arg0, Arg1, Arg2);
		static Res VoidFunc(Arg0, Arg1, Arg2){return Res();}
	public:
		template<class ObjT>
		explicit Delegate3(ObjT &obj, Res (ObjT::*func)(Arg0, Arg1, Arg2))
			:_classFunc(reinterpret_cast<ClassFuncPtr>(func)),
			_pThis(reinterpret_cast<GenericClass*>(&obj))
		{}

		explicit Delegate3(FreeFuncPtr func)
			:_freeFunc(func),
			_pThis(0)
		{}

		Delegate3()
			:_freeFunc(VoidFunc),
			_pThis(0)
		{}

		Delegate3& operator=(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
			return *this;
		}

		Res operator ()(Arg0 arg0, Arg1 arg1, Arg2 arg2)const
		{
			if(_pThis)
				return (_pThis->*_classFunc)(arg0, arg1, arg2);
			return _freeFunc(arg0, arg1, arg2);
		}
	private:
		union
		{
			ClassFuncPtr _classFunc;
			FreeFuncPtr _freeFunc;
		};
		GenericClass *_pThis;
	};
}
