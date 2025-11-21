/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:30:21 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/19 18:01:48 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

# include "ConfigFile.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerSettings.hpp"
# include "utils.hpp"
# include <unistd.h>
# include <ctime>


class CGIHandler {
private:
	const HttpRequest& 					_request;
	HttpResponse&						_response;
	ServerSettings&						_server;
	
	bool								_isRunning;
	int									_fileOutFd;
	int									_fileInFd;
	pid_t 								_pid;
	long long							_startedTime;
	std::string 						_tempFileName;
	char **								_env;
	char **								_cgiArgs;
	std::string							_cgiPath;
	std::map<std::string, std::string>	_cgiEnv;
	
	
	std::string _getMethod(Method method);
	void _getCgiEnv();
	void _openFile();
	void _forkProcess();
	void _cgiGetExec();
	void _cgiPostExec();
	void _getRequiredCgiArgs();
	void _setEnv();
	void _execute();
	
public:
	CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server);
	// CGIHandler(const CGIHandler& src);
	// CGIHandler& operator=(const CGIHandler& src);
	~CGIHandler();

	void run();
	bool isCgiRunning();
	bool cgiDone();

	void setCgiHeader();
	
};

#endif // CGIHANDLER_HPP