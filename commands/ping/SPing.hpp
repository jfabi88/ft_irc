/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SPing.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 18:48:56 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/17 18:49:58 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPING_HPP
#define SPING_HPP

#include "../ICommand.hpp"

class SPing : public ICommand
{
	public:
        SPing();
        ~SPing();

        int    exec(Message message, Client *client, Server server);
	private:
};

#endif
