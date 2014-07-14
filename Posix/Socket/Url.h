/* 
 * File:   Url.h
 * Author: alexey
 *
 * Created on June 10, 2014, 4:45 PM
 */

#ifndef URL_H
#define	URL_H

#include <string>
#include <map>
#include <vector>
#include "Map.h"

namespace Posix{
    
    typedef std::string String;
    
    class Url
    {
        public:
            Url();
            Url(const String & value);
            void accept(const String & value);
            String schema()const;
            void setSchema(const String & schema);
            String login()const;
            void setLogin(const String & login);
            String password()const;
            void setPassword(const String & password);
            String host()const;
            void setHost(const String & host);
            int port()const;
            void setPort(int port);
            String path()const;
            String fullPath()const;
            void setPath(const String & path);
            Map query()const;
            void setQuery(const Map & query);
            String anchor()const;
            void setAnchor(const String & anchor);
            
            String toString()const;
            
        public:
            static String urlEncode(const String &value);
            static String urlDecode(const String &value);
            
        private:
            String getValue(const String & source, const String & delim, int & lCursor);
            
        private:
            String m_Schema;
            String m_Login;
            String m_Password;
            String m_Host;
            int    m_Port;
            String m_Path;
            Map    m_Query;
            String m_Anchor;
    };
    
}

#endif	/* URL_H */

