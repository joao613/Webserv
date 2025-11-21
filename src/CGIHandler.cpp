/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:18:33 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/20 14:34:38 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <sys/wait.h>
#include <fcntl.h>

/* Constructor and destructor */
CGIHandler::CGIHandler(const HttpRequest& request, HttpResponse& response, ServerSettings& server) : _request(request), _response(response), _server(server)
{
	_isRunning = false;
	_fileInFd = -1;
	_fileOutFd = -1;
	_env = NULL;
	_cgiArgs = NULL;	
}

CGIHandler::~CGIHandler()
{
	if (_env)
	{
		for (size_t i = 0; _env[i] != NULL; ++i)
			delete[] _env[i];
		delete[] _env;
	}

	if (_cgiArgs)
	{
		for (size_t i = 0; _cgiArgs[i] != NULL; ++i)
			delete[] _cgiArgs[i];
		delete[] _cgiArgs;
	}
	if (_fileOutFd != -1)
		close(_fileOutFd);
	if (!_tempFileName.empty())
		std::remove(_tempFileName.c_str());
}

/* Private Methods */

std::string CGIHandler::_getMethod(Method method)
{
	const char* methods[] = {"GET", "POST"};
    return methods[method];
}

void CGIHandler::_setEnv()
{
	_cgiEnv["SERVER_NAME"] = _server.getServerName();
	_cgiEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	_cgiEnv["PATH_INFO"] = _cgiPath;
	_cgiEnv["REQUEST_METHOD"] = _getMethod(_request.getMethod());
	_cgiEnv["SCRIPT_FILENAME"] = _cgiPath;
	_cgiEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	std::map<std::string, std::string>::const_iterator it = _request.getHeaders().find("CONTENT_TYPE");
	if (_server.getQueryString() != ""){
		_cgiEnv["QUERY_STRING"] = _server.getQueryString();
	}
	if (it != _request.getHeaders().end()){
		_cgiEnv["CONTENT_TYPE"] = it->second;		
	}
	else{
		_cgiEnv["CONTENT_TYPE"] = "text/plain";
	}
	if (_request.getMethod() == POST)
	{
		size_t number = _request.getBodySize();
    	std::ostringstream oss;
    	oss << number;
		_cgiEnv["CONTENT_LENGTH"] = oss.str();
	}
}

void CGIHandler::_getCgiEnv()
{
	std::vector<std::string> envStrings;
    std::vector<char*> envPointers;
	
    for (std::map<std::string, std::string>::const_iterator it = _cgiEnv.begin(); it != _cgiEnv.end(); ++it) 
	envStrings.push_back(it->first + "=" + it->second);
	
    _env = new char*[envStrings.size() + 1];
    for (size_t i = 0; i < envStrings.size(); ++i)
	{
		_env[i] = new char[envStrings[i].size() + 1];
		
		for (size_t j = 0; j < envStrings[i].size(); ++j)
		_env[i][j] = envStrings[i][j];
		
		_env[i][envStrings[i].size()] = '\0';
	}
	
    _env[envStrings.size()] = NULL; 
}

void CGIHandler::_getRequiredCgiArgs()
{
	std::string	cgiPath = _server.cgiExtensionHasASpecifcScript(_cgiPath);
	
	if (!cgiPath.empty()) {
		_cgiArgs = new char*[2];
		_cgiArgs[0] = new char[cgiPath.size() + 1];
		for (size_t i = 0; i < cgiPath.size(); ++i)
			_cgiArgs[0][i] = cgiPath[i];
		_cgiArgs[0][cgiPath.size()] = '\0';
		_cgiArgs[1] = NULL;
	} else {
		if (_cgiPath.rfind(".py") == _cgiPath.length() - 3)
		{
			_cgiArgs = new char*[3];
			std::string cgiExec = "/usr/bin/python3";
			_cgiArgs[0] = new char[cgiExec.size() + 1];
			for (size_t i = 0; i < cgiExec.size(); ++i)
				_cgiArgs[0][i] = cgiExec[i];
			_cgiArgs[0][cgiExec.size()] = '\0';
			
			_cgiArgs[1] = new char[_cgiPath.size() + 1];
			for (size_t i = 0; i < _cgiPath.size(); ++i)
				_cgiArgs[1][i] = _cgiPath[i];
			_cgiArgs[1][_cgiPath.size()] = '\0';
			_cgiArgs[2] = NULL;
		} else {
			throw std::runtime_error("Extension not recognized.");
		}
	}
}

void CGIHandler::_openFile()
{
	std::srand(std::time(0));
    std::ostringstream fileNameStream;
    
    fileNameStream << "root/cgi-bin/cgioutfile";
    fileNameStream << std::rand();
		
	_tempFileName = fileNameStream.str();
	_fileOutFd = open(_tempFileName.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
	
	if (_fileOutFd == -1){
		throw std::runtime_error("CGI file result creat failed.");
	}

	if (_request.getMethod() == POST)
	{
		_fileInFd = open(_response.cgiFile.c_str(), O_RDONLY);
		if (_fileInFd == -1){
			throw std::runtime_error("CGI open file failed.");
		}
	}
}

void CGIHandler::_forkProcess()
{
	_pid = fork();
	if (_pid == -1)
		throw std::runtime_error("Fork failed.");
}

void CGIHandler::_cgiGetExec()
{
	if (_pid == 0)
	{
		dup2(_fileOutFd, STDOUT_FILENO);
		close(_fileOutFd);
		
		execve(_cgiArgs[0], _cgiArgs, _env);
		
		std::cerr << "CGI Execution failed!" << std::endl;
		std::exit(1);
	}
}

void CGIHandler::_cgiPostExec()
{	
	if (_pid == 0)
	{
		dup2(_fileInFd, STDIN_FILENO);
		close(_fileInFd);
		
		dup2(_fileOutFd, STDOUT_FILENO);
		close(_fileOutFd);
		
		execve(_cgiArgs[0], _cgiArgs, _env);
		std::cerr << "CGI Execution failed!" << std::endl;
		std::exit(1);
	}
}

void CGIHandler::_execute()
{
	_getCgiEnv();
	_isRunning = true;
	_openFile();
	_forkProcess();

	if (_request.getMethod() == GET)
		_cgiGetExec();
	if (_request.getMethod() == POST)
		_cgiPostExec();

	_startedTime = getCurrentTimestamp();
}

/* Public Methods */

bool CGIHandler::isCgiRunning(){return (_isRunning);}

bool CGIHandler::cgiDone()
{
	int status;

	pid_t pidCheck = waitpid(_pid, &status, WNOHANG);
	if (pidCheck > 0)
	{
		if (WIFSIGNALED(status))
		{
			_response.setStatusCode(502); /* Bad gateway */
			return (true);
		}
		if (WEXITSTATUS(status) != 0) {
			_response.setStatusCode(500);/* internal server error */

		} else {
			_response.setBodyPath(_tempFileName);
		}
		if (_fileInFd >= 0)
		{
		    close(_fileInFd);
		    _fileInFd = -1;
		}
		if (!_response.cgiFile.empty())
			std::remove(_response.cgiFile.c_str());
		return (true);
	}
	
	if (hasTimedOut(_startedTime, 59))
	{
		std::cout << "\nCGI TimedOut\n" << std::endl;
		kill(_pid, SIGKILL);
		_response.setStatusCode(500); /* in this case, cgi is internally processed, so the error is 500, not 502?*/
		if (_fileInFd >= 0)
		{
		    close(_fileInFd);
		    _fileInFd = -1;
		}
		if (!_response.cgiFile.empty())
		    std::remove(_response.cgiFile.c_str());
		return (true);
	}
	
	return (false);
}

void CGIHandler::run()
{
	_cgiPath = _request.getTarget();
	_setEnv();
	_getRequiredCgiArgs();
	_execute();	
}

void CGIHandler::setCgiHeader()
{
	if (_response.getStatusCode() != 200){
		return ; // So that the CGI does not modify the headers if anything is wrong
	}
	
	std::ifstream file(_tempFileName.c_str());
    std::string line;
    if (!file) {
        throw std::runtime_error("CGI file open failed.");
    }

    while (std::getline(file, line) && line != "\n") {
		
		std::string fieldName = line.substr(0, line.find(':'));
		std::string fieldValue = line.substr(line.find(':') + 1, line.size() - 1);
		
		if (line == ""){
			std::string next;
			std::getline(file, next);
			if (next == "")
				break;
		}
		_response.addHeaderCgi(fieldName, fieldValue);
    }

	_response.cgiBodyStarted = file.tellg();
	file.seekg(0, std::ios::end);
    std::streampos endPos = file.tellg();
	
	std::streampos size = endPos - _response.cgiBodyStarted;
	
	if (size == 0){
		_response.setStatusCode(500);
		return ;
	}
	std::stringstream ss;
	ss << size;
	_response.addHeaderCgi("Content-Length", ss.str());

    file.close();
}

