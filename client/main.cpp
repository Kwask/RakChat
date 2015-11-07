#include <iostream>
#include <string.h>
#include <thread>
#include "../raknet/RakPeerInterface.h"
#include "../raknet/MessageIdentifiers.h"
#include "../raknet/BitStream.h"
#include "../raknet/RakNetTypes.h"  // MessageID
#include "../_defines.cpp"

void sendRakMessage( std::string message, RakNet::RakPeerInterface* peer, RakNet::SystemAddress address = RakNet::UNASSIGNED_SYSTEM_ADDRESS )
{
	RakNet::BitStream outgoing;
	outgoing.Write(( RakNet::MessageID )ID_GAME_MESSAGE );
	outgoing.Write( message.c_str() );
	peer->Send( &outgoing, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, true );
}

void handlePacket( RakNet::Packet* packet, RakNet::RakPeerInterface* peer, std::string username )
{
	switch( packet->data[0] )
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
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf( "Our connection request has been accepted.\n" );

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				std::string message = username + " has connected.";
				sendRakMessage( message, peer );
			}
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf( "The server is full.\n" );
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			printf( "We have been disconnected.\n" );
			break;
		case ID_CONNECTION_LOST:
			printf( "Connection lost.\n" );
			break;
		case ID_GAME_MESSAGE:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn( packet->data, packet->length, false );
				bsIn.IgnoreBytes( sizeof( RakNet::MessageID ));
				bsIn.Read( rs );
				printf( "%s\n", rs.C_String() );
			}
			break;
		default:
			printf( "Message with identifier %i has arrived.\n", packet->data[0] );
			break;
	}
}

void userInput( RakNet::RakPeerInterface* peer, std::string username )
{
	while( true )
	{
		std::string input;

		getline( std::cin, input );
		
		std::string message = username + ": " + input;
		sendRakMessage( message, peer );
	}
}

int main(void)
{
	std::string input;
	std::string username;

	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet *packet;

	RakNet::SocketDescriptor sd;
	peer->Startup( 1, &sd, 1 );

	printf( "Enter server IP or hit enter for 127.0.0.1\n" );
	std::getline( std::cin, input );

	if( !input.length() )
	{
		input = "127.0.0.1";
	}

	printf( "Starting the client.\n" );
	peer->Connect( input.c_str(), SERVER_PORT, 0, 0 );

	printf( "Enter a username\n" );
	std::getline( std::cin, username );

	std::thread chat( userInput, peer, username );

	while( true )
	{
		for( packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive() )
		{
			handlePacket( packet, peer, username );
		}
	}

	chat.join();

	RakNet::RakPeerInterface::DestroyInstance( peer );

	return 0;
}
