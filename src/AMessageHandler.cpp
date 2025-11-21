/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:51:30 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 17:37:46 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessageHandler.hpp"

AMessageHandler::AMessageHandler(HttpRequest& request, HttpResponse& response)
    :_request(request), _response(response), _isDone(false) {}
AMessageHandler::~AMessageHandler(){}

bool    AMessageHandler::isDone(void) const {return (_getIsDone());}

void    AMessageHandler::_setIsDone(bool newValue) {_isDone = newValue;}
bool    AMessageHandler::_getIsDone(void) const {return (_isDone);}