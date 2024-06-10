/*============================================================
 * Project: Ascencia
 * File: network.h
 * Author: Phraggers
 * Date: Tue May 14 2024
============================================================*/

#ifdef ASC_HEAD

enum NETPACKET_TYPE : u8
{
    NETPACKET_NONE = 0,
    NETPACKET_CLIENT_REQUEST_DATA,
    NETPACKET_CLIENT_PROVIDE_DATA,
    NETPACKET_SERVER_REQUEST_DATA,
    NETPACKET_SERVER_PROVIDE_DATA,
};

enum NETPACKET_DATA_TYPE : u8
{
    NETPACKET_DATA_NONE = 0,
    NETPACKET_DATA_ID,
    NETPACKET_DATA_USERLOGIN,
};

typedef struct
{
    u16 target;
    u16 source;
    NETPACKET_TYPE type;
    NETPACKET_DATA_TYPE dataType;
    u8 dataSize;
    u8 data[0xff];
} ASC_NetPacket;

typedef struct
{
    bool running;
    u16 localID; // temporary, used to request ID from server
    u16 clientID;
    SDL_Semaphore *dataLock;
    SDL_Thread *thread;
} ASC_NetThreadData;

struct ASC_Network
{
    #ifdef ASC_WINDOWS
    WSADATA wsadata;
    #endif

    int handle;
    u16 port;
    sockaddr_in address;

    ASC_NetThreadData netThread;
};

static bool ASC_NetInit(void);
static void ASC_NetQuit(void);
static bool ASC_NetFrame(void);
static bool ASC_NetSendPacket(sockaddr_in dest, ptr data, int size);
static sockaddr_in ASC_NetAddress(u8 a, u8 b, u8 c, u8 d, u16 p);

int ASC_NetThread(ptr data);

#endif

/*=================================================================================*/

#ifdef ASC_IMPL

static bool ASC_NetInit(void)
{
    bool result = 0;

    #ifdef ASC_WINDOWS
    result = (WSAStartup(MAKEWORD(2,2), &gApp->network.wsadata) == NO_ERROR) ? 1 : 0;
    #else
    result = 1;
    #endif

    gApp->network.handle = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(gApp->network.handle <= 0)
    {
        ASC_Error("ASC_NetInit: Failed to create socket.");
        return 0;
    }

    gApp->network.port = gApp->config.server_port;
    gApp->network.address.sin_family = AF_INET;
    gApp->network.address.sin_addr.S_un.S_addr = INADDR_ANY;
    gApp->network.address.sin_port = htons(gApp->network.port);

    if(bind(gApp->network.handle, (const sockaddr*)&gApp->network.address, sizeof(sockaddr_in)) < 0)
    {
        ASC_Error("ASC_NetInit: Failed to bind socket to port %u", gApp->network.port);
        return 0;
    }
    
    ASC_InfoLog("ASC_NetInit: Bound network socket to port %u", gApp->network.port);

    #ifdef ASC_WINDOWS
    DWORD nonblocking = 1;
    if(ioctlsocket(gApp->network.handle, FIONBIO, &nonblocking) != 0)
    {
        ASC_Error("ASC_NetInit: Failed to set non-blocking socket");
        return 0;
    }

    #else
    int nonblocking = 1;
    if(fcntl(gApp->network.handle, F_SETFL, O_NONBLOCK, nonblocking) == -1)
    {
        ASC_Error("ASC_NetInit: Failed to set non-blocking socket");
        return 0;
    }
    #endif

    gApp->network.netThread.dataLock = SDL_CreateSemaphore(1);
    gApp->network.netThread.running = 1;
    gApp->network.netThread.thread = SDL_CreateThread(ASC_NetThread, "ASC_NetThread", (ptr)&gApp->network.netThread);

    return result;
}

static void ASC_NetQuit(void)
{
    SDL_WaitSemaphore(gApp->network.netThread.dataLock);
    gApp->network.netThread.running = 0;
    SDL_PostSemaphore(gApp->network.netThread.dataLock);
    SDL_WaitThread(gApp->network.netThread.thread, 0);

    #ifdef ASC_WINDOWS
    closesocket(gApp->network.handle);
    WSACleanup();
    #else
    close(gApp->network.handle);
    #endif
}

static bool ASC_NetFrame(void)
{
    SDL_WaitSemaphore(gApp->network.netThread.dataLock);
    u16 clientID = gApp->network.netThread.clientID;
    while(!gApp->network.netThread.localID) // using while so it can't be 0
    {
        gApp->network.netThread.localID = ASC_RANDU16;
    }
    u16 localID = gApp->network.netThread.localID;
    SDL_PostSemaphore(gApp->network.netThread.dataLock);

    if(!clientID)
    {
        if(gApp->timer.ticks % 50 == 0)
        {
            ASC_NetPacket packet = {0};
            packet.target = 1;
            packet.source = localID;
            packet.type = NETPACKET_CLIENT_REQUEST_DATA;
            packet.dataType = NETPACKET_DATA_ID;

            ASC_NetSendPacket(ASC_NetAddress(gApp->config.server_ip[0], gApp->config.server_ip[1], gApp->config.server_ip[2], gApp->config.server_ip[3], gApp->config.server_port), (ptr)&packet, sizeof(packet));

            ASC_InfoLog("ASC_NetFrame: Requested ID from server (localID %u)", localID);
        }
    }

    

    return 1;
}

static bool ASC_NetSendPacket(sockaddr_in dest, ptr data, int size)
{
    SDL_WaitSemaphore(gApp->network.netThread.dataLock);
    int bytesSent = sendto(gApp->network.handle, (const cstr)data, size, 0, (sockaddr*)&dest, (int)sizeof(sockaddr_in));
    SDL_PostSemaphore(gApp->network.netThread.dataLock);

    if(bytesSent != size)
    {
        ASC_Error("ASC_NetSendPacket: Failed to send packet");
        return 0;
    }

    ASC_DebugLog("ASC_NetSendPacket: Packet Sent [%u:%u] (%d bytes)", ntohl(dest.sin_addr.S_un.S_addr), ntohs(dest.sin_port), size);

    return 1;
}

static sockaddr_in ASC_NetAddress(u8 a, u8 b, u8 c, u8 d, u16 p)
{
    sockaddr_in result;
    memset(&result, 0, sizeof(sockaddr_in));
    u32 address = (a<<24) | (b<<16) | (c<<8) | d;
    result.sin_family = AF_INET;
    result.sin_addr.S_un.S_addr = htonl(address);
    result.sin_port = htons(p);
    return result;
}

int ASC_NetThread(ptr data)
{
    ASC_InfoLog("NetThread Spawned");
    ASC_NetThreadData *tdata = (ASC_NetThreadData*)data;
    bool running = 1;
    while(running)
    {
        SDL_Delay(20);
        SDL_WaitSemaphore(tdata->dataLock);
        running = tdata->running;

        ASC_NetPacket packet = {0};
        sockaddr_in from;
        #ifdef ASC_WINDOWS
        typedef int socklen_t;
        #endif
        socklen_t fromLength = sizeof(from);

        int bytes = recvfrom(gApp->network.handle, (cstr)&packet, sizeof(ASC_NetPacket), 0, (sockaddr*)&from, &fromLength);

        if(bytes > 0)
        {
            if(packet.source == 1)
            {
                if(!tdata->clientID) // not yet connected
                {
                    if(packet.target == tdata->localID)
                    {
                        if(packet.type == NETPACKET_SERVER_PROVIDE_DATA && 
                        packet.dataType == NETPACKET_DATA_ID)
                        {
                            if(packet.dataSize != sizeof(u16))
                            {
                                ASC_Error("NetThread: received invalid ID packet from server");
                            }
                            else
                            {
                                memcpy(&tdata->clientID, (ptr)packet.data, sizeof(u16));
                                ASC_InfoLog("NetThread: Connected to server with ID: %u", tdata->clientID);
                            }
                        }
                    }
                }
                else // connected to server (tdata->clientID > 0)
                {
                    switch(packet.type)
                    {
                        case NETPACKET_SERVER_REQUEST_DATA:
                        {
                            switch(packet.dataType)
                            {
                                case NETPACKET_DATA_USERLOGIN:
                                {
                                    //TODO
                                } break;

                                default:
                                {
                                    ASC_Error("NetThread: Server sent invalid packet datatype (%u)", packet.dataType);
                                } break;
                            }
                        } break;

                        case NETPACKET_SERVER_PROVIDE_DATA:
                        {

                        } break;
                     
                        default:
                        {
                            ASC_Error("NetThread: Server sent invalid packet type (%u)", packet.type);
                        } break;
                    }
                }
            }
            else //msg not from server
            {
                u32 fromAddress = ntohl(from.sin_addr.S_un.S_addr);
                u16 fromPort = ntohs(from.sin_port);
                ASC_DebugLog("NetThread: Packet Received [%u:%u] (%d bytes)",fromAddress, fromPort, bytes);
            }
        }

        SDL_PostSemaphore(tdata->dataLock);
    }

    ASC_InfoLog("NetThread Quit");
    return 1;
}

#endif //NETWORK_H_