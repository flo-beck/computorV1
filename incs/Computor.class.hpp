/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:07:06 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 19:18:59 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

#include <list>
#include "Token.class.hpp"

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
	std::list<Token *>		_tokensLhs;
	std::list<Token *>		_tokensRhs;
	//Parser						_parser;
	int						_polyDegree;
	int						_discriminant;

	//Token *		_createToken(void);
	void		_readInput(char *input);
	void		_reduceInput(std::list<Token *> inputL, std::list<Token *> inputR);
	void		_getPolynomialDegree(std::list<Token *> & list);
	void		_calculateDiscriminant(std::list<Token *> & list);


	void		_moveTokensToLhs(std::list<Token *> & lhs, std::list<Token *> & rhs);
	void		_mergeTokens(std::list<Token *> & list);
	void		_printReducedForm(std::list<Token *> & lhs);

};

#endif
