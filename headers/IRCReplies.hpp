/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCReplies.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 05:14:34 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 18:55:40 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCREPLIES_HPP
#define IRCREPLIES_HPP
#pragma once

#include <stdexcept>
#include <string>
#include "Channel.hpp"
#include "client.hpp"
#include "server.hpp"

namespace IRCReplies {

inline void RPL_CHANNELMODEIS(Client& client, std::string mode, std::string channelname){
    client.send(":irc.1337.com 324 " + client.getNickName() + " "+ channelname + " " + mode +  "\r\n");
}

inline void RPL_TOPIC(Client& client, const Channel* channel) {
    client.send(":irc.1337.com 332 " + client.getNickName() + " " + channel->name + " :" + channel->topic + "\r\n");
}

inline void RPL_NOTOPIC(Client& client, const Channel* channel) {
    client.send(":irc.1337.com 331 " + client.getNickName() + " " + channel->name +
                        ": No topic is set\r\n");
}


inline void RPL_ENDOFNAMES(Client &client, std::string channelname){
   client.send(":irc.1337.com 366 "+ client.getNickName() + " " + channelname +" :End of /NAMES list\r\n");
}

inline void RPL_INVITING(Client& client,
                         std::string channelname,
                         std::string guest) {
    client.send(":irc.1337.com 341 " + client.getNickName() + " "  +channelname+ " " + guest + "\r\n");
};

inline void RPL_WELCOME(Client& client) {
    std::string str = ":irc.1337.com 001 "+ client.getNickName() + " :Welcome to the Internet Relay Network " + client.getNickName() + "!" 
                    + client.getUserName()+"@"+client.getHostname()+ "\r\n";
        client.send(str);
}


inline void RPL_YOURHOST(Client& client) {
    std::string str = ":irc.1337.com 002 Your host is irc.1337.com , running version rfc2812\r\n";
    client.send(str);
}

inline void RPL_CREATED(Client& client) {
    std::string str = ":irc.1337.com 003 This server was created 29-07-2023\r\n";
    client.send(str);
}

inline void RPL_MYINFO(Client& client) {
    std::string str = ":irc.1337.com 004 irc.1337.com rfc2812 <none> <itkol>\r\n";
    client.send(str);
}


};

#endif