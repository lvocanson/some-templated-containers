#include "../include/stc/lazy_singleton.h"
#include <iostream>

class MyLazySingleton : public stc::lazy_singleton<MyLazySingleton>
//                                                 ^ This is CRTP (Curiously Recurring Template Pattern)
{
	// The line below allows the construction of MyLazySingleton even if the constructor is private.
	// A private constructor is recommended to prevent any other instances of the class.
	friend lazy_singleton<MyLazySingleton>;

	MyLazySingleton()
	{
		std::cout << "MyLazySingleton instance created." << std::endl;
	}

	~MyLazySingleton()
	{
		std::cout << "MyLazySingleton instance destroyed." << std::endl;
	}

	int i = 0;

public:

	void Print() { std::cout << "i = " << i++ << std::endl; }
};

int main()
{
	// MyLazySingleton instance is not yet constructed.
	std::cout << "Example start!" << std::endl;

	// The instance is default-constructed and then returned.
	MyLazySingleton::instance().Print();

	// The existing instance is returned.
	MyLazySingleton::instance().Print();

	// Alternative to the above line (same instance).
	stc::lazy_singleton<MyLazySingleton>::instance().Print();

	std::cout << "Example end!" << std::endl;
}
