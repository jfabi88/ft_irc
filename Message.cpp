/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:49:07 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/09 14:49:21 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/**CONSTRUCTOR**/

Message::Message()
{
    this->prefix = "";
    this->command = "";
    this->text = "";
    this->lastParameter = NULL;
    std::cout << "Message created" << std::endl;
}

Message::Message(const Message &copy)
{
    std::vector<std::string>::iterator it;

    this->prefix = copy.getPrefix();
    this->command = copy.getCommand();
    this->text = copy.getText();
    this->lastParameter = copy.getLastParameter();
    for (it = copy.getParameters().begin(); it < copy.getParameters().end(); it++)
        this->parameters.push_back(*it);
    std::cout << "Message created" << std::endl;
}

Message::Message(std::string text)
{
    std::cout << "Il messaggio dentro message: " << text << std::endl;
    this->setMessage(text);
    this->text = text;
    std::cout << "Message created" << std::endl;
}

Message::~Message()
{
    std::cout << "Message deleted" << std::endl;
}

/**GET-SET**/

std::string Message::getPrefix() const
{
    return (this->prefix);
}

std::string Message::getCommand() const
{
    return (this->command);
}

std::string Message::getParametersIndex(int i) const
{
    return (this->parameters[i]);
}

const char *Message::getLastParameter() const
{
    return (lastParameter);
}

std::vector<std::string> Message::getParameters() const
{
    return (this->parameters);
}

std::vector<std::string> ft_split(std::string text, char delimiter)
{
    std::vector<std::string>    ret;
    int i;
    int lastPosition;

    i = 0;
    lastPosition = -1;
    while (i < text.size() && text[i] == delimiter)
        i++;
    if (i < text.size())
        lastPosition = text.find(delimiter, i);
    while (lastPosition != -1)
    {
        ret.push_back(text.substr(i, lastPosition - i));
        i = lastPosition;
        while (i < text.size() && text[i] == delimiter)
            i++;
        if (i < text.size())
            lastPosition = text.find(delimiter, i);
    }
    if (lastPosition == -1)
        ret.push_back(text.substr(i, text.size() - i));
    return (ret);
}

std::vector<std::string> Message::getLastParameterMatrix() const
{
    return (ft_split(this->getLastParameter(), ' '));
}

std::string Message::getText() const
{
    return (this->text);
}

void Message::setMessage(std::string text)
{
    int last_pos = 0;
    int end;
    int i = 0;
    int add = 0;

    this->prefix = "";
    this->text = text;
    if (text == "")
        return ;
    if (text[0] == ':')
        last_pos = this->ft_set_element(text, 0, &(this->prefix));
    last_pos = this->ft_set_element(text, last_pos, &(this->command));
    std::cout << "Last command: " << last_pos << " " << text[last_pos] << std::endl;
    end = text.find(" ", last_pos);
    while (end >= 0 && text[last_pos] != ':')
    {
        this->parameters.push_back("");
        last_pos = this->ft_set_element(text, last_pos, &(this->parameters[i]));
        i++;
        end = text.find(" ", last_pos);
    }
    end = text.find(DEL, last_pos);
    if (end >= 0)
    {
        add = (text[last_pos] == ':');
        this->lastParameter = text.substr(last_pos + add, end - (last_pos + add)).c_str();
    }
}

Message &Message::operator=(const Message &copy)
{
    if (this == &copy)
        return (*this);
    this->prefix = copy.getPrefix();
    this->command = copy.getCommand();
    for (int i = 0; i < 15; i++)
        this->parameters[i] = copy.getParametersIndex(i);
    std::cout << "Message created" << std::endl;
    return (*this);
}

/**UTILS**/

int Message::ft_set_element(std::string text, int start, std::string *element)
{
    int next_pos;

    next_pos = text.find(" ", start);
    if (next_pos != -1)
    {
        *element = text.substr(start, next_pos - start);
        while (text[next_pos] == ' ')
            next_pos++;
    }
    else
    {
        std::cout << text << std::endl;
        next_pos = text.find(DEL, start);
        std::cout << "La sub: " << text.substr(start, next_pos) << std::endl;
        *element = text.substr(start, next_pos);
    }
    return (next_pos);
}

/**EXTERNAL-FUNCTIONS**/

std::ostream& operator<<(std::ostream& os, const Message &copy)
{
    os << "Prefix: " << copy.getPrefix() << "\n";
    os << "Command: " << copy.getCommand() << "\n";
    os << "Parameters:\n";
    for (int i = 0; i < 15; i++)
        os << i << ": " << copy.getParametersIndex(i) << "\n";
    return (os);
}
