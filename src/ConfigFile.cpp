/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:30:33 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/17 16:55:59 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(int argc, char **argv)
{
	if (argc > 2)
		throw std::invalid_argument("Only one file is accepted.");
	if (argc == 1)
		createFile();
	argc == 1 ? _filePath = "configFileDefault.conf" :_filePath = argv[1];
}

ConfigFile::ConfigFile(const ConfigFile& src)
{
	_filePath = src._filePath;
}

ConfigFile& ConfigFile::operator=(const ConfigFile& src)
{
	if (this != &src)
		_filePath = src._filePath;
	return (*this);
}

ConfigFile::~ConfigFile(){}

void ConfigFile::createFile()
{
	std::ofstream file("configFileDefault.conf");

    if (!file)
        return ;

    file << "server {\n"
	"\tserver_name localhost;\n"
    "\tlisten 1234;\n"
	//"\troot ./;\n"
    "\tclient_body_size 1000000;\n"
	"\tindex html/index.html;\n"
    "\terror_page 404 error_pages/404.html;\n"
    "\terror_page 401 error_pages/401.html;\n"
	"\n"
    "\tlocation / {\n"
    "\t\tallow_methods GET DELETE POST;\n" 
    "\t\tautoindex off;\n"
    //"\t\troot ./;\n"
	"\t}\n"
    "}" << std::endl;

    file.close();
    return ;
}

void ConfigFile::run()
{
	this->isConfigFilePath();
	this->readingFile();
	this->splitServers();
	this->clearDuplicateServers();
	this->setPorts();
}

const std::string& ConfigFile::getContent() const
{
	return (_content);
} 

size_t ConfigFile::getNbrOfServers() const
{
	return (_nbrOfServers);
}

Server ConfigFile::getServer(std::string serverName, int port) const
{
	for (size_t i = 0; i < _serverObjs.size(); i++)
	{
		if (!serverName.compare(_serverObjs[i].getServerName()))
		{
			int aux = _serverObjs[i].getListenSize();
			for (int j = 0; j < aux; j++)
			{
				if (_serverObjs[i].getListen(j) == port)
					return (_serverObjs[i]);
			}
		}
	}
	// Server defaultServer;
	for (size_t i = 0; i < _serverObjs.size(); i++)
	{
		int aux = _serverObjs[i].getListenSize();
		for (int j = 0; j < aux; j++)
		{
			if (_serverObjs[i].getListen(j) == port)
				return (_serverObjs[i]);
		}
	}
	return (_serverObjs[0]); /* If no server_name matches the request, Nginx will use the first server block in the configuration file as the default server.  */
}

bool hasDifferentPort(std::vector<int> first, std::vector<int> second)
{
	if (first.size() != second.size())
		return (true);
	std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
	if (first == second)
		return (false);
	return (true);
}

void ConfigFile::clearDuplicateServers()
{
	std::vector<Server>::iterator it = _serverObjs.begin();
	while (it != _serverObjs.end())
	{
		std::string servername = it->getServerName();
		std::vector<Server>::iterator copy = _serverObjs.begin();
		while (copy != it)
		{
			if (copy->getServerName() == servername)
			{
				if (!hasDifferentPort(copy->getListenVec(), it->getListenVec())){
					it = _serverObjs.erase(it);
					it = _serverObjs.begin();
					break ;
				}
				++copy;
			}
			else
				++copy;
		}
		++it;
	}
}

void ConfigFile::isConfigFilePath()
{
	struct stat statbuf;
	
	if (stat(_filePath.c_str(), &statbuf) == 0)
		if (S_ISREG(statbuf.st_mode) && statbuf.st_mode & S_IRUSR)
			return ;
	throw std::invalid_argument("Configuration File: Invalid path.");
}

void ConfigFile::readingFile()
{
	std::ifstream inFile;
	size_t lineNbr = 1;
	std::string auxStr;

	inFile.open(_filePath.c_str(), std::ios::out);
	if (!inFile.good())
		throw std::runtime_error("Can't open configuration file.");
	
	std::string line;
	while (getline(inFile, line))
	{
		checkLine(line, lineNbr);
		lineNbr++;
		if (line.size() == 0)
			continue ;
		_content = _content + line + "\n";
	}
	
	if (_content.empty() || _content.length() == 0)
		throw std::runtime_error("Configuration File is empty.");
	for (size_t i = 0; i <= _content.length(); i++)
	{
		if (!std::isspace(_content[i]))
			break ;
		if (i == _content.length() - 1)
			throw std::runtime_error("Configuration File is empty.");
	}
}

void ConfigFile::splitServers()
{
	if (std::count(_content.begin(), _content.end(), '{') != std::count(_content.begin(), _content.end(), '}'))
		throw std::runtime_error("The number of opening curly brace is different of the number of closing curly brace.");
	
	_serverStr = splitServerBlocks(_content);
	_nbrOfServers = _serverStr.size();
	
	for (size_t i = 0; i < _serverStr.size(); ++i){
		_serverObjs.push_back(fillServersObjs(_serverStr[i], i + 1));
	}

}

Server ConfigFile::fillServersObjs(std::string& serverStr, size_t serverId)
{
	std::vector<std::string> serverVector = splitStr(serverStr, '\n');
	Server realServer;

	realServer.setServerId(serverId);
	for (size_t i = 0; i < serverVector.size(); ++i) 
	{
		if (serverVector[i] == "}")
			continue ;
		if (serverVector[i].find("location") != std::string::npos)
		{
			realServer.setLocation(serverVector, i);
						
			while (i < serverVector.size() - 1 && serverVector[i].find("}") == std::string::npos)
                ++i;
		}
		else if (i != 0)
			realServer.setElements(serverVector[i]);		
	}

	return (realServer);
}

bool ConfigFile::alreadyExist(int newPort)
{
	for (size_t i = 0; i <_ports.size(); i++){
		if (_ports[i] == newPort){
			return (true);
		}
	}
	return (false);
}

void ConfigFile::setPorts()
{
	for (size_t i = 0; i <_serverObjs.size(); i++)
	{
		int aux = _serverObjs[i].getListenSize();
		for (int j = 0; j < aux; j++){
			if (!alreadyExist(_serverObjs[i].getListen(j))){
				_ports.push_back(_serverObjs[i].getListen(j));
			}
		}
	}
}

const std::vector<Server>& ConfigFile::getServers() const
{
	return (_serverObjs);
}

const std::vector<int>& ConfigFile::getPorts() const
{
	return (_ports);
}
