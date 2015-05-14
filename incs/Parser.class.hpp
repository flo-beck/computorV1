/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 13:06:24 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/14 19:48:10 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CLASS_CPP
# define PARSER_CLASS_CPP

#include "Token.class.hpp"
#include <vector>
#include <string>
#include <exception>

class Parser {

	public:

		Parser (void);
		Parser (Parser const & ref);
		virtual ~Parser (void);

		Parser &	operator= (Parser const & ref);

		void	setLists(std::vector<Token *> * lhs, std::vector<Token *> * rhs);
		void	parse(char * input);

		class DivisionError : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class BadPowerError : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class UnsetLists : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class BadInput : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class TokenNotFound : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

	private:

		std::vector<Token *> *		_lhs;
		std::vector<Token *> *		_rhs;

		std::string		_insertAdd(std::string & str);
		void	_lexer(char * str);
		void	_tokenise(std::string str, std::vector<Token *> & list);
		void	_createToken(std::string & segment,	std::vector<Token *> & list);
};

#endif
