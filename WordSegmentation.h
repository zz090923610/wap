//
//  WordSegmentation.h
//  proc
//
//  Created by 张 钊 on 12-11-11.
//	version 20130214

#ifndef __WORDSEGMENTATION_H__
#define __WORDSEGMENTATION_H__

#include <iostream>
#include <string>
#include "imageSet.h"
#include "ImagePreTreatment.h"

class WordSegmentation
{
public:
//function
	WordSegmentation(ImageSet * img, int posX, int posY, int width);
	~WordSegmentation();

////////////Request For Optimize 000002////////////////////////
	void chopToLine();//mark out every row positon
	void chopToWord();
	void convergence();
	void markMatrix();//mark out every matrix position
////////////End of Request For Optimize 000002/////////////////	
	
	void locateOuterBoundary();//mark out the outer boundary, for more info, check out issue RFO #000002

//variable
	
	int averageLengthOfLine;
	int numOfLine;

	
	
	ImageSet * currentImage;
	};



#endif /* defined(__proc__findMatrix__) */
