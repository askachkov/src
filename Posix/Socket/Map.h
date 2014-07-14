/* 
 * File:   Map.h
 * Author: alexey
 *
 * Created on June 11, 2014, 11:59 AM
 */

#ifndef MAP_H
#define	MAP_H

#include <string>
#include <map>
#include <vector>

namespace Posix{
    
    typedef std::string String;
    typedef std::map<String, String> StringMap;
    typedef std::vector<String> StringList;
    
    class Map
    {
        public:
            Map();
            Map(const String & rawData);
            String toString(bool isUrlEncoded = false)const;
            void fromString(const String & rawData);
            String& operator[](const String & key);
            bool contains(const String & key)const;
            size_t size() const;
            StringList keys()const;
            
            void setPairDelim(const String & value);
            String pairDelim()const;
            void setKeyDelim(const String & value);
            String keyDelim()const;
            
        private:
            static StringList split(const String & source, const String & delim);
            
        private:
            StringList   m_Keys;
            StringMap    m_Map;
            String       m_PairDelim;
            String       m_KeyDelim;
    };
}

#endif	/* MAP_H */

