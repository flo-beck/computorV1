/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:07:06 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/11 16:46:27 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

#include <vector>
#include "Token.class.hpp"
#include "Parser.class.hpp"

class Computor {

  public:

    Computor (void);
    Computor (Computor const & ref);
    virtual ~Computor (void);

    Computor & operator= (Computor const & rhs);

	void       compute(char *input);

	class TooComplicated : public std::exception {
		public:
			virtual const char * what() const throw ();
	};


  private:
	std::vector<Token *>	_tokensLhs;
	std::vector<Token *>	_tokensRhs;
	Parser					_parser;
	int						_polyDegree;
	double					_discriminant;
	double					_a;
	double					_b;
	double					_c;

	void		_readInput(char *input);
	void		_parseInput(char *input);
	void		_reduceInput(std::vector<Token *> & inputL,
			std::vector<Token *> & inputR);
	void		_getPolynomialDegree(std::vector<Token *> & list);
	void		_calculateDiscriminant(std::vector<Token *> & list);
	void		_calculateX(void);
	void		_calculate2solutions(void);
	void		_calculate1solution(void);
	void		_calculateImaginarySolution(void);
	void		_solveSimple(void);


	void		_moveTokensToLhs(std::vector<Token *> & lhs, std::vector<Token *> & rhs);
	void		_mergeTokens(std::vector<Token *> & list);
	void		_printReducedForm(std::vector<Token *> & lhs);


	//TODO Take OUT!!!!
	void		_printLists(void);
};

#endif
