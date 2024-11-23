/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircExceptions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 05:14:26 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 01:10:30 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEEPTIONS_HPP
#define IRCEXCEEPTIONS_HPP
#include <stdexcept>
#include <string>

namespace ircExceptions {

struct ERR_NOSUCHNICK : public std::runtime_error {
    ERR_NOSUCHNICK(std::string nickname)
        : std::runtime_error(":irc.1337.com 401 * " + nickname + " :No such nick/channel\r\n"){};
};

struct ERR_NOSUCHSERVER : public std::runtime_error {
    ERR_NOSUCHSERVER(std::string servername)
        : std::runtime_error(":irc.1337.com 402 * " + servername + " :No such server\r\n"){};
};

struct ERR_NOSUCHCHANNEL : public std::runtime_error {
    ERR_NOSUCHCHANNEL(std::string channel)
        : std::runtime_error(":irc.1337.com 403 * " + channel + " :No such channel\r\n"){};
};

struct ERR_CANNOTSENDTOCHAN : public std::runtime_error {
    ERR_CANNOTSENDTOCHAN(std::string channelname)
        : std::runtime_error(":irc.1337.com 404 * " + channelname +
                             " :Cannot send to channel\r\n"){};
};

struct ERR_TOOMANYCHANNELS : public std::runtime_error {
    ERR_TOOMANYCHANNELS(std::string channelname)
        : std::runtime_error(":irc.1337.com 405 * " + channelname +
                             " :You have joined too many channels\r\n"){};
};

struct ERR_WASNOSUCHNICK : public std::runtime_error {
    ERR_WASNOSUCHNICK(std::string nickname)
        : std::runtime_error(":irc.1337.com 406 * " + nickname +
                             " :There was no such nickname\r\n"){};
};

struct ERR_TOOMANYTARGETS : public std::runtime_error {
    ERR_TOOMANYTARGETS(std::string target)
        : std::runtime_error(
               ":irc.1337.com 407 * " + target +
              " :Too many recipients. Try using a distribution list\r\n"){};
};

struct ERR_NOORIGIN : public std::runtime_error {
    ERR_NOORIGIN() : std::runtime_error(":irc.1337.com 409 * :No origin specified\r\n"){};
};

struct ERR_NORECIPIENT : public std::runtime_error {
    ERR_NORECIPIENT(std::string command)
        : std::runtime_error(":irc.1337.com 411 * " + command + " :No recipient given\r\n"){};
};

struct ERR_NOTEXTTOSEND : public std::runtime_error {
    ERR_NOTEXTTOSEND(std::string command)
        : std::runtime_error(":irc.1337.com 412 * " + command + " :No text to send\r\n"){};
};

struct ERR_NOTOPLEVEL : public std::runtime_error {
    ERR_NOTOPLEVEL(std::string mask)
        : std::runtime_error(":irc.1337.com 413 * " + mask +
                             " :No top-level domain specified\r\n"){};
};

struct ERR_WILDTOPLEVEL : public std::runtime_error {
    ERR_WILDTOPLEVEL(std::string mask)
        : std::runtime_error(":irc.1337.com 414 * " + mask +
                             " :Wildcard in top-level domain\r\n"){};
};

struct ERR_UNKNOWNCOMMAND : public std::runtime_error {
    ERR_UNKNOWNCOMMAND(std::string command)
        : std::runtime_error(":irc.1337.com 421 * " + command + " :Unknown command\r\n"){};
};

struct ERR_NOMOTD : public std::runtime_error {
    ERR_NOMOTD() : std::runtime_error(":irc.1337.com 422 * :MOTD file is missing\r\n"){};
};

struct ERR_NOADMININFO : public std::runtime_error {
    ERR_NOADMININFO(std::string servername)
        : std::runtime_error(":irc.1337.com 423 * " + servername +
                             " :No administrative info available\r\n"){};
};

struct ERR_FILEERROR : public std::runtime_error {
    ERR_FILEERROR(std::string file)
        : std::runtime_error(":irc.1337.com 424 * " + file +
                             " :File error doing the operation\r\n"){};
};

struct ERR_NONICKNAMEGIVEN : public std::runtime_error {
    ERR_NONICKNAMEGIVEN() : std::runtime_error(":irc.1337.com 431 * :No nickname given\r\n"){};
};

struct ERR_ERRONEUSNICKNAME : public std::runtime_error {
    ERR_ERRONEUSNICKNAME(std::string nickname)
        : std::runtime_error(":irc.1337.com 432 * " + nickname + " :Erroneous nickname\r\n"){};
};

struct ERR_NICKNAMEINUSE : public std::runtime_error {
    ERR_NICKNAMEINUSE(std::string nickname)
        : std::runtime_error(":irc.1337.com 433 * " + nickname +
                             " :Nickname is already in use\r\n"){};
};

struct ERR_NICKCOLLISION : public std::runtime_error {
    ERR_NICKCOLLISION(std::string nickname)
        : std::runtime_error(":irc.1337.com 436 * " + nickname +
                             " :Nickname collision KILL\r\n"){};
};

struct ERR_UNAVAILRESOURCE : public std::runtime_error {
    ERR_UNAVAILRESOURCE(std::string resource)
        : std::runtime_error(":irc.1337.com 437 * " + resource +
                             " :Nick/channel is temporarily unavailable\r\n"){};
};

struct ERR_USERNOTINCHANNEL : public std::runtime_error {
    ERR_USERNOTINCHANNEL(std::string nickname, std::string channel)
        : std::runtime_error(":irc.1337.com 441 * " + nickname + " " + channel +
                             " :They aren't on that channel\r\n"){};
};

struct ERR_NOTONCHANNEL : public std::runtime_error {
    ERR_NOTONCHANNEL(std::string channel)
        : std::runtime_error(":irc.1337.com 442 * " + channel +
                             " :You're not on that channel\r\n"){};
};

struct ERR_USERONCHANNEL : public std::runtime_error {
    ERR_USERONCHANNEL(std::string user, std::string channel)
        : std::runtime_error(":irc.1337.com 443 * " + user + " " + channel +
                             " :is already on channel\r\n"){};
};

struct ERR_NOLOGIN : public std::runtime_error {
    ERR_NOLOGIN(std::string user)
        : std::runtime_error(":irc.1337.com 444 * " + user + " :User not logged in\r\n"){};
};

struct ERR_NOTREGISTERED : public std::runtime_error {
    ERR_NOTREGISTERED()
        : std::runtime_error(":irc.1337.com 451 * :You have not registered\r\n"){};
};

struct ERR_NEEDMOREPARAMS : public std::runtime_error {
    ERR_NEEDMOREPARAMS(std::string command)
        : std::runtime_error(":irc.1337.com 461 * " + command + " :Not enough parameters\r\n"){};
};

struct ERR_ALREADYREGISTRED : public std::runtime_error {
    ERR_ALREADYREGISTRED()
        : std::runtime_error(":irc.1337.com 462 * ::You may not reregister\r\n"){};
};

struct ERR_NOPERMFORHOST : public std::runtime_error {
    ERR_NOPERMFORHOST()
        : std::runtime_error(":irc.1337.com 463 * :Your host isn't among the privileged\r\n"){};
};

struct ERR_PASSWDMISMATCH : public std::runtime_error {
    ERR_PASSWDMISMATCH() : std::runtime_error(":irc.1337.com 464 * :Password incorrect\r\n"){};
};

struct ERR_YOUREBANNEDCREEP : public std::runtime_error {
    ERR_YOUREBANNEDCREEP()
        : std::runtime_error(":irc.1337.com 465 * :You are banned from this server\r\n"){};
};

struct ERR_KEYSET : public std::runtime_error {
    ERR_KEYSET(std::string channel)
        : std::runtime_error(":irc.1337.com 467 * " + channel +
                             " :Channel key already set\r\n"){};
};

struct ERR_CHANNELISFULL : public std::runtime_error {
    ERR_CHANNELISFULL(std::string channel)
        : std::runtime_error(":irc.1337.com 471 * " + channel +
                             " :Cannot join channel (+l)\r\n"){};
};

struct ERR_UNKNOWNMODE : public std::runtime_error {
    ERR_UNKNOWNMODE(std::string mode)
        : std::runtime_error(":irc.1337.com 472 * " + mode +
                             " :is unknown mode char to me\r\n"){};
};

struct ERR_INVITEONLYCHAN : public std::runtime_error {
    ERR_INVITEONLYCHAN(std::string channel)
        : std::runtime_error(":irc.1337.com 473 * " + channel +
                             " :Cannot join channel (+i)\r\n"){};
};

struct ERR_BANNEDFROMCHAN : public std::runtime_error {
    ERR_BANNEDFROMCHAN(std::string channel)
        : std::runtime_error(":irc.1337.com 474 * " + channel +
                             " :Cannot join channel (+b)\r\n"){};
};

struct ERR_BADCHANNELKEY : public std::runtime_error {
    ERR_BADCHANNELKEY(std::string channel)
        : std::runtime_error(":irc.1337.com 475 * " + channel +
                             " :Cannot join channel (+k)\r\n"){};
};

struct ERR_BADCHANMASK : public std::runtime_error {
    ERR_BADCHANMASK(std::string mask)
        : std::runtime_error(":irc.1337.com 476 * " + mask + " :Bad channel mask\r\n"){};
};

struct ERR_NOCHANMODES : public std::runtime_error {
    ERR_NOCHANMODES(std::string channel)
        : std::runtime_error(":irc.1337.com 477 * " + channel +
                             " :Channel doesn't support modes\r\n"){};
};

struct ERR_BANLISTFULL : public std::runtime_error {
    ERR_BANLISTFULL(std::string channel)
        : std::runtime_error(":irc.1337.com 478 * " + channel +
                             " :Channel ban/ignore list is full\r\n"){};
};

struct ERR_NOPRIVILEGES : public std::runtime_error {
    ERR_NOPRIVILEGES()
        : std::runtime_error(
               ":irc.1337.com 481 * :Permission Denied- You're not an IRC operator\r\n"){};
};

struct ERR_CHANOPRIVSNEEDED : public std::runtime_error {
    ERR_CHANOPRIVSNEEDED(std::string channel)
        : std::runtime_error(":irc.1337.com 482 * " + channel +
                             " :You're not channel operator\r\n"){};
};


struct ERR_CANTKILLSERVER : public std::runtime_error {
    ERR_CANTKILLSERVER()
        : std::runtime_error(":irc.1337.com 483 * :You can't kill a server!\r\n"){};
};



struct ERR_RESTRICTED : public std::runtime_error {
    ERR_RESTRICTED()
        : std::runtime_error(":irc.1337.com 484 * :Your connection is restricted!\r\n"){};
};

struct ERR_UNIQOPPRIVSNEEDED : public std::runtime_error {
    ERR_UNIQOPPRIVSNEEDED(std::string channel)
        : std::runtime_error(":irc.1337.com 485 * " + channel +
                             " :You're not the original channel operator\r\n"){};
};

struct ERR_NOOPERHOST : public std::runtime_error {
    ERR_NOOPERHOST() : std::runtime_error(":irc.1337.com 491 * :No O-lines for your host\r\n"){};
};

struct ERR_ISOPERLCHAN : public std::runtime_error {
    ERR_ISOPERLCHAN(std::string channel)
        : std::runtime_error(
               ":irc.1337.com 498 * " + channel +
              " :Channel is permanently locked and unavailable\r\n"){};
};

struct ERR_CHANOWNPRIVNEEDED : public std::runtime_error {
    ERR_CHANOWNPRIVNEEDED(std::string channel)
        : std::runtime_error(":irc.1337.com 499 * " + channel +
                             " :You're not the channel owner\r\n"){};
};

struct ERR_UMODEUNKNOWNFLAG : public std::runtime_error {
    ERR_UMODEUNKNOWNFLAG() : std::runtime_error(":irc.1337.com 501 * :Unknown MODE flag\r\n"){};
};

struct ERR_USERSDONTMATCH : public std::runtime_error {
    ERR_USERSDONTMATCH()
        : std::runtime_error(":irc.1337.com 502 * :Cannot change mode for other users\r\n"){};
};

struct ERR_PASSNEEDED: public std::runtime_error {
    ERR_PASSNEEDED()
        : std::runtime_error(":irc.1337.com 700 * :Please enter the password first\r\n"){};
};

};

#endif