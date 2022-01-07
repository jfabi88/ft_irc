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
    this->lastParameter = "";
    std::cout << "Message vuoto created" << std::endl;
}

Message::Message(const Message &copy)
{
    this->prefix = copy.getPrefix();
    this->command = copy.getCommand();
    this->text = copy.getText();
    this->setMessage(text);
    std::cout << "Message copia created" << std::endl;
}

Message::Message(std::string text)
{
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
    int size;

    size = this->parameters.size();
    std::cout << size << std::endl;
    if (i >= size)
        return ("");
    return (this->parameters[i]);
}

std::string Message::getLastParameters() const
{
    return (this->lastParameter);
}

std::string Message::getLastParameter() const
{
    std::vector<std::string>::const_iterator  it;

    it = this->parameters.begin();
    if (it == this->parameters.end())
        return ("");
    else
    {
        while (it + 1 < this->parameters.end())
            it++;
    }
    return (*(it));
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
    return (ft_split(this->getLastParameters(), ' '));
}

std::string Message::getText() const
{
    return (this->text);
}

int Message::getSize() const
{
    return (this->parameters.size());
}

void Message::setMessage(std::string text)
{
    int last_pos = 0;
    int end;
    int add = 0;
    std::string tmp;

    this->prefix = "";
    this->text = text;
    this->lastParameter = "";
    this->parameters.clear();
    if (text == "")
        return ;
    if (text[0] == ':')
        last_pos = this->ft_set_element(text, 0, &(this->prefix));
    last_pos = this->ft_set_element(text, last_pos, &(this->command));
    end = text.find(" ", last_pos);
    while (end >= 0 && text[last_pos] != ':')
    {
        last_pos = this->ft_set_element(text, last_pos, &(tmp));
        this->parameters.push_back(tmp);
        end = text.find(" ", last_pos);
    }
    end = text.find(DEL, last_pos);
    if (end >= 0)
    {
        add = (text[last_pos] == ':');
        if (add)
            lastParameter = text.substr(last_pos + add, end - (last_pos + add));
        this->parameters.push_back(text.substr(last_pos + add, end - (last_pos + add)));
    }
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
        *element = text.substr(start, next_pos);
    }
    return (next_pos);
}

/**EXTERNAL-FUNCTIONS**/

std::ostream& operator<<(std::ostream& os, const Message &copy)
{
    std::vector<std::string>::iterator it;

    os << "Prefix: " << copy.getPrefix() << "\n";
    os << "Command: " << copy.getCommand() << "\n";
    os << "Parameters: ";
    it = copy.getParameters().begin();
    if (it < copy.getParameters().end())
    {
        std::cout << *it; 
        it++;
    }
    while (it < copy.getParameters().end())
    {
        std::cout << ", " << *it; 
        it++;
    }
    return (os);
}
