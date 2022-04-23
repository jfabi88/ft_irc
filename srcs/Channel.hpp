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

# define MODES "opsitnmlbvk"
# define AMODES "olbvk"

/*Prefix
    Founder "~" "+q"
    Protected "&" "+a"
    Operator "@" "+o"
    Halfop "%" "+h"
    Voice "+" "+v"
*/

# define O 1
# define V 2

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
        typedef std::vector<Client *>                                       usr_c_list;
        typedef std::pair<int, Client *>                                    usr_pair;
        typedef std::vector<std::pair<int, Client *> >                      usr_pair_list;
        typedef std::vector<std::pair<int, Client *> >::const_iterator      usr_pos;

        Channel(std::string chName, std::string key);
        ~Channel();

        //* ################# GETTERS #################

        int                     getNClient() const;
        int                     getLimit() const;
        std::string             getName() const;
        std::string	            getTopic() const;
        std::string             getModes() const;
        std::string             getAllMessages() const;
        std::string             getMaxPrefix(std::string client);
        Client                  *getClient(int fd) const;
        Client                  *getClient(std::string name) const;
        usr_pair_list           getClients();
        std::vector<Client *>   getOperator();
        usr_pos                 getFirstClient() const;
        usr_pos                 getLastClient() const;
        usr_pair                getPairClient(std::string name);
        std::string             getTopicSet() const;
        std::string             getTopicTime() const;
        
        //* ################# OPERATIONS #################

        int		addClient(Client *client, std::string password, char letter);
        int     addClient(Client *client, char letter);
        int     addInvitedClient(Client *client);
        int     removeInvitedClient(Client *client);
        int		removeClient(std::string CNick);
        int		removeClient(int fd);
        int     setMode(char m, int flag);
        void    setKey(std::string key, int flag);
        void	setOperator(std::string client, int flag);
        void    setModeratorPermission(std::string client, int flag);
        void    setLimit(int limit);
        void    addMessage(std::string message);
        int     setBanMask(std::string mask, int flag);
        void	setTopic(std::string topic);
        int		sendToAll(std::string text);
        int     sendToAll(std::string text, Client *client);
        void    setTopicSetter(std::string setter);
        void    setTopicTime();
        void    emptyInvited();

        std::string ltop(int c);
        //* ################# CHECKS #################

        int		isBanned(std::string CNick, std::string CUser, std::string CReal);
        int     isOnChannel(std::string nickname);
        int     isInvited(std::string cNick);
        int     clientHasMode(std::string CNick, char c);
        int     checkBanMask(std::string banMask);
        int		hasMode(char m);

    private:
        std::string                 _chName;
        std::string                 _chKey;
        std::string                 _topic;
        usr_pair_list               _clients;
        usr_c_list                  _invitedClients;
        int                         _chMode;
        int	                        _clientLimit;
        int	                        _clientNumber;
        std::vector<std::string>    _banMask;
        std::vector<std::string>    _messages;
        std::string                 _topicSetter;
        std::string                 _topicTime;

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
