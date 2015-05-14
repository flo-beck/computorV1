/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:09:16 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/14 20:02:08 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Computor.class.hpp"

Computor::Computor(void) : _debug(false), _polyDegree(0), _discriminant(0), _res(ALL)
{
}

Computor::~Computor(void)
{
	std::vector<Token *>::iterator it;

	for (it = this->_tokensLhs.begin(); it != this->_tokensLhs.end(); it++)
		delete *it;

	for (it = this->_tokensRhs.begin(); it != this->_tokensRhs.end(); it++)
		delete *it;
}

void			Computor::setDebugMode(void)
{
	this->_debug = true;
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
	this->_parser.setLists(&this->_tokensLhs, &this->_tokensRhs);
	this->_parser.parse(input);
}

void		Computor::_printLists(void)
{
	std::cout << " * working * Equation understood as : ";
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
	if (this->_debug)
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

		move->setCoeff( -((*it)->getCoeff()));
		lhs.push_back(move);
		Token * tmp = *it;
		rhs.erase(it);
		delete tmp;
		it = rhs.begin();
	}
}

void		Computor::_mergeTokens(std::vector<Token *> & list)
{
	std::vector<Token *>::iterator it;

	it = list.begin();
	while (it != list.end())
	{
		std::vector<Token *>::iterator it2(it);
		it2++;
		while (it2 != list.end())
		{
			if ((*it2)->getPower() == (*it)->getPower())
			{
				double c = (*it)->getCoeff() + (*it2)->getCoeff();
				(*it)->setCoeff(c);
				Token * tmp = *it2;
				list.erase(it2);
				delete tmp;
				it2 = it;
			}
			it2++;
		}
		it++;
	}

	//Take out any that have cancelled each other out
	it = list.begin();
	while (it != list.end())
	{
		if ((*it)->getCoeff() == 0)
		{
			Token * tmp = *it;
			list.erase(it);
			delete tmp;
			it = list.begin();
			if (it == list.end())
				break;
		}
		it++;
	}
}

void		Computor::_printReducedForm(std::vector<Token *> & lhs)
{
	std::cout << "Reduced form: ";
	if (lhs.size() < 1)
	{
		std::cout << "0 = 0" << std::endl;
		this->_res = INF;
		return;
	}

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

	if (this->_polyDegree > 0)
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

	double part1 = (this->_b * this->_b);
	double part2 = (4 * this->_a * this->_c);

	if (this->_debug)
	{
		std::cout << " * working * a: " << this->_a << " b: " << this->_b << " c: " << this->_c << std::endl;
		std::cout << " * working * discriminant = " << part1 << " - " << part2 << std::endl;
	}
	this->_discriminant = part1 - part2;
	std::cout << "Discriminant = " << this->_discriminant << std::endl;
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
	if (n == 0)
		return 0;
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

	std::cout << "The discriminant is positive, the two solutions are: " << std::endl;

	if (this->_debug)
		std::cout << " * working * calculation: (- " << this->_b << " +/- sqrt(" << this->_discriminant << ")) / (2 * " << this->_a << ")" << std::endl;

	x1 = (-this->_b + squ_root(this->_discriminant)) / (2 * this->_a);
	x2 = (-this->_b - squ_root(this->_discriminant)) / (2 * this->_a);
	std::cout << x1 << std::endl;
	std::cout << x2 << std::endl;
}

void		Computor::_calculate1solution(void)
{
	double x;

	std::cout << "The solution is : " << std::endl;
	if (this->_debug)
		std::cout << " * working * calculation: (- " << this->_b << ") / (2 * " << this->_a << ")" << std::endl;
	x = -this->_b / (2 * this->_a);
	std::cout << x << std::endl;
}

void		Computor::_displayComplexSolution(char op, double b, double iCoeff)
{
	if (iCoeff == 0)
	{
		std::cout << b << std::endl;
		return;
	}
	if (b != 0)
	   std::cout << b << " " << op << " ";
	else if (b == 0 && op == '-')
		std::cout << op;
	if (iCoeff != 1)
		std::cout << iCoeff;
	std::cout << "i" << std::endl;
	return;
}

void		Computor::_calculateImaginarySolution(void)
{
	double iCoeff = squ_root(-this->_discriminant);

	std::cout << "The discriminant is negative, there are no real solutions.  The complex solutions are: " << std::endl;
	if (this->_debug)
		std::cout << " * working * calculation: (- " << this->_b << " +/- " << iCoeff << "i )) / (2 * " << this->_a << ")" << std::endl;

	double b = -this->_b / (2 * this->_a);
	iCoeff = iCoeff / (2 * this->_a);

	if (iCoeff > 0 && iCoeff < 1 && b != 0)
	{
		double v = 1 / iCoeff;
		b = b * v;
		iCoeff = iCoeff * v;
	}

	this->_displayComplexSolution('+', b, iCoeff);
	this->_displayComplexSolution('-', b, iCoeff);
}

void		Computor::_solveSimple(void)
{
	if (this->_tokensLhs.size() <= 1)
	{
		if (this->_res == INF)
			std::cout << "Infinite solutions for X" << std::endl;
		else
			std::cout << "No solution for X" << std::endl;
		return;
	}

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

