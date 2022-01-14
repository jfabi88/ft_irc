/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:29:16 by jfabi             #+#    #+#             */
/*   Updated: 2021/12/14 18:29:17 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int hasCapability(std::vector<std::string> array, std::string name)
{
    std::vector<std::string>::const_iterator  it;
    std::string tmp;

    it = array.begin();
    tmp = name;
    if (name != "" && name[0] == '-')
        tmp = name.substr(1, name.size());
    while (!(*it).compare(tmp) && it != array.end())
        it++;
    if ((*it) != "")
        return (1);
    return (0);
}

int hasCapabilities(std::vector<std::string> array, std::vector<std::string> prefix)
{
    std::vector<std::string>::iterator it;

    for (it = prefix.begin(); it != prefix.end(); it++)
    {
        if (!hasCapability(array, *it))
            return (0);
    }
    return (1);
}
