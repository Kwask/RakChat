#include <stdio.h>
#include <string.h>
#include "../raknet/RakPeerInterface.h"
#include "../raknet/MessageIdentifiers.h"
#include "../raknet/BitStream.h"
#include "../raknet/RakNetTypes.h"  // MessageID
#include "../_defines.cpp"

void handlePacket( RakNet::Packet* packet, RakNet::RakPeerInterface* peer )
{
	switch (packet->data[0])
	{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf( "Another client has disconnected.\n" );
			break;
		case ID_REMOTE_CONNECTION_LOST:
			printf( "Another client has lost the connection.\n" );
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf( "Another client has connected.\n" );
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf( "A connection is incoming.\n" );
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf( "The server is full.\n" );
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			printf( "A client has disconnected.\n" );
			break;
		case ID_CONNECTION_LOST:
			printf( "A client lost the connection.\n" );
			break;
		case ID_GAME_MESSAGE:
			{
				RakNet::BitStream bs( packet->data, packet->length, false );
				peer->Send( &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true ); 
			}
			break;
		default:
			printf( "Message with identifier %i has arrived.\n", packet->data[0] );
			break;
	}
}

int main(void)
{
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	RakNet::SocketDescriptor sd( SERVER_PORT,0 );
	peer->Startup( MAX_CLIENTS, &sd, 1 );
	isServer = true;

	printf( "Starting the server.\n" );
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections( MAX_CLIENTS );

	while (1)
	{
		for( packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive() )
		{
			handlePacket( packet, peer );
		}
	}

	RakNet::RakPeerInterface::DestroyInstance( peer );

	return 0;
}
