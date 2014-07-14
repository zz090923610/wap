//
//  particlePosition.h
//  mapTest
//
//  Created by 张 钊 on 9/18/13.
//
//

#ifndef __mapTest__particlePosition__
#define __mapTest__particlePosition__

#include <iostream>


class particlePosition
{
public:
	particlePosition();
	particlePosition(int stepNum, int posX, int posY, int timeWhenStart);
	int stepNum;
	int posX;
	int posY;
	int timeWhenStart;

};


#endif /* defined(__mapTest__particlePosition__) */
