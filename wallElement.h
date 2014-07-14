//
//  wallElement.h
//  mapTest
//
//  Created by 张 钊 on 9/2/13.
//
//

#ifndef __mapTest__wallElement__
#define __mapTest__wallElement__

#include <iostream>

class wallElement
{
public:
	wallElement();
	wallElement(int tlX,int tlY, int drX, int drY);

		int topLeftX;
		int topLeftY;
		int downRightX;
		int downRightY;
	
};

#endif /* defined(__mapTest__wallElement__) */
