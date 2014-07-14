//
//  wordMessage.cpp
//  proc
//
//  Created by 张 钊 on 12-11-15.
//
//

#include "wordMessage.h"
wordMessage::wordMessage()
{
	this -> upBoundary = 0;
	this -> downBoundary = 0;
	this -> leftBoundary = 0;
	this -> rightBoundary = 0;
	this -> isMatrix = false;
	this -> next = NULL;
}