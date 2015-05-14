/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 13:11:00 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/14 19:21:56 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include "Parser.class.hpp"

/* -------------------------- CONSTRUCTORS --------------------------- */
Parser::Parser(void) : _lhs(nullptr), _rhs(nullptr)
{
}

Parser::Parser(Parser const & ref)
{
    *this = ref;
}

/* --------------------------  DESTRUCTOR  --------------------------- */
Parser::~Parser(void)
{
}

/* --------------------------  OPERATOR =  --------------------------- */
Parser & Parser::operator=(Parser const & ref)
{
	this->_lhs = ref._lhs;
	this->_rhs = ref._rhs;
    return *this;
}

/* ------------------------------------------------------------------- */

void				Parser::setLists(std::vector<Token *> * lhs,
		std::vector<Token *> * rhs)
{
	this->_lhs = lhs;
	this->_rhs = rhs;
}

std::string			Parser::_insertAdd(std::string str)
{
	std::regex	f1("- -");
	std::string	r1("+ ");
	std::string tmp(std::regex_replace(str, f1, r1));

	std::regex	f2("-");
	std::string	r2("+ - ");
	return (std::regex_replace(tmp, f2, r2));
}

void				Parser::_createToken(std::string & segment,
		std::vector<Token *> & list)
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
		else if (*it == "X" ||
				(std::regex_match(*it, sm, std::regex("^X\\^([0-9]+)$")) &&
				 sm.size() > 1))
		{
			if (*it == "X")
				hadPower++;
			else
			{
				std::string strm = sm[1].str();
				int power = std::stoi(strm);
				t->setPower(power);
				hadPower++;
			}
		}
	}
	if (!hadPower)
		t->setPower(0);
	list.push_back(t);
}

void				Parser::_tokenise(std::string str, std::vector<Token *> & list)
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
		if (it->size() > 0 && *it != " ")
		{
			//std::cout << "CREATING TOKEN FOR [" << *it << "]" << std::endl;
			this->_createToken(*it, list);
		}
	}
}

void                Parser::parse(char * input)
{
	if (this->_lhs == nullptr || this->_rhs == nullptr)
		throw UnsetLists();

	std::string str(input);

	//Check str does not contain a /
	std::size_t pos = str.find("/");
	if (pos != std::string::npos)
		throw DivisionError();

	this->_lexer(input);

	//Check powers
	std::regex findNegPowers(".*\\^[+,-].*");
	std::regex findDecPowers(".*\\^[0-9]+\\.[0-9]*.*");
	if (std::regex_match(str, findNegPowers) || std::regex_match(str, findDecPowers))
		throw BadPowerError();

	//Check the string has an =
	pos = str.find("=");
	if (pos == std::string::npos ||
			str.substr(pos + 1).find("=") != std::string::npos)
		throw BadInput();

	std::string left = str.substr(0, pos);
	std::string right = str.substr(pos + 1);

	//Add in + before any - and tokenise
	this->_tokenise(this->_insertAdd(left), *(this->_lhs));
	this->_tokenise(this->_insertAdd(right), *(this->_rhs));
}

void		Parser::_lexer(char * str)
{
	std::vector<char> tokens = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '+', '-', '*', 'X', '^', '=', '.'};

	for (int i = 0; str[i] != '\0'; i++)
	{
		int	found = 0;
		for (unsigned n = 0; n < tokens.size(); n++)
		{
			if (tokens[n] == str[i])
			{
				found = 1;
				break;
			}
		}
		if (!found)
			throw TokenNotFound();
	}
}

const char* Parser::DivisionError::what(void) const throw()
{
	return ("Error: division is not handled\nExpression must be in the form of A * X^P");
}

const char* Parser::TokenNotFound::what(void) const throw()
{
	return ("Error: unrecognised character in input");
}

const char* Parser::BadPowerError::what(void) const throw()
{
	return ("Error: all powers must be positive whole numbers");
}

const char* Parser::UnsetLists::what(void) const throw()
{
	return ("Error : must call set lists before parsing");
}

const char* Parser::BadInput::what(void) const throw()
{
	return ("Error : incorrect format\nExpression must be in the form of A * X^P");
}
