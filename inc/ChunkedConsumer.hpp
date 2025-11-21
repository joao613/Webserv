/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedConsumer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:58:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/13 11:15:03 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNKED_CONSUMER_HPP
# define CHUNKED_CONSUMER_HPP

# include "ABodyConsumer.hpp"
# include "StateMachine.hpp"
# include <cstdlib> //for strtol
# include <fstream>
# include "utils.hpp"

class ChunkedConsumer: public ABodyConsumer
{
public:
    ChunkedConsumer(HttpResponse&  response, bool shouldPerformPost/*, std::string saveFileName*/);
    ~ChunkedConsumer();

    size_t  consume(data_t& data);
private:
    enum State {
        CHUNK_SIZE,
        CHUNK,
        STATE_AM
    };
    StateMachine<State> _stateMachine;
    
    data_t          _data;
    size_t          _currentChunkSize;
    //std::ofstream  _saveFile;  
    
    void    _parseChunkSize(void);
    void    _parseChunk(void);
};

#endif