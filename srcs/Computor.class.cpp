/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:09:16 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/24 20:13:43 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Computor.class.hpp"

Computor::Computor(void)
{
}

Computor::Computor(Computor const & ref)
{
	*this = ref;
}

Computor::~Computor(void)
{
}

Computor &		Computor::operator=(Computor const & rhs)
{
	(void) rhs;
	return *this;
}

void			Computor::compute(char **av)
{

	(void)av;
	std::cout << "Coucou! :)" << std::endl;
}

