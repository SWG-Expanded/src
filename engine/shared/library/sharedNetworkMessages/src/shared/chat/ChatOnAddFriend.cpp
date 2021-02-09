// ChatOnAddFriend.cpp
// Copyright 2000-02, Sony Online Entertainment Inc., all rights reserved. 
// Author: Justin Randall

//-----------------------------------------------------------------------

#include "sharedNetworkMessages/FirstSharedNetworkMessages.h"
#include "sharedNetworkMessages/ChatOnAddFriend.h"

//-----------------------------------------------------------------------

ChatOnAddFriend::ChatOnAddFriend(const unsigned int s, const unsigned int r) :
GameNetworkMessage("ChatOnAddFriend"),
result(r),
sequence(s)
{
	addVariable(result);
	addVariable(sequence);
}

//-----------------------------------------------------------------------

ChatOnAddFriend::ChatOnAddFriend(Archive::ReadIterator & source) :
GameNetworkMessage("ChatOnAddFriend"),
result(),
sequence()
{
	addVariable(result);
	addVariable(sequence);
	unpack(source);
}

//-----------------------------------------------------------------------

ChatOnAddFriend::~ChatOnAddFriend()
{
}

//-----------------------------------------------------------------------

