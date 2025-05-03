#include "../include/stc/explicit_singleton.h"
#include <iostream>

class MyExplicitSingleton : public stc::explicit_singleton<MyExplicitSingleton>
//                                                         ^ This is CRTP (Curiously Recurring Template Pattern)
{
	// The line below allows the construction of MyExplicitSingleton even if the constructor is private.
	// A private constructor is recommended to prevent any other instances of the class.
	friend explicit_singleton;

	MyExplicitSingleton(int value)
		: i(value)
	{
		std::cout << "MyExplicitSingleton instance created with value " << i << "." << std::endl;
	}


	~MyExplicitSingleton()
	{
		std::cout << "MyExplicitSingleton instance destroyed." << std::endl;
	}

public:

	void Print() { std::cout << "i = " << i++ << std::endl; }

private:

	int i = 0;
};

int main()
{
	// MyExplicitSingleton instance is not yet constructed.
	std::cout << "Example start!" << std::boolalpha << std::endl;

	// Check if the instance is constructed.
	std::cout << "Constructed: " << MyExplicitSingleton::instance_constructed() << std::endl;

	// This is UB (access to uninitialized memory).
	// MyExplicitSingleton::instance().Print();

	// The instance is constructed and then returned.
	MyExplicitSingleton::construct_instance(3).Print();

	// The existing instance is returned.
	MyExplicitSingleton::instance().Print();

	// Destruct and reconstruct the instance.
	MyExplicitSingleton::construct_instance(8).Print();
	std::cout << "Constructed: " << MyExplicitSingleton::instance_constructed() << std::endl;

	// Alternative to MyExplicitSingleton::instance().
	stc::explicit_singleton<MyExplicitSingleton>::instance().Print();

	// Destruction.
	MyExplicitSingleton::destruct_instance();
	std::cout << "Constructed: " << MyExplicitSingleton::instance_constructed() << std::endl;

	// Alive until the end of the program.
	MyExplicitSingleton::construct_instance(-2).Print();

	std::cout << "Example end!" << std::endl;
}
