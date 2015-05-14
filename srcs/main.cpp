/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 19:55:07 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/14 19:24:45 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>

#include "computorv1.hpp"
#include "Computor.class.hpp"

void usage(void)
{
	std::cout << "Usage: ./computor [-d] (expression)" << std::endl;
	std::cout << "Expression must be in the form of A * X^P where P is a positive whole number" << std::endl;
}


int  main (int ac, char **av)
{
	if (ac < 2 || ac > 3 || (ac == 3 && strcmp(av[1], "-d") != 0))
	{
		usage();
		return (0);
	}

	Computor c;
	if (ac == 3)
	{
		c.setDebugMode();
		c.compute(av[2]);
	}
	else
		c.compute(av[1]);
  return (0);
}
