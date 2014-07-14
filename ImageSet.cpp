//	OCR Part
//  imageSet.cpp
//
//  Created by 张 钊 on 12-10-27.
//  Copyright (c) 2012年 张 钊. All rights reserved.
//	version 20130214

#include "ImageSet.h"


ImageSet::ImageSet()
{
	this -> width = 0, this ->height = 0, this->pixelNum = 0;
	this->name = "NULL";
	this->mapData = NULL;
	this ->canvas = NULL;
	this->grayMap = NULL;
	this->doorPosMap = NULL;

}//end of InageSet()

ImageSet::ImageSet(string fileName)
{
	this -> width = 0, this ->height = 0, this->pixelNum = 0;
	this->name = fileName;
	this->mapData = NULL;
	this ->canvas = NULL;
	this->grayMap = NULL;
	this->doorPosMap = NULL;

}//end of ImageSet(string fileName)

ImageSet::~ImageSet()
{
	delete this->mapData;
	std::cout<<"map data cleaned\n";
	if(this->grayMap != NULL)
	{
		for(int loop = 0;loop < this->height; loop ++ )
		{
			if(this->grayMap[loop] != NULL) delete this->grayMap[loop];
		}
	}
	if(this->doorPosMap != NULL)
	{
		for(int loop = 0;loop < this->height; loop ++ )
		{
			if(this->doorPosMap[loop] != NULL) delete this->doorPosMap[loop];
		}
	}
	std::cout<<"gray data cleaned\n";
}//end of ~ImageSet()


void ImageSet::loadMap(string path)
{
	this->wallNumber = 0;
	std::ifstream fin;
	fin.open(path.c_str(),std::ios::in);
	if(!fin.is_open())
	{
		std::cout<< "Wrong Path\n";
		fin.clear();
	}
	while(!fin.eof())
	{
		 wallElement temp ;
		fin >> temp.topLeftX >>temp.topLeftY >> temp.downRightX >> temp.downRightY;
		
		this->wallNumber ++;
	}
	this->wallNumber --;
	std::cout<<this->wallNumber<<"\n";
	fin.clear();
	fin.close();
	
	this->mapData = new wallElement[this->wallNumber];
	fin.open(path.c_str(), std::ios::in);
	for(int loop = 0; loop < this->wallNumber; loop ++)
	{
		fin >>this->mapData[loop].topLeftX >>this->mapData[loop].topLeftY >>this->mapData[loop].downRightX >>this->mapData[loop].downRightY;
		this->mapData[loop].topLeftX /= ZOOM_SCALE;
		this->mapData[loop].topLeftY /= ZOOM_SCALE;
		this->mapData[loop].downRightX /= ZOOM_SCALE;
		this->mapData[loop].downRightY /= ZOOM_SCALE;
		if(this->mapData[loop].downRightX > this->height) this->height = this->mapData[loop].downRightX;
		if(this->mapData[loop].downRightY > this->width) this->width = this->mapData[loop].downRightY;
		
	}
	fin.clear();
	fin.close();
	
	this->grayMap = new int *[this ->height];
	for(int loop = 0; loop < height; loop ++)
	{
		this ->grayMap [loop] = new int [this->width];
		for(int loopy = 0; loopy < this->width; loopy ++)
		{
			this->grayMap[loop][loopy] = 255;
		}
	}
	
	for(int loopWall = 0; loopWall < this->wallNumber; loopWall ++)
	{
		for(int loopx = this->mapData[loopWall].topLeftX; loopx < this->mapData[loopWall].downRightX; loopx ++)
		{
			for(int loopy = this->mapData[loopWall].topLeftY; loopy < this->mapData[loopWall].downRightY; loopy ++)
			{
				this->grayMap[loopx][loopy] = 0;
			}
		}
	}
	
	
	
}

void ImageSet::loadCanvas(int row, int col, int type)
{
	this->canvas = cvCreateImage(cvSize(this->width, this->height),8, 1);
}

void ImageSet::drawMap()
{
	CvScalar iter;
	iter.val[0] = 255;
	iter.val[1] = 0, iter.val[2] = 0, iter.val[3] = 0;
	for (int loopx=0; loopx<this->height; loopx++)
	{
		for (int loopy=0; loopy<this->width; loopy++)
		{
			cvSet2D (this->canvas, loopx,loopy,iter);
		}
	}
	iter.val[0] = 200;
	for(int loopWall = 0; loopWall < this->wallNumber; loopWall ++)
	{
		for(int loopx = this->mapData[loopWall].topLeftX; loopx < this->mapData[loopWall].downRightX; loopx ++)
		{
			for(int loopy = this->mapData[loopWall].topLeftY; loopy < this->mapData[loopWall].downRightY; loopy ++)
			{
				cvSet2D (this->canvas, loopx,loopy,iter);
			}
		}
	}
	iter.val[0] = 240;
	for(int loopx = 0; loopx < this->height; loopx ++)
	{
		for(int loopy = 0; loopy < width; loopy ++)
		{
			if(this->doorPosMap[loopx][loopy] < 255)
			{
				cvSet2D (this->canvas, loopx,loopy,iter);
				//std::cout<< loopx<<" "<<loopy<<" "<<this->doorPosMap[loopx][loopy]<<" ";
			}
		}
	}
	

}

void ImageSet::loadDoorPosMap(string path)
{
	
	int doorNumber = 0;
	std::ifstream fin;
	fin.open(path.c_str(),std::ios::in);
	if(!fin.is_open())
	{
		std::cout<< "Wrong Path\n";
		fin.clear();
	}
	while(!fin.eof())
	{
		std::string tmp1, tmp2;
		int tmp3,tmp4, tmp5, tmp6, doorLeft, doorRight, doorUp, doorDown;
		fin >> tmp1 >> tmp2 >> tmp3 >> tmp4 >> tmp5 >> tmp6 >> doorLeft >> doorRight >> doorUp >> doorDown;

		
		doorNumber ++;
	}
	std::cout<<doorNumber<<"\n";
	fin.clear();
	fin.close();
	
	this->doorPosMap = new int *[this ->height];
	for(int loop = 0; loop < height; loop ++)
	{
		this ->doorPosMap [loop] = new int [this->width];
		for(int loopy = 0; loopy < this->width; loopy ++)
		{
			this->doorPosMap[loop][loopy] = 255;
		}
	}
	
	fin.open(path.c_str(), std::ios::in);
	std::cout<<"door\n";
	for(int loop = 0; loop < doorNumber; loop ++)
	{
		std::string tmp1, tmp2;
		int tmp3,tmp4, tmp5, tmp6, doorLeft, doorRight, doorUp, doorDown;
		fin >> tmp1 >> tmp2 >> tmp3 >> tmp4 >> tmp5 >> tmp6 >> doorLeft >> doorRight >> doorUp >> doorDown;
		std::cout << doorLeft<<" "<<doorRight<<" "<<doorUp<<" "<<doorDown<<"\n";
		for(int loopx = doorUp; loopx < doorDown; loopx ++)
		{
			for(int loopy = doorLeft; loopy < doorRight; loopy ++)
			{
				this->doorPosMap[loopx][loopy] = 0;
				
			}
		}
		
	}
	std::cout<<"door\n";
	fin.clear();
	fin.close();

}







