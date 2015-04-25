/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:09:16 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 19:44:28 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <regex>
#include "Computor.class.hpp"
#include "Token.class.hpp"

Computor::Computor(void) : _polyDegree(0), _discriminant(0)
{
}

Computor::Computor(Computor const & ref)
{
	*this = ref;
}

Computor::~Computor(void)
{
	std::list<Token *>::iterator it;

	for (it = this->_tokensLhs.begin(); it != this->_tokensLhs.end(); it++)
		delete *it;

	for (it = this->_tokensRhs.begin(); it != this->_tokensRhs.end(); it++)
		delete *it;
}

Computor &		Computor::operator=(Computor const & rhs)
{
	(void) rhs;
	return *this;
}

void			Computor::compute(char *input)
{
	try
	{
		this->_readInput(input);
		this->_reduceInput(this->_tokensLhs, this->_tokensRhs);
		this->_getPolynomialDegree(this->_tokensLhs);
		this->_calculateDiscriminant(this->_tokensLhs);
	//	calculateX();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}

void		Computor::_readInput(char *input)
{
	// ** TO DO :: IMPROVE THIS FT ***
	std::istringstream ss(input);
	std::string tmp;
	std::list<std::string> tmpList;

	//Break input down into tokens
	while (ss >> tmp)
		tmpList.push_back(tmp);


	//PARSE HERE AND ADD IN THINGS IF NECESSARY
	//parse list of tokens - will raise exception if there is an error
	//	this->_parser.parse(this->_tokens);

	int	isNeg = 0;
	int	rhs = 0;
	Token * t = nullptr;
	std::smatch sm;    // same as std::match_results<string::const_iterator> sm;

	std::list<std::string>::iterator it;
	for (it = tmpList.begin(); it != tmpList.end(); it++)
	{
		std::cout << " [" << *it << "] " << std::endl;
		if (*it == "=")
		{
			if (t)
				this->_tokensLhs.push_back(t);
			t = new Token;
			rhs = 1;
		}
		if (*it == "+" || *it == "-")
		{
			isNeg = (*it == "-") ? 1 : 0;
			if (rhs && t)
				this->_tokensRhs.push_back(t);
			else if (t)
				this->_tokensLhs.push_back(t);
			t = new Token;
			if (isNeg)
				t->setNeg(true);
		}
		//if a number
		else if (std::regex_match(*it, std::regex("^[0-9]+(\\.[0-9]+[0-9]*)?$")))
		{
			if (!t)
				t = new Token;
			//
			//**TO DO : Protect this with try for std::exception **
			double coeff = std::stod(*it);
			if (t->isNeg())
				coeff = -coeff;
			t->setCoeff(coeff);
		}
		else if (std::regex_match(*it, sm, std::regex("^X\\^([0-9]+)$")))
		{
			if (sm.size() <= 1)
				std::cout << "OOOOPES?" << std::endl;
			else
			{
				std::string strm = sm[1].str();
				if (!t)
					t = new Token;
				int power = std::stoi(strm);
				t->setPower(power);
			}
		}
	}
	this->_tokensRhs.push_back(t);

}

void		Computor::_reduceInput(std::list<Token *> lhs, std::list<Token *> rhs)
{
	this->_moveTokensToLhs(lhs, rhs);

	this->_mergeTokens(lhs);

	this->_printReducedForm(lhs);
}

void		Computor::_moveTokensToLhs(std::list<Token *> & lhs, std::list<Token *> & rhs)
{
	std::list<Token *>::iterator it = rhs.begin();

	while (it != rhs.end())
	{
		Token * move = new Token(**it);
		if ((*it)->isNeg())
			move->setNeg(false);
		else
			move->setNeg(true);
		move->setCoeff(-(move->getCoeff()));
		lhs.push_back(move);
		rhs.pop_front();
		it = rhs.begin();
	}
}

void		Computor::_mergeTokens(std::list<Token *> & list)
{
	std::list<Token *>::iterator it;

	for (it = list.begin(); it != list.end(); it++)
	{
		std::list<Token *>::iterator it2 = it;
		for (it2++; it2 != list.end(); it2++)
		{
			if ((*it2)->getPower() == (*it)->getPower())
			{
				double c;

				c = (*it)->getCoeff() + (*it2)->getCoeff();

				(*it)->setCoeff(c);
				list.erase(it2);
			}
		}
	}
}

void		Computor::_printReducedForm(std::list<Token *> & lhs)
{
	std::cout << "Reduced form: ";
	for (std::list<Token *>::iterator it = lhs.begin(); it != lhs.end();)
	{
		std::cout << **it;
		if (++it != lhs.end())
			std::cout << " + ";
	}
	std::cout << " = 0" << std::endl;
}

void			Computor::_getPolynomialDegree(std::list<Token *> & list)
{
	std::list<Token *>::iterator it;

	for (it = list.begin(); it != list.end(); it++)
	{
		int currP = (*it)->getPower();
		if (currP > this->_polyDegree)
			this->_polyDegree = currP;
	}

	std::cout << "Polynomial degree: " << this->_polyDegree << std::endl;
	if (this->_polyDegree > 2)
		throw TooComplicated();
}

void		Computor::_calculateDiscriminant(std::list<Token *> & list)
{
	double a;
	double b;
	double c;

	for (std::list<Token *>::iterator it = list.begin(); it != list.end(); ++it)
	{
		if ((*it)->getPower() == 2)
			a = (*it)->getCoeff();
		else if ((*it)->getPower() == 1)
			b = (*it)->getCoeff();
		else if ((*it)->getPower() == 0)
			c = (*it)->getCoeff();
	}

	std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
	this->_discriminant = (b * b) - (4 * a * c);
	std::cout << "Discriminant = " << this->_discriminant << std::endl;
	if (this->_discriminant > 0)
		std::cout << "The discriminant is positive, the two solutions are: " << std::endl;
	else if (this->_discriminant == 0)
		std::cout << "The discriminant = 0, the solution is : " << std::endl;
	else
		std::cout << "The discriminant is negative, there are no real soltions.  The imaginary solutions are: " << std::endl;

	(void) list;

}

const char* Computor::TooComplicated::what(void) const throw()
{
	return ("Error : the polynomial degree is greater than 2");
}
