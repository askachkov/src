/* 
 * File:   Socket.cpp
 * Author: alexey
 * 
 * Created on June 10, 2014, 3:15 PM
 */

#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

namespace Posix{

Socket::Socket():m_Sockfd(-1), m_IsOpen(false)
{
}

Socket::~Socket() 
{
    if ( isOpen() && m_Sockfd != -1 ){
        close(m_Sockfd);
    }
}

bool Socket::open(const String & host, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    m_Sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_Sockfd < 0) {
        return false;
    }
    server = gethostbyname(host.c_str());
    if (server == 0) {
        return false;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
    m_IsOpen = connect(m_Sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) == 0;
    return isOpen();
}

bool Socket::isOpen()const
{
    return m_IsOpen;
}

Socket::String Socket::readData()
{
    String result;
    const int size = 1024;
    char buffer[size];
    int readResult = 0;
    do {
        switch ( (readResult = recv(m_Sockfd, buffer, size, 0)) ){
            case -1:{ //Error
                m_IsOpen = false;
                break;
            }
            case 0: { //EOF
                break;
            }
            default:{ // in readResult number of red bytes
                result.append(buffer, readResult);
            }
        }
    } while ( readResult > 0 );
    return result;
}

void Socket::writeData(const String & data, int size)
{
    if (send(m_Sockfd, data.c_str(), size == -1 ? data.size() : size, 0) == -1 ){
        m_IsOpen = false;
    }
}


}