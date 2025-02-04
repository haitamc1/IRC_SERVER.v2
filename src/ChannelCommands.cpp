/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 05:09:40 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 02:43:11 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../headers/Channel.hpp"

using std::string;

bool ChannelsHandler::Joining_Channel(const string& Channelname, const string& user, const string& k)
{
    std::string userkey = k;
    if (Chlist[Channelname]->Invite_only == 1)
    {
        if (Chlist[Channelname]->guest.find(user) == Chlist[Channelname]->guest.end())
            throw ircExceptions::ERR_INVITEONLYCHAN(Channelname);
        else
            userkey = Chlist[Channelname]->key;
    }
    if (is_channel_member(Channelname, user))
        throw ircExceptions::ERR_USERONCHANNEL(Channelname, user);
    if (Chlist[Channelname]->is_limited) {
        if (Chlist[Channelname]->users.size() + 1 >
            Chlist[Channelname]->limit) {
            throw ircExceptions::ERR_CHANNELISFULL(Channelname);
        }
    }
    if (Chlist[Channelname]->key == userkey)
    {
        Chlist[Channelname]->users.push_back(user);
        Chlist[Channelname]->admins.insert(Chlist[Channelname]->users[0]);
    }
    else
        throw ircExceptions::ERR_BADCHANNELKEY(Channelname);
    return 1;
}

void ChannelsHandler::Leaving_Channel(const string& Channelname, const string& user)
{
    check_channel_target(Channelname, user);
    std::vector<string>::iterator it = std::find(Chlist[Channelname]->users.begin(), Chlist[Channelname]->users.end(), user);
    if (it != Chlist[Channelname]->users.end())
        Chlist[Channelname]->users.erase(it);
    if (is_channel_admin(Channelname, user))
    {
        std::set<string>::iterator iter = Chlist[Channelname]->admins.find(user);
        if (iter != Chlist[Channelname]->admins.end())
            Chlist[Channelname]->admins.erase(iter);
        if (Chlist[Channelname]->admins.size()== 0 && Chlist[Channelname]->users.size() != 0)
             Chlist[Channelname]->admins.insert(Chlist[Channelname]->users[0]);
    }
}

void ChannelsHandler::check_channel(string Channelname, string user)
{
    if (is_channel_existing(Channelname) == 0)
        throw ircExceptions::ERR_NOSUCHCHANNEL(Channelname);
    if (is_channel_member(Channelname, user) == 0)
        throw ircExceptions::ERR_NOTONCHANNEL(Channelname);
}

void ChannelsHandler::check_channel_target(string Channelname, string user)
{
    if (is_channel_existing(Channelname) == 0)
        throw ircExceptions::ERR_NOSUCHCHANNEL(Channelname);
    if (is_channel_member(Channelname, user) == 0)
        throw ircExceptions::ERR_USERNOTINCHANNEL(user, Channelname);
}

void ChannelsHandler::check_admin(string Channelname, string admin)
{
    if (is_channel_existing(Channelname) == 0)
        throw ircExceptions::ERR_NOSUCHCHANNEL(Channelname);
    if (is_channel_member(Channelname, admin) == 0)
        throw ircExceptions::ERR_NOTONCHANNEL(Channelname);
    if (is_channel_admin(Channelname, admin) == 0)
        throw ircExceptions::ERR_CHANOPRIVSNEEDED(Channelname);
}

void ChannelsHandler::set_restrictedTopic(string Channelname) 
{
    Chlist[Channelname]->restrictedTopic = true;
}

void ChannelsHandler::remove_restrictedTopic(string Channelname)
{
    Chlist[Channelname]->restrictedTopic = false;
}

bool ChannelsHandler::is_channel_admin(string Channelname, string user) {  // chan exist
    std::set<string>::iterator it = Chlist[Channelname]->admins.find(user);
    if (it != Chlist[Channelname]->admins.end())
        return 1;
    return 0;
}

void ChannelsHandler::add_channel_admin(string Channelname, string user) {
    check_channel_target(Channelname, user);
    Chlist[Channelname]->admins.insert(user);
}

void ChannelsHandler::add_channel_guest(std::string Channelname, std::string user)
{
    Chlist[Channelname]->guest.insert(user);
}

void ChannelsHandler::remove_channel_admin(string Channelname,
                                           string user) {
    check_channel_target(Channelname, user);
    std::set<string>::iterator it = Chlist[Channelname]->admins.find(user);
    if (it != Chlist[Channelname]->admins.end())
        Chlist[Channelname]->admins.erase(it);
}

bool isStringDigits(const string& str) {
    for (int i = 0; str[i]; i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}
