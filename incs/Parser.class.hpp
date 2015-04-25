/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 13:06:24 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 13:39:01 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CLASS_CPP
# define PARSER_CLASS_CPP



#include <list>
#include <string>
#include <exception>

class Parser {

	public:

		Parser (void);
		Parser (Parser const & ref);
		virtual ~Parser (void);

		Parser &                        operator= (Parser const & ref);

		bool                            canCalculate(void);
		int                             parse(std::list<std::string> tokens);


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

	private:

		bool                        _calculate;

		int                         _lexical_analysis(std::list<std::string> tokens);
		int                         _syntax_analysis(std::list<std::string> tokens);

};

#endif
