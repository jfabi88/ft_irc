/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:47:28 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/15 14:47:29 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"

Pass::Pass()
{
    this->command = "PASS";
    std::cout << "Pass created" << std::endl;
}

Pass::~Pass()
{
    std::cout << "Pass deleted" << std::endl;
}

void Pass::exec(Message message, Client client, Server server)
{
    RepliesCreator  reply;
    std::string     cNick;

    cNick = client.getNickname();
    if (client.getRegistered())
        reply.makeErrorAlreadyRegistered(cNick);
    else if (message.getParametersIndex(0) == "")
        reply.makeErrorNeedMoreParams(cNick, "PASS");
    else
        client.setPassword(message.getParametersIndex(0));
}
