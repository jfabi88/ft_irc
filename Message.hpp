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
#include <vector>

#define DEL "\n"
#define  DELSIZE 1

class Message
{
    public:
        Message();
        Message(std::string text);
        Message(const Message &copy);
        ~Message();

        std::string getPrefix() const;
        std::string getCommand() const;
        std::string getParametersIndex(int indx) const;
        std::string getLastParameters() const;
        std::string getLastParameter() const;
        std::vector<std::string> getParameters() const;
        std::vector<std::string> getLastParameterMatrix() const;
        std::string getText() const;
        int getSize() const;
        void setMessage(std::string text);
    private:
        std::string prefix;
        std::string command;
        std::vector<std::string> parameters;
        std::string text;
        std::string lastParameter;
        int         ft_set_element(std::string text, int start, std::string *element);
};

std::ostream& operator<<(std::ostream& os, const Message &copy);

#endif