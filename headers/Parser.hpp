/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 14:12:47 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 17:22:35 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <utility>
#include "authenticate.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>

class Parser;

typedef	struct kickArg
{
	std::vector<std::string>	channels;
	std::vector<std::string>	users;
	std::string msg;
}	kickArg;

class Parser{
    
        
    public:
        typedef enum authentication {
            PASS,
            NICK,
            USER,
            NOT_REGISTRED
        } authentication;
        
        typedef enum CommandType {
            PRIVMSG,
			JOIN,
			KICK,
			INVITE,
			TOPIC,
			MODE,
            QUIT,
			BOT,
            PART,
            NOT_FOUND
        } CommandType;
        
        struct Command {
            CommandType command;
            authentication authenticate;
            std::vector<std::string> args;
            std::vector<std::pair<std::string, std::string> > chan_key;
			kickArg kickarg;
            Command() {}
        };

        Command operator()(std::string str);
    private:
        static std::pair<std::string, std::string::iterator> parse_argument( std::string::iterator it,
            std::string::iterator end);
        static std::vector<std::string> parse_user(std::string::iterator start, 
                            std::string::iterator end);
        static std::vector<std::pair<std::string, std::string> > parseChannel(std::vector<std::string> args);

		static std::vector<std::string>		parsePrivmsg(std::string &s);
		static std::vector<std::string>		parseMode(std::string &s, const std::string cmd);
		static	kickArg 					parseKick(std::string s);
		static	std::vector<std::string>	parseTopic(std::string s);
		static	std::vector<std::string>	parseInvite(std::string s);
};


// std::vector<std::string>	parseKick(const std::string &command);

#endif