//
//  wordMessage.h
//  proc
//
//  Created by 张 钊 on 12-11-15.
//
//

#ifndef __proc__wordMessage__
#define __proc__wordMessage__

#include <iostream>

class wordMessage
{
public:
	wordMessage();
	
	int upBoundary;
	int downBoundary;
	int leftBoundary;
	int rightBoundary;
	bool isMatrix;
	wordMessage* next;
};


#endif /* defined(__proc__wordMessage__) */
