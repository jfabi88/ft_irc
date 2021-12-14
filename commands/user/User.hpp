/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:30:10 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/14 19:30:11 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "../ICommand.hpp"

class User : public ICommand
{
    public:
        User(Message message, Server server, Client client);
        ~User();

        void    exec();
    private:
};

#endif

