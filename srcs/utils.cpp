/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 16:37:15 by jfabi             #+#    #+#             */
/*   Updated: 2022/01/18 16:37:17 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

//* ################# Message #################

int _ft_atoi(std::string text)
{
    int ret;

    ret = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] >= 58 || text[i] <= 47)
            return (-1);
        if (ret + text[i] - 48 > 2147483647)
            return (-1);
        ret += text[i]  - 48;        
    }
    return (ret);
}

int _ft_set_element(std::string _text, int start, std::string *element)
{
    int next_pos;

    next_pos = _text.find(" ", start);
    if (next_pos != -1)
    {
        *element = _text.substr(start, next_pos - start);
        while (_text[next_pos] == ' ')
            next_pos++;
    }
    else
    {
        std::cout << _text << std::endl;
        next_pos = _text.find(DEL, start);
        *element = _text.substr(start, next_pos);
    }
    return (next_pos);
}

std::vector<std::string> ft_split(std::string _text, char delimiter)
{
    std::vector<std::string>    ret;
    size_t                      i;
    size_t                      lastPosition;

    i = 0;
    std::cout << "SIAMO dentro ft_split. Il valore di text é: " << _text << std::endl; 
    lastPosition = std::string::npos;
    while (i < _text.size() && _text[i] == delimiter)
        i++;
    if (i < _text.size())
        lastPosition = _text.find(delimiter, i);
    while (lastPosition != std::string::npos)
    {
        ret.push_back(_text.substr(i, lastPosition - i));
        i = lastPosition;
        while (i < _text.size() && _text[i] == delimiter)
            i++;
        if (i < _text.size())
            lastPosition = _text.find(delimiter, i);
    }
    if (lastPosition == std::string::npos && i != _text.size())
        ret.push_back(_text.substr(i, _text.size() - i));
    return (ret);
}
