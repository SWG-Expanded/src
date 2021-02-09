// ConnectionServerConnection.cpp
// Copyright 2000-02, Sony Online Entertainment Inc., all rights reserved. 

//-----------------------------------------------------------------------
#include "FirstCustomerServiceServer.h"

#include "CustomerServiceServer.h"
#include "ConfigCustomerServiceServer.h"
#include "ConnectionServerConnection.h"
#include "serverNetworkMessages/GameConnectionServerMessages.h"
#include "sharedFoundation/NetworkIdArchive.h"
#include "sharedLog/Log.h"
#include "sharedMessageDispatch/Transceiver.h"
#include "sharedNetworkMessages/AppendCommentMessage.h"
#include "sharedNetworkMessages/CancelTicketMessage.h"
#include "sharedNetworkMessages/ConnectPlayerMessage.h"
#include "sharedNetworkMessages/CreateTicketMessage.h"
#include "sharedNetworkMessages/GetArticleMessage.h"
#include "sharedNetworkMessages/GetCommentsMessage.h"
#include "sharedNetworkMessages/GetTicketsMessage.h"
#include "sharedNetworkMessages/NewTicketActivityMessage.h"
#include "sharedNetworkMessages/RequestCategoriesMessage.h"
#include "sharedNetworkMessages/SearchKnowledgeBaseMessage.h"
#include "sharedNetwork/NetworkSetupData.h"
#include "UnicodeUtils.h"

#include "sharedFoundation/CrcConstexpr.hpp"

//-----------------------------------------------------------------------

ConnectionServerConnection::ConnectionServerConnection(const std::string & a, const unsigned short p) :
ServerConnection(a, p, NetworkSetupData())
{
	LOG("ConnectionServerConnection", ("Connection created...listening on (%s:%d)", a.c_str(), static_cast<int>(p)));
}

//-----------------------------------------------------------------------

ConnectionServerConnection::~ConnectionServerConnection()
{
	CustomerServiceServer::getInstance().connectionServerConnectionDestroyed(this);
}

//-----------------------------------------------------------------------

void ConnectionServerConnection::onConnectionClosed()
{
	LOG("ConnectionServerConnection", ("onConnectionClosed()"));
}

//-----------------------------------------------------------------------

void ConnectionServerConnection::onConnectionOpened()
{
	LOG("ConnectionServerConnection", ("onConnectionOpened()"));
}

//-----------------------------------------------------------------------

void ConnectionServerConnection::onReceive(const Archive::ByteStream & message)
{
	Archive::ReadIterator ri = message.begin();
	GameNetworkMessage m(ri);

	ri = message.begin();

	if (m.getType() == constcrc("GameClientMessage"))
	{
		GameClientMessage c(ri);
		Archive::ReadIterator cri = c.getByteStream().begin();
		GameNetworkMessage cm(cri);
		
		const uint32 messageType = cm.getType();

		std::vector<NetworkId>::const_iterator i;
		for (i = c.getDistributionList().begin(); i != c.getDistributionList().end(); ++i)
		{
			cri = c.getByteStream().begin();

			NetworkId const &networkId = (*i);

			switch(messageType) {
				case constcrc("RequestCategoriesMessage") :
				{
					RequestCategoriesMessage message(cri);
					CustomerServiceServer::getInstance().requestCategories(networkId, message.getLanguage());
					break;
				}
				case constcrc("ConnectPlayerMessage") :
				{
					ConnectPlayerMessage message(cri);
					CustomerServiceServer::getInstance().requestRegisterCharacter(networkId, this, message.getStationId());
					break;
				}
				case constcrc("DisconnectPlayerMessage") :
				{
					CustomerServiceServer::getInstance().requestUnRegisterCharacter(networkId);
					break;
				}
				case constcrc("CreateTicketMessage") :
				{
					CreateTicketMessage message(cri);

					CustomerServiceServer::getInstance().createTicket(networkId, 
						message.getStationId(), message.getCharacterName(),
						message.getCategory(), message.getSubCategory(),
						message.getDetails(), message.getHiddenDetails(),
						message.getHarassingPlayerName(),
						message.getLanguage(), message.isBug()
						);
					break;
				}
				case constcrc("AppendCommentMessage") :
				{
					AppendCommentMessage message(cri);
					CustomerServiceServer::getInstance().appendComment(networkId, message.getStationId(), message.getCharacterName(), message.getTicketId(), message.getComment());
					break;
				}
				case constcrc("CancelTicketMessage") :
				{
					CancelTicketMessage message(cri);
					CustomerServiceServer::getInstance().cancelTicket(networkId, message.getStationId(), message.getTicketId(), message.getComment());
					break;
				}
				case constcrc("GetTicketsMessage") :
				{
					GetTicketsMessage message(cri);
					CustomerServiceServer::getInstance().getTickets(networkId, message.getStationId());
					break;
				}
				case constcrc("GetCommentsMessage") :
				{
					GetCommentsMessage message(cri);
					CustomerServiceServer::getInstance().getComments(networkId, 
						message.getTicketId());
					break;
				}
				case constcrc("SearchKnowledgeBaseMessage") :
				{
					SearchKnowledgeBaseMessage message(cri);
					CustomerServiceServer::getInstance().searchKnowledgeBase(networkId, message.getSearchString(), message.getLanguage());
					break;
				}
				case constcrc("GetArticleMessage") :
				{
					GetArticleMessage message(cri);
					CustomerServiceServer::getInstance().getArticle(networkId, message.getId(), message.getLanguage());
					break;
				}
				case constcrc("NewTicketActivityMessage") :
				{
					NewTicketActivityMessage message(cri);

					CustomerServiceServer::getInstance().requestNewTicketActivity(networkId, message.getStationId());
					break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------

void ConnectionServerConnection::sendToClient(const NetworkId & clientId, const GameNetworkMessage & message) 
{
	static std::vector<NetworkId> v;
	v.clear();
	v.push_back(clientId);
	GameClientMessage msg(v, true, message);
	ServerConnection::send(msg, true);
}

//-----------------------------------------------------------------------
