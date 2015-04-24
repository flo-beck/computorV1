/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.class.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbeck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/24 20:07:06 by fbeck             #+#    #+#             */
/*   Updated: 2015/04/24 20:13:47 by fbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

class Computor {

  public:

    Computor (void);
    Computor (Computor const & ref);
    virtual ~Computor (void);

    Computor & operator= (Computor const & rhs);

	void       compute(char **av);

  private:


};

#endif
