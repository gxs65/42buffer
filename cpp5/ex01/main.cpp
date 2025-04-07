#include "Bureaucrat.hpp"

int	main(void)
{
	Bureaucrat*	ann = NULL;
	Bureaucrat*	bob = NULL;
	Form*		f1 = NULL;
	Form*		f2 = NULL;
	Form*		f3 = NULL;

	ann = new Bureaucrat("Ann", 2);
	bob = new Bureaucrat("Bob", 149);
	try
	{
		f1 = new Form("form1", 100, 100);
		f2 = new Form("form2", 100, 100);
		f3 = new Form("form3", 0, 151);
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << "\n";
	}

	try
	{
		std::cout << "--> Ann signs 1 / Bob signs 2\n";
		ann->signForm(*f1);
		bob->signForm(*f2);
		std::cout << "(successfully reached end of try block)\n";
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

	delete ann;
	delete bob;
	delete f1;
	delete f2;
	delete f3;
}