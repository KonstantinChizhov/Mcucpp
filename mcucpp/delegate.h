
namespace Mcucpp
{
	struct GenericClass{};

	template<class Res>
	class delegate
	{
		typedef Res (GenericClass::*ClassFuncPtr)();
		typedef Res (*FreeFuncPtr)();
	public:
		template<class ObjT>
		explicit delegate(ObjT &obj, Res (ObjT::*func)())
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit delegate(FreeFuncPtr func)
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
	class delegate1
	{
		typedef Res (GenericClass::*ClassFuncPtr)(Arg0);
		typedef Res (*FreeFuncPtr)(Arg0);
	public:
		template<class ObjT>
		explicit delegate1(ObjT &obj, Res (ObjT::*func)(Arg0))
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit delegate1(FreeFuncPtr func)
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
	class delegate2
	{
		typedef Res (GenericClass::*ClassFuncPtr)(Arg0, Arg1);
		typedef Res (*FreeFuncPtr)(Arg0, Arg1);
	public:
		template<class ObjT>
		explicit delegate2(ObjT &obj, Res (ObjT::*func)(Arg0, Arg1))
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit delegate2(FreeFuncPtr func)
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
	class delegate3
	{
		typedef Res (GenericClass::*ClassFuncPtr)(Arg0, Arg1, Arg2);
		typedef Res (*FreeFuncPtr)(Arg0, Arg1);
	public:
		template<class ObjT>
		explicit delegate3(ObjT &obj, Res (ObjT::*func)(Arg0, Arg1, Arg2))
		{
			_classFunc = reinterpret_cast<ClassFuncPtr>(func);
			_pThis = reinterpret_cast<GenericClass*>(&obj);
		}

		explicit delegate3(FreeFuncPtr func)
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