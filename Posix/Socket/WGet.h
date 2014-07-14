/* 
 * File:   WGet.h
 * Author: alexey
 *
 * Created on June 10, 2014, 5:23 PM
 */

#ifndef WGET_H
#define	WGET_H

#include <string>
#include "Url.h"

namespace Posix{

    typedef std::string String;
    
    struct Header
    {
        String protocol;
        int code;
        String codeString;
        Map fields;
    };
    
    struct Response
    {
        Header header;
        String body;
    };
    
    struct MapEntity
    {
        String key;
        String value;
    };
    
    class WGet
    {
        public:
            static Response get(const String & url);
            static Response post(const String & url, const String & postData);
            
        private:
            static String makeGetRequest(const Url & url);
            static String makePostRequest(const Url & url, const String & postData);
            static int getPort(const Url & url);
            static Response makeResponse(const String & rawData);
            static Header readHeader(const String & data);
            static MapEntity readMapEntity(const String & data);
            static String toBase64(const String & data);
            static String fromBase64(const String & data);
    };
    
}

#endif	/* WGET_H */

