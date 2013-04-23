
namespace Mcucpp
{
	namespace Private
	{
		template<class T, int Shift, bool Skip=(sizeof(T)*8 <= Shift)>
		struct ShiftIf
		{
			static T ShiftAdd(T value)
			{
				return value;
			}
		};
		
		template<class T, int Shift>
		struct ShiftIf<T, Shift, false>
		{
			static T ShiftAdd(T value)
			{
				return value + (value>>Shift);
			}
		};
	}
	
	template<class T>
	struct divmod10_t
	{
		T quot;
		uint8_t rem;
	};

	template<class T>
	inline static divmod10_t<T> divmodu10(T n)
	{
		divmod10_t<T> res;
		res.quot = n >> 1;
		res.quot += res.quot >> 1;
		res.quot += res.quot >> 4;
		//res.quot += res.quot >> 8;
		//res.quot += res.quot >> 16;

		res.quot = Private::ShiftIf<T, 8>::ShiftAdd(res.quot);
		res.quot = Private::ShiftIf<T, 16>::ShiftAdd(res.quot);
		res.quot = Private::ShiftIf<T, 32>::ShiftAdd(res.quot);
		
		T qq = res.quot;
		res.quot >>= 3;
		res.rem = uint8_t(n - ((res.quot << 1) + (qq & ~((T)7))));
		if(res.rem > 9)
		{
			res.rem -= 10;
			res.quot++;
		}
		return res;
	}

}