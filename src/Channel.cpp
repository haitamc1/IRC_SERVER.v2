/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 05:09:33 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 15:35:44 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/ircExceptions.hpp"

void ChannelsHandler::Create_Channel(const std::string& str,
                                     const std::string& key) { 
    Channel* newchannel = new Channel;
    newchannel->name = str;
    newchannel->key = key;
    newchannel->Invite_only = 0;
    newchannel->is_limited = 0;
    newchannel->restrictedTopic = 0;
    newchannel->limit = 0;
    Chlist[str] = newchannel;
}

const std::map<std::string, Channel*>& ChannelsHandler::get_List() {
    return Chlist;
}

const Channel* ChannelsHandler::get_Channel(std::string Channelname) {
    return Chlist[Channelname];
}

const std::vector<std::string>& ChannelsHandler::get_Users(
    std::string Channelname) {
    return get_Channel(Channelname)->users;
}

bool ChannelsHandler::is_channel_existing(std::string Channelname) {
    std::map<std::string, Channel*>::iterator it = Chlist.find(Channelname);
    if (it != Chlist.end())
        return 1;
    return 0;
}

bool ChannelsHandler::is_channel_member(std::string Channelname,
                                        std::string user) {
    std::vector<std::string> myvec = Chlist[Channelname]->users;
    std::vector<std::string>::iterator it = myvec.begin();
    for (; it != myvec.end(); it++) {
        if (*it == user)
            return 1;
    }
    return 0;
}

ChannelsHandler::~ChannelsHandler() {
    for (std::map<std::string, Channel*>::iterator it = Chlist.begin();
         it != Chlist.end(); ++it) {
        delete it->second;
    }
    Chlist.clear();
}

std::string ChannelsHandler::get_topic(std::string Channelname) {
    if (Chlist[Channelname]->topic.empty())
        return ("");
    return Chlist[Channelname]->topic;
}

void ChannelsHandler::set_topic(std::string T,
                                std::string Channelname,
                                std::string user)
{
    check_channel(Channelname, user);
    if (Chlist[Channelname]->restrictedTopic == true){
        check_admin(Channelname, user);
        Chlist[Channelname]->topic = T;
    }
    else
        Chlist[Channelname]->topic = T;
}

void ChannelsHandler::set_key(std::string K, std::string Channelname) {
    if (Chlist[Channelname]->key != "")
        throw ircExceptions::ERR_KEYSET(Channelname);
    Chlist[Channelname]->key = K;
}

std::string ChannelsHandler::get_key(std::string Channelname) {
    return Chlist[Channelname]->key;
}

void ChannelsHandler::remove_key(std::string Channelname) {
    Chlist[Channelname]->key.replace(0, Chlist[Channelname]->key.length(),
                                     "");
}

void ChannelsHandler::set_Invite_only(std::string Channelname) {
    Chlist[Channelname]->Invite_only = 1;
}

void ChannelsHandler::remove_Invite_only(std::string Channelname) {
    Chlist[Channelname]->Invite_only = 0;
}

void ChannelsHandler::set_limit(std::string Channelname, std::string limit)
{
    if (!isStringDigits(limit))
        throw ircExceptions::ERR_UNKNOWNCOMMAND("MODE +l " + limit);
    else {
        Chlist[Channelname]->is_limited = true;
        std::stringstream ss;
        ss << limit;
        ss >> Chlist[Channelname]->limit;
    }
}

void ChannelsHandler::remove_limit(std::string Channelname) {
    if (!Chlist[Channelname]->is_limited)
        return;
    Chlist[Channelname]->is_limited = false;
    Chlist[Channelname]->limit = 0;
}

void ChannelsHandler::update_user_nickname(std::string& Channelname, std::string oldNick, std::string newNick)
{
    check_channel(Channelname, oldNick);
    std::vector<std::string>::iterator it = std::find(Chlist[Channelname]->users.begin(), Chlist[Channelname]->users.end(), oldNick);
    if (it != Chlist[Channelname]->users.end())
        Chlist[Channelname]->users.erase(it);
        Chlist[Channelname]->users.push_back(newNick);
    if (is_channel_admin(Channelname, oldNick))
    {
        std::set<std::string>::iterator iter = Chlist[Channelname]->admins.find(oldNick);
        if (iter != Chlist[Channelname]->admins.end())
            Chlist[Channelname]->admins.erase(iter);
        Chlist[Channelname]->admins.insert(newNick);
    }
    std::set<std::string>::iterator iter = Chlist[Channelname]->guest.find(oldNick);
    if (iter != Chlist[Channelname]->guest.end())
    {
        Chlist[Channelname]->guest.erase(iter);
        Chlist[Channelname]->guest.insert(newNick);
    }
}


std::string ChannelsHandler::Current_mode(std::string channelname)
{
    std::string mode = "+";
    std::vector<std::string> param;
    const Channel  *channel = get_Channel(channelname);
    
    if (get_key(channelname).empty() && !channel->Invite_only && !channel->is_limited
            && !channel->restrictedTopic && channel->admins.size() == 1)
            return "";
    if (channel->Invite_only)
        mode += "i";
    if  (!get_key(channelname).empty())
    {
        mode += "k";
        param.push_back(get_key(channelname));
    }
    if (channel->is_limited){
        mode += "l";
        std::stringstream ss;
        ss << channel->limit;
       param.push_back(ss.str());   
    }
    if (channel->admins.size() > 1){
        mode += "o";
        for (std::set<std::string>::iterator it = channel->admins.begin(); it != channel->admins.end(); ++it)
            param.push_back(*it);
    }
    if (channel->restrictedTopic)
        mode += "t";
    for (size_t i = 0; i < param.size(); i ++)
    {
        mode += " ";
        mode += param[i];
    }
    return mode;
}