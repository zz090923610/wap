//	OCR part
//  WordSegmentation.cpp
//
//  Created by 张 钊 on 12-11-11.
//	version 20130214

#include "WordSegmentation.h"
#include "lineMessage.h"
#include <sstream>
#define segThreshold 0.985

WordSegmentation::WordSegmentation(ImageSet * img,int posX, int posY, int width)
{
	this->currentImage = img;
	this->averageLengthOfLine = 0;
	this->numOfLine = 0;
}

WordSegmentation::~WordSegmentation()
{
}



void WordSegmentation::chopToLine()
{

	for(int loop0 =0; loop0 < this->currentImage->height; loop0 ++)
	{
		double sumOfLine = 0;
		for(int loop1 = 0; loop1 < this->currentImage->width; loop1 ++)
		{
			sumOfLine += this->currentImage->data[loop0][loop1];
		}
		if(sumOfLine < segThreshold * this->currentImage->width * 255)
		{
			//mark a line
			int tempUpBoundary = loop0;
			int tempDownBoundary = loop0;
			
			for(int loop2 = loop0; loop2 < this->currentImage->height; loop2 ++)
			{
				double sumOfLine = 0;
				for(int loop3 = 0; loop3 < this->currentImage->width; loop3 ++)
				{
					sumOfLine += this->currentImage->data[loop2][loop3];
				}
				if(sumOfLine >= segThreshold * this->currentImage->width * 255)
				{
					tempDownBoundary = loop2;
					loop0 = loop2;
					if(this->currentImage-> line == NULL)
					{
						this->currentImage->line = new lineMessage;
						this->currentImage->lineHead = this->currentImage->line;
						this->currentImage->line ->next = NULL;
					}
					else
					{
						this->currentImage->line->next = new lineMessage;
						this->currentImage->line = this->currentImage->line->next;
						this->currentImage->line ->next = NULL;
					}
					this->currentImage->line->upBoundary = tempUpBoundary ;
					this->currentImage->line->downBoundary = tempDownBoundary;
					this->currentImage->line->length = this->currentImage->line->downBoundary - this->currentImage->line ->upBoundary;
					this->numOfLine ++;
					this->averageLengthOfLine += this->currentImage->line->length;
					

					std::cout<<"the line pos is :"<<this->currentImage->line->upBoundary<<" "<<this->currentImage->line->downBoundary<<std::endl;
					break;
				}
			}

		}
		//this->averageLengthOfLine /= this->numOfLine;
	}

}

void WordSegmentation::chopToWord()
{
	lineMessage * loop = this->currentImage->lineHead;
	while(loop != NULL)
	{
		for(int loop1 = 0; loop1 < this->currentImage->width; loop1 ++)
		{
			double sumOfLine = 0;
			for(int loop0 = loop->upBoundary; loop0 < loop->downBoundary; loop0 ++)
			{
				sumOfLine += this->currentImage->data[loop0][loop1];
			}
			
			if(sumOfLine < segThreshold * (loop->downBoundary - loop->upBoundary ) * 255)
			{
				int tempLeftBoundary = loop1;
				int tempRightBoundary = loop1;
				for(int loop3 = loop1; loop3< this->currentImage->width; loop3 ++)
				{
					double sumOfLine = 0;
					for(int loop2 = loop->upBoundary ; loop2 < loop->downBoundary; loop2 ++)
					{
						sumOfLine += this->currentImage->data[loop2][loop3];
					}
					if(sumOfLine >= segThreshold * (loop->downBoundary - loop->upBoundary ) * 255)
					{
						tempRightBoundary = loop3;
						loop1 = loop3;
						if(this->currentImage-> word == NULL)
						{
							this->currentImage-> word = new wordMessage;
							this->currentImage->wordHead = this->currentImage->word;
							this->currentImage->word ->next = NULL;
						}
						else
						{
							this->currentImage->word->next = new wordMessage;
							this->currentImage->word = this->currentImage->word->next;
							this->currentImage->word ->next = NULL;
						}
						this->currentImage->word->upBoundary = loop->upBoundary ;
						this->currentImage->word->downBoundary =loop ->downBoundary;
						this->currentImage->word->leftBoundary = tempLeftBoundary;
						this->currentImage->word->rightBoundary = tempRightBoundary;
						std::cout<<"the word pos is :"<<this->currentImage->word->upBoundary<<" "<<this->currentImage->word->downBoundary<<" "<<this->currentImage->word->leftBoundary<<" "<<this->currentImage->word->rightBoundary<<std::endl;
						break;

					}
					
				}
			}


		}
		loop = loop ->next;
	}
}


void WordSegmentation::convergence()
{
	wordMessage * tempWord = this->currentImage->wordHead;
	while(tempWord != NULL)
	{
		for(int loop0 = tempWord->upBoundary; loop0 <= tempWord->downBoundary; loop0 ++)
		{
			double sumOfLine = 0;
			for(int loop1 = tempWord->leftBoundary; loop1 <= tempWord->rightBoundary; loop1 ++)
			{
				sumOfLine += this->currentImage->data[loop0][loop1];
			}
			if(sumOfLine >= segThreshold * (tempWord->rightBoundary - tempWord->leftBoundary + 1) * 255)
			{
				tempWord->upBoundary = loop0;
			}
			else break;
		}
		for(int loop0 = tempWord ->downBoundary; loop0 >= tempWord->upBoundary; loop0 --)
		{
			double sumOfLine = 0;
			for(int loop1 = tempWord->leftBoundary; loop1 <= tempWord->rightBoundary; loop1 ++)
			{
				sumOfLine += this->currentImage->data[loop0][loop1];
			}
			if(sumOfLine >= segThreshold * (tempWord->rightBoundary - tempWord->leftBoundary + 1) * 255)
			{
				tempWord->downBoundary = loop0;
			}
			else break;
		}
		
		tempWord = tempWord->next;
	}


}


void WordSegmentation::locateOuterBoundary()
{
	double * compressedX = new double [ this->currentImage->width];
	double * compressedY = new double [ this->currentImage->height];
	for(int loop = 0 ;loop < this->currentImage->width; loop ++)
	{
		compressedX [loop] = 0;
		for(int loop2 = 0; loop2 < this->currentImage->height; loop2 ++)
		{
			compressedX[loop] += this->currentImage->data[loop2][loop];
		}
		compressedX[loop] /= this->currentImage->width;
	}
	for(int loop = 0; loop < this->currentImage ->height; loop ++)
	{
		compressedY [loop] = 0;
		for(int loop2 = 0; loop2 < this->currentImage->width; loop2 ++)
		{
			compressedY[loop] += this->currentImage->data[loop][loop2];
		}
		compressedY[loop] /= this->currentImage->width;
	}
	
	for(int loop = 0 ;loop < this->currentImage->width; loop ++)
	{
		if(compressedX[loop] != 255)
		{
			this->currentImage ->dataLeftColumn = loop;
			break;
		}
	}
	for(int loop = this->currentImage->width - 1; loop >= 0; loop --)
	{
		if(compressedX[loop] != 255)
		{
			this->currentImage ->dataRightColumn = loop;
			break;
		}
	}
	for(int loop = 0 ;loop < this->currentImage->height; loop ++)
	{
		if(compressedY[loop] != 255)
		{
			this->currentImage ->dataUpRow = loop;
			break;
		}
	}
	for(int loop = this->currentImage->height - 1; loop >= 0; loop --)
	{
		if(compressedY[loop] != 255)
		{
			this->currentImage ->dataDownRow = loop;
			break;
		}
	}
	delete compressedX;
	delete compressedY;
}


void WordSegmentation::markMatrix()
{
	/*old version, test result is so terrible 
	 
	lineMessage * temp = this->currentImage->lineHead;
	while(temp != NULL)
	{
		std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
		std::cout<<"Current line : "<<temp->upBoundary<<" "<<temp->downBoundary<<std::endl;
		double lineAve = 0;
		double * compressedTemp = new double [this->windowNum];
		for(int loop = 0; loop < this->windowNum ; loop ++)	compressedTemp [loop] = 0;
		for(int loop1 = 0; loop1 <this->windowNum;loop1 ++)
		{
			for(int loop0 = temp->upBoundary; loop0 < temp->downBoundary; loop0 ++)
			{
				compressedTemp [loop1]+= this->compressed[loop0][loop1];
			}
		}
		for(int loop = 0; loop < this->windowNum; loop ++)
			lineAve += compressedTemp[loop];
		lineAve /= this->windowNum;
		std::cout<<" line ave : "<<lineAve<<std::endl;
		for(int loop0 = 0; loop0 < this->windowNum; loop0 ++)
		{
			if(compressedTemp[loop0] >= lineAve)
			{
				//std::cout<<compressedTemp[loop0]<< " ";
				double leftBrackleLength = compressedTemp[loop0];
				double rightBrackleLength = 0;
				for(int loop1 = loop0 + 1; loop1 < this->windowNum; loop1 ++)
				{
					if( (leftBrackleLength - compressedTemp[loop1]) / leftBrackleLength <=0.05)
					{
						rightBrackleLength = compressedTemp[loop1];
						if(this->currentImage->matrix == NULL)
						{
							this->currentImage->matrix = new wordMessage;
							this->currentImage->matrixHead = this->currentImage->matrix;
							this->currentImage->matrix ->next = NULL;
						}
						else
						{
							this->currentImage->matrix->next = new wordMessage;
							this->currentImage->matrix = this->currentImage->matrix->next;
							this->currentImage->matrix ->next = NULL;
						}
						this->currentImage-> matrix->upBoundary = temp->upBoundary ;
						this->currentImage->matrix->downBoundary = temp->downBoundary;
						this->currentImage->matrix->leftBoundary = loop0 * this->windowWidth;
						this->currentImage->matrix->rightBoundary = (loop1 + 1) * this->windowWidth;
						std::cout<<"Matrix pos is : "<<this->currentImage->matrix->upBoundary<<" "<<this->currentImage->matrix->downBoundary<<" "<<this->currentImage->matrix->leftBoundary<<" "<<this->currentImage->matrix->rightBoundary<<std::endl;
						loop0 = loop1 + 1;
						break;
					}
					if(rightBrackleLength == 0)
					{
						
					}
				}

			}
			
			
		}

		temp = temp->next;
		delete  compressedTemp;
	}
	 end of old version */
	
	//new version is not yet finished
	

}










