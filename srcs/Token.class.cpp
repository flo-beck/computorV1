/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.class.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 14:21:49 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 19:33:52 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.class.hpp"
#include <iostream>
#include <sstream>

Token::Token(void) : _coeff(1), _neg(false), _power(1)
{
}

Token::Token(Token const & ref)
{
	*this = ref;
}

Token::~Token(void)
{
}

Token &		Token::operator=(Token const & rhs)
{
	this->_coeff = rhs._coeff;
	this->_neg = rhs._neg;
	this->_power = rhs._power;
	return *this;
}


void		Token::setCoeff(double coeff)
{
	this->_coeff = coeff;
}

double		Token::getCoeff(void) const
{
	return this->_coeff;
}

void		Token::setNeg(bool n)
{
	this->_neg = n;
}

bool		Token::isNeg(void) const
{
	return this->_neg;
}

void		Token::setPower(int p)
{
	this->_power = p;
}

int			Token::getPower(void) const
{
	return this->_power;
}

std::string	Token::toString(void) const
{
	std::stringstream ss;
	ss << this->_coeff << " * X^" << this->_power;
	return (ss.str());
}

std::ostream & operator<<(std::ostream & o, Token const & rhs)
{
	o << rhs.toString();
	return o;
}