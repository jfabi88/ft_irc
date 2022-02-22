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

# define DEL "\n"
# define DELSIZE 1
# define USERLEN 16
# define CHANLIMIT 2

//* ################# Message #################

int _ft_atoi(std::string text);
int _ft_set_element(std::string _text, int start, std::string *element);
std::vector<std::string> ft_split(std::string _text, char delimiter);


#endif