/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:07:06 by fbeck             #+#    #+#             */
/*   Updated: 2015/05/14 19:43:55 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

#include <vector>
#include "Token.class.hpp"
#include "Parser.class.hpp"

class Computor {

	# define ALL	0
	# define NAN	1
	# define INF	2

  public:

    Computor (void);
    virtual ~Computor (void);

	void		compute(char *input);
	void		setDebugMode(void);

	class TooComplicated : public std::exception {
		public:
			virtual const char * what() const throw ();
	};

  private:
	bool					_debug;
	std::vector<Token *>	_tokensLhs;
	std::vector<Token *>	_tokensRhs;
	Parser					_parser;
	int						_polyDegree;
	double					_discriminant;
	int						_res;
	double					_a;
	double					_b;
	double					_c;

	void		_readInput(char *input);
	void		_reduceInput(std::vector<Token *> & inputL, std::vector<Token *> & inputR);
	void		_getPolynomialDegree(std::vector<Token *> & list);
	void		_calculateDiscriminant(std::vector<Token *> & list);
	void		_calculateX(void);
	void		_calculate2solutions(void);
	void		_calculate1solution(void);
	void		_calculateImaginarySolution(void);
	void		_displayComplexSolution(char op, double b, double iCoeff);
	void		_solveSimple(void);

	void		_moveTokensToLhs(std::vector<Token *> & lhs, std::vector<Token *> & rhs);
	void		_mergeTokens(std::vector<Token *> & list);
	void		_printReducedForm(std::vector<Token *> & lhs);


	void		_printLists(void);

    Computor (Computor const & ref);
    Computor & operator= (Computor const & rhs);
};

#endif
