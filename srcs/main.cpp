/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 19:55:07 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 13:34:12 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "computorv1.hpp"
#include "Computor.class.hpp"

void usage(void)
{
	std::cout << "Usage: ./computor (expression)" << std::endl;
	std::cout << "Expression must be in the form of a * x^p" << std::endl;
}


int  main (int ac, char **av)
{
	(void)av;
	if (ac != 2)
	{
		usage();
		return (0);
	}

	Computor c;
	c.compute(av[1]);
  return (0);
}
