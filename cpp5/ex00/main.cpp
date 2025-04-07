#include "Bureaucrat.hpp"

int	main(void)
{
	Bureaucrat*	ann = NULL;
	Bureaucrat*	bob = NULL;
	Bureaucrat*	clo = NULL;

	try
	{
		ann = new Bureaucrat("Ann", 2);
		bob = new Bureaucrat("Bob", 149);
		clo = new Bureaucrat("Clo", 0);
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << "\n";
	}

	try
	{
		std::cout << "--> Promoting " << *ann << " two times\n";
		ann->promote();
		ann->promote();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	try
	{
		std::cout << "--> Demoting " << *bob << " two times\n";
		bob->demote();
		bob->demote();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	delete ann;
	delete bob;
	delete clo;
}