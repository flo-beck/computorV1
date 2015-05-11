/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 13:06:24 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/11 16:44:44 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CLASS_CPP
# define PARSER_CLASS_CPP

#include "Token.class.hpp"
#include <list>
#include <string>
#include <exception>

class Parser {

	public:

		Parser (void);
		Parser (Parser const & ref);
		virtual ~Parser (void);

		Parser &                        operator= (Parser const & ref);

		void							setLists(std::vector<Token *> * lhs, std::vector<Token *> * rhs);
		bool                            canCalculate(void);
		void							parse(char * input);

		class UnknownTokenError : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class ExpectedValueError : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class ExpectedNewLineError : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class ExpectedInstructionError : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

		class BadInput : public std::exception {
			public:
				virtual const char * what() const throw ();
		};

	private:

		bool                        _calculate;
		std::vector<Token *> *		_lhs;
		std::vector<Token *> *		_rhs;

//		int                         _lexical_analysis(std::list<std::string> tokens);
//		int                         _syntax_analysis(std::list<std::string> tokens);

		std::string					_insertAdd(std::string str);
		void						_tokenise(std::string str, std::vector<Token *> & list);
		void						_createToken(std::string & segment,	std::vector<Token *> & list);
};

#endif
