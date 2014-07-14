//	OCR part
//  ImagePreTreatment.h
//
//  Created by 张 钊 on 12-10-29.
//  Copyright (c) 2012年 张 钊. All rights reserved.
//	version 20130214


#ifndef __IMAGEPRETREATMENT_H_
#define __IMAGEPRETREATMENT_H_
#include "ImageSet.h"
#include <cv.h>
#include <highgui.h>

class ImagePreTreatment
{
public:
//function
	ImagePreTreatment(ImageSet * image);
	ImagePreTreatment();
	~ImagePreTreatment();//destructor
	
/////////////Request For Optimize 000001//////////////////////////////////////
	
	void toBinary(double lambda, double alpha,int scale);//function to convert data from gray_scale to binary, unperfect version
	void calcGrayAve();//calculate the average gray value of the whole image
	void shadowProc();//shadow treatment, unperfect version
	void tiltRectification();// tilt Rectification function, not yet started
////////////End of Request For Optimize 000001////////////////////////////////
	
	
	
//variable
	int numOfWindows;
	ImageSet * currentImage;
//////////variables for RFO 000001/////////////////
	
	double * compressedX;//used for calculate average gary value of a pixel row
	double * compressedY;//used for calculate average gary value of a pixel column
	long double grayAve;//average gray value of the whole image
	
//////////End of variables for RFO 000001//////////

};


#endif