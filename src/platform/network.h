/*============================================================
 * Project: Ascencia
 * File: network.h
 * Author: Phraggers
 * Date: Tue May 14 2024
============================================================*/

#ifdef ASC_HEAD

typedef struct
{
    bool running;
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

    gApp->network.port = NETWORK_PORT;
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
    if(gApp->timer.ticks % 50 == 0)
    {
        ASC_NetSendPacket(ASC_NetAddress(127,0,0,1, 32000), (ptr)"hello world", 12);
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

    ASC_InfoLog("ASC_NetSendPacket: Packet Sent [%u:%u] (%d bytes)", ntohl(dest.sin_addr.S_un.S_addr), ntohs(dest.sin_port), size);

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

        u8 packetData[256];
        u32 maxPacketSize = sizeof(packetData);
        sockaddr_in from;
        #ifdef ASC_WINDOWS
        typedef int socklen_t;
        #endif
        socklen_t fromLength = sizeof(from);

        int bytes = recvfrom(gApp->network.handle, (cstr)packetData, maxPacketSize, 0, (sockaddr*)&from, &fromLength);

        if(bytes > 0)
        {
            u32 fromAddress = ntohl(from.sin_addr.S_un.S_addr);
            u16 fromPort = ntohs(from.sin_port);
            ASC_InfoLog("NetThread: Packet Received [%u:%u] (%d bytes)",fromAddress, fromPort, bytes);
        }

        SDL_PostSemaphore(tdata->dataLock);
    }

    ASC_InfoLog("NetThread Quit");
    return 1;
}

#endif //NETWORK_H_