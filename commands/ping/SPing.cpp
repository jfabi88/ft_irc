/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SPing.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 18:48:47 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/17 18:48:49 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SPing.hpp"

SPing::SPing()
{
    std::cout << "SPing created" << std::endl;
}

SPing::~SPing()
{
    std::cout << "SPing deleted" << std::endl;
}

int SPing::exec(Message message, Client *client, Server server)
{
    std::string text;

    text = "PING SiamoTuttiSimpatici";
    send(client->getSocketFd(), text.c_str(), text.size(), 0);
    return (0);
}
