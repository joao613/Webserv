/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:28:15 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/17 16:11:17 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "utils.hpp"
#include "Server.hpp"

class ConfigFile {
private:
	std::string _filePath;
	std::string _content;
	size_t _nbrOfServers;
	std::vector<std::string> _serverStr;
	std::vector<Server> _serverObjs;
	std::vector<int> _ports;
	
	ConfigFile();
	bool alreadyExist(int);

public:
	ConfigFile(int argc, char **argv);
	ConfigFile(const ConfigFile& src);
	ConfigFile& operator=(const ConfigFile& src);
	~ConfigFile();

	void run();
	void createFile();
	
	const std::string& getContent() const;
	size_t getNbrOfServers() const;
	Server getServer(std::string serverName, int port) const;
	const std::vector<Server>& getServers() const;
	const std::vector<int>& getPorts() const;

	void setPorts();
	void clearDuplicateServers();
	void isConfigFilePath();
	void readingFile();
	void splitServers();
	Server fillServersObjs(std::string& serverStr, size_t serverId);

};

#endif // CONFIGFILE_HPP