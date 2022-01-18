/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smenna <smenna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:45:06 by jfabi             #+#    #+#             */
/*   Updated: 2022/01/18 15:46:41 by smenna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include <vector>

# include <utils.hpp>
class Message
{
    public:
        typedef std::vector<std::string>        str_list;

        Message();
        Message(std::string text);
        Message(const Message &copy);
        ~Message();

        //* ################# GETTERS #################

        std::string     getPrefix() const;
        std::string     getCommand() const;
        std::string     getParametersIndex(int indx) const;
        std::string     getLastParameters() const;
        std::string     getLastParameter() const;
        str_list        getParameters() const;
        str_list        getLastParameterMatrix() const;
        std::string     getText() const;
        int             getSize() const;

        //* ################# SETTERS #################

        void            setMessage(std::string text);

    private:

        std::string _prefix;
        std::string _command;
        std::string _text;
        std::string _lastParameter;
        str_list    _parameters;
};

std::ostream& operator<<(std::ostream& os, const Message &copy);

#endif
