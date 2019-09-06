
#include <gtest/gtest.h>
#include <filter/linear.h>
#include <vector>
#include <array>

TEST(LinearFilter, Filter_int16_t_1)
{
	Mcucpp::Filter::LinearFilter<int16_t, 4, 4> filter;
	filter.SetA({1, 1, 1, 1});
	filter.SetB({1, 1, 1, 1});
	
	std::array<int16_t, 20> data = {1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10};
	std::array<int16_t, 20> result;
	filter.Filter(std::begin(data), std::end(data), std::begin(result));
	for(auto v : result)
	{
		std::cout << v << ", ";
	}
	std::cout << std::endl;
}


