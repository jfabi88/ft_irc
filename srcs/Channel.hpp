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
# define NOCHAR ", "

# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <sstream>

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

# define Q 1
# define A 2
# define O 4
# define H 8
# define V 16

//* ::smenna
//? Mi sono permesso di effettuare alcune modifiche all'interfaccia, più precisamente
//? 1. Ho aggiunto un copy constructor/overload 
//? 2. Tutti gli oggetti non sono passati by value ma come const reference (per semplici ragioni di efficienza)
//? 3. Dove possibile ho aggiunto dei typedef che rendono più comprensibili i tipi con cui stiamo lavorando

//? Questo rappresenta un SINGOLO CLIENT connesso al canale
//! Questo è da rivedere, credo ci sia un modo migliore per implementare questo struct

class Client;

class Channel
{
    public:
        
        typedef std::vector<std::string>                                    usr_list;
        typedef std::pair<int, Client *>                                    usr_pair;
        typedef std::vector<std::pair<int, Client *> >                      usr_pair_list;
        typedef std::vector<std::pair<int, Client *> >::const_iterator      usr_pos;

        Channel(std::string chName, std::string chKey, Client *chOperator);
        ~Channel();

        //* ################# GETTERS #################

        int                     getNClient() const;
        std::string             getName() const;
        std::string	            getTopic() const;
        std::string             getModes() const;
        Client                  *getClient(int fd) const;
        Client                  *getClient(std::string name) const;
        std::vector<Client *>   getOperator();
        usr_pos                 getFirstClient() const;
        usr_pos                 getLastClient() const;
        usr_pair                getPairClient(std::string name) const;
        
        //* ################# OPERATIONS #################

        int		addClient(Client *client, std::string password, char letter);
        int		removeClient(std::string CNick);
        int		removeClient(int fd);
        int     setMode(char m, int flag);
        void    setKey(std::string key, int flag);
        void	setOperator(std::string client, int flag);
        void    setModeratorPermission(std::string client, int flag);
        int     setBanMask(std::string mask, int flag);
        void	setTopic(std::string topic);
        int		sendToAll(std::string text);

        //* ################# CHECKS #################

        int		isBanned(std::string CNick, std::string CUser, std::string CReal);
        int     isOnChannel(std::string nickname);
        int     clientHasMode(std::string CNick, char c);
        int		hasMode(char m);

    private:
        std::string                 _chName;
        std::string                 _chKey;
        std::string                 _topic;
        std::string                 _password;
        usr_pair_list               _clients;
        int                         _chMode;
        int	                        _clientLimit;
        int	                        _clientNumber;
        std::vector<std::string>    _banMask;

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

        int	ft_converter(char c);
        int	ft_client_converter(char c);
        int	checkChName(std::string name);
};

#endif
