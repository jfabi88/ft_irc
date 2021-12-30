/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 10:02:21 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/30 10:07:22 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define NOCHAR ", ˆG"
#define NOCHARSIZE 3

#include <iostream>
#include <vector>
#include "Client.hpp"

//Mode "ntsmkIileb//

#define B 1
#define E 2
#define L 4
#define I 8
#define II 16
#define K 32
#define M 64
#define S 128
#define T 256
#define N 512

class Channel
{
	public:
		Channel(std::string name, std::string key, Client *clientOperator);
		~Channel();

		std::string getName() const;
		Client *getClient(int fd) const;
		Client *getClient(std::string name) const;
		Client *getOperator() const;
		
		int		addClient(Client *client, std::string password);
		void	setOperator(Client client);
		void	setMode(std::string m, int flag);
		void	addBanned(std::string CNick, std::string cUser);
		int		isBanned(std::string CNick, std::string CUser);

		void	removeBanned(std::string CNick, std::string cUser);
		int		hasMode(std::string m);
	private:
		std::string name;
		std::string password;
		std::vector<Client *> listClient;
		Client *chop;
		std::vector<std::string> banned;
		int	mode;

		class WrongCharacter : public std::exception
        {
            public:
                const char* what() const throw();
        };

		int	ft_converter(std::string m);
		int	ft_test_name(std::string name);
};

#endif
