/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 15:58:56 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/28 15:59:24 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Message.hpp"
#include "Client.hpp"

class Server;

int execCap(Message message, Client *client, Server *server);
int execNick(Message message, Client *client, Server *server);
int execPass(Message message, Client *client);
int execRPing(Message message, Client *client);
int execSPing(Message message, Client *client);
int execPrivmsg(Message message, Client *client, Server *server);
int execUser(Message message, Client *client);

#endif
