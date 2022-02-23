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
    _prefix(""), _command(""), _text(""), _lastParameter(""), _isLastParameter(false) {
        std::cout << "Message Default Constructor called (empty message)" << std::endl;
}

Message::Message(const Message &copy) : _parameters(copy.getParametersBegin(), copy.getParametersEnd())
{
    this->_prefix = copy.getPrefix();
    this->_command = copy.getCommand();
    this->_text = copy.getText();
    this->_lastParameter = copy.getLastParameter();
    this->_isLastParameter = copy.getIsLastParameter();
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

std::string Message::getParametersIndex(size_t i) const {
    return (i >= this->_parameters.size()) ? ("") : (this->_parameters[i]); 
}

bool Message::getIsLastParameter() const { return (this->_isLastParameter); }

//* ::smenna
//? Queste due sono strane, la seconda dovrebbe almeno teoricamente essere il setter della prima
std::string Message::getLastParameter() const { return (this->_lastParameter); }

std::vector<std::string> Message::getParameters()                   { return (this->_parameters); }
std::vector<std::string> Message::getLastParameterMatrix()
{
    std::vector<std::string>    ret;

    ret = ft_split(this->getLastParameter(), ' ');
    return (ret);
}

std::vector<std::string>::const_iterator    Message::getParametersBegin() const { return (this->_parameters.begin()); };
std::vector<std::string>::const_iterator    Message::getParametersEnd() const { return (this->_parameters.end()); };
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
    this->_isLastParameter = false;
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
    if (end != last_pos)
    {
        add = (_text[last_pos] == ':');
        if (add)
        {
            std::cout << "Qui abbiamo settato last parameter" << std::endl;
            _lastParameter = _text.substr(last_pos + add, end - (last_pos + add));
            _isLastParameter = true;
        }
        this->_parameters.push_back(_text.substr(last_pos + add, end - (last_pos + add)));
    }
}

//* ################# EXTERNAL FUNCTIONS #################

std::ostream& operator<<(std::ostream& os, const Message &copy)
{
    std::vector<std::string>::const_iterator  it;

    os << "_prefix: " << copy.getPrefix() << "\n";
    os << "_command: " << copy.getCommand() << "\n";
    os << "_parameters: ";
    it = copy.getParametersBegin();
    if (it < copy.getParametersEnd())
    {
        os << *it; 
        it++;
    }
    while (it < copy.getParametersEnd())
    {
        os << ", " << *it; 
        it++;
    }
    os << "\n";
    os << "_lastparameter: " << copy.getLastParameter() << "\n";
    os << "Is the _last parameter setted? " << copy.getIsLastParameter() << "\n";
    return (os);
}
