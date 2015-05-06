/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:09:16 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/06 19:01:19 by fbeck            ###   ########.fr       */
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
	this->_polyDegree = rhs._polyDegree;
	this->_discriminant = rhs._discriminant;

	/*std::list<Token *>::iterator it = rhs. _tokensLhs.begin();
	for ( ; it != rhs._tokensLhs.end(); it++)
	{
		Token * newtoken(*it);
		this->_tokensLhs.push_back(newtoken);
	}

	for (it = rhs._tokensRhs.begin(); it != rhs._tokensRhs.end(); it++)
	{
		Token * newtoken(*it);
		this->_tokensRhs.push_back(newtoken);
	}*/
	return *this;
}

void			Computor::compute(char *input)
{
	try
	{
		this->_readInput(input);
		this->_reduceInput(this->_tokensLhs, this->_tokensRhs);
		this->_getPolynomialDegree(this->_tokensLhs);
		if (this->_polyDegree > 1)
		{
			this->_calculateDiscriminant(this->_tokensLhs);
			this->_calculateX();
		}
		else
			this->_solveSimple();
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
		else if (*it == "+" || *it == "-")
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

void		Computor::_printLists(void)
{
	std::cout << "PRINT LIST" << std::endl;
	std::list<Token *>::iterator it;
	std::list<Token *>::iterator next;

	for (it = this->_tokensLhs.begin(); it != this->_tokensLhs.end(); it++)
	{
		next = it;
		next++;
		std::cout << *(*it);
		if (next != this->_tokensLhs.end())
		{
			if (!(*next)->isNeg())
				std::cout << " + ";
		}
	}
	std::cout << " = " ;
	for (it = this->_tokensRhs.begin(); it != this->_tokensRhs.end(); it++)
	{
		next = it;
		next++;
		std::cout << *(*it);
		if (next != this->_tokensRhs.end())
		{
			if (!(*next)->isNeg())
				std::cout << " + ";
		}
	}
	std::cout << std::endl ;
}

void		Computor::_reduceInput(std::list<Token *> & lhs, std::list<Token *> & rhs)
{
	this->_printLists();
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

	std::cout <<" BEFORE MERGING" << std::endl;
	for (it = list.begin(); it != list.end(); it++)
	{
		std::cout << *(*it) << std::endl;
	}

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
		if ((*it)->getCoeff() == 0)
			list.erase(it);
	}

}

void		Computor::_printReducedForm(std::list<Token *> & lhs)
{
	std::cout << "Reduced form: ";
	for (std::list<Token *>::iterator it = lhs.begin(); it != lhs.end();)
	{
		std::cout << **it;
		if (++it != lhs.end())
		{
			if (!(*it)->isNeg())
				std::cout << " + ";
		}
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
	for (std::list<Token *>::iterator it = list.begin(); it != list.end(); ++it)
	{
		if ((*it)->getPower() == 2)
			this->_a = (*it)->getCoeff();
		else if ((*it)->getPower() == 1)
			this->_b = (*it)->getCoeff();
		else if ((*it)->getPower() == 0)
			this->_c = (*it)->getCoeff();
	}

	std::cout << "a: " << this->_a << " b: " << this->_b << " c: " << this->_c << std::endl;
	this->_discriminant = (this->_b * this->_b) - (4 * this->_a * this->_c);
	std::cout << "Discriminant = " << this->_discriminant << std::endl;
	if (this->_discriminant > 0)
		std::cout << "The discriminant is positive, the two solutions are: " << std::endl;
	else if (this->_discriminant == 0)
		std::cout << "The discriminant = 0, the solution is : " << std::endl;
	else
		std::cout << "The discriminant is negative, there are no real soltions.  The imaginary solutions are: " << std::endl;
}

void		Computor::_calculateX(void)
{
	if (this->_discriminant > 0)
		this->_calculate2solutions();
	else if (this->_discriminant == 0)
		this->_calculate1solution();
	else
		this->_calculateImaginarySolution();
}

double		power(double n, int p)
{
	double res = 1;

	for (int i = 0; i < p; ++i)
		res = res * n;
	return res;
}

double		calc_seed(double num)
{
	double a = num;
	int n = 0;

	while (a > 100 )
	{
		a = a / 100;
		n++;
	}

	int val = (a >= 10) ? 6 : 2;
	return val * power(10, n);
}

double		squ_root(double n)
{

	int i = 0;
	double e2 = calc_seed(n);
	double e1 = 0;

	while (e1 != e2)
	{
		e1 = e2;
		e2 = (e1 + (n / e1)) / 2;
		i++;
	}

	return e1;
}

void		Computor::_calculate2solutions(void)
{
	double x1;
	double x2;

	x1 = (-this->_b + squ_root(this->_discriminant)) / (2 * this->_a);
	x2 = (-this->_b - squ_root(this->_discriminant)) / (2 * this->_a);
	std::cout << x1 << std::endl;
	std::cout << x2 << std::endl;
}

void		Computor::_calculate1solution(void)
{
	double x;

	x = (-this->_b + squ_root(this->_discriminant)) / 2 * this->_a;
	std::cout << x << std::endl;
}

void		Computor::_calculateImaginarySolution(void)
{
	std::cout << "Imagine anything. its imaginary!!" << std::endl;

}

void		Computor::_solveSimple(void)
{
	//TO DO :: Check for when there is no solution, or ANY real mumber is a solution
	std::list<Token *>::iterator it;
	double val = 0;

	for (it = this->_tokensLhs.begin(); it != this->_tokensLhs.end(); ++it)
	{
		if ((*it)->getPower() == 0)
			val -= (*it)->getCoeff();
	}
	for (it = this->_tokensLhs.begin(); it != this->_tokensLhs.end(); ++it)
	{
		if ((*it)->getPower() == 1)
			val = val / (*it)->getCoeff();
	}
	std::cout << "The solution is: " << std::endl;
	std::cout << val << std::endl;
}

const char* Computor::TooComplicated::what(void) const throw()
{
	return ("Error : the polynomial degree is greater than 2");
}
