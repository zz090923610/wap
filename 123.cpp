bool bresenhanLineHitCheck( double ** data, int pt1x, int pt1y, int pt2x, int pt2y)
{
  
	int x = 0, y = 0, stemx = 0, stepy = 0, p = 0, i = 0, j = 0, k = 0, change = 0;
  	int tempx = 0, tempy = 0;
  	tempx = pt2x - pt1x;
	tempy = pt2y - pt1y;
	if(tempx > 0) stepx = 1;
	if(tempy > 0) stepy = 1;
	tempx = std::abs(tempx);
	tempy = std::abs(tempy);
	if(tempy > tempx)
	{
		int swap = tempx;
		tempx = tempy;
		tempy = swap;
		change = 1;
	}
	
	p = (tempy << 1) - tempx;
	x = pt1x;
	y = pt1y;
	
	for(i = 1; i <= tempx; i++)
	{
		if(data[x][y] < 255) return true;
	
	}
	if(p >= 0)
	{
		if(change == 0) y+= stepy;
		else x += stepx;
		p -= (tempx << 1);
	}
	if(change == 0)x += stepx;
	else y += stepy;
	p += (tempy << 1);
	return false;
	
}
