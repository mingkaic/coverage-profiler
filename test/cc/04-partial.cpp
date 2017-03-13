//
// Created by Mingkai Chen on 2017-03-12.
//

#include <cstdlib>
#include "templatecheck.hpp"

void Foo (bool a)
{
	size_t res = 0;
	if (a)
	{
		res = Bar<size_t>(2, 4);
	}
	else
	{
		res = (size_t) Bar<double>(0.5, 19.2);
	}
}