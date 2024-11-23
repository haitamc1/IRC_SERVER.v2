/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 14:17:21 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 17:28:41 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Parser.hpp"

std::pair<std::string, std::string::iterator> Parser::parse_argument(std::string::iterator it,
                                                std::string::iterator end) {
    std::string arg;
    if (it == end)
        arg = "";
    if (*it == ':') {
        ++it;
        for (; it != end; it++) {
            arg += *it;
        }
    } else {
        for (; it != end; it++) {
            if (*it == ' ') {
                if (arg.empty())
                    throw ircExceptions::ERR_UNKNOWNCOMMAND("");
                return make_pair(arg, ++it);
            }
            arg += *it;
        }
    }
    if (arg.empty())
        throw ircExceptions::ERR_UNKNOWNCOMMAND("");
    return make_pair(arg, it);
}

Parser::Command Parser::operator()(std::string str) {
    Command cmd;
    cmd.authenticate = NOT_REGISTRED;
	cmd.command = NOT_FOUND;
    std::pair<std::string, std::string::iterator> p =
        parse_argument(str.begin(), str.end());
    upperIt(p.first);
    if (p.first == "PASS") {
        cmd.authenticate = PASS;
        std::string arg = std::string(p.second, str.end());
        cmd.args.push_back(arg);
    }
    else if (p.first == "NICK") {
        cmd.authenticate = NICK;
		std::string nick;
        std::string arg = std::string(p.second, str.end());
		std::istringstream iss(arg);
		iss >> nick;
        cmd.args.push_back(nick);
    } 
    else if (p.first == "USER") {
        cmd.authenticate = USER;
        cmd.args = parse_user(p.second, str.end());
        if (cmd.args.size() < 4)
            throw ircExceptions::ERR_NEEDMOREPARAMS("USER");
    }
	else if (p.first == "PRIVMSG") {
        cmd.command = PRIVMSG;
        std::string param = std::string(p.second, str.end());
        cmd.args = parsePrivmsg(param);
    }
	else if (p.first == "JOIN"){
		cmd.command = JOIN;
		std::string param = std::string(p.second, str.end());
		cmd.args = parseMode(param, "JOIN");
		cmd.chan_key = parseChannel(cmd.args);
	}
	else if (p.first == "KICK"){
		cmd.command = KICK;
		cmd.kickarg = parseKick(std::string(p.second, str.end()));
	}
	else if (p.first == "INVITE"){
		cmd.command = INVITE;
		cmd.args = parseInvite(std::string(p.second, str.end()));
	}
	else if (p.first == "TOPIC"){
		cmd.command = TOPIC;
		cmd.args = parseTopic(std::string(p.second, str.end()));
	}
	else if (p.first == "MODE"){
		cmd.command = MODE;
		std::string param = std::string(p.second, str.end());
		cmd.args = parseMode(param, "MODE");
	}
	else if (p.first == "QUIT"){
		cmd.command = QUIT;
		std::string param = std::string(p.second, str.end());
		cmd.args.push_back(param);
	}
	else if (p.first == "PART"){
		cmd.command = PART;
        std::string param = std::string(p.second, str.end());
        cmd.args = parsePrivmsg(param);
	}
	else if (p.first == "BOT"){
		cmd.command = BOT;
		std::string param = std::string(p.second, str.end());
		cmd.args.push_back(param);
	}
	else
		cmd.args.push_back(p.first);
	for (size_t i = 0; i < cmd.args.size(); i++){
		std::cout<<"arg["<<i<<"]: "<<cmd.args[i]<<std::endl;
	}

    return cmd;
}


std::vector<std::string> Parser::parse_user(std::string::iterator it, 
                            std::string::iterator end)
{
    std::vector<std::string> args;
    std::string str = "";
    if (it == end)
            throw ircExceptions::ERR_NEEDMOREPARAMS("USER");
    while (it != end){
		str = "";
        if (*it == ':') {
            ++it;
            for (; it != end; it++)
                str += *it;
        } 
        else {
            for (; it != end; it++) {
                if (*it == ' ') {
					it++;
                    break;
                }
                str += *it;
            }   
        }
		if (str != "")
        	args.push_back(str);
    }
    return args;
}

std::vector<std::string> Parser	::	parsePrivmsg(std::string &s)
{
	std::string	receiver = "";
	std::string msg = "";
	size_t i = 0;
	std::vector<std::string> args;

	skipSpace(s, i);
	for (; i < s.size(); i++)
	{
		if (s[i] == ' ' || s[i] == ':')
			break;
		receiver += s[i];
	}
	skipSpace(s, i);
	if (receiver == "")
		ircExceptions::ERR_NORECIPIENT("PRIVMSG");
	else if (i == s.size())
		throw ircExceptions::ERR_NOTEXTTOSEND("PRIVMSG");
	args.push_back(receiver);
	if (s[i] == ':')
	{
		i++;
		skipSpace(s, i);
		if (i == s.size())
			throw ircExceptions::ERR_NOTEXTTOSEND("PRIVMSG");
		msg = s.substr(i, s.size());
	}
	else{
		skipSpace(s, i);
		if (i == s.size())
			throw ircExceptions::ERR_NOTEXTTOSEND("PRIVMSG");
		for(; i < s.size() && s[i] != ' '; i++)
		{
			if (s[i] == ':')
				throw ircExceptions::ERR_TOOMANYTARGETS("PRIVMSG");
			msg += s[i];
		}
	}
	args.push_back(msg);
	return args;
}

std::vector<std::string>	Parser	::	parseMode(std::string &s, const std::string cmd)
{
	std::vector<std::string> prm;
	std::string tmp;
	size_t	pos;
	size_t i = 0;
	
	if (s == "")
		throw ircExceptions::ERR_NEEDMOREPARAMS(cmd);
	while (s.find(' ') != std::string::npos)
	{
		pos = s.find(' ');
		tmp = s.substr(0, pos);
		skipSpace(s, pos);
		s = s.substr(pos, s.size());
		if (tmp != "")
			prm.push_back(tmp);
	}
	tmp = "";
	skipSpace(s, i);
	for (; i < s.size() && s[i] != ' '; i++)
		tmp += s[i];
	if (tmp != "")
		prm.push_back(tmp);
	return prm;
}

std::vector<std::pair<std::string, std::string> > Parser::parseChannel(std::vector<std::string> args) {

    std::vector<std::pair<std::string, std::string> > chan_key;
    std::vector<std::string> chan;
	
	std::string str = "";
    if (args[0].find(',') != std::string::npos){
		std::stringstream ss(args[0]);
		while (getline(ss, str, ',')) {
        	chan.push_back(str);
    	}
		str = "";
	}
    else
        chan.push_back(args[0]);
    for (size_t i = 0; i < chan.size(); i++) {
        	chan_key.push_back(std::make_pair(chan[i], ""));
    }
    if (args.size() == 2) {
        std::vector<std::string> key;
        if (args[1].find(',') != std::string::npos){
			std::stringstream s2(args[1]);
			while (getline(s2, str, ',')) {
        	key.push_back(str);
    	}
		}
        else
            key.push_back(args[1]);
        for (size_t i = 0; i < key.size() && key.size() <= chan.size(); i++)
            chan_key[i].second = key[i];
    }
    return chan_key;
}


// std::vector<std::string> parseKick(const std::string &command) {
//     std::istringstream iss(command);
//     std::string channels, comment,Users;
// 	std::vector<std::string> args;

//     iss >> channels;
// 	if (channels == "")
// 		throw ircExceptions::ERR_NEEDMOREPARAMS("KICK");
// 	args.push_back(channels);
//     iss >> Users;
// 	if (Users == "")
// 		throw ircExceptions::ERR_NEEDMOREPARAMS("KICK");
// 	args.push_back(Users);
	
//     size_t commentPos = command.find(':', Users.size());
//     if (commentPos != std::string::npos) {
//         comment = command.substr(commentPos + 1);
// 		args.push_back(comment);
//     }

//     // std::istringstream channelsUsersStream(channels);
//     // std::string channelUser;
//     // std::vector<std::string> channelsAndUsers;

//     // while (std::getline(channelsUsersStream, channelUser, ',')) {
// 	// 	std::cout<<channelUser<<std::endl;
//     //     channelsAndUsers.push_back(channelUser);
//     // }
// 	return args;
// }
 
std::vector<std::string> Parser::parseInvite(std::string command){
    std::istringstream iss(command);
    std::string channels, nickname;
	std::vector<std::string> args;

    iss >> nickname;
	if (nickname == "")
		throw ircExceptions::ERR_NEEDMOREPARAMS("INVITE");
	args.push_back(nickname);
    iss >> channels;
	if (channels == "")
		throw ircExceptions::ERR_NEEDMOREPARAMS("INVTE");
	args.push_back(channels);
	return args;
}

std::vector<std::string> chainedArg(std::string &s, size_t &i)
{
	std::vector<std::string>	chain;
	std::string	receiver;
	bool flag = true;

	for (; i < s.size(); i++)
	{
		receiver = "";
		for (; i < s.size() && s[i] != ',' && flag; i++)
		{
			if (s[i] == ' ' || s[i] == ':')
			{
				flag = false;
				i++;
				break;
			}
			receiver += s[i];
		}
		if (receiver !=  "")
			chain.push_back(receiver);
		if (!flag)
			break;
	}
	return chain;
}

kickArg Parser	::	parseKick(std::string s)
{
	kickArg	obj;
	size_t i = 0;

	obj.msg = "";
	skipSpace(s, i); 
	if (s[i] == ':')
		throw ircExceptions::ERR_NEEDMOREPARAMS("KICK");
	obj.channels = chainedArg(s, i);
	skipSpace(s, i);
	obj.users = chainedArg(s, i);
	skipSpace(s, i);
	if (i == s.size() || obj.users.size() == 0)
		throw ircExceptions::ERR_NEEDMOREPARAMS("KICK");
	else if (s[i] == ':')
		i++;
	skipSpace(s, i);
	if (i < s.size()){
		s = s.substr(i, s.size() - i);
		obj.msg = s;
	}
	return obj;
}

std::vector<std::string>	Parser	::	parseTopic(std::string s)
{
	std::string	channel = "";
	std::string msg = "";
	size_t i = 0;
	std::vector<std::string> args;

	skipSpace(s, i);
	for (; i < s.size(); i++)
	{
		if (s[i] == ' ' || s[i] == ':')
			break;
		channel += s[i];
	}
	skipSpace(s, i);
	if (channel == "")
		ircExceptions::ERR_NEEDMOREPARAMS("TOPIC");
	else if (i == s.size())
	{
		args.push_back(channel);
		return args;
	}
	args.push_back(channel);
	if (s[i] == ':'){
		i++;
		if (i == s.size())
			args.push_back("");
		else
		{
			msg = s.substr(i, s.size());
			args.push_back(msg);
		}
	}
	else{
		skipSpace(s, i);
		if (i == s.size())
			args.push_back("");
		else{
			msg = s.substr(i, lastSpace(s) - i + 1);
			args.push_back(msg);
		}
	}
	return args;
}