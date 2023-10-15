/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraidry <edraidry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 10:07:41 by edraidry          #+#    #+#             */
/*   Updated: 2023/10/06 12:48:05 by edraidry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"RPN.hpp"

int main(int ac , char **av)
{
   try{ 
    
    if(ac != 2)
    {
        std::cout<<"Error in agrment"<<std::endl;
        return 1;
    }
    std::string rpnExpressino = av[1];
    char delimiter = ' ';
    std::string token;
    split(rpnExpressino,delimiter,token);
    evaluateRPN(token);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what()<<std::endl;
    }
   return 0;
}