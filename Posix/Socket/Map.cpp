/* 
 * File:   Map.cpp
 * Author: alexey
 * 
 * Created on June 11, 2014, 11:59 AM
 */

#include "Map.h"
#include "Url.h"

namespace Posix{
    
    const char * DEFAULT_PAIR_DELIM = "&";
    const char * DEFAULT_KEY_DELIM = "=";
    
    Map::Map() : m_PairDelim(DEFAULT_PAIR_DELIM), m_KeyDelim(DEFAULT_KEY_DELIM)
    {
        
    }
    
    Map::Map(const String & rawData) : m_PairDelim(DEFAULT_PAIR_DELIM), m_KeyDelim(DEFAULT_KEY_DELIM)
    {
        fromString(rawData);
    }
    
    String Map::toString(bool encode)const
    {
        String result;
        for ( int i = 0; i < m_Keys.size(); ){
            String key = m_Keys[i];
            String value = m_Map.at(key);
            if ( encode ){
                result.append(Url::urlEncode(key) + m_KeyDelim + Url::urlEncode(value));
            } else {
                result.append(key + m_KeyDelim + value);
            }
            if ( ++i < m_Keys.size() ){
                result.append(m_PairDelim);
            }
        }
        return result;
    }
    
    StringList Map::split(const String & source, const String & delim)
    {
        StringList result;
        int cursor, lastPos = 0;
        while ( (cursor = source.find(delim, lastPos)) != -1 ){
            result.push_back(source.substr(lastPos, cursor - lastPos));
            lastPos = cursor + delim.size();
        }
        result.push_back(source.substr(lastPos, source.size() - lastPos));
        return result;
    }
    
    void Map::fromString(const String & rawData)
    {
        m_Map.clear();
        m_Keys.clear();
        StringList pairs = split(rawData, m_PairDelim);
        for ( int i = 0; i < pairs.size(); ++i ){
            if ( !pairs[i].empty() ){
                StringList keyValue = split(pairs[i], m_KeyDelim);
                if ( keyValue.size() == 2 ){
                    m_Map.insert(std::make_pair(keyValue[0], keyValue[1]));
                    m_Keys.push_back(keyValue[0]);
                }
            }
        }
    }
    
    String& Map::operator[](const String & key)
    {
        if ( !contains(key) ){
            m_Keys.push_back(key);
        }
        return m_Map[key];
    }
    
    bool Map::contains(const String& key) const 
    {
        return m_Map.find(key) != m_Map.end();
    }
 
    size_t Map::size() const
    {
        return m_Map.size();
    }

    StringList Map::keys()const
    {
        return m_Keys;
    }
    
    void Map::setPairDelim(const String & value)
    {
        m_PairDelim = value;
    }
    
    String Map::pairDelim()const
    {
        return m_PairDelim;
    }
    
    void Map::setKeyDelim(const String & value)
    {
        m_KeyDelim = value;
    }
    
    String Map::keyDelim()const
    {
        return m_KeyDelim;
    }
    
}

