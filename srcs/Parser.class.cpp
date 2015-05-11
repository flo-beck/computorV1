/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 13:11:00 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/11 16:45:30 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <regex>
#include "Parser.class.hpp"

/* -------------------------- CONSTRUCTORS --------------------------- */
Parser::Parser(void) : _calculate(true), _lhs(nullptr), _rhs(nullptr)
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
    (void)ref;
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
	std::regex	e("-");   // matches "-"
	std::string	r("+ - ");
	return (std::regex_replace(str, e, r ));
}

bool                Parser::canCalculate(void)
{
    return this->_calculate;
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
		// REGEX HERE TO CHECK FORMAT??
		if (it->size() > 0 && *it != " ")
		{
			std::cout << "CREATING TOKEN FOR [" << *it << "]" << std::endl;
			this->_createToken(*it, list);
		}
	}
}

void                Parser::parse(char * input)
{
	if (this->_lhs != nullptr && this->_rhs != nullptr)
	{
	std::string str(input);

	//Check the string has an =
	std::size_t pos = str.find("=");
	if (pos == std::string::npos ||
			str.substr(pos + 1).find("=") != std::string::npos)
		throw BadInput();

	std::string left = str.substr(0, pos);
	std::string right = str.substr(pos + 1);

	//Add in + before any - and tokenise
	this->_tokenise(this->_insertAdd(left), *(this->_lhs));
	this->_tokenise(this->_insertAdd(right), *(this->_rhs));
	}
	else
		std::cout << "Error - must set left and right lists" << std::endl;
}
/*
int             Parser::_lexical_analysis(std::list<std::string> tokens)
{

    // Check first character not a comment
    if (line.length() > 0 && !this->isComment(line))
    {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word)
        {
            // stop when find a comment
            if (this->isComment(word))
                break;
            else
            {
                if (!this->_is_valid_word(word))
                {
                    this->_execute = 0;
                    throw UnknownTokenError();
                }
            }
        }
    }
    if (!this->_execute)
        return 0;
    return 1;
}*/

/*int             Parser::_syntax_analysis(std::list<std::string> tokens)
{

    // Check first character not a comment
    if (line.length() > 0 && this->isComment(line) == false)
    {
        std::istringstream iss(line);
        std::string w;
        while (iss >> w)
        {
            // stop when find a comment
            if (this->isComment(w))
                break;
            else
                tokens.push_back(w);
        }
        this->_check_tokens_syntax(tokens, lineNum);
    }
    if (!this->_execute)
        return 0;
    return 1;
}
*/
const char* Parser::UnknownTokenError::what(void) const throw()
{
    return ("Unknown token");
}

const char* Parser::ExpectedValueError::what(void) const throw()
{
    return ("Expected operand after instruction");
}

const char* Parser::ExpectedNewLineError::what(void) const throw()
{
    return ("Expected new line");
}

const char* Parser::ExpectedInstructionError::what(void) const throw()
{
    return ("Expected Instruction before operand");
}

const char* Parser::BadInput::what(void) const throw()
{
	return ("Error : the input is not in a format I can understand");
}
