/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 05:09:26 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/04 20:55:14 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "ircExceptions.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <set>

struct Channel {
    std::string name;
    std::string topic;
    std::string key;
    std::vector<std::string> users;
    std::set<std::string> admins;
    std::set<std::string>   guest;
    bool Invite_only;
    bool is_limited;
    bool restrictedTopic;
    size_t limit;
};

class ChannelsHandler {
   private:
    std::map<std::string, Channel*> Chlist;

   public:
    // Channels();
    ~ChannelsHandler();
    void Create_Channel(const std::string& str, const std::string& key);
    bool Joining_Channel(const std::string& Channelname,
                         const std::string& user,
                         const std::string& key);
    void Leaving_Channel(const std::string& Channelname,
                         const std::string& user);

    const std::map<std::string, Channel*>& get_List();
    const Channel* get_Channel(std::string Channelname);
    const std::vector<std::string>& get_Users(std::string Channelname);

    std::string get_topic(std::string Channelname);
    std::string get_key(std::string Channelname);

    bool is_channel_existing(std::string Channelname);
    bool is_channel_member(std::string Channelname, std::string user);

    void set_topic(std::string T, std::string Channelname, std::string user);

    void set_key(std::string K, std::string Channelname);
    void remove_key(std::string Channelname);

    bool is_channel_admin(std::string Channelname, std::string user);

    void add_channel_admin(std::string Channelname, std::string user);

    void add_channel_guest(std::string Channelname, std::string user);
    
    void remove_channel_admin(std::string Channelname, std::string user);

    void set_Invite_only(std::string Channelname);
    void remove_Invite_only(std::string Channelname);

    void set_limit(std::string Channelname, std::string limit);
    void remove_limit(std::string Channelname);

    void set_restrictedTopic(std::string Channelname);
    void remove_restrictedTopic(std::string Channelname);

    void    check_channel(std:: string Channelname, std::string user);
    void    check_admin(std::string Channelname, std::string admin);

    void    update_user_nickname(std::string& Channelname, std::string oldNick, std::string newNick);
    
    void check_channel_target(std::string Channelname, std::string user);
    
    std::string Current_mode(std::string channelname);

};

bool isStringDigits(const std::string& str);
#endif