
namespace Mcucpp
{
	template<class T>
	struct divmod10_t
	{
		T quot;
		uint8_t rem;
	};

	inline static divmod10_t<uint32_t> divmodu10(uint32_t n)
	{
		divmod10_t<uint32_t> res;
		res.quot = n >> 1;
		res.quot += res.quot >> 1;
		res.quot += res.quot >> 4;
		res.quot += res.quot >> 8;
		res.quot += res.quot >> 16;
		uint32_t qq = res.quot;
		res.quot >>= 3;
		res.rem = uint8_t(n - ((res.quot << 1) + (qq & ~7ul)));
		if(res.rem > 9)
		{
			res.rem -= 10;
			res.quot++;
		}
		return res;
	}

	inline static divmod10_t<uint16_t> divmodu10(uint16_t n)
	{
		divmod10_t<uint16_t> res;
		res.quot = n >> 1;
		res.quot += res.quot >> 1;
		res.quot += res.quot >> 4;
		res.quot += res.quot >> 8;
		uint16_t qq = res.quot;
		res.quot >>= 3;
		res.rem = uint8_t(n - ((res.quot << 1) + (qq & ~7ul)));
		if(res.rem > 9)
		{
			res.rem -= 10;
			res.quot++;
		}
		return res;
	}

	inline static divmod10_t<uint8_t> divmodu10(uint8_t n)
	{
		divmod10_t<uint8_t> res;
		res.quot = n >> 1;
		res.quot += res.quot >> 1;
		res.quot += res.quot >> 4;
		uint8_t qq = res.quot;
		res.quot >>= 3;
		res.rem = uint8_t(n - ((res.quot << 1) + (qq & ~7ul)));
		if(res.rem > 9)
		{
			res.rem -= 10;
			res.quot++;
		}
		return res;
	}

}