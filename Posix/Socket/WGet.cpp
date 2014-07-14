/* 
 * File:   WGet.cpp
 * Author: alexey
 * 
 * Created on June 10, 2014, 5:23 PM
 */

#include "WGet.h"
#include "Socket.h"
#include <iostream>
#include <sstream>

namespace Posix{

    template<typename T>
    String toString(const T & value)
    {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    template<typename T>
    T fromString(const String & value)
    {
        T result;
        std::istringstream iss(value);
        iss >> result;
        return result;
    }
    
    int WGet::getPort(const Url& url)
    {
        if ( url.port() != -1 )
            return url.port();
        if ( url.schema() == "http" )
            return 80;
        //Other unsuppoted
        return 80;
    }
    
    String WGet::makeGetRequest(const Url& url)
    {
        String nLine = "\r\n";
        Map header;
        header.setKeyDelim(": ");
        header.setPairDelim(nLine);
        header["Host"] = url.host() + (url.port() != -1 ? ":" + toString(getPort(url)) : "");
        header["Accept"] = "*/*";
        header["Connection"] = "close";
        header["Accept-Encoding"] = "deflate";
        
        std::ostringstream oss;
        oss << "GET " + url.fullPath() + " HTTP/1.0" + nLine;
        oss << nLine;
        oss << header.toString() + nLine;
        oss << nLine;
        oss << nLine;
        return oss.str();
    }
    
    String WGet::makePostRequest(const Url& url, const String& postData)
    {
        String nLine = "\r\n";
        Map header;
        header.setKeyDelim(": ");
        header.setPairDelim(nLine);
        header["Host"] = url.host() + (url.port() != -1 ? ":" + toString(getPort(url)) : "");
        header["Accept"] = "*/*";
        header["Connection"] = "close";
        header["Accept-Encoding"] = "deflate";
        header["Content-Length"] = toString(postData.size());
        
        std::ostringstream oss;
        oss << "POST " + url.fullPath() + " HTTP/1.0" + nLine;
        oss << header.toString() + nLine;
        oss << nLine;
        oss << postData;
        return oss.str();
    }
    
    Response WGet::get(const String & url)
    {
        Url u(url);
        Socket socket;
        String request = makeGetRequest(u);
        if ( socket.open(u.host(), getPort(u)) ) {
            socket.writeData(request, request.size() + 1);
            return makeResponse(socket.readData());
        }
        Response response = {{"", 0, "", Map()}, ""};
        return response;
    }
    
    Response WGet::makeResponse(const String & rawData)
    {
        using namespace std;
        Header header;
        String buffer;
        istringstream iss(rawData);
        if (getline(iss, buffer)){
            header = readHeader(buffer);
        }
        while ( getline(iss, buffer) && buffer.size() > 3 ){
            MapEntity entity = readMapEntity(buffer);
            header.fields[entity.key] = entity.value;
        }
        Response response = {header, ""};
        if ( header.fields.contains("Content-Length") ){
            int size = fromString<int>(header.fields["Content-Length"]);
            char buffer[size];
            if (iss.read(buffer, size))
                response.body.append(buffer, size);
        }
        return response;
    }
    
    Header WGet::readHeader(const String & data)
    {
        Header header;
        std::istringstream iss(data);
        iss >> header.protocol >> header.code >> header.codeString;
        return header;
    }
    
    MapEntity WGet::readMapEntity(const String & data)
    {
        using namespace std;
        istringstream iss(data);
        String key, value;
        getline(iss, key, ':');
        getline(iss, value);
        MapEntity entity = {key, value};
        return entity;
    }
    
    Response WGet::post(const String & url, const String & postData)
    {
        Url u(url);
        Socket socket;
        String request = makePostRequest(u, postData);
        if ( socket.open(u.host(), getPort(u)) ) {
            socket.writeData(request, request.size() + 1);
            return makeResponse(socket.readData());
        }
        Response response = {{"", 0, "", Map()}, ""};
        return response;
    }
    
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

String WGet::toBase64(const String & data)
{
    unsigned char const* bytes_to_encode = reinterpret_cast<unsigned char const*>(data.c_str());
    unsigned int in_len = static_cast<unsigned int>(data.size());
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
      char_array_3[i++] = *(bytes_to_encode++);
      if (i == 3) {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for(i = 0; (i <4) ; i++)
          ret += base64_chars[char_array_4[i]];
        i = 0;
      }
    }

    if (i)
    {
      for(j = i; j < 3; j++)
        char_array_3[j] = '\0';

      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (j = 0; (j < i + 1); j++)
        ret += base64_chars[char_array_4[j]];

      while((i++ < 3))
        ret += '=';

    }

    return ret;
}

String WGet::fromBase64(const String & data)
{
    int in_len = data.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( data[in_] != '=') && is_base64(data[in_])) {
      char_array_4[i++] = data[in_]; in_++;
      if (i ==4) {
        for (i = 0; i <4; i++)
          char_array_4[i] = base64_chars.find(char_array_4[i]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (i = 0; (i < 3); i++)
          ret += char_array_3[i];
        i = 0;
      }
    }

    if (i) {
      for (j = i; j <4; j++)
        char_array_4[j] = 0;

      for (j = 0; j <4; j++)
        char_array_4[j] = base64_chars.find(char_array_4[j]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

}