/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 18:30:42 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/17 18:30:44 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPING_HPP
#define RPING_HPP

#include "../ICommand.hpp"

class RPing : public ICommand
{
    public:
        RPing();
        ~RPing();

        int    exec(Message message, Client *client, Server server);
    private:
};

#endif
