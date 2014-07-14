//	OCR part
//  image.h
//
//  Created by 张 钊 on 12-10-27.
//  Copyright (c) 2012年 张 钊. All rights reserved.
//	version 20130214

#ifndef __imageSet____imageSet__
#define __imageSet____imageSet__

#include "wallElement.h"
#include "Constants.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <fstream>

using std::string;

class ImageSet
{
public:
//function part
	//constructor(without arg)
	ImageSet();
	ImageSet(string fileName);//constructor(with arg file name)
	~ImageSet();//destructor

	
	
	void drawMap();

	void loadCanvas(int row, int col, int type);
	void loadMap(string path);
	void loadDoorPosMap(string path);
//variable part
	
	int width, height, pixelNum;             //image size parameters
	string name;		//image name
	int wallNumber;
	wallElement * mapData;
	IplImage * canvas;
	int ** grayMap;
	int ** doorPosMap;


	
	
};
#endif
