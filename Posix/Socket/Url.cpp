/* 
 * File:   Url.cpp
 * Author: alexey
 * 
 * Created on June 10, 2014, 4:45 PM
 */

#include "Url.h"
#include <algorithm>
#include <iterator>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <assert.h>

namespace Posix{
    
    template<typename T>
    T fromString(const String & string)
    {
        T result;
        std::istringstream iss(string);
        iss >> result;
        return result;
    }
    
    Url::Url():m_Port(-1)
    {
    }
    
    Url::Url(const String & value):m_Port(-1)
    {
        accept(value);
    }
    //<схема>://<логин>:<пароль>@<хост>:<порт>/<URL‐путь>?<параметры>#<якорь>
    
    String Url::toString() const
    {
        std::ostringstream oss;
        oss << m_Schema << "://";
        if ( !m_Login.empty() ){
            oss << m_Login << ":" << m_Password << "@";
        }
        oss << m_Host;
        if ( m_Port != -1 ){
            oss << ":" << m_Port;
        }
        oss << "/" << m_Path;
        if ( m_Query.size() > 0 ){
            oss << "?" << m_Query.toString(true);
        }
        if ( !m_Anchor.empty() ){
            oss << "#" << m_Anchor;
        }
        return oss.str();
    }
    
    String Url::fullPath() const
    {
        std::ostringstream oss;
        oss << "/" << m_Path;
        if ( m_Query.size() > 0 ){
            oss << "?" << m_Query.toString(true);
        }
        return oss.str();
    }
    
    void Url::accept(const String & value)
    {
        int lastLoc = 0;
        m_Schema = getValue(value, "://", lastLoc);
        if ( value.find("@", lastLoc) != -1 ){
            m_Login = getValue(value, ":", lastLoc);
            m_Password = getValue(value, "@", lastLoc);
        }
        if ( value.find(":", lastLoc) != -1 ){
            m_Host = getValue(value, ":", lastLoc);
            m_Port = fromString<int>(getValue(value, "/", lastLoc));
        } else {
            m_Host = getValue(value, "/", lastLoc);
        }
        bool hasAnchor = value.find("#", lastLoc) != -1;
        if ( value.find("?", lastLoc) != -1 ){
            m_Path = getValue(value, "?", lastLoc);
            if ( hasAnchor ){
                m_Query = getValue(value, "#", lastLoc);
                m_Anchor = urlDecode(value.substr(lastLoc, value.size()-1));
            } else {
                m_Query = urlDecode(value.substr(lastLoc, value.size()-1));
            }
        } else {
            if ( hasAnchor ){
                m_Path = getValue(value, "#", lastLoc);
                m_Anchor = urlDecode(value.substr(lastLoc, value.size()-1));
            } else {
                m_Path = urlDecode(value.substr(lastLoc, value.size()-1));
            }
        }
    }
    
    String Url::getValue(const String & source, const String & delim, int & lCursor)
    {
        String result;
        int pos = source.find(delim, lCursor);
        if ( pos != -1 ){
            result = urlDecode(source.substr(lCursor, pos - lCursor));
            lCursor = pos + delim.size();
        }
        return result;
    }
    
    String Url::schema()const
    {
        return m_Schema;
    }
    
    String Url::login()const
    {
        return m_Login;
    }
    
    String Url::password()const
    {
        return m_Password;
    }
    
    String Url::host()const
    {
        return m_Host;
    }
    
    int Url::port()const
    {
        return m_Port;
    }
    
    String Url::path()const
    {
        return m_Path;
    }
    
    Map Url::query()const
    {
        return m_Query;
    }
    
    String Url::anchor()const
    {
        return m_Anchor;
    }
    
    void Url::setSchema(const String & schema)
    {
        m_Schema = schema;
    }

    void Url::setLogin(const String & login)
    {
        m_Login = login;
    }
    
    void Url::setPassword(const String & password)
    {
        m_Password = password;
    }

    void Url::setHost(const String & host)
    {
        m_Host = host;
    }

    void Url::setPort(int port)
    {
        m_Port = port;
    }

    void Url::setPath(const String & path)
    {
        m_Path = path;
    }
    
    void Url::setQuery(const Map & query)
    {
        m_Query = query;
    }

    void Url::setAnchor(const String & anchor)
    {
        m_Anchor = anchor;
    }
    
    const char HEX2DEC[256] = 
    {
        /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
        /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

        /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
    };
    
    // Only alphanum is safe.
    const char SAFE[256] =
    {
        /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
        /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

        /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
        /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
        /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
        /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

        /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

        /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
    };
    
    String Url::urlEncode(const String& value)
    {
        const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
        const unsigned char * pSrc = (const unsigned char *)value.c_str();
        const int SRC_LEN = value.length();
        unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
        unsigned char * pEnd = pStart;
        const unsigned char * const SRC_END = pSrc + SRC_LEN;

        for (; pSrc < SRC_END; ++pSrc)
        {
           if (SAFE[*pSrc]) 
              *pEnd++ = *pSrc;
           else
           {
              // escape this char
              *pEnd++ = '%';
              *pEnd++ = DEC2HEX[*pSrc >> 4];
              *pEnd++ = DEC2HEX[*pSrc & 0x0F];
           }
        }

        std::string sResult((char *)pStart, (char *)pEnd);
        delete [] pStart;
        return sResult;
    }
    
    String Url::urlDecode(const String& value)
    {
        // Note from RFC1630:  "Sequences which start with a percent sign
        // but are not followed by two hexadecimal characters (0-9, A-F) are reserved
        // for future extension"
    
        const unsigned char * pSrc = (const unsigned char *)value.c_str();
        const int SRC_LEN = value.length();
        const unsigned char * const SRC_END = pSrc + SRC_LEN;
        const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%' 

        char * const pStart = new char[SRC_LEN];
        char * pEnd = pStart;

        while (pSrc < SRC_LAST_DEC){
            if (*pSrc == '%'){
                char dec1, dec2;
                if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                    && -1 != (dec2 = HEX2DEC[*(pSrc + 2)])){
                    *pEnd++ = (dec1 << 4) + dec2;
                    pSrc += 3;
                    continue;
                }
            }
            *pEnd++ = *pSrc++;
        }

        // the last 2- chars
        while (pSrc < SRC_END)
            *pEnd++ = *pSrc++;

        std::string sResult(pStart, pEnd);
        delete [] pStart;
	return sResult;
    }
}

