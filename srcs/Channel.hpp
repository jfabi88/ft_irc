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
# define CHANNEL_HPP

//? Questi sono i caratteri che non possono essere contenuti nel nome del canale
# define NOCHAR ", ˆG"

# include <iostream>
# include <vector>
# include <sys/socket.h>

//Mode "ntsmkIileb//

# define B 1
# define E 2
# define L 4
# define I 8
# define II 16
# define K 32
# define M 64
# define S 128
# define T 256
# define N 512

/*Prefix
    Founder "~" "+q"
    Protected "&" "+a"
    Operator "@" "+o"
    Halfop "%" "+h"
    Voice "+" "+v"
*/

//* ::smenna
//? Mi sono permesso di effettuare alcune modifiche all'interfaccia, più precisamente
//? 1. Ho aggiunto un copy constructor/overload 
//? 2. Tutti gli oggetti non sono passati by value ma come const reference (per semplici ragioni di efficienza)
//? 3. Dove possibile ho aggiunto dei typedef che rendono più comprensibili i tipi con cui stiamo lavorando

//? Questo rappresenta un SINGOLO CLIENT connesso al canale
//! Questo è da rivedere, credo ci sia un modo migliore per implementare questo struct

class Client;

typedef struct PChannel
{
    char prefix;
    char modeLetter[2];
    Client *client;
}				t_PChannel;

class Channel
{
    public:
        
        typedef std::vector<std::string>                    usr_list;
        typedef std::vector<t_PChannel>::const_iterator     usr_pos;

        Channel(std::string chName, std::string chKey, Client *chOperator);
        ~Channel();

        //* ################# GETTERS #################

        std::string     getName() const;
        std::string	    getTopic() const;
        Client          *getClient(int fd) const;
        Client          *getClient(std::string name) const;
        Client          *getOperator() const;
        usr_pos         getFirstClient() const;
        usr_pos         getLastClient() const;
        t_PChannel      getT_PChannel(std::string name) const;
        char	        getSymbol() const; 
        
        //* ################# OPERATIONS #################

        int		addClient(Client *client, std::string password, char prefix, char letter);
        int		removeClient(std::string CNick);
        int		removeClient(int fd);
        void	setOperator(Client *client);
        void	setMode(std::string m, int flag);
        void	addBanned(std::string CNick, std::string cUser);
        void	setSymbol(char c);
        void	setTopic(std::string topic);
        int		sendToAll(std::string text);
        void	removeBanned(std::string CNick, std::string cUser);

        //* ################# CHECKS #################

        int		isBanned(std::string CNick, std::string CUser);
        int		hasMode(std::string m);

    private:
        std::string _chName;
        std::string _chKey;
        std::string	_topic;
        std::vector<t_PChannel> _clientList; //? Questa è la lista di client connessi a quel canale
        Client      *_chOperator;
        usr_list    _bannedClients; 
        int	        _chMode;
        int	        _clientLimit;
        int	        _clientNumber;
        char        _symbol; //? Questo symbol è necessario per le numeric replies

        class NoSuchChannel : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class WrongCharacter : public std::exception
        {
            public:
                const char* what() const throw();
        };

        int	ft_converter(std::string m);
        int	checkChName(std::string name);
};

#endif