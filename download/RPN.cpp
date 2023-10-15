/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraidry <edraidry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 10:07:35 by edraidry          #+#    #+#             */
/*   Updated: 2023/10/06 18:34:15 by edraidry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"RPN.hpp"

void split( std::string &input ,char delimiter,std::string &token)
{
    // std::string token;
    for(size_t i = 0;i<input.length();i++)
    {
        if(input[i] != delimiter)
        {
            token += input[i];
        }
        // else
        // {
        //     tokens.push_back(token);
        //     // token.clear();
        // }
    }
    // if(!token.empty())
    // {
    //     tokens.push_back(token);
    // }
    std::cout << token << "\n";
    // return tokens;
}

// bool isnumandOper( std::string &token)
// {
//     for( size_t i = 0; i < token.size(); i++)
//     {
//         std::string str = token[i];
//         if(str.length() == 1)
//         {
//             char ch = token[i][0];
//             if(!(ch >= '0' && ch <= '9')  && ch != '+' && ch != '*' && ch != '/' && ch != '-' )
//                 return false;
//         }
//         else
//             return false;
//     }
//     return true;
// }

void evaluateRPN( std::string &token)
{
    std::list<int> operandlist;
    //size_t i = 0;
    // if(!isnumandOper(token))
    // {
    //     std::cout << "Error character " << std::endl;
    //     exit(0);
    // }
    if ( token.size() == 1)
    {
        std::cerr << "Error" << std::endl;
        exit(1);
        
    }
    for( size_t i = 0; i < token.size();i++)
    {
        
        char ch = token[i];
        if(isdigit(ch))
            operandlist.push_back(ch - 48); // here
        else
        {
            if(operandlist.size() < 2)
                throw std::runtime_error("Error: not enough operator");
            int num1 = operandlist.back();
            operandlist.pop_back(); 
            int num2 = operandlist.back();
            operandlist.pop_back();
            if(ch == '+')
                operandlist.push_back(num2 + num1);
            else if(ch == '-')
                 operandlist.push_back(num2 - num1);
            else if(ch == '*')
                    operandlist.push_back(num2 * num1);
            else if(ch == '/')
            {
                if(num2 == 0)
                    throw std::runtime_error("Error :division by zero");
                operandlist.push_back(num2 / num1);
            }
        }
    }
    if(operandlist.size() > 1) // here
        throw std::runtime_error("Error: not enough operator");
    std::cout <<operandlist.front() <<std::endl;
}