/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfabi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 14:37:35 by jfabi             #+#    #+#             */
/*   Updated: 2022/01/05 14:37:37 by jfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"


int main(void)
{
    Message firstMessage;
    std::string firstTest;

    firstTest.append("PASS passsword");
    firstTest.append(DEL);
    firstMessage.setMessage(firstTest);
    std::cout << "Il messaggio é: " << firstTest << std::endl;
    std::cout << firstMessage << std::endl;
    std::cout << "---------------------"<< std::endl;
    firstTest = "";
    firstTest.append("USER gianni * * :localhost533 AS pp");
    firstTest.append(DEL);
    firstMessage.setMessage(firstTest);
    std::cout << "Il messaggio é: " << firstTest << std::endl;
    std::cout << firstMessage << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "TEST ft_split" << std::endl;
    std::cout << "Last parameter: " << firstMessage.getLastParameter() << std::endl;
    firstTest = "";
    firstTest.append("JOIN #channel");
    firstTest.append(DEL);
    firstMessage.setMessage(firstTest);
    std::cout << "Il messaggio é: " << firstTest << std::endl;
    std::cout << firstMessage << std::endl;
    std::cout << "---------------------"<< std::endl;
}
