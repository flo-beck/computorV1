/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/25 14:20:08 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/25 17:34:42 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>

class Token {

  public:

    Token (void);
    Token (Token const & ref);
    virtual ~Token (void);

    Token & operator= (Token const & rhs);

	void			setCoeff(double coeff);
	double			getCoeff(void) const;
	void			setNeg(bool n);
	bool			isNeg(void) const;
	void			setPower(int p);
	int				getPower(void) const;

	std::string		toString(void) const;

  private:

	double	_coeff;
	bool	_neg;
	int		_power;

};

std::ostream &      operator<<(std::ostream & o, Token const & rhs);

#endif
