//
//  lineMessage.h
//  proc
//
//  Created by 张 钊 on 12-11-15.
//
//

#ifndef __proc__lineMessage__
#define __proc__lineMessage__

#include <iostream>

class lineMessage
{
public:
	lineMessage();
	
	int upBoundary;
	int downBoundary;
	int length;
	lineMessage * next;
};


#endif /* defined(__proc__lineMessage__) */
