
#define NONE				0

//by the search order:
#define ATIT_TOWARDS		1
#define ATIT_TOW_LAT1		2
#define ATIT_TOW_LAT2		3
#define ATIT_LATERAL1		4
#define ATIT_LATERAL2		5
#define ATIT_AGA_LAT1		6
#define ATIT_AGA_LAT2		7
#define ATIT_AGAINST		8

#define ATIT_FAILED				255

//we take into consideration U+L=UL:	//LDRU
#define DIRECTION_U		1 + 0xF			//0001
#define DIRECTION_R		2 + 0xF			//0010
#define DIRECTION_UR	3 + 0xF			//0011
#define DIRECTION_D		4 + 0xF			//0100
#define DIRECTION_DR	6 + 0xF			//0110
#define DIRECTION_DL	12 + 0xF		//1100
#define DIRECTION_L		8 + 0xF			//1000
#define DIRECTION_UL	9 + 0xF			//1001

#define MOVE_YDOWN		 1
#define MOVE_YUP		-1
#define MOVE_YNOT		 0
#define MOVE_XLEFT		-1
#define MOVE_XRIGHT		 1
#define MOVE_XNOT		 0

//attitude: towards, against, lateral1, 2, etc
#define ATITUDE_OFBYTE(b) (b & 0xF)
#define DIRECTION_OFBYTE(b) (b >> 4)
#define MAKEDIRECTION(direction, attitude) (direction + attitude)

extern CPoint Source, Dest;
extern BYTE* Map;

struct VEC_NODE
{
	//the positions:
	int x,y;
	//the direction:
	byte direction;
	VEC_NODE(CPoint point)
	{
		direction = NONE;
		x = point.x;
		y = point.y;
	}

	VEC_NODE(BYTE pox, BYTE poy)
	{
		direction = NONE;
		x = pox;
		y = poy;
	}

	bool operator!=(CPoint point)
	{
		return point.x != x || point.y != y;
	}
};

struct LINE
{
	byte x1,x2,y1,y2;
};

inline bool GetDirection(BYTE& dir)
{
	//if direction is none, the first attempt is with the actual desired direction:
	if (dir == NONE)
	{
		byte direction = 0;
		if (Dest.x > Source.x)			//S.x		D.x
			direction |= DIRECTION_R;
		else if (Dest.x < Source.x)		//D.x		S.x
			direction |= DIRECTION_L;

		if (Dest.y > Source.y)				//S.y
			direction |= DIRECTION_D;		//D.y
		else if (Dest.y < Source.y)					//D.y
			direction |= DIRECTION_U;				//S.y

		dir = MAKEDIRECTION(direction, ATIT_TOWARDS);
	}
	//if we already sought direction, we have 2 possibilities: 1. already failed; 2. seek further
	else if (dir & ATIT_FAILED) return FALSE;
	else dir = MAKEDIRECTION(DIRECTION_OFBYTE(dir), ATITUDE_OFBYTE(dir)+1);
	return TRUE;
}

inline bool CanGoInDirection(VEC_NODE& position, int& dx, int& dy)
{
	//we first check the actual direction we have to go on:
	byte dir = DIRECTION_OFBYTE(position.direction);
	byte attitude = ATITUDE_OFBYTE(position.direction);

	//all directions are calculated clockwise:
	//u,r,d,l; r,d,l,u;  d,l,u,r; l,u,r,d;

	//so we have attitude (which path we next try) and direction (the initial, desired direction)
	//we should use dx and dy, by which we calculate the node (square)
	dx = MOVE_XNOT; dy = MOVE_YNOT;

	if (attitude + 1 > ATIT_AGAINST)
	{
		position.direction = ATIT_FAILED;
		return false;
	}
	
	if (attitude == ATIT_TOWARDS)
	{
		//if towards = up => our new direction is up
		if (dir & DIRECTION_U) dy = MOVE_YUP;
		else if (dir & DIRECTION_D) dy = MOVE_YDOWN;
		if (dir & DIRECTION_L) dx = MOVE_XLEFT;
		else if (dir & DIRECTION_R) dx = MOVE_XRIGHT;
	}

	else if (attitude == ATIT_TOW_LAT1)
	{
		//if towards = up => our new direction is u+r
		switch (dir)
		{
		case DIRECTION_U: dy = MOVE_YUP; dx = MOVE_XRIGHT; break;
		case DIRECTION_UR: dy = MOVE_YNOT; dx = MOVE_XRIGHT; break;
		case DIRECTION_R: dy = MOVE_YDOWN; dx = MOVE_XRIGHT; break;
		case DIRECTION_DR: dy = MOVE_YDOWN; dx = MOVE_XNOT; break;
		case DIRECTION_D: dy = MOVE_YDOWN; dx = MOVE_XLEFT; break;
		case DIRECTION_DL: dy = MOVE_YNOT; dx = MOVE_XLEFT; break;
		case DIRECTION_L: dy = MOVE_YUP; dx = MOVE_XLEFT; break;
		case DIRECTION_UL: dy = MOVE_YUP; dx = MOVE_XNOT; break;
		}
	}

	else if (attitude == ATIT_TOW_LAT2)
	{
		//if towards = up => our new direction is u+r
		switch (dir)
		{
		case DIRECTION_U: dy = MOVE_YUP; dx = MOVE_XLEFT; break;
		case DIRECTION_UR: dy = MOVE_YUP; dx = MOVE_XNOT; break;
		case DIRECTION_R: dy = MOVE_YUP; dx = MOVE_XRIGHT; break;
		case DIRECTION_DR: dy = MOVE_YNOT; dx = MOVE_XRIGHT; break;
		case DIRECTION_D: dy = MOVE_YDOWN; dx = MOVE_XRIGHT; break;
		case DIRECTION_DL: dy = MOVE_YDOWN; dx = MOVE_XNOT; break;
		case DIRECTION_L: dy = MOVE_YDOWN; dx = MOVE_XLEFT; break;
		case DIRECTION_UL: dy = MOVE_YNOT; dx = MOVE_XLEFT; break;
		}
	}

	else if (attitude == ATIT_LATERAL1)
	{
		//if towards = up => our new direction is right
		switch (dir)
		{
		case DIRECTION_U: dy = MOVE_YNOT; dx = MOVE_XRIGHT; break;
		case DIRECTION_UR: dy = MOVE_YDOWN; dx = MOVE_XRIGHT; break;
		case DIRECTION_R: dy = MOVE_YDOWN; dx = MOVE_XNOT; break;
		case DIRECTION_DR: dy = MOVE_YDOWN; dx = MOVE_XLEFT; break;
		case DIRECTION_D: dy = MOVE_YNOT; dx = MOVE_XLEFT; break;
		case DIRECTION_DL: dy = MOVE_YUP; dx = MOVE_XLEFT; break;
		case DIRECTION_L: dy = MOVE_YUP; dx = MOVE_XNOT; break;
		case DIRECTION_UL: dy = MOVE_YUP; dx = MOVE_XRIGHT; break;
		}
	}

	else if (attitude == ATIT_LATERAL2)
	{
		//if towards = up => our new direction is LEFT
		switch (dir)
		{
		case DIRECTION_U: dy = MOVE_YNOT; dx = MOVE_XLEFT; break;
		case DIRECTION_UR: dy = MOVE_YUP; dx = MOVE_XLEFT; break;
		case DIRECTION_R: dy = MOVE_YUP; dx = MOVE_XNOT; break;
		case DIRECTION_DR: dy = MOVE_YUP; dx = MOVE_XRIGHT; break;
		case DIRECTION_D: dy = MOVE_YNOT; dx = MOVE_XRIGHT; break;
		case DIRECTION_DL: dy = MOVE_YDOWN; dx = MOVE_XRIGHT; break;
		case DIRECTION_L: dy = MOVE_YDOWN; dx = MOVE_XNOT; break;
		case DIRECTION_UL: dy = MOVE_YDOWN; dx = MOVE_XLEFT; break;
		}
	}

	else if (attitude == ATIT_AGA_LAT1)
	{
		//if towards = up => our new direction is RIGHT+DOWN
		switch (dir)
		{
		case DIRECTION_U: dy = MOVE_YDOWN; dx = MOVE_XRIGHT; break;
		case DIRECTION_UR: dy = MOVE_YDOWN; dx = MOVE_XNOT; break;
		case DIRECTION_R: dy = MOVE_YDOWN; dx = MOVE_XLEFT; break;
		case DIRECTION_DR: dy = MOVE_YNOT; dx = MOVE_XLEFT; break;
		case DIRECTION_D: dy = MOVE_YUP; dx = MOVE_XLEFT; break;
		case DIRECTION_DL: dy = MOVE_YUP; dx = MOVE_XNOT; break;
		case DIRECTION_L: dy = MOVE_YUP; dx = MOVE_XRIGHT; break;
		case DIRECTION_UL: dy = MOVE_YNOT; dx = MOVE_XRIGHT; break;
		}
	}

	else if (attitude == ATIT_AGA_LAT2)
	{
		//if towards = up => our new direction is RIGHT+DOWN
		switch (dir)
		{
		case DIRECTION_U: dy = MOVE_YDOWN; dx = MOVE_XLEFT; break;
		case DIRECTION_UR: dy = MOVE_YNOT; dx = MOVE_XLEFT; break;
		case DIRECTION_R: dy = MOVE_YUP; dx = MOVE_XLEFT; break;
		case DIRECTION_DR: dy = MOVE_YUP; dx = MOVE_XNOT; break;
		case DIRECTION_D: dy = MOVE_YUP; dx = MOVE_XRIGHT; break;
		case DIRECTION_DL: dy = MOVE_YNOT; dx = MOVE_XRIGHT; break;
		case DIRECTION_L: dy = MOVE_YDOWN; dx = MOVE_XRIGHT; break;
		case DIRECTION_UL: dy = MOVE_YDOWN; dx = MOVE_XNOT; break;
		}
	}

	else if (attitude == ATIT_AGAINST)
	{
		//if towards = up => our new direction is down
		if (dir & DIRECTION_U) dy = MOVE_YDOWN;
		else if (dir & DIRECTION_D) dy = MOVE_YUP;
		if (dir & DIRECTION_L) dx = MOVE_XRIGHT;
		else if (dir & DIRECTION_R) dx = MOVE_XLEFT;
	}

	byte direction = MAKEDIRECTION(dir, attitude + 1);
	position.direction = direction;

	VEC_NODE new_point(position.x + dx, position.y + dy);
	if (new_point.x < 0 || new_point.y < 0 || new_point.x >= dNrColumns || new_point.y >= dNrRounds)
		return FALSE;

	if (Map[new_point.y, new_point.x] == 1) 
		return false;
	else if (dx && dy && Map[position.x + dx, position.y] == 1 && Map[position.x, position.y + dy] == 1)
	{
		return false;
	}

	return true;
}

inline void FindNextNode(VEC_NODE& the_node, int dx, int dy, LINE* pLine)
{
	//we CAN go in this direction, we checked previously
	VEC_NODE new_point(the_node.x + dx, the_node.y + dy);

	bool stop = false;
	BYTE isBlocked = 0; //1 = horiz; 2 = vert; 3 = diagonal
	do 
	{
		new_point.y += dy; new_point.x += dx;

		if (new_point.x < 0 || new_point.y < 0 || new_point.x >= dNrColumns || new_point.y >= dNrRounds)
		stop = true;

		//if diagonal movement:
		if (dx && dy && Map[the_node.x + dx, the_node.y] == 1 && Map[the_node.x, the_node.y + dy] == 1)
		{
			stop = true;
			isBlocked = 3;
			pLine->x1 = the_node.x + dx;
			pLine->y1 = the_node.y;
			pLine->x2 = the_node.x;
			pLine->y2 = the_node.y + dy;
		}
		//if not diagonal movement, we just check if blocked
		else if (dx && Map[the_node.x + dx, the_node.y] == 1) //dy = 0, we move horizontal only
		{
			stop = true;
			isBlocked = 1;
			pLine->x1 = the_node.x + dx;
			pLine->y1 = the_node.y;
		}
		else if (dy && Map[the_node.x, the_node.y + dy] == 1) //dy = 0, we move vertical only
		{
			stop = true;
			isBlocked = 2;
			pLine->x1 = the_node.x;
			pLine->y1 = the_node.y + dy;
		}


		//also we stop if THE WALL WE FOLLOW ENDS or if we stepped over the destination!
		//if we stepped over the destination: may have problems: the_node to happen to be equal to dest.x
		//initial	-	Dest	-	the_node
		if (Dest.x > the_node.x && new_point.x > Dest.x ||
			Dest.x < the_node.x && new_point.x < Dest.x ||
			Dest.y > the_node.y && new_point.y > Dest.y ||
			Dest.y < the_node.y && new_point.y < Dest.y)
			stop = true;

		//or we just place points {(x,y), (x+1,y), (x+2,y)} -> {(x,y),(x, x+2)} and check each of the

	} while (stop==false);

	if (isBlocked)
	{
		//we must extend the line..
		if (isBlocked == 1) //horiz:
		{
			//seek left:
			VEC_NODE left(the_node.x + dx, the_node.y);
			do
			{
				dx += dx;
			} while (Map[left.x + dx, left.y] == 1);

			pLine->x1 = left.x; pLine->y1 = left.y;
		}

		else if (isBlocked == 2) //vert:
		{
		}
	}

	//at the loop, we added one more than possible (a blocked the_node), but we need the last good the_node
	new_point.x -= dx; new_point.y -= dy;
	the_node = new_point;
}

inline bool AlgVectors(deque<VEC_NODE> Vectors)
{
	VEC_NODE currPos(Source);
	int dx, dy;

	while (currPos != Dest)
	{
		//seek best direction:

		//here we do whatever required, until when we find a direction to go to.
		do
		{
			//if we found no direction to go to, from the current position, exit the loop.
			if (FALSE == GetDirection(currPos.direction)) break;
			//we stop this when we found a new position.
		} while (CanGoInDirection(currPos, dx, dy) == FALSE);

		//so now, we either found a new direction (we have dx,dy non-null), or there is no direction...
		if (currPos.direction == ATIT_FAILED) {}; //we do that later...

		//if we found a new direction, follow that direction by conditions and find the next node:
		FindNextNode(currPos, dx, dy, NULL);
		
		//we push the current position:
		Vectors.push_back(currPos);
	}

	return true;
}
