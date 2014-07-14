//
//  WordSegmentation.h
//  proc
//
//  Created by 张 钊 on 12-11-11.
//	version 20130214

#ifndef __PROBTABLEELEMENT_H__
#define __PROBTABLEELEMENT_H__


class ProbTableElement
{
public:
	ProbTableElement(int idNum, double start, double end);
	
	int idNum;
	double start;
	double end;
	
};



#endif
