//
//  hexdump.h
//  PTOBD
//
//  Created by niexuejin on 2017/4/25.
//  Copyright © 2017年 Protruly Inc. All rights reserved.
//

#ifndef hexdump_h
#define hexdump_h

#ifdef __cpluscplus
extern "C"
{
#endif
    void hexlog(char *desc, ...);
    void hexDump (char *desc, void *addr, int len);
    char*   log_Time(void);
    
#ifdef __cpluscplus
};
#endif


#endif /* hexdump_h */
