/*============================================================
 * Project: Ascencia
 * File: network.h
 * Author: Phraggers
 * Date: Tue May 14 2024
============================================================*/

#ifdef ASC_HEAD

typedef struct
{
    ENetHost *client;
} ASC_Network;

static bool ASC_NetInit(void);
static void ASC_NetQuit(void);
static bool ASC_NetHandleEvents(void);

#endif

/*=================================================================================*/

#ifdef ASC_IMPL

static bool ASC_NetInit(void)
{
    ASC_Network *network = &gApp->network;

    if(enet_initialize())
    {
        ASC_Error("ASC_NetInit: An error occurred while initializing ENet.");
        return 0;
    }
    else
    {
        ASC_InfoLog("ASC_NetInit: ENet Lib [%d.%d.%d]", ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH);
    }

    u64 maxOutgoingConnections = 1; //client only needs 1
    u64 numChannels = 2;
    u32 bandwidthIn = 0;
    u32 bandwidthOut = 0;

    network->client = enet_host_create(0, maxOutgoingConnections, numChannels, bandwidthIn, bandwidthOut);

    if(!network->client)
    {
        ASC_Error("ASC_NetInit: An error occurred while trying to create an ENet client.");
        return 0;
    }

    return 1;
}

static void ASC_NetQuit(void)
{
    enet_host_destroy(gApp->network.client);
    enet_deinitialize();
}

static bool ASC_NetHandleEvents(void)
{
    ASC_Network *network = &gApp->network;
    
    //TODO: move this from main thread to new thread
    ENetEvent event;
    while(enet_host_service(network->client, &event, 1) > 0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                ASC_DebugLog("ASC_NetEvent: Connected with peer: %x:%u", event.peer->address.host, event.peer->address.port);
                event.peer->data = "PeerInfo"; //store any relevant peer data here
            } break;

            case ENET_EVENT_TYPE_RECEIVE:
            {
                ASC_DebugLog("ASC_NetEvent: Received packet: size[%u bytes] from[%s] channel[%u]", event.packet->dataLength, event.peer->data, event.channelID);
                enet_packet_destroy(event.packet);
            } break;

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                ASC_DebugLog("ASC_NetEvent: Disconnected from %s", event.peer->data);
                event.peer->data = 0;
            } break;
        }
    }

    return 1;
}

#endif //NETWORK_H_