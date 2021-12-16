/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:04:35 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/14 18:04:37 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "../ICommand.hpp"

class Nick : public ICommand
{
    public:
        Nick();
        ~Nick();

        void    exec(Message message, Client client, Server server);
    private:
};

#endif
