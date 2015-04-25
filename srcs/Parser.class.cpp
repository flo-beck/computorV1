/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 13:11:00 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 13:41:06 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.class.hpp"
#include <string>
#include <list>

/* -------------------------- CONSTRUCTORS --------------------------- */
Parser::Parser(void) : _calculate(true)
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

bool                Parser::canCalculate(void)
{
    return this->_calculate;
}

int                 Parser::parse(std::list<std::string> tokens)
{
	(void) tokens;
    /*int             res1;
    int             res2;

    res1 = this->_lexical_analysis(line, lineNum);
    res2 = this->_syntax_analysis(line, lineNum);

    if (res1 < 1 || res2 < 1)
        return 0;*/
    return 1;
}

int             Parser::_lexical_analysis(std::list<std::string> tokens)
{
	(void) tokens;

    /*(void)lineNum;
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
        return 0;*/
    return 1;
}

int             Parser::_syntax_analysis(std::list<std::string> tokens)
{
	(void) tokens;

    /*std::list<std::string>  tokens;

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
        return 0;*/
    return 1;
}

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
