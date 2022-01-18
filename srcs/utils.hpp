/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:29:26 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/14 18:29:31 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//* ::smenna
//? Utils.hpp è stato reworkato, ora contiene i defines ed alcune funzioni che non appartengono a nessuna classe 
//? ma che vengono utilizzate da alcuni metodi

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <vector>

//* ################# DEFINES #################

# define DEL "\r\n"
# define DELSIZE 2
# define USERLEN 16
# define CHANLIMIT 2

//* ################# Message #################

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
    int                         lastPosition;

    i = 0;
    lastPosition = -1;
    while (i < _text.size() && _text[i] == delimiter)
        i++;
    if (i < _text.size())
        lastPosition = _text.find(delimiter, i);
    while (lastPosition != -1)
    {
        ret.push_back(_text.substr(i, lastPosition - i));
        i = lastPosition;
        while (i < _text.size() && _text[i] == delimiter)
            i++;
        if (i < _text.size())
            lastPosition = _text.find(delimiter, i);
    }
    if (lastPosition == -1)
        ret.push_back(_text.substr(i, _text.size() - i));
    return (ret);
}


#endif