//
//  wifiVector.h
//  mapTest
//
//  Created by 张 钊 on 9/29/13.
//
//

#ifndef __mapTest__wifiVector__
#define __mapTest__wifiVector__

#include <iostream>

class wifiVector
{
public:
	wifiVector();
	wifiVector(long timeStamp, bool isStable, int orientation);
	long timeStamp;
	bool isStable;
	int orientation;
};

#endif /* defined(__mapTest__wifiVector__) */
