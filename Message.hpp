/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:45:06 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/09 14:45:40 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>

class Message
{
    public:
        Message();
        Message(std::string text);
        Message(const Message &copy);
        ~Message();

        Message &operator=(const Message &copy);
        std::string getPrefix() const;
        std::string getCommand() const;
        std::string getParametersIndex(int indx) const;
        void setMessage(std::string text);
    private:
        std::string prefix;
        std::string command;
        std::string parameters[15];
        int         ft_set_element(std::string text, int start, std::string *element);
};

std::ostream& operator<<(std::ostream& os, const Message &copy);

#endif
