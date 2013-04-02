#pragma once

using namespace std;

extern CPoint Source, Dest;
extern int dHeuristic;
extern int nrInClosedList, dNrColumns, dNrRounds;
extern BYTE* Map;

#define MANHATTAN			0
#define EUCLIDIAN			1
#define DIAGONAL_SHORTCUT	2
#define NO_HEURISTIC		3

#define D_UP		1
#define D_DOWN		2
#define D_LEFT		4
#define D_RIGHT		8

struct NODE
{
	int x,y;
	int f, g,h;
	//STATE_BLOCK, STATE_FAILED, STATE_THEREBEFORE, STATE_FREE
	//the no. in the closed list
	int dParent;
	//only for display purpose:
	int nrTries;
/*	bool operator > (const NODE& node) const {
		return g > node.g;}
	bool operator < (const NODE& node) const {
		return g > node.g;}
	bool operator == (const NODE& node) const {
		return g == node.g;}*/
};


inline int heuristic(int x1, int y1, int x2, int y2)
{
	if (dHeuristic == MANHATTAN)
		return (10 * (abs(x1 - x2) + abs(y1 - y2)));
	else if (dHeuristic == EUCLIDIAN)
	{
		return ((int)(10 * sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
	}

	else if (dHeuristic == DIAGONAL_SHORTCUT)
	{
		int xDistance = abs(x1 - x2);
		int yDistance = abs(y1 - y2);
		if (xDistance > yDistance)
			return 14 * yDistance + 10 * (xDistance - yDistance);
		else return 14* xDistance + 10 * (yDistance - xDistance);
	}

	else if (dHeuristic == NO_HEURISTIC)
	{
		return 0;
	}

	return 0;
}

inline BOOL InList(int& x, int& y, deque<NODE>::iterator& iter, deque<NODE>& the_list)
{
	for (iter = the_list.begin(); iter != the_list.end(); iter++)
	{
		if ((*iter).x == x && (*iter).y == y)
			return TRUE;
	}
	return FALSE;
}

inline bool CheckNeighbour(NODE& picked, deque<NODE>& openlist, deque<NODE>& closedlist, BOOL diagonal = FALSE)
{
	if (Map[picked.y * dNrColumns + picked.x]!=0) return 0;
	
	if (diagonal) picked.g = closedlist.back().g+14;
	else picked.g = closedlist.back().g+10;

	picked.h = heuristic(picked.x, picked.y, Dest.x, Dest.y);
/*	//added recently:
	if (picked.h >= closedlist.back().h) picked.h +=10;
	//end added recently*/
	picked.f = picked.g + picked.h;
	picked.dParent = nrInClosedList;
	picked.nrTries = 1;

	deque<NODE>::iterator vectorI;

	//check if we have already added this map point... 1. in the openlist.
	

	if (InList(picked.x, picked.y, vectorI, openlist))
	{
		//check if, the found one is better than the old one.
		if (picked.g < (*vectorI).g) 
		{
			(*vectorI).dParent = nrInClosedList;
			(*vectorI).f = picked.f;
			(*vectorI).g = picked.g;
			(*vectorI).h = picked.h;

			//if we did, we won't put it again in the open list
			//but we will change min accordingly
		}
		(*vectorI).nrTries++;
		return 1;
	}
	//or maybe in the close list!
	if (InList(picked.x, picked.y, vectorI, closedlist))
	{
		//if we did... check if this is a better route.
		if (picked.g < (*vectorI).g)
		{
			(*vectorI).dParent = nrInClosedList;
			(*vectorI).f = picked.f;
			(*vectorI).g = picked.g;
			(*vectorI).h = picked.h;
				
			//if we did, we won't put it again in the open list:
		}
		(*vectorI).nrTries++;
		//in any case, we do NOT place it into the openlist.
		return 1;
	}
	openlist.push_back(picked);
	return 1;
}

bool min_of_elem ( NODE& elem1, NODE& elem2 )
{
   return elem1.f < elem2.f;
};


inline BOOL AClassic(deque<NODE>& closedlist, deque<NODE>& openlist)
{
	//so we first put in the closedlist the first element. picked is first, the start position.

//	deque<NODE> closedlist;
//	deque<NODE> openlist;

	
	NODE picked;
	picked.x = Source.x;
	picked.y = Source.y;

	picked.g = 0;
	picked.h = heuristic(picked.x, picked.y, Dest.x, Dest.y);
	picked.f = picked.g + picked.h;
	nrInClosedList = 0;
	picked.dParent = -1;
	picked.nrTries = 1;

	//we place the first node in closed list;
	closedlist.push_back(picked);
	//we use only one searching vector
	NODE current_node = closedlist.back();
	//D_UP; D_DOWN; D_LEFT; D_RIGHT;
	BYTE directions;

	do
	{
		directions = 0;
		//adding neighbours to the openlist and calculating their score;

		//checking neighbours, setting the scores and adding them to the openlist:
		
		//we check left of Current:
		if (current_node.x > 0)
		{
			picked.x = current_node.x - 1;
			picked.y = current_node.y;
			if (CheckNeighbour(picked, openlist, closedlist))
				directions |= D_LEFT;
		}


		//checking up of current:
		if (current_node.y > 0)
		{
			picked.x = current_node.x;
			picked.y = current_node.y-1;
			if (CheckNeighbour(picked, openlist, closedlist))
				directions |= D_UP;
		}

		//checking right of current:
		if (current_node.x < dNrColumns - 1)
		{
			picked.x = current_node.x+1;
			picked.y = current_node.y;
			if (CheckNeighbour(picked, openlist, closedlist))
				directions |= D_RIGHT;
		}

		//checking down of current:
		if (current_node.y < dNrRounds-1)
		{
			picked.x = current_node.x;
			picked.y = current_node.y+1;
			if (CheckNeighbour(picked, openlist, closedlist))
				directions |= D_DOWN;
		}

		//if  left & up we check left+up
		if ((directions & D_UP || directions & D_LEFT) && current_node.y > 0 && current_node.x > 0)
		{
			picked.x = current_node.x-1;
			picked.y = current_node.y-1;
			CheckNeighbour(picked, openlist, closedlist, TRUE);
		}

		
		//if up & right, checking up + right
		if ((directions & D_UP || directions & D_RIGHT) && current_node.y > 0 && current_node.x < dNrColumns - 1)
		{
			picked.x = current_node.x+1;
			picked.y = current_node.y-1;
			CheckNeighbour(picked, openlist, closedlist, TRUE);
		}

		
		//if left & down, checking left + down
		if ((directions & D_LEFT || directions & D_DOWN) && current_node.y < dNrRounds-1 && current_node.x > 0)
		{
			picked.x = current_node.x-1;
			picked.y = current_node.y+1;
			CheckNeighbour(picked, openlist, closedlist, TRUE);
		}

		
		//if  right & down, checking right + down
		if ((directions & D_RIGHT || directions & D_DOWN) && current_node.y < dNrRounds-1 && current_node.x < dNrColumns - 1)
		{
			picked.x = current_node.x+1;
			picked.y = current_node.y+1;
			CheckNeighbour(picked, openlist, closedlist, TRUE);
		}

//		//we need the last item to have the lowest score, etc.:
//		sort(openlist.begin(), openlist.end(), NODEg_greater);

		//so now we have the best solution! we have to remove it from open and add it to close.
		//trying with min predicate
		deque<NODE>::iterator minI;
		minI = min_element(openlist.begin(), openlist.end(), min_of_elem);
		if (openlist.empty()) break;
		closedlist.push_back(*minI);
		openlist.erase(minI);
		nrInClosedList++;
/*		deque<NODE>::iterator minI, minFound;
		int minValue = openlist.front().f;
		minFound = openlist.begin();
		for (minI = openlist.begin()+1; minI<openlist.end(); minI++)
			if (minI->f < minValue)
			{
				minValue = minI->f;
				minFound = minI;
			}
		if (openlist.empty()) break;
		closedlist.push_back(*minFound);
		openlist.erase(minFound);
		nrInClosedList++;*/
/*		
		if (openlist.empty()) break;
		closedlist.push_back(openlist.back());
		nrInClosedList++;

		
		openlist.pop_back();*/


		current_node = closedlist.back();

	}while (closedlist.back().x != Dest.x || closedlist.back().y != Dest.y);

	//if the openlist is empty (return false), then we have not found the path
	//if the openlist is not empty (return true), then we have found the path
	return (!openlist.empty());
}