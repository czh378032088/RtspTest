


#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifndef __cplusplus
extern "C" {
#endif

#include <android/log.h>

#define MY_DEBUG

#ifdef MY_DEBUG


#define Debug_Printf(...) __android_log_print(ANDROID_LOG_INFO, "jni-inf", __VA_ARGS__)
#define Debug_ShowHex(buff,size)  \
do{\
  unsigned char *debugbuff = (unsigned char *)buff;\
  for(int debugindex = 0; debugindex < size ; debugindex ++) \
  {\
        Debug_Printf("%02x ",debugbuff[debugindex]);\
  }\
  if(size > 0)\
       Debug_Printf("\n");\
}while(0)

#define Debug_ShowIp(ip,port)\
{\
    Debug_Printf("%d.%d.%d.%d:%d",ip & 0xff,(ip >> 8) & 0xff,(ip >> 16) & 0xff,(ip >> 24) & 0xff,htons(port));\
}while(0)

#else

#define Debug_Printf(a,...) 
#define Debug_ShowHex(buff,size)

#endif


#ifndef __cplusplus
}
#endif

#endif

