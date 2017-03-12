//
// Created by Mingkai Chen on 2017-03-12.
//

#include <string>
#include <iostream>

class Foo
{
public:
	void set (std::string s, bool left)
	{
		if (left)
		{
			left_ = s;
		}
		else
		{
			right_ = s;
		}
	}

	void print (void)
	{
		std::cout << left_ << "|" << right_ << std::endl;
	}

private:
	std::string left_;
	std::string right_;
};

int main(int argc, char* argv[])
{
	Foo a;
	a.set("left", argc % 2);
	a.set("right", argc % 2 == 0);
	a.print();
	return 0;
}