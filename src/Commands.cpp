/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsarir <zsarir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 05:25:38 by zsarir            #+#    #+#             */
/*   Updated: 2023/09/07 19:16:12 by zsarir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include "../headers/server.hpp"
#include "../headers/ircExceptions.hpp"

#include "../headers/IRCReplies.hpp"

void server::PASS(Client& client, const Parser::Command& cmd){
		if (authenticate::checkPass(client, password ,cmd.args[0]) == 0)
			return ;
		if (client.getNickName() == "/Bot")
			_nickname_pool["/Bot"] = &client;
		client.setPassword(cmd.args[0]);
		client.setPass(true);
}


void server::USER(Client& client, const Parser::Command& cmd)
{
    	client.setUserName(cmd.args[0]);
		client.setservername(cmd.args[2]);
		client.setrealname(cmd.args[3]);
		std::cout << "hostname : " << client.getHostname() << std::endl;
}

void server::NICK(Client &client, const Parser::Command& cmd)
{
	if(authenticate::checkNick(_nickname_pool ,cmd.args[0])){
		if (client.getNickName() != ""){
			std::map<std::string, Client *>::iterator it;
            std::string oldNick = client.getNickName();
			it = _nickname_pool.find(oldNick);
			_nickname_pool.erase(it);
            std::vector<std::string> myChannel = client.get_myChannsList();
            for (size_t i =0 ; i < myChannel.size(); i++){
                channels.update_user_nickname(myChannel[i], oldNick, cmd.args[0]);
            }
		}
		client.setNickName(cmd.args[0]);
		_nickname_pool[cmd.args[0]] = &client;
	}
}

void	server::MODE(Client& client,  Parser::Command& cmd) {
    std::string admin = client.getNickName();
	size_t prmIndex = 2;
    channels.check_channel(cmd.args[0], admin);
    if (cmd.args.size() < 2){
        IRCReplies::RPL_CHANNELMODEIS(client, channels.Current_mode(cmd.args[0]), cmd.args[0]);
        return ;   
    }
    channels.check_admin(cmd.args[0], admin);
	if (cmd.args[1][0] == '-' || cmd.args[1][0] == '+')
	{
		for (size_t i = 1; i < cmd.args[1].size(); i++)
		{
			for (size_t j = i + 1; j < cmd.args[1].size(); j++)
				if (cmd.args[1][i] == cmd.args[1][j])
					cmd.args[1].erase(j, 1);
		}
	}
	if (cmd.args[1][0] == '-'){
		for (size_t i = 1; i < cmd.args[1].size(); i++){
			if (cmd.args[1][i] == 't')
				channels.remove_restrictedTopic(cmd.args[0]);
			else if (cmd.args[1][i] == 'o')
			{
				if (!(prmIndex < cmd.args.size()))
					throw ircExceptions::ERR_NEEDMOREPARAMS("MODE");
				channels.remove_channel_admin(cmd.args[0], cmd.args[prmIndex]);
				this->broadcastMessage(cmd.args[0], ":" + client.getNickName() + " MODE "+ cmd.args[0] +" -o " + cmd.args[prmIndex] + "\r\n", client.getNickName());
				client.send(":" + client.getNickName() + " MODE "+ cmd.args[0] +" -o " + cmd.args[prmIndex] + "\r\n");
				prmIndex++;
			}
			else if (cmd.args[1][i] == 'k')
				channels.remove_key(cmd.args[0]);
			else if (cmd.args[1][i] == 'i')
				channels.remove_Invite_only(cmd.args[0]);
			else if (cmd.args[1][i] == 'l')
				channels.remove_limit(cmd.args[0]);
			else
        		throw ircExceptions::ERR_UNKNOWNMODE(cmd.args[1]);
		}
	}
	else if (cmd.args[1][0] == '+'){
		for (size_t i = 1; i < cmd.args[1].size(); i++){
			if (cmd.args[1][i] == 't')
				channels.set_restrictedTopic(cmd.args[0]);
			else if (cmd.args[1][i] == 'o')
			{
				if (!(prmIndex < cmd.args.size()))
					throw ircExceptions::ERR_NEEDMOREPARAMS("MODE");
				channels.add_channel_admin(cmd.args[0], cmd.args[prmIndex]);
				this->broadcastMessage(cmd.args[0], ":" + client.getNickName() + " MODE "+ cmd.args[0] +" +o " + cmd.args[prmIndex] + "\r\n", client.getNickName());
				client.send(":" + client.getNickName() + " MODE "+ cmd.args[0] +" +o " + cmd.args[prmIndex] + "\r\n");
				prmIndex++;
			}
			else if (cmd.args[1][i] == 'k'){
				if (!(prmIndex < cmd.args.size()))
					throw ircExceptions::ERR_NEEDMOREPARAMS("MODE");
				channels.set_key(cmd.args[prmIndex++], cmd.args[0]);
			}
			else if (cmd.args[1][i] == 'i')
				channels.set_Invite_only(cmd.args[0]);
			else if (cmd.args[1][i] == 'l'){
				if (!(prmIndex < cmd.args.size()))
					throw ircExceptions::ERR_NEEDMOREPARAMS("MODE");
				channels.set_limit(cmd.args[0], cmd.args[prmIndex++]);
			}
			else
        		throw ircExceptions::ERR_UNKNOWNMODE(cmd.args[1]);
		}
	}
    else
        throw ircExceptions::ERR_UNKNOWNMODE(cmd.args[1]);
}

void server::JOIN(Client& client, const Parser::Command& cmd) {
    if (cmd.args[0] == "0") {
        std::vector<std::string> mychannels = client.get_myChannsList();
        for (size_t i = 0; i < mychannels.size(); i++)
            PART(mychannels, client);
        return;
    }
    for (size_t i = 0; i < cmd.chan_key.size(); i++) {
        try{
            if (channels.is_channel_existing(cmd.chan_key[i].first) == 0){
        	    if (cmd.chan_key[i].first[0] != '#')
        	     throw ircExceptions::ERR_NOSUCHCHANNEL(cmd.chan_key[i].first);
                channels.Create_Channel(cmd.chan_key[i].first,
                                    cmd.chan_key[i].second);
                 std::cout<<"channel created "<<cmd.chan_key[i].first<<std::endl;
            }
            client.add_myChannsList(cmd.chan_key[i].first);
            if (channels.Joining_Channel(cmd.chan_key[i].first, client.getNickName(),
                                             cmd.chan_key[i].second)) {
                    if (!channels.get_topic(cmd.chan_key[i].first).empty())
                        IRCReplies::RPL_TOPIC(
                            client, channels.get_Channel(cmd.chan_key[i].first));
                    RPL_NAMREPLY(client, cmd.chan_key[i].first, channels);
                    IRCReplies::RPL_ENDOFNAMES(client, cmd.chan_key[i].first);
                }
		}
		    catch (std::exception& e){
		    	client.send(e.what());
		    }
        }
}

void server::TOPIC(Client& client, const Parser::Command& cmd) {
    if (!channels.is_channel_existing(cmd.args[0]))
        throw ircExceptions::ERR_NOSUCHCHANNEL(cmd.args[0]);
    if (cmd.args.size() > 1)
        channels.set_topic(cmd.args[1], cmd.args[0], client.getNickName());
    if (channels.get_topic(cmd.args[0]).empty())
        IRCReplies::RPL_NOTOPIC(client, channels.get_Channel(cmd.args[0]));
    else
        IRCReplies::RPL_TOPIC(client, channels.get_Channel(cmd.args[0]));
}

void server::PART(std::vector<std::string> mychannel, Client& client) {
    for (size_t i = 0; i < mychannel.size(); i++) {
        channels.Leaving_Channel(mychannel[i], client.getNickName());
        client.remove_channel(mychannel[i]);
    }
}

void server::INVITE( Client& client,const Parser::Command& cmd) {
    channels.check_admin(cmd.args[1], client.getNickName());
     Client* cl =  get_client(cmd.args[0]);
        cl->send(":" + client.getNickName() + "!~@" + client.getUserName()+ "@"+ client.getHostname()+ " INVITE "+ cmd.args[0] + " " + cmd.args[1] +"\r\n");
        IRCReplies::RPL_INVITING(client, cmd.args[1], cmd.args[0]);
        channels.add_channel_guest(cmd.args[1], cmd.args[0]);
}

void server::KICK(Client& client,const Parser::Command& cmd) {
    std::string reason = ""; 
    if(cmd.kickarg.msg.empty() == 0)
        reason = cmd.kickarg.msg;
        // there MUST beeither one channel parameter and multiple user parameter, or as many
        // channel parameters as there are user parameters.
    if (cmd.kickarg.channels.size() != cmd.kickarg.users.size() && cmd.kickarg.channels.size()== 1)
        return ;
   for (size_t j = 0; j < cmd.kickarg.channels.size(); j++) {
        channels.check_admin(cmd.kickarg.channels[j], client.getNickName());
        for (size_t i =0 ; i < cmd.kickarg.users.size(); i++){
            channels.Leaving_Channel(cmd.kickarg.channels[j], get_client(cmd.kickarg.users[i])->getNickName());
            get_client(cmd.kickarg.users[i])->remove_channel(cmd.kickarg.channels[j]);
            get_client(cmd.kickarg.users[i])->send( ":" + client.getNickName() + "!~@"+ client.getHostname()
                    +" KICK "  +cmd.kickarg.channels[j]+ " " + cmd.kickarg.users[i] + "\r\n");
            std::vector<std::string> userList = this->channels.get_Users(cmd.kickarg.channels[j]);
            for (size_t k = 0; k < userList.size(); k++)
                get_client(userList[k])->send(":" + client.getNickName() + "!~@"+ client.getHostname()+" KICK " 
                    +cmd.kickarg.channels[j]+ " " + cmd.kickarg.users[i] +  "\r\n");
        }
	}
}

void server::broadcastMessage(const std::string& channelName,  const std::string& message, std::string sender) {
    std::vector<std::string> userList = this->channels.get_Users(channelName);
    for (size_t i = 0; i < userList.size(); i++) {
        if (userList[i] != sender)
            get_client(userList[i])->send(message);
 
    }
}

void ft_send(Client& client, std::vector<std::string> msg) {
    for (size_t i = 0; i < msg.size(); i++)
        client.send(msg[i] + " ");
    client.send("\n");
}

void server::PRIVMSG(Client& client, const Parser::Command& cmd)
{
    Client	*clt;


	std::string msg = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getHostname() + " PRIVMSG " +
										cmd.args[0] + " :" + cmd.args[1] + "\r\n";
	if (cmd.args[0][0] == '#'){
        channels.check_channel(cmd.args[0], client.getNickName());
		broadcastMessage(cmd.args[0], msg, client.getNickName());
    }
	else{
		clt = get_client(cmd.args[0]);
		clt->send(msg);
	}
}

void	server::BOT(Client &client, const Parser::Command& cmd)
{
	std::map<std::string, Client*>::iterator	it = _nickname_pool.find("/Bot");
	if (it == _nickname_pool.end())
		client.send(":irc.1337.com 712 No Bot is available at the moment\r\n");
	else
	{
		Client *clt = it->second;
		clt->send(client.getNickName() + " " +  cmd.args[0]);

	}
}