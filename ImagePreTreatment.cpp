//	OCR part
//  ImagePreTreatment.cpp
//
//  Created by 张 钊 on 12-10-29.
//  Copyright (c) 2012年 张 钊. All rights reserved.
//	version 20130214

#include "ImagePreTreatment.h"
#include "ImageSet.h"
#include <cmath>
ImagePreTreatment::ImagePreTreatment(ImageSet * image)
{
	this->numOfWindows = 0;
	this->currentImage = image;
	this->grayAve = 0;
	this->compressedX = new double [ image->width];
	this->compressedY = new double [ image->height];
	for(int loop = 0 ;loop < image->width; loop ++)
	{
		this->compressedX [loop] = 0;
		for(int loop2 = 0; loop2 < image->height; loop2 ++)
		{
			this->compressedX[loop] += image->data[loop2][loop];
		}
		this->compressedX[loop] /= image->width;
	}
	for(int loop = 0; loop < image ->height; loop ++)
	{
		this->compressedY [loop] = 0;
		for(int loop2 = 0; loop2 < image->width; loop2 ++)
		{
			this->compressedY[loop] += image->data[loop][loop2];
		}
		this->compressedY[loop] /= image->width;
	}
}
ImagePreTreatment::ImagePreTreatment()
{
	this->numOfWindows = 0;
	this->currentImage = NULL;
	this->compressedX = NULL;
	this->compressedY = NULL;
	std::cerr<<"No image pointer referred\n";
	exit(1);
	
}
ImagePreTreatment::~ImagePreTreatment()
{
	if(this->compressedX!= NULL )
		delete  this->compressedX;
	if(this->compressedY != NULL)
		delete  this->compressedY;
	
}
void ImagePreTreatment::calcGrayAve()
{
	double pixelNum = this->currentImage->height * this->currentImage->width;
	for(int loop0 = 0 ;loop0 < this->currentImage->height;loop0++)
	{
	    for(int loop1=0; loop1 < this->currentImage->width;loop1++)
	    {
			this -> grayAve += this->currentImage->data[loop0][loop1];
	    }
	}
	this->grayAve /= pixelNum;
	std::cout<<"gary ave "<<grayAve<<std::endl;
}

void ImagePreTreatment::toBinary(double lambda, double alpha, int scale)
{
	
	 double lagestSquareRatio = 0;
	 double selectedThresholdValue = 0;
	 double pixelNum = this->currentImage->height * this->currentImage->width;
	 
	 long double variance = 0,variance1 = 0, variance2 = 0;
	 //calc gray average
	 calcGrayAve();
	 
	for(double thresholdValue = 1; thresholdValue <=255;thresholdValue ++)
	{
	 
	  double eitaSquareW = 0;
	  double eitaSquareB = 0;
	  variance = 0, variance1 = 0, variance2 = 0;
	  long double u1 =0, u2 = 0;
	  double numOfU1 = 0;
	  for(int loop0 = 0 ;loop0 < this->currentImage->height;loop0++)
	  {
	    for(int loop1=0; loop1 < this->currentImage->width;loop1++)
	    {
		  
		  if(this->currentImage->data[loop0][loop1] <= thresholdValue)
		  {
		      u1+= this->currentImage->data[loop0][loop1];
		      variance1 += (this->currentImage->data[loop0][loop1] -this -> grayAve) * (this->currentImage->data[loop0][loop1] -this -> grayAve);
		      numOfU1++;
		  }
		  else
		  {
		    u2+= this->currentImage->data[loop0][loop1];
		    variance2 += (this->currentImage->data[loop0][loop1] - this -> grayAve) * (this->currentImage->data[loop0][loop1] - this -> grayAve);
		  }
	    }
	  }
	  if(numOfU1 == 0 || numOfU1 == pixelNum)  continue;
		variance1 /= numOfU1;
		variance2 /= (pixelNum - numOfU1);
		variance = (variance1 + variance2) / 2;
		u1 /= numOfU1;
	  u2 /= (pixelNum - numOfU1);
	  for(int loop0 = 0 ;loop0 < this->currentImage->height;loop0++)
	  {
	    for(int loop1=0; loop1 < this->currentImage->width;loop1++)
	    {
		  if(this->currentImage->data[loop0][loop1] <= thresholdValue)
		  {
		     eitaSquareW += (this->currentImage->data[loop0][loop1] - u1) *
				    (this->currentImage->data[loop0][loop1] - u1) ;
		      eitaSquareB += (u1 - this -> grayAve) * (u1 - this -> grayAve) * numOfU1;
		  }
		  else
		  {
		    eitaSquareW += (this->currentImage->data[loop0][loop1] - u2) *
				    (this->currentImage->data[loop0][loop1] - u2); 
		    eitaSquareB += (u2 - this -> grayAve) * (u2 - this -> grayAve) *(pixelNum - numOfU1);
		  }
	    }
	  }
	  if(eitaSquareB / eitaSquareW > lagestSquareRatio)
	  {
	    selectedThresholdValue = thresholdValue;
		lagestSquareRatio =  eitaSquareB / eitaSquareW ;
	  }
	 
	}
	 std::cout<<"Threshold "<<selectedThresholdValue<<std::endl;
  //window operation
	for(int loop =0; loop < this->currentImage->height; loop+= 20)
	{
		for(int loop1 = 0; loop1 < currentImage -> width; loop1 += 20)
		{
		  long double windowVariance = 0;
		  long double windowAve = 0;
		  int x = 0 , y = 0;
		  for(x = 0; x < 20 && loop + x < this->currentImage->height ; x ++)
		  {
		    for(y = 0; y < 20 && loop1 + y < this->currentImage->width; y++)
		      windowAve += this->currentImage->data[loop + x][loop1 + y];
		  }
		  windowAve /= (x * y);
		
		  for(x = 0; x < 20 && loop + x < this->currentImage->height; x ++)
		  {
		    for(y = 0; y < 20 && loop1 + y < this->currentImage->width; y++)
		    {
		      windowVariance+= (this->currentImage->data[loop + x][loop1 + y] - windowAve) * (this->currentImage->data[loop + x][loop1 + y] - windowAve);
	
		    }
		  }
		  windowVariance /= (x * y);
		  
		  //shadow
		  int justiceValueX = 0;
		  for(int loop = 0; loop < this->currentImage->height; loop++)
		  {
		      justiceValueX += this->compressedY[loop];
		  }
		  justiceValueX /= this->currentImage->height;
		  
		  
		  int justiceValueY = 0;
		  for(int loop = 0; loop < this->currentImage->width; loop++)
		  {
			 justiceValueY += this->compressedY[loop];
		  }
			  justiceValueY /= this->currentImage->width;
		  
		  
		  
		  for(int x = 0; x < 20 && loop + x < this->currentImage->height; x ++)
		  {
		    for(int y = 0; y < 20 && loop1 + y < this->currentImage->width; y++)
		    {
				if(windowVariance < lambda * variance)
				{
					if(this->currentImage->data[loop + x][loop1 + y] > selectedThresholdValue && this->currentImage->data[loop + x][loop1 + y] > justiceValueX)
						this->currentImage->data[loop + x][loop1 + y] -= (this->currentImage->data[loop + x][loop1 + y] - justiceValueX) /scale;
					if(this->currentImage->data[loop + x][loop1 + y] <= selectedThresholdValue && this->currentImage->data[loop + x][loop1 + y] < justiceValueX)
						this->currentImage->data[loop + x][loop1 + y] +=  (justiceValueX - this->currentImage->data[loop + x][loop1 + y]) /scale ;
					
				}
			 	if(windowVariance >= lambda * variance)
				//else
				{
					if(this->currentImage->data[loop + x][loop1 + y]>= (1 + alpha) * selectedThresholdValue && this->currentImage->data[loop + x][loop1 + y] > justiceValueX)
						this->currentImage->data[loop + x][loop1 + y] -= (this->currentImage->data[loop + x][loop1 + y] - justiceValueX) /scale;
					else if(this->currentImage->data[loop + x][loop1 + y]<= (1 - alpha) * selectedThresholdValue&& this->currentImage->data[loop + x][loop1 + y] < justiceValueX)
					this->currentImage->data[loop + x][loop1 + y] +=  (justiceValueX - this->currentImage->data[loop + x][loop1 + y]) /scale;
					else ;/////////////////////////////
				}
			
		    }
		  }

		}
	}
  	calcGrayAve();
	for(int loop0 = 0; loop0 < this->currentImage->height; loop0++)
	{
		for(int loop1 = 0; loop1 < this->currentImage->width; loop1 ++)
		{
			this->currentImage->data[loop0][loop1] += (this->currentImage->data[loop0][loop1] - grayAve) * 150/ 255;
		}
	}
	IplImage * test =  this->currentImage->toOpenCV();
	IplImage * dst = cvCreateImage(cvGetSize(test),8,1);
	cv::Mat dsti = dst;
	cv::Mat testi = test;
	cv::threshold(testi,dsti,cvMean(test)* 0.9 , 255,CV_THRESH_BINARY);
	this->currentImage->fromOpenCV(dst);
	this->currentImage->isBinary = true;
}



void ImagePreTreatment::shadowProc()
{
  int justiceValue = 0;
  for(int loop = 0; loop < this->currentImage->height; loop++)
  {
    justiceValue += this->compressedY[loop];
  }
  justiceValue /= this->currentImage->height;
  for(int loop0 = 0; loop0 < this->currentImage->height; loop0++)
  {
    for(int loop1 = 0; loop1 < this->currentImage->width;loop1++)
    {
      if(this->compressedY[loop0] < justiceValue)
	this->currentImage->data[loop0][loop1] += (justiceValue - this->compressedY[loop0])/4 ;
      if(this->compressedY[loop0] > justiceValue)
	this->currentImage->data[loop0][loop1] -= ( this->compressedY[loop0] - justiceValue) /4;
    }
  }
  justiceValue = 0;
   for(int loop = 0; loop < this->currentImage->width; loop++)
  {
    justiceValue += this->compressedY[loop];
  }
  justiceValue /= this->currentImage->width;
  for(int loop0 = 0; loop0 < this->currentImage->height; loop0++)
  {
    for(int loop1 = 0; loop1 < this->currentImage->width;loop1++)
    {
      if(this->compressedX[loop0] < justiceValue)
	this->currentImage->data[loop0][loop1] += (justiceValue - this->compressedX[loop0])/4;
      if(this->compressedX[loop0] > justiceValue)
	this->currentImage->data[loop0][loop1] -= ( this->compressedX[loop0] - justiceValue)/4;
    }
  }
}


void ImagePreTreatment::tiltRectification()
{
	// not yet started

}






