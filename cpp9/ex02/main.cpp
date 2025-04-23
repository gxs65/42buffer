#include <string>
#include <iostream>
#include <vector>
#include "PmergeMe.hpp"

// #f
// 		- check valeurs égales dans la séquence non triée
// 		- modifier la boucle étape C : calcul Jacobsthal en fin de boucle
// 		- passer en template pour des containers vector ou deque
// 		- réécrire binaryInsertionIndex pour utiliser des index au lieu d'iterateurs
// 		   ou bien insertElement pour utiliser un iterateur au lieu d'index
// 		- petite optimisation : réduire insertionSeqSize quand un élément est inséré après le max
// 			ou aussi que sur la fin de séquence on a pas autant d'éléments que prescrit par nbs de Jacobsthal
// 		- fonction d'affichage du vecteur qui colore le nb de fin d'élément
// 		- fonction de comparaison qui compte le nb d'appels

int	main(int ac, char **av)
{
	std::vector<int>	v;
	PmergeMe			FJSorter;

	if (ac < 2)
		return (1);
	else if (ac == 2)
		strToIntArray(v, std::string(av[1]));
	else
		argsToIntArray(v, av + 1);
	std::cout << "Before sorting :\n";
	logVector(v);
	std::cout << "\n";

	FJSorter.sort(v, v.size() - 1, 0, 1);
	std::cout << "\nAfter sorting :\n";
	logVector(v);
	std::cout << "Is sorted : " << isSorted(v) << "\n";

	return (0);
}
