#include "../include/stc/explicit_singleton.h"
#include <iostream>

class MyExplicitSingleton : public stc::explicit_singleton<MyExplicitSingleton>
//                                                         ^ This is CRTP (Curiously Recurring Template Pattern)
{
	// Note: we need to make the constructor and destructor public to meet the requirements of std::optional, which is used internally by stc::explicit_singleton.
	// Making these members public ensures that std::optional can correctly manage the creation and destruction of the instance.
public:

	MyExplicitSingleton(int value)
		: i(value)
	{
		std::cout << "MyExplicitSingleton instance created with value " << i << "." << std::endl;
	}


	~MyExplicitSingleton()
	{
		std::cout << "MyExplicitSingleton instance destroyed." << std::endl;
	}

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

	try
	{
		// Can't access the instance, it's not constructed yet!
		MyExplicitSingleton::instance().Print();
	}
	catch (const std::bad_optional_access&)
	{
		std::cout << "Oops, almost crashed!" << std::endl;
	}

	// The instance is constructed and then returned.
	MyExplicitSingleton::construct_instance(3).Print();

	// The existing instance is returned.
	MyExplicitSingleton::instance().Print();

	// Destruct and reconstruct the instance.
	MyExplicitSingleton::construct_instance(8).Print();
	std::cout << "Constructed: " << MyExplicitSingleton::instance_constructed() << std::endl;

	// Alternative to MyExplicitSingleton::instance().
	stc::explicit_singleton<MyExplicitSingleton>::instance().Print();

	// Deconstruction.
	stc::explicit_singleton<MyExplicitSingleton>::deconstruct_instance();
	std::cout << "Constructed: " << MyExplicitSingleton::instance_constructed() << std::endl;

	// Alive until the end of the program.
	MyExplicitSingleton::construct_instance(-2).Print();

	std::cout << "Example end!" << std::endl;
}
