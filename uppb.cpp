/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: upload.cpp,v 1.13 2007/07/02 18:48:19 sebdiaz Exp $
 *
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *  Part of the GNU cgicc library, http://www.gnu.org/software/cgicc
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA 
 */

/*! \file upload.cpp
 * \brief File upload demo
 *
 * Tests and demonstrates how to handle uploaded files using the 
 * GNU %cgicc library.
 */

#include <new>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <zlib.h>
#include    <lz4.h>
#if HAVE_SYS_UTSNAME_H
#  include <sys/utsname.h>
#endif

#if HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif
#include "hexdump.h"
#include "styles.h"
#include "log_logstores.pb-c.h"
#define log_unpack                  sls_logs__log_group__unpack
#define log_free_unpack             sls_logs__log_group__free_unpacked
using namespace std;
using namespace cgicc;

int zlibumcompress(const unsigned char *text, char **out, int *len){
    if(text && out && len){
        uLong tlen = *len;  /* 需要把字符串的结束符'\0'也一并处理 */
        unsigned char* buf = NULL;
        uLong blen;
        int ur;
        /* 计算缓冲区大小，并为其分配内存 */
        blen = 0; blen = tlen *100; ur = 0;
        buf = (unsigned char *) malloc(blen);
        memset(buf, 0, blen);
        /* 压缩 */
//        hexDump("oridata",(void*)text, tlen);
        if((ur = uncompress(buf, &blen, (unsigned char*)text, tlen) )!= Z_OK)
        {
            printf("compress failed!%d, error = %d\n", tlen,ur);
            return -1;
        }else{
//           printf((char*)buf);
        }
        
        *out = (char*)buf; *len = blen;
        return 0;
    }
    return -1;
}
int lz4umcompress(const unsigned char *text, char **out, int *len){
    if(text && out && len){
        uLong tlen = *len;  /* 需要把字符串的结束符'\0'也一并处理 */
        unsigned char* buf = NULL;
        uLong blen;
        int ur;
        /* 计算缓冲区大小，并为其分配内存 */
        blen = 0; blen = tlen *100; ur = 0;
        buf = (unsigned char *) malloc(blen);
        memset(buf, 0, blen);
        /* 压缩 */
        //        hexDump("oridata",(void*)text, tlen);
        if((ur = LZ4_uncompress_unknownOutputSize((const char*)text,(char*) buf, *len, blen) )<= Z_OK)
        {
            printf("compress failed!%d, error = %d\n", tlen,ur);
            return -1;
        }else{
            //           printf((char*)buf);
        }
        
        *out = (char*)buf; *len = ur;
        return 0;
    }
    return -1;
}

// Main Street, USA
int
main(int /*argc*/,  char ** /*argv*/)
{
  try {

    // Create a new Cgicc object containing all the CGI data
    Cgicc cgi;
      cout << "Content-Type: text/html;charset=utf-8\n\n";// << endl;
    // Redirect output, if desired
    if(cgi.queryCheckbox("redirect")) {
      const_file_iterator file = cgi.getFile("userfile");

      // Only redirect a valid file
      if(file != cgi.getFiles().end()) {
        cout << HTTPContentHeader(file->getDataType());
        file->writeToStream(cout);

        return EXIT_SUCCESS;
      }
    }
    // Get a pointer to the environment
    const CgiEnvironment& env = cgi.getEnvironment();
      const_file_iterator file;
      file = cgi.getFile("userfile");
    if(file != cgi.getFiles().end()) {

        if(stringsAreEqual(file->getDataType(), "application/zip")){
            //解压
            string ss = file->getData();
            char *ubuf;
            unsigned long ulen;
            int ur;
            ulen = file->getDataLength()*50;
            ubuf = (char*)malloc(file->getDataLength()*50);
            memset (ubuf, 0, file->getDataLength()*50);
            if((ur=uncompress((unsigned char*)ubuf, &ulen, (unsigned char*)ss.c_str(), file->getDataLength())) != Z_OK)
            {
                cout<<ss<<endl;
            }else{
                cout<< ubuf;
            }
            free(ubuf);
        }else{
            file->writeToStream(cout);
        }

    }else{
        if(stringsAreEqual(env.getRequestMethod(), "post")){
            char *buf=NULL;
            int len,start;
            len = env.getContentLength();
//            hexDump("oridata",(void*)env.getPostData().c_str(), len);
            if(lz4umcompress(( unsigned char *)env.getPostData().c_str(), &buf, &len)==0){
                cout<<"xlen="<<env.getContentLength()<<",len="<<len<<endl; start =0;
                if(buf[0] == ' ') start =1;
                SlsLogs__LogGroup *grp;
//                hexDump("oridata",(void*)buf, len);
                grp = log_unpack(NULL, len-start, (const uint8_t*)buf+start);
                if(grp){
                    cout<<grp->topic<<endl;
                    int i,j,k;
                    for(i=0;i < grp->n_logs;i++){
                        cout<<grp->logs[i]->time;
                        cout<<" ";
                        for(j=0; j < grp->logs[i]->n_contents;j++){
                            cout<<grp->logs[i]->contents[j]->key;
                        }
                        cout<<endl;
                    }
                    log_free_unpack(grp,NULL);
                }else{
                    cout <<"error";
                }
            }
            if(buf) free(buf);
        }
    }
    // No chance for failure in this example
    return EXIT_SUCCESS;
  }

  // Did any errors occur?
  catch(const std::exception& e) {

    html::reset(); 	head::reset(); 		body::reset();
    title::reset(); 	h1::reset(); 		h4::reset();
    comment::reset(); 	td::reset(); 		tr::reset(); 
    table::reset();	cgicc::div::reset(); 	p::reset(); 
    a::reset();		h2::reset(); 		colgroup::reset();

    return EXIT_SUCCESS;
  }
}
