//
//  hexdump.c
//  PTOBD
//
//  Created by niexuejin on 2017/4/25.
//  Copyright © 2017年 Protruly Inc. All rights reserved.
//

#include <stdio.h>
#include "hexdump.h"
#define SIZE 255

#include <sys/timeb.h>
#include <time.h>
#include <string.h>

#include    <stdarg.h>

char*  log_Time(void)
{
    struct  tm      *ptm;
    struct  timeb   stTimeb;
    static  char    szTime[19];
    
    ftime(&stTimeb);
    ptm = localtime(&stTimeb.time);
    sprintf(szTime, "%02d-%02d %02d:%02d:%02d.%03d",
            ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm);
    szTime[18] = 0;
    return szTime;
}
void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buffLine[17];
    unsigned char *pc = (unsigned char*)addr;

    if (desc != NULL){
        
        printf ("%s %s: len = %d \n", log_Time(), desc,len);
    }else ;
    
//    if(len>64) return;
    
    for (i = 0; i < len; i++) {
        
        if ((i % 16) == 0) {
            
            if (i != 0)
                
                printf ("  %s\n", buffLine);
            
            printf ("%04x ", i);
        }
        
        // Prints the HEXCODES that represent each chars.
        printf ("%02x", pc[i]);
        if ((i % 16) == 7) printf (" ");
        printf (" ");
        
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)){
            buffLine[i % 16] = '.';
        }
        
        else{
            
            buffLine[i % 16] = pc[i];
        }
        
        buffLine[(i % 16) + 1] = '\0'; //Clears the next array buffLine
        
    }
    
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }
    
    printf ("  %s\n", buffLine);
}

void printd(int n)//把整形按字符型输出
{
    if (n < 0)
    {
        putchar('-');
    }
    if (n)
    {
        printd(n /10);
        putchar(n % 10 + '0');  
    }  
}

//void hexlog(char *val, ...){
//    int ch;
//    va_list arg;
//    va_start(arg, val);
//    while (*val != '\0')
//    {
//        switch (*val)
//        {
//            case '%':            //遇到%执行switch case语句
//            {
//                if (*(val + 1) == 'c')//输出字符
//                {
//                    ch = va_arg(arg, char);
//                    putchar(ch);
//                    val++;                //指针变量向下偏移一个单位
//                }
//                else if (*(val + 1) == 'd')
//                {
//                    ch = va_arg(arg, char);//输出整形
//                    printd(ch);
//                    val++;
//                }
//                else if (*(val + 1) == 's')//输出字符串
//                {
//                    char*p = va_arg(arg, char*);
//                    while (*p != '\0')
//                    {
//                        putchar(*p);
//                        p++;
//                    }
//                    val++;                //指向头一变量的下一个字符
//                }
//                else
//                    putchar('%');
//                break;
//            }
//            default:
//            {
//                putchar(*val);
//                break;
//            }
//        }
//        val++;
//    }
//    va_end(arg);
//}

