#include <jni.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include "debug.h"
#include "commonfuc.h"
#include "clientdevice.h"


#define DEF_JNI_FUNC(func_name,...) JNICALL Java_com_example_rtsp_rtsptest_P2PCtrl_##func_name(JNIEnv *env,jobject obj,__VA_ARGS__)
#define DEF_JNI_FUNC_VOID(func_name) JNICALL Java_com_example_rtsp_rtsptest_P2PCtrl_##func_name(JNIEnv *env,jobject obj)

ClientDevice *p_client;
bool p2pIsConnected;

char savePort[32];
bool setupFlag1 = false,setupFlag2 = false;
uint32_t lastTime = 0;
uint32_t lastLen = 0;

int ClientDataMtoLCallBack(ConnectChannel *pChannel,uint8_t data[],int len)
{
     //Debug_Printf("ClientDataLtoMCallBack%d\n",len);
    uint32_t nowTime = CommonFuc::GetTimeMs();
    if(nowTime - lastTime >= 1000)
    {
        float disTime = nowTime - lastTime;
        uint32_t nowLen = pChannel->GetTotalRemoteLen();
        Debug_Printf("ClientDataMtoLCallBack:%.2fkbps\n",(nowLen - lastLen) / disTime);
        lastLen = nowLen;
        lastTime = nowTime;
    }
    return len;
}

int ClientLtoMCallBack(ConnectChannel *pChannel,uint8_t data[],int len)
{
    int newLen = len;
    data[len] = 0;
    if(CommonFuc::IsRequestType((char *)data,"SETUP"))
    {
        int port1,port2;
        CommonFuc::GetPortNum((char *)data,"client_port=",savePort,len);
        sscanf((char*)savePort,"%d-%d",&port1,&port2);
        Debug_Printf("port:%d,%d\n",port1,port2);
        ConnectChannel *pChannel1 = pChannel->GetClientConnect()->CreateConnectChannel(0);
        pChannel1->BindPort(8012);
        pChannel1->ConnectAddr("127.0.0.1",port1);
        pChannel1->SetCallBack(NULL,ClientDataMtoLCallBack);
        ConnectChannel *pChannel2 = pChannel->GetClientConnect()->CreateConnectChannel(0);
        pChannel2->BindPort(8013);
        pChannel2->ConnectAddr("127.0.0.1",port2);
        setupFlag1 = true;
    }
    data[newLen] = 0;
    Debug_Printf("%s\n",data);
    return newLen;
}

int ClientMtoLCallBack(ConnectChannel *pChannel,uint8_t data[],int len)
{
    int newLen = len;
    data[len] = 0;
    newLen = CommonFuc::ReplaceString((char *)data,"rtsp://","/","127.0.0.1:8011",newLen);
    if(setupFlag1)
    {
        newLen = CommonFuc::ReplacePortNum((char *)data,"server_port=","8012-8013",newLen);
        newLen = CommonFuc::ReplacePortNum((char *)data,"client_port=",savePort,newLen);
        setupFlag1 = false;
    }
    data[newLen] = 0;
    Debug_Printf("%s\n",data);
    return newLen;
}

int ClientConnectCallBack(ClientConnect*pclent)
{
    Debug_Printf("ClientConnectCallBack %s\n",pclent->GetClientDevice()->GetName().c_str());
    ConnectChannel *pChannel = pclent->CreateConnectChannel(1);
    pChannel->BindPort(8011);
    pChannel->SetCallBack(ClientLtoMCallBack,ClientMtoLCallBack);
    p2pIsConnected = true;
    return 0;
}


extern "C" JNIEXPORT void DEF_JNI_FUNC(p2pstart,jstring ip,jstring ownername,jstring remotename)
{
    const char* ownernamestr;
    const char* remotenamestr;
    const char* ipstr;

    ownernamestr = env->GetStringUTFChars(ownername,NULL);
    remotenamestr = env->GetStringUTFChars(remotename,NULL);
    ipstr = env->GetStringUTFChars(ip,NULL);

    Debug_Printf("%s,%s,%s\n",ipstr, ownernamestr, remotenamestr);

    p_client = new ClientDevice(ownernamestr);
    p_client->StartDevice(ipstr,8010);
    p_client->SetConnectedCallBack((void*)&ClientConnectCallBack);
    p_client->CreatClientConnect(8010,remotenamestr);
    p2pIsConnected = false;
}

extern "C" JNIEXPORT jboolean DEF_JNI_FUNC_VOID(p2pisconnected)
{
    return p2pIsConnected;
}
