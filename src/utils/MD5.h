/*
 * MD5.h
 *
 *  Created on: Apr 12, 2018
 *      Author: root
 */

#ifndef MD5_H_
#define MD5_H_

#include <openssl/md5.h>
#include <string>


std::string MD5(const std::string& src )
{
    MD5_CTX ctx;

    std::string md5_string;
    unsigned char md[16] = { 0 };
    char tmp[33] = { 0 };

    MD5_Init( &ctx );
    MD5_Update( &ctx, src.c_str(), src.size() );
    MD5_Final( md, &ctx );

    for( int i = 0; i < 16; ++i )
    {
        memset( tmp, 0x00, sizeof( tmp ) );
        sprintf( tmp, "%02x", md[i] );
        md5_string += tmp;
    }
    return md5_string;
}


#endif /* MD5_H_ */
