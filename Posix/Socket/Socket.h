/* 
 * File:   Socket.h
 * Author: alexey
 *
 * Created on June 10, 2014, 3:15 PM
 */

#ifndef SOCKET_H
#define	SOCKET_H

#include <string>

namespace Posix{

class Socket
{
    public:
        typedef std::string String;
        
        Socket();
        ~Socket();
        bool open(const String & host, int port);
        bool isOpen()const;
        String readData();
        void writeData(const String & data, int size = -1);
        
    private:
        int     m_Sockfd;
        bool    m_IsOpen;
};

}

#endif	/* SOCKET_H */

