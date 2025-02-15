#include "../include/stc/eager_singleton.h"
#include <iostream>

class MyEagerSingleton : public stc::eager_singleton<MyEagerSingleton>
//                                                   ^ This is CRTP (Curiously Recurring Template Pattern)
{
	// The line below allows the construction of MyEagerSingleton even if the constructor is private.
	// A private constructor is recommended to prevent any other instances of the class.
	friend eager_singleton<MyEagerSingleton>;

	MyEagerSingleton()
	{
		std::cout << "MyEagerSingleton instance created." << std::endl;
	}

	~MyEagerSingleton()
	{
		std::cout << "MyEagerSingleton instance destroyed." << std::endl;
	}

	int i = 0;

public:

	void Print() { std::cout << "i = " << i++ << std::endl; }
};

int main()
{
	// MyEagerSingleton instance is already constructed.
	std::cout << "Example start!" << std::endl;

	// The existing instance is returned.
	MyEagerSingleton::instance().Print();
	MyEagerSingleton::instance().Print();

	// Alternative to the above lines (same instance).
	stc::eager_singleton<MyEagerSingleton>::instance().Print();

	std::cout << "Example end!" << std::endl;
}
