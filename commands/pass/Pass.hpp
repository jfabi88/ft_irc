/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:47:16 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/15 14:47:18 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include "../ICommand.hpp"

class Pass : public ICommand
{
    public:
        Pass();
        ~Pass();

        int    exec(Message message, Client *client, Server server);
    private:
};

#endif

