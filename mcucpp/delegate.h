
namespace Mcucpp
{
	struct GenericClass{};

	template<class Res>
	class Delegate
	{
		typedef Res (GenericClass::*ClassFuncPtr)();
		typedef Res (*FreeFuncPtr)();
	public:
		template<class ObjT>
		explicit Delegate(ObjT &obj, Res (ObjT::*func)())
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit Delegate(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
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
	public:
		template<class ObjT>
		explicit Delegate1(ObjT &obj, Res (ObjT::*func)(Arg0))
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit Delegate1(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
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
	public:
		template<class ObjT>
		explicit Delegate2(ObjT &obj, Res (ObjT::*func)(Arg0, Arg1))
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit Delegate2(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
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
	public:
		template<class ObjT>
		explicit Delegate3(ObjT &obj, Res (ObjT::*func)(Arg0, Arg1, Arg2))
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit Delegate3(FreeFuncPtr func)
		{
			_freeFunc = func;
			_pThis = 0;
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