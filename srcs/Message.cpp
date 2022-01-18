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

Message::Message() : \
    _prefix(""), _command(""), _text(""), _lastParameter("") {
        std::cout << "Message Default Constructor called (empty message)" << std::endl;
}

Message::Message(const Message &copy) : \
    _prefix(copy.getPrefix()), _command(copy.getCommand()), _text(copy.getText()) {
        this->setMessage(_text);
        std::cout << "Message Copy Constructor called" << std::endl;
}

Message::Message(std::string _text) {
    this->setMessage(_text);
    this->_text = _text;
    std::cout << "Message Constructor called" << std::endl;
}

Message::~Message() {
    std::cout << "Message destructor called" << std::endl;
}

//* ################# GETTERS #################

std::string Message::getPrefix() const  { return (this->_prefix);  }
std::string Message::getCommand() const { return (this->_command); }

std::string Message::getParametersIndex(size_t i) const
{
    std::cout << "Parameters size: " << this->_parameters.size() << std::endl;
    return (i >= this->_parameters.size()) ? ("") : (this->_parameters[i]); 
}

//* ::smenna
//? Queste due sono strane, la seconda dovrebbe almeno teoricamente essere il setter della prima
std::string Message::getLastParameters() const { return (this->_lastParameter); }
std::string Message::getLastParameter() const
{
    std::vector<std::string>::const_iterator  it;

    it = this->_parameters.begin();
    if (it == this->_parameters.end())
        return ("");
    else
    {
        while (it + 1 < this->_parameters.end())
            it++;
    }
    return (*(it));
}

std::vector<std::string> Message::getParameters() const             { return (this->_parameters); }
std::vector<std::string> Message::getLastParameterMatrix() const    { return (ft_split(this->getLastParameters(), ' ')); }
std::string Message::getText() const                                { return (this->_text); }
int Message::getSize() const                                        { return (this->_parameters.size()); }

//* ################# SETTERS #################

void Message::setMessage(std::string _text)
{
    int last_pos = 0;
    int end;
    int add = 0;
    std::string tmp;

    this->_prefix = "";
    this->_text = _text;
    this->_lastParameter = "";
    this->_parameters.clear();
    if (_text == "")
        return ;
    if (_text[0] == ':')
        last_pos = _ft_set_element(_text, 0, &(this->_prefix));
    last_pos = _ft_set_element(_text, last_pos, &(this->_command));
    end = _text.find(" ", last_pos);
    while (end >= 0 && _text[last_pos] != ':')
    {
        last_pos = _ft_set_element(_text, last_pos, &(tmp));
        this->_parameters.push_back(tmp);
        end = _text.find(" ", last_pos);
    }
    end = _text.find(DEL, last_pos);
    if (end >= 0)
    {
        add = (_text[last_pos] == ':');
        if (add)
            _lastParameter = _text.substr(last_pos + add, end - (last_pos + add));
        this->_parameters.push_back(_text.substr(last_pos + add, end - (last_pos + add)));
    }
}

//* ################# EXTERNAL FUNCTIONS #################

std::ostream& operator<<(std::ostream& os, const Message &copy)
{
    std::vector<std::string>::iterator it;

    os << "_prefix: " << copy.getPrefix() << "\n";
    os << "_command: " << copy.getCommand() << "\n";
    os << "_parameters: ";
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
