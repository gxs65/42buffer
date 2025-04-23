#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "PmergeMe.hpp"

// A faire algo
// 		+ modifier la boucle étape C : calcul Jacobsthal en fin de boucle
// 		+ reecrire binaryInsertionIndex pour utiliser des index au lieu d'iterateurs
// 		   ou bien insertElement pour utiliser un iterateur au lieu d'index
// 		+ restructurer la fonction principale en sous-fonctions
// 		- passer en template pour des containers vector ou deque
// 		- reserver l'espace des conteneurs utilises pour qu'ils ne soient pas realloues (`.reserve`)
// 		- reflechir a insertion de tous les nbs d'un element d'un coup, pour eviter multiples decalages

// A faire finitions
// 		+ fonction d'affichage du vecteur qui colore le nb de fin d'élément
// 		+ fonction de comparaison qui compte le nb d'appels
// 		+ determiner plus precisement le nb de nombres de Jacobsthal a generer
// 		+ ne pas mettre les nbs de jacobsthal dans un vecteur, plutot dans un simple array
// 		- mesure du temps pris par la resolution
// 		- remplacer les insertions a la fin de container par des push_back
// 		- terminer la forme orthodoxe canonique
// 		- utiliser des attributs internes plutot qu'une flopee de parametres
// 		- script qui enleve toutes les lignes de log

// tests a faire :
// 		- addition iterator + entiers donne le resultat voulu sur conteneurs non contigus comme une liste
// 		- specialisation des methodes d'une classe template, selon les types voulus dans le template

// small optimisation not implemented : during binary insertion,
// reduce the range of insertion when an element is inserted after the last element in range
// (or when, at the end of pending, there are not enough elements to reach the next Jacobsthal number)

int	sortFJ(std::vector<int>& v, bool display)
{
	PmergeMe			FJSorter(v.size());
	int					nbComps;

	if (display)
	{
		std::cout << "\nBefore sorting :\n{ ";
		for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++)
			std::cout << *it << " ";
		std::cout << "}\n";
	}

	FJSorter.sort(v, 0, 1);
	nbComps = FJSorter.getNbComps();

	if (display)
	{
		std::cout << "\nAfter sorting :\n{ ";
		for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++)
			std::cout << *it << " ";
		std::cout << "}\nIs sorted : " << isSorted(v) << "\n";
		std::cout << "Number of comparisons : " << nbComps << "\n";
	}

	return (nbComps);
}

int	sortArgs(int ac, char **av, bool display)
{
	std::vector<int>	v;

	if (ac < 2)
		return (-1);
	else if (ac == 2)
		strToIntArray(v, std::string(av[1]));
	else
		argsToIntArray(v, av + 1);
	return (sortFJ(v, display));
}

int	sortRandom(int seqSize, bool display)
{
	std::vector<int>	v;
	int					ind;

	if (seqSize <= 0)
		return (-1);
	for (ind = 0; ind < seqSize; ind++)
		v.push_back(ind + 1);
	random_shuffle(v.begin(), v.end());
	return (sortFJ(v, display));
}

void	statsSortRandom(void)
{
	int	seqSize;
	int	ind;
	int	nbComps;
	int	maxNbComps;

	for (seqSize = 1; seqSize <= 30; seqSize++)
	{
		maxNbComps = 0;
		for (ind = 0; ind < 100; ind++)
		{
			nbComps = sortRandom(seqSize, 0);
			if (nbComps > maxNbComps)
				maxNbComps = nbComps;
		}
		std::cout << "Sequence of size " << seqSize
			<< ", max nb of comparisons over 100 sorts : " << maxNbComps << "\n";
	}
}

int	main(int ac, char **av)
{
	(void)av;
	if (ac <= 1)
		statsSortRandom();
	else
		sortRandom(static_cast<int>(strtol(av[1], NULL, 10)), 1);
		//sortArgs(ac, av, 1);
	return (0);
}
