/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:46:42 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/09 17:47:35 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>

class Channel;

class Client
{
    public:
        
        typedef std::vector<std::string>                        str_list;
        typedef std::vector<Channel *>::const_iterator          ch_iter;
        typedef std::vector<Channel *>                          ch_list;

        Client();
        Client(const Client &copy);
        ~Client();

        Client &operator=(const Client &copy);
        bool operator==(const Client &secondClient) const;
        bool operator!=(const Client &secondClient) const;
        //* ################# GETTERS #################

        std::string         getNickname() const;
        std::string         getPassword() const;
        std::string         getUsername() const;
        std::string         getRealname() const;
        Channel             *getChannel(int indx) const;
        Channel             *getChannel(std::string name) const;
        ch_iter             getFirstChannel() const;
        ch_iter             getLastChannel() const;
        str_list            getCapabilities() const;
        std::string         getAwayMessage() const;
        bool                getAwayStatus() const;
        bool                getRegisteredStatus() const;
        int                 getSocketFd() const;
        int                 getChannelSub() const;
        int                 getAccess() const;
        int                 getRecFlag() const;

        //* ################# SETTERS #################

        void                setNickname(std::string newname);
        void                setPassword(std::string newpassword);
        void                setUsername(std::string newusername);
        void                setRealname(std::string newrealname);
        void                addChannel(Channel *newChannel);
        void                removeChannel(std::string channelName);
        void                setCapabilities(str_list newVector);
        void                setAway(bool flag, std::string message);
        void                setRegistered(bool flag);
        void                setSocketFd(int fd);
        void                setAccess(int flag);
        void                setRecFlag(int flag);

        //* ################# CHECKS #################

        int                 hasCapability(std::string name) const;
        int                 hasCapabilities(str_list prefix) const;
        int                 hasMode(char c) const;
        int                 isOperator(Channel *channel) const;
    private:

        std::string         _nickname;
        std::string         _password;
        std::string         _username;
        std::string         _realname;
        std::string         _awayMessage;
        std::string         _mode;
        ch_list             _subChannels;
        str_list            _capabilities;
        int                 _subChannelsNum;
        int                 _socket;
        int                 _access;
        int                 _recFlag;
        bool                _isAway;
        bool                _isRegistered;
};

std::ostream& operator<<(std::ostream& os, const Client &copy);

#endif
