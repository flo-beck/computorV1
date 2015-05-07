/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:09:16 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/07 19:24:19 by fbeck            ###   ########.fr       */
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
	std::vector<Token *>::iterator it;

	for (it = this->_tokensLhs.begin(); it != this->_tokensLhs.end(); it++)
		delete *it;

	for (it = this->_tokensRhs.begin(); it != this->_tokensRhs.end(); it++)
		delete *it;
}

Computor &		Computor::operator=(Computor const & rhs)
{
	this->_polyDegree = rhs._polyDegree;
	this->_discriminant = rhs._discriminant;

	/*std::vector<Token *>::iterator it = rhs. _tokensLhs.begin();
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

std::string		insertAdd(std::string str)
{
	std::regex	e("-");   // matches "-"
	std::string	r("+ - ");
	return (std::regex_replace(str, e, r ));
}

void			createToken(std::string & segment, std::vector<Token *> & list)
{
	Token * t = new Token;

	std::istringstream			ss(segment);
	std::string					tmp;
	std::vector<std::string>	tmpList;
	int 						hadPower = 0;

	//split by word
	while (ss >> tmp)
		tmpList.push_back(tmp);

	std::smatch sm;  // AN std::match_results<string::const_iterator>;
	std::vector<std::string>::iterator it;

	for (it = tmpList.begin(); it != tmpList.end(); it++)
	{
		if (*it == "-")
			t->setNeg(true);
		//IF A COEFFICIENT
		else if (std::regex_match(*it, std::regex("^[0-9]+(\\.[0-9]+[0-9]*)?$")))
		{
			double coeff = std::stod(*it);
			if (t->isNeg())
				coeff = -coeff;
			t->setCoeff(coeff);
		}
		//IF AN X^POWER
		else if (std::regex_match(*it, sm, std::regex("^X\\^([0-9]+)$")) &&
				sm.size() > 1)
		{
			std::string strm = sm[1].str();
			int power = std::stoi(strm);
			t->setPower(power);
			hadPower++;
		}
	}
	if (!hadPower)
		t->setPower(0);
	list.push_back(t);
}

void			tokenise(std::string str, std::vector<Token *> & list)
{
	//split each str by +
	std::istringstream       iss(str);
	std::string			     segment;
	std::vector<std::string> seglist;

	while(std::getline(iss, segment, '+'))
		seglist.push_back(segment);

	//create a token for each
	std::vector<std::string>::iterator it;
	for (it = seglist.begin(); it != seglist.end(); it++)
	{
		// REGEX HERE TO CHECK FORMAT??
		if (it->size() > 0 && *it != " ")
		{
			std::cout << "CREATING TOKEN FOR [" << *it << "]" << std::endl;
			createToken(*it, list);
		}
	}
}

void		Computor::_parseInput(char *input)
{
	std::string str(input);

	std::size_t pos = str.find("=");
	if (pos == std::string::npos ||
			str.substr(pos + 1).find("=") != std::string::npos)
		throw BadInput();

	std::string left = str.substr(0, pos);
	std::string right = str.substr(pos + 1);

	//Add in + before any - and tokenise

	tokenise(insertAdd(left), this->_tokensLhs);
	tokenise(insertAdd(right), this->_tokensRhs);
}

void		Computor::_readInput(char *input)
{
	this->_parseInput(input);
}

void		Computor::_printLists(void)
{
	std::cout << "PRINT LIST" << std::endl;
	std::vector<Token *>::iterator it;
	std::vector<Token *>::iterator next;

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

void		Computor::_reduceInput(std::vector<Token *> & lhs, std::vector<Token *> & rhs)
{
	this->_printLists();
	this->_moveTokensToLhs(lhs, rhs);
	this->_mergeTokens(lhs);
	this->_printReducedForm(lhs);
}

void		Computor::_moveTokensToLhs(std::vector<Token *> & lhs, std::vector<Token *> & rhs)
{
	std::vector<Token *>::iterator it = rhs.begin();

	while (it != rhs.end())
	{
		Token * move = new Token(**it);
		if ((*it)->isNeg())
			move->setNeg(false);
		else
			move->setNeg(true);
		move->setCoeff(-(move->getCoeff()));
		lhs.push_back(move);
		rhs.erase(it);
		it = rhs.begin();
	}
}

void		Computor::_mergeTokens(std::vector<Token *> & list)
{
	std::vector<Token *>::iterator it;

	for (it = list.begin(); it != list.end(); it++)
	{
		std::vector<Token *>::iterator it2(it);
		it2++;
		while (it2 != list.end())
		{
			if ((*it2)->getPower() == (*it)->getPower())
			{
				double c = (*it)->getCoeff() + (*it2)->getCoeff();
				(*it)->setCoeff(c);
				list.erase(it2);
				it2 = it;
			}
			it2++;
		}
	}
	it = list.begin();
	while (it != list.end())
	{
		if ((*it)->getCoeff() == 0)
		{
			list.erase(it);
			it = list.begin();
		}
		it++;
	}
}

void		Computor::_printReducedForm(std::vector<Token *> & lhs)
{
	std::cout << "Reduced form: ";
	for (std::vector<Token *>::iterator it = lhs.begin(); it != lhs.end();)
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

void			Computor::_getPolynomialDegree(std::vector<Token *> & list)
{
	std::vector<Token *>::iterator it;
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

void		Computor::_calculateDiscriminant(std::vector<Token *> & list)
{
	for (std::vector<Token *>::iterator it = list.begin(); it != list.end(); ++it)
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
	std::vector<Token *>::iterator it;
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

const char* Computor::BadInput::what(void) const throw()
{
	return ("Error : the input is not in a format I can understand");
}
