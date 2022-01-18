/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smenna <smenna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:29:26 by jfabi             #+#    #+#             */
/*   Updated: 2022/01/18 17:31:54 by smenna           ###   ########.fr       */
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
# define NOCHAR ", ˆG" //? Questi sono i caratteri che non possono essere contenuti nel nome del canale

//* ################# Message #################

int _ft_set_element(std::string _text, int start, std::string *element);
std::vector<std::string> ft_split(std::string _text, char delimiter);


#endif