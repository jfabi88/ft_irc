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

Pass::Pass(Message newmessage, Server newserver, Client newclient) : ICommand(newmessage, newserver, newclient)
{
    std::cout << "Pass created" << std::endl;
}

Pass::~Pass()
{
    std::cout << "Pass deleted" << std::endl;
}

void Pass::exec()
{
    RepliesCreator  reply;

    if (this->client.getRegistered())
        reply.makeErrorAlreadyRegistered(this->client);
    else if (this->message.getParametersIndex(0) == "")
        reply.makeErrorNeedMoreParams(this->client, "PASS");
    else
        this->client.setPassword(this->message.getParametersIndex(0));
}
