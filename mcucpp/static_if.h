
#pragma once
namespace Mcucpp
{
	template<bool condition, class TypeIfTrue, class TypeIfFale>
	struct StaticIf
	{
		typedef TypeIfTrue Result;
	};

	template<class TypeIfTrue, class TypeIfFale>
	struct StaticIf<false, TypeIfTrue, TypeIfFale>
	{
		 typedef TypeIfFale Result;
	};
}