// A_starView.cpp : implementation of the CA_starView class
//

#include "stdafx.h"
#include "A_star.h"

#include "A_starDoc.h"
#include "A_starView.h"
#include "RandomSel.h"
#include "OptionsDlg.h"
#include "MainFrm.h"

#include <deque>
#include <algorithm>
#include <math.h>
#include "ClassicalAStar.h"
#include "VectorAlgorithm.h"


using namespace std;
//#define COMPLEX_WAY

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Map will be a VECTOR in the form of a matrix;

extern HBITMAP hBlock;
extern HBITMAP hDest;
extern HBITMAP hSource;

#define PLACE_NOTHING	0
#define PLACE_BLOCK		1
#define PLACE_START		2
#define PLACE_END		3
#define SCROLL_MODE		4

extern BOOL StoredOnce;

#define D_UP		1
#define D_DOWN		2
#define D_LEFT		4
#define D_RIGHT		8
#define D_STRAIGHT	1000

#define ASTAR_CLASSIC	1
#define ALG_VECTORS		2

#define MANHATTAN			0
#define EUCLIDIAN			1
#define DIAGONAL_SHORTCUT	2
#define NO_HEURISTIC		3

deque<PATHPOINT> result;
deque<SEEKPOINT> searches;
int dHeuristic = MANHATTAN;
BYTE* Map = NULL;
int dNrColumns;
int dNrRounds;
int dNrRandoms;
BOOL bFoundPath;
BOOL bViewPointsChecked;
int nrInClosedList = -1;

CPoint Source, Dest;

// CA_starView

IMPLEMENT_DYNCREATE(CA_starView, CView)

BEGIN_MESSAGE_MAP(CA_starView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_COMMAND(ID_EDIT_MAKENEW, &CA_starView::OnEditMakenew)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_PUTBLOCK, &CA_starView::OnEditPutblock)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PUTBLOCK, &CA_starView::OnUpdateEditPutblock)
//	ON_WM_CLOSE()
ON_COMMAND(ID_EDIT_FINDPATH, &CA_starView::OnEditFindpath)
ON_UPDATE_COMMAND_UI(ID_EDIT_FINDPATH, &CA_starView::OnUpdateEditFindpath)
ON_COMMAND(ID_EDIT_CLASSICALA, &CA_starView::OnEditClassicala)
ON_COMMAND(ID_FILE_NEW, &CA_starView::OnFileNew)
ON_COMMAND(ID_VIEW_POINTSCHECKED, &CA_starView::OnViewPointschecked)
ON_UPDATE_COMMAND_UI(ID_VIEW_POINTSCHECKED, &CA_starView::OnUpdateViewPointschecked)
//ON_COMMAND(ID_FILE_OPEN, &CA_starView::OnFileOpen)
ON_COMMAND(ID_HEURISTIC_MANHATTANDISTANCE, &CA_starView::OnHeuristicManhattandistance)
ON_UPDATE_COMMAND_UI(ID_HEURISTIC_MANHATTANDISTANCE, &CA_starView::OnUpdateHeuristicManhattandistance)
ON_COMMAND(ID_HEURISTIC_EUCLIDIANDISTANCE, &CA_starView::OnHeuristicEuclidiandistance)
ON_UPDATE_COMMAND_UI(ID_HEURISTIC_EUCLIDIANDISTANCE, &CA_starView::OnUpdateHeuristicEuclidiandistance)
ON_COMMAND(ID_EDIT_PLACESTARTPOINT, &CA_starView::OnEditPlacestartpoint)
ON_COMMAND(ID_EDIT_PLACEENDPOINT, &CA_starView::OnEditPlaceendpoint)
ON_UPDATE_COMMAND_UI(ID_EDIT_PLACEENDPOINT, &CA_starView::OnUpdateEditPlaceendpoint)
ON_UPDATE_COMMAND_UI(ID_EDIT_PLACESTARTPOINT, &CA_starView::OnUpdateEditPlacestartpoint)
ON_COMMAND(ID_EDIT_CLEARPATH, &CA_starView::OnEditClearpath)
ON_COMMAND(ID_EDIT_CLEARMAP, &CA_starView::OnEditClearmap)
ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARPATH, &CA_starView::OnUpdateEditClearpath)
ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARMAP, &CA_starView::OnUpdateEditClearmap)
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_HEURISTIC_DIAGONALSHORTCUT, &CA_starView::OnHeuristicDiagonalshortcut)
ON_UPDATE_COMMAND_UI(ID_HEURISTIC_DIAGONALSHORTCUT, &CA_starView::OnUpdateHeuristicDiagonalshortcut)
ON_COMMAND(ID_HEURISTIC_NOHEURISTIC, &CA_starView::OnHeuristicNoheuristic)
ON_UPDATE_COMMAND_UI(ID_HEURISTIC_NOHEURISTIC, &CA_starView::OnUpdateHeuristicNoheuristic)
ON_COMMAND(ID_VIEW_OPTIONS, &CA_starView::OnViewOptions)
ON_UPDATE_COMMAND_UI(ID_EDIT_SCROLLMODE, &CA_starView::OnUpdateEditScrollmode)
ON_COMMAND(ID_EDIT_SCROLLMODE, &CA_starView::OnEditScrollmode)
ON_WM_SETCURSOR()
ON_COMMAND(ID_ALGORITHM_VECTORS, &CA_starView::OnAlgorithmVectors)
ON_UPDATE_COMMAND_UI(ID_EDIT_CLASSICALA, &CA_starView::OnUpdateEditClassicala)
ON_UPDATE_COMMAND_UI(ID_ALGORITHM_VECTORS, &CA_starView::OnUpdateAlgorithmVectors)
END_MESSAGE_MAP()

// CA_starView construction/destruction

CA_starView::CA_starView()
: m_dSquareSize(20)
, m_LastPoint(0)
, m_ScrollPosition(0)
, m_MaxScrollPosition(0)
{
	// TODO: add construction code here
	Source.x = -1;
	Source.y = -1;

	Dest.x = -1;
	Dest.y = -1;

	dNrRounds = -1;
	dNrColumns = -1;

	m_dAlgorithm = ASTAR_CLASSIC;
	m_dPlace = PLACE_NOTHING;
	bFoundPath = FALSE;
	bViewPointsChecked = TRUE;

	dHeuristic = MANHATTAN;
	m_dPlace = PLACE_NOTHING;

	//loading all Bitmap Images;
	bmpBlock.LoadBitmapW(IDB_BLOCK);
	bmpDest.LoadBitmapW(IDB_DEST);
	bmpSource.LoadBitmapW(IDB_SURSA);
	m_dUserObjSize = 20;
	m_dNeutObjSize = 15;

	m_dUserObjSizeOption = 2;
	m_dNeutObjSizeOption = 1;
}

CA_starView::~CA_starView()
{
	delete[] Map;
	//result.clear();			//I believe it clears it itself
}

BOOL CA_starView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CA_starView drawing

void CA_starView::OnDraw(CDC* /*pDC*/)
{
	CA_starDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CA_starView printing

BOOL CA_starView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CA_starView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CA_starView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CA_starView diagnostics

#ifdef _DEBUG
void CA_starView::AssertValid() const
{
	CView::AssertValid();
}

void CA_starView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CA_starDoc* CA_starView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CA_starDoc)));
	return (CA_starDoc*)m_pDocument;
}
#endif //_DEBUG


void CA_starView::OnPaint()
{
	//this is the primary surface:
	CPaintDC Paintdc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	//back buffer:
	//drawing white rectangles; black border;
	CDC dc;
	if (dc.CreateCompatibleDC(&Paintdc)==false) return;

	//we create a memory dc (dc) to avoid clipping;
	//but first... check the size!
	int dNrColumnsInView, dNrRoundsInView;
	int dNrColumnsUse, dNrRoundsUse;
	CRect rect; this->GetClientRect(rect);
	//check how many squares (including partial square) fit on the screen:
	int maxTilesHoriz = rect.Width() / m_dSquareSize;
	if (rect.Width() % m_dSquareSize != 0) maxTilesHoriz++;
	int maxTilesVert = rect.Height() / m_dSquareSize;
	if (rect.Height() % m_dSquareSize != 0) maxTilesVert++;

	//2 posibilities:
	//1. There are less squares actually drawn than they fit in.
	if (dNrRounds <= maxTilesVert)
		dNrRoundsUse = dNrRounds;
	//2. The image is larger:
	//		a) if it aligns with a scroll grid, we need no aditional tile
	//does not fit correctly on the screen: (we need additional tile)
	//		b) it algigns not: we add one tile
	else 
	{
		if (m_ScrollPosition.y % m_dSquareSize == 0)
			dNrRoundsUse = maxTilesVert;
		else
			dNrRoundsUse = maxTilesVert + 1;
	}

	//the same we do for the columns:
	if (dNrColumns <= maxTilesVert)
		dNrColumnsUse = dNrColumns;
	else 
	{
		if (m_ScrollPosition.x % m_dSquareSize == 0)
			dNrColumnsUse = maxTilesHoriz;
		else
			dNrColumnsUse = maxTilesHoriz + 1;
	}
	
	//we now have all we require to build our back buffer... now we build it!

	CBitmap bmp;
	if (bmp.CreateCompatibleBitmap(&Paintdc, dNrColumnsUse * m_dSquareSize, dNrRoundsUse * m_dSquareSize) == 0)
	{
		return;
	}
	//the surface will be only the place we draw onto!
	CBitmap* pOldMemBitmap = dc.SelectObject(&bmp);

	//before starting to draw onto the backbuffer, we must check the first horiz tile, first vert tile, 
	//last horiz tile, last vert tile, that fits on our buffer, taking into consideration the scroll position.
	int _1HTile, _1VTile, _2HTile, _2VTile;
	//first tiles are at the position scrollpos in our map, so the squares they are into are:
	_1HTile = m_ScrollPosition.x / m_dSquareSize;
	_1VTile = m_ScrollPosition.y / m_dSquareSize;
	//last tiles are at the position scrollpos + nrTiles[H/V] - 1
	_2HTile = _1HTile + dNrColumnsUse - 1;
	_2VTile = _1VTile + dNrRoundsUse - 1;


	//a kind of clearscreen :))
	RECT clRect;
	GetClientRect(&clRect);
	dc.Rectangle(0, 0, clRect.right, clRect.bottom);

	//now, draw white rectangles (initialize the page):
	//we ALWAYS have in the back buffer (dc) full squares, so here we just draw into the buffer squares!
	for (int i = 0; i < dNrRoundsUse; i++)
		for (int j = 0; j < dNrColumnsUse; j++)
		{
			dc.Rectangle(j * m_dSquareSize, i * m_dSquareSize, (j + 1) * m_dSquareSize, (i + 1) * m_dSquareSize);
		}

	//preparing for displaying the images;
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBitmap = MemDC.SelectObject(&bmpBlock);

	//Displaying all blocks on the map;
	//here we take into consideration the map, so we should take ONLY necessary elements of the matrix.
	int i2, j2;
	for (int i = 0; i < dNrRoundsUse; i++)
		for (int j = 0; j <dNrColumnsUse; j++)//i din matrice este y din coordonate; j este x
		{
			i2 = i + _1VTile;
			j2 = j + _1HTile;
			//we should never get i2 > _2VTile or j2 > _2Htile
			if (i2 > _2VTile) i2 = _2VTile;
			if (j2 > _2HTile) j2 = _2HTile;

			if (i2 < 0) i2 = 0;
			if (j2 < 0) j2 = 0;

/*			CString output_str;
			output_str.Format(L"i=%d; j=%d; i2=%d; j2=%d; _1VTile=%d; _1HTile=%d; _2VTile=%d; _2HTile=%d; scroll_pos.x=%d; scroll_pos.y=%d; maxS.x=%d; maxS.y=%d",
				i, j, i2, j2, _1VTile, _1HTile, _2VTile, _2HTile, m_ScrollPosition.x, m_ScrollPosition.y, m_MaxScrollPosition.x, m_MaxScrollPosition.y);
			((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(0, output_str);*/

			if (Map[i2*dNrColumns + j2]==1)
				dc.StretchBlt(j * m_dSquareSize, i * m_dSquareSize, m_dSquareSize, m_dSquareSize, &MemDC, 0, 0,
				20, 20, SRCCOPY);
		}

	//if we are asked to display the nrTries, we do so.

	if (bViewPointsChecked)
	{
		//we display EVERY point:
		deque<SEEKPOINT>::iterator searchI;
		CBrush brush;
		for (searchI = searches.begin(); searchI < searches.end(); searchI++)
		{
			//check which we can draw:
			if (_1HTile <= searchI->x && searchI->x <= _2HTile && //_1HTile <= j <= _2HTile
				_1VTile <= searchI->y && searchI->y <= _2VTile)
			{
				//and also, we get their position on the screen!
				i2 = searchI->y - _1VTile;
				j2 = searchI->x - _1HTile;

				brush.CreateSolidBrush(RGB(255,255,searchI->value));
				dc.SelectObject(&brush);
				dc.Rectangle(/*searchI->x*/ j2 * m_dSquareSize, /*searchI->y*/ i2 * m_dSquareSize, 
(/*searchI->x*/ j2 + 1) * m_dSquareSize, (/*searchI->y*/ i2 + 1) * m_dSquareSize);
				brush.DeleteObject();
			}
		}
	}

	//Displaying the Source
	if (Source.x > -1 && Source.y > -1 && Source.x >= _1HTile && Source.x <= _2HTile &&
		Source.y >= _1VTile && Source.y <= _2VTile)
	{
		//display at the position on the view!
		i2 = Source.y - _1VTile;
		j2 = Source.x - _1HTile;

		pOldBitmap = MemDC.SelectObject(&bmpSource);
		//we have to draw smaller images in the center of the squares!
		int posX = j2 * m_dSquareSize + (m_dSquareSize - m_dUserObjSize)/2;
		int posY = i2 * m_dSquareSize + (m_dSquareSize - m_dUserObjSize)/2;
/*		dc.TransparentBlt(j2 * m_dSquareSize, i2 * m_dSquareSize, m_dSquareSize, m_dSquareSize, &MemDC, 0, 0,
			20, 20, RGB(255,255,255));*/
		dc.TransparentBlt(posX, posY, m_dUserObjSize, m_dUserObjSize, &MemDC, 0, 0,
			20, 20, RGB(255,255,255));
	}
	//Displaying the Destination
	if (Dest.x > -1 && Dest.y > -1 && Dest.x >= _1HTile && Dest.x <= _2HTile &&
		Dest.y >= _1VTile && Dest.y <= _2VTile)
	{
		//display at the position on the view!
		i2 = Dest.y - _1VTile;
		j2 = Dest.x - _1HTile;

		pOldBitmap = MemDC.SelectObject(&bmpDest);
		dc.TransparentBlt(/*Dest.x*/j2 * m_dSquareSize, /*Dest.y*/i2 * m_dSquareSize, m_dSquareSize, m_dSquareSize, &MemDC, 0, 0,
			20, 20, RGB(255,255,255));
	}

	//if we are asked to calculate, then we should display:
	if (bFoundPath)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
//		pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
		dc.SelectObject(&pen);
		deque<PATHPOINT>::iterator curI;
		curI = result.begin();

		//we have to SEEK every point that ENTERS the screen and every point that EXISTS the screen:
		int pointFlag = 0; //0 - unknown/not in the view;  2 - inside;
		for (curI = result.begin(); curI < result.end(); curI++)
		{
			//seek if the point can be drawn:
			if ((curI)->x >= _1HTile && (curI)->x <= _2HTile && (curI)->y >= _1VTile && (curI)->y <= _2VTile)
			{
				//the point CAN be drawn: we check and reset the flag:
				if (pointFlag == 0) // if the last node did not enter the view, we draw the line, and reset flag:
				{
					pointFlag = 2;
				}
				//display at the position on the view!
				i2 = (curI)->y - _1VTile;
				j2 = (curI)->x - _1HTile;

				//to begin the drawing of the line FROM the first point, not from 0,0 to the first point:
				if (curI==result.begin())
				{
					dc.MoveTo(j2 * m_dSquareSize + m_dSquareSize/2, i2 * m_dSquareSize + m_dSquareSize/2);
				}

				dc.LineTo(j2 * m_dSquareSize + m_dSquareSize/2, i2 * m_dSquareSize + m_dSquareSize/2);
			}
			else
				//if outside
			{
				//display at the position on the view!
				i2 = (curI)->y - _1VTile;
				j2 = (curI)->x - _1HTile;

				//if we were inside (=2) we still draw this line (the line exists)
				if (pointFlag == 2)
				{
					pointFlag = 0;

					dc.LineTo(j2 * m_dSquareSize + m_dSquareSize/2, i2 * m_dSquareSize + m_dSquareSize/2);
				}

				//if we were not inside (and here is still outside), we just move to this position:
				dc.MoveTo(j2 * m_dSquareSize + m_dSquareSize/2, i2 * m_dSquareSize + m_dSquareSize/2);
			}
			
		}
//
//		//we move the first position, then start to draw to the other points.
//		dc.MoveTo(curI->x * m_dSquareSize + m_dSquareSize/2, curI->y * m_dSquareSize + m_dSquareSize/2);
//		//we get the first item from the result
//		for (curI = result.begin(); curI < result.end(); curI++)
//		{
//			dc.LineTo((curI)->x * m_dSquareSize + m_dSquareSize/2, (curI)->y * m_dSquareSize + m_dSquareSize/2);
//			
//		}
		pen.DeleteObject();
	}

	Paintdc.BitBlt(0, 0, dNrColumnsUse * m_dSquareSize, dNrRoundsUse * m_dSquareSize,
		&dc, m_ScrollPosition.x % m_dSquareSize, m_ScrollPosition.y % m_dSquareSize, SRCCOPY); 

	dc.SelectObject(pOldMemBitmap);

	MemDC.SelectObject(pOldBitmap);
}

void CA_starView::OnLButtonDown(UINT nFlags, CPoint pointV)
{
	// TODO: Add your message handler code here and/or call default
	//checking in which box (if any) the pointer is

	//first, convert point from view to map:
	CPoint point;
	point.x = pointV.x + m_ScrollPosition.x;
	point.y = pointV.y + m_ScrollPosition.y;


	if (m_dPlace == PLACE_BLOCK)
	{
		if (/*point.x > 20 && point.y > 20 && */
			point.x < /*20 + */dNrColumns * m_dSquareSize && point.y < /*20 + */dNrRounds * m_dSquareSize)
		{
			int i,j;
			i = (point.y/* - 20*/)/m_dSquareSize;
			j = (point.x/* - 20*/)/m_dSquareSize;

			Map[i*dNrColumns + j] = !Map[i*dNrColumns + j];
			if (i == Source.y && j == Source.x)
			{
				Source.x = -1; Source.y = -1;
			}

			if (i == Dest.y && j == Dest.x)
			{
				Dest.x = -1; Dest.y = -1;
			}

			InvalidateRect(0,0);
			this->GetDocument()->SetModifiedFlag();
			bFoundPath = false;
			bViewPointsChecked = FALSE;
			nrInClosedList = -1;
			result.clear();
			searches.clear();
		}
		return;
	}

	else if (m_dPlace == PLACE_START)
	//pacing source
	{
		if (/*point.x > 20 && point.y > 20 && */
			point.x < /*20 + */dNrColumns * m_dSquareSize && point.y < /*20 + */dNrRounds * m_dSquareSize)
		{
			int i,j;
			i = (point.y/* - 20*/)/m_dSquareSize;
			j = (point.x/* - 20*/)/m_dSquareSize;

			if (Map[i*dNrColumns + j]==0 && !(Dest.y == i && Dest.x == j))
			{
				Source.x = j;//x=j
				Source.y = i;
				InvalidateRect(0,0);
				this->GetDocument()->SetModifiedFlag();

				bFoundPath = false;
				bViewPointsChecked = FALSE;
				nrInClosedList = -1;
				result.clear();
				searches.clear();
			}
		}
	}

	else if (m_dPlace == PLACE_END)
	{
		if (/*point.x > 20 && point.y > 20 && */
			point.x < /*20 + */dNrColumns * m_dSquareSize && point.y < /*20 + */dNrRounds * m_dSquareSize)
		{
			int i,j;
			i = (point.y/* - 20*/)/m_dSquareSize;
			j = (point.x/* - 20*/)/m_dSquareSize;

			if (Map[i*dNrColumns + j]==0 && !(i==Source.y && j==Source.x))
			{
				Dest.x = j;//x=j
				Dest.y = i;
				InvalidateRect(0,0);
				this->GetDocument()->SetModifiedFlag();

				bFoundPath = false;
				bViewPointsChecked = FALSE;
				nrInClosedList = -1;
				result.clear();
				searches.clear();
			}
		}
	}

	else if (m_dPlace == SCROLL_MODE)
	{
		m_LastPoint.x = pointV.x;
		m_LastPoint.y = pointV.y;

		//calculate maximum positions:
		CRect cl_rect;
		this->GetClientRect(cl_rect);
		m_MaxScrollPosition.x = dNrColumns * m_dSquareSize - cl_rect.Width();
		m_MaxScrollPosition.y = dNrRounds * m_dSquareSize - cl_rect.Height();
	}

//	CView::OnLButtonUp(nFlags, point);

}

void CA_starView::OnRButtonDown(UINT nFlags, CPoint pointV)
{
	// TODO: Add your message handler code here and/or call default

//	CView::OnRButtonDown(nFlags, point);

	CPoint point;
	point.x = pointV.x + m_ScrollPosition.x;
	point.y = pointV.y + m_ScrollPosition.y;

	if (/*point.x > 20 && point.y > 20 && */
		point.x < /*20 + */dNrColumns * m_dSquareSize && point.y < /*20 + */dNrRounds * m_dSquareSize)
	{
		m_dPlace = PLACE_END;

		int i,j;
		i = (point.y/* - 20*/)/m_dSquareSize;
		j = (point.x/* - 20*/)/m_dSquareSize;

		if (Map[i*dNrColumns + j]==0 && !(i==Source.y && j==Source.x))
		{
			Dest.x = j;//x=j
			Dest.y = i;
			InvalidateRect(0,0);
			this->GetDocument()->SetModifiedFlag();

			bFoundPath = false;
			bViewPointsChecked = FALSE;
			nrInClosedList = -1;
			result.clear();
			searches.clear();
		}
	}
}


void CA_starView::OnEditPutblock()
{
	// TODO: Add your command handler code here
	m_dPlace = PLACE_BLOCK;
}

void CA_starView::OnUpdateEditPutblock(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(dNrRounds != -1);
	pCmdUI->SetCheck(m_dPlace == PLACE_BLOCK);
}

void CA_starView::OnUpdateEditFindpath(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(dNrRounds!=-1 && Source.x > -1 && Dest.x > -1);
}




bool NODEg_greater (NODE elem1, NODE elem2)
{
	return elem1.f > elem2.f;
}

inline void ConstructResult(deque<NODE>& closedlist)
{
	//how many values do we have for path?
	NODE closed;
	deque<PATHPOINT>::iterator resultI;
	PATHPOINT pathpoint;


	//so we create a normal vector of nr elements:
	result.clear();
	int direction;

	//now we place the elements and the directions of them! (we need the normal order, not the reversed one)
	int direction_next = -1;
	closed = closedlist.back();
	do
	{
		pathpoint.x = closed.x;
		pathpoint.y = closed.y;
		if (direction_next != -1) pathpoint.direction_next = direction_next;

		//closed = next of current
		if (closed.x == closedlist.front().x && closed.y == closedlist.front().y)
		{
			result.insert(result.begin(), pathpoint);
			break;
		}
		closed = closedlist[closed.dParent];

		//now we check where it is placed the next vs the precedent:
		if (closed.x == pathpoint.x || closed.y == pathpoint.y)
		{
			//same level
			direction_next = D_STRAIGHT;
		}

		else
		{
			//if x1!=x2 && y1!=y2: maybe we have free way
			//check first the direction of point 2 (pathpoint) vs point 1 (closed)
			direction = 0;
			if (pathpoint.x > closed.x) direction |= D_RIGHT;
			else /*pathpoint.x < (resultI-1).x*/ direction |= D_LEFT;//we don't have same X

			if (pathpoint.y > closed.y) direction |= D_DOWN;
			else /*pathpoint.y < (resultI-1).y*/ direction |= D_UP;

			//so we have left + up/down or right + up/down.
			if (direction & D_LEFT)
			{
				if (direction & D_UP)
				{
					//left + up; check up point, left point
					if (Map[(closed.y - 1) * dNrColumns + closed.x] == 0)
					{
						//we can go up... can we go left?
						if (Map[(closed.y) * dNrColumns + closed.x-1] == 0)
						{
							//we can also go left. so STRAIGHT DIRECTION
							direction_next = D_STRAIGHT;
						}
						else
						//else, we can go only up.
						direction_next = D_UP;
					}
					else
					{
						//we can clearly go in one direction. up is blocked, so left must be not!
						direction_next = D_LEFT;
					}
				}

				else
				{
					//we can go left, but we cannot go up, so we can go down!
					//left + down; check down point, left point;
					if (Map[(closed.y + 1) * dNrColumns + closed.x] == 0)
					{
						//we can go down... can we go left?
						if (Map[(closed.y) * dNrColumns + closed.x-1] == 0)
						{
							//we can also go left. so STRAIGHT DIRECTION
							direction_next = D_STRAIGHT;
						}
						else
						//else, we can go only down.
						direction_next = D_DOWN;
					}
					else
					{
						//we can clearly go in one direction. down is blocked, so left must be not!
						direction_next = D_LEFT;
					}
				}
			}

			else
			{
				//our direction is not left... so our direction is right!
				if (direction & D_UP)
				{
					//right + up; check up point, right point
					if (Map[(closed.y - 1) * dNrColumns + closed.x] == 0)
					{
						//we can go up... can we go right?
						if (Map[(closed.y) * dNrColumns + closed.x+1] == 0)
						{
							//we can also go right. so STRAIGHT DIRECTION
							direction_next = D_STRAIGHT;
						}
						else
						//else, we can go only up.
						direction_next = D_UP;
					}
					else
					{
						//we can clearly go in one direction. up is blocked, so right must be not!
						direction_next = D_RIGHT;
					}
				}

				else
				{
					//we can go right, but we cannot go up, so we can go down!
					//right + down; check down point, right point;
					if (Map[(closed.y + 1) * dNrColumns + closed.x] == 0)
					{
						//we can go down... can we go right?
						if (Map[(closed.y) * dNrColumns + closed.x+1] == 0)
						{
							//we can also go right. so STRAIGHT DIRECTION
							direction_next = D_STRAIGHT;
						}
						else
						//else, we can go only down.
						direction_next = D_DOWN;
					}
					else
					{
						//we can clearly go in one direction. down is blocked, so right must be not!
						direction_next = D_RIGHT;
					}
				}
			}
		}
		result.insert(result.begin(), pathpoint);
	} while (0==0);
}

inline void ConstructSearches(deque<NODE>& openlist, deque<NODE>& closedlist)
{
	if (!searches.empty())
	{
		searches.clear();
	}

	SEEKPOINT sp;
	deque<NODE>::iterator vI;
	for (vI = openlist.begin(); vI < openlist.end(); vI++)
	{
		sp.x = vI->x;
		sp.y = vI->y;
		//we check the color we put for it; min = 1; max = 8; 9 total (to differentiate those not checked)
		sp.value = 255 - vI->nrTries * 28 - 3;
		searches.push_back(sp);
	}

	//now, the whole closed list:
	for (vI = closedlist.begin(); vI < closedlist.end(); vI++)
	{
		sp.x = vI->x;
		sp.y = vI->y;
		//we check the color we put for it; min = 1; max = 8; 9 total (to differentiate those not checked)
		sp.value = 255 - vI->nrTries * 28 - 3;
		searches.push_back(sp);
	}
}

void CA_starView::OnEditFindpath()
{
	((CMainFrame*)GetParentFrame())->m_wndStatusBar.SetPaneText(0, L"Seeking path. Please wait...");

	if (m_dAlgorithm == ASTAR_CLASSIC)
	{
		LARGE_INTEGER Frequency, Beginning, Ending, Delta;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&Beginning);

		deque<NODE> closedlist;
		deque<NODE> openlist;

		bool result = AClassic(closedlist, openlist);

		QueryPerformanceCounter(&Ending);
		Delta.QuadPart = Ending.QuadPart - Beginning.QuadPart;
		double fSeconds = (double)Delta.QuadPart/Frequency.QuadPart;

		CString Output = _T("");
		Output.Format(L"seconds needed to find path: %.10f; aprox memory used in path finding: %d BYTES", fSeconds, 
			(openlist.size() + closedlist.size()) * sizeof(NODE));
		((CMainFrame*)GetParentFrame())->m_wndStatusBar.SetPaneText(0, Output);

		if (result)
		{
			ConstructResult(closedlist);
			ConstructSearches(openlist, closedlist);

			bFoundPath = true;
			InvalidateRect(0, 0);
		}
		else ((CMainFrame*)GetParentFrame())->m_wndStatusBar.SetPaneText(0, L"Could not find path!");
	}

	else if (m_dAlgorithm == ALG_VECTORS)
	{
		LARGE_INTEGER Frequency, Beginning, Ending, Delta;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&Beginning);

//		deque<NODE> closedlist;
//		deque<NODE> openlist;

//		bool result = AlgVectors();					*******************************************

		QueryPerformanceCounter(&Ending);
		Delta.QuadPart = Ending.QuadPart - Beginning.QuadPart;
		double fSeconds = (double)Delta.QuadPart/Frequency.QuadPart;

		CString Output = _T("");
//		Output.Format(L"seconds needed to find path: %.10f; aprox memory used in path finding: %d BYTES", fSeconds, 
//			(openlist.size() + closedlist.size()) * sizeof(NODE));
		((CMainFrame*)GetParentFrame())->m_wndStatusBar.SetPaneText(0, Output);

//		if (result)			***************************
		{
//			ConstructResult(closedlist);
//			ConstructSearches(openlist, closedlist);

			bFoundPath = true;
			InvalidateRect(0, 0);
		}
//		else ((CMainFrame*)GetParentFrame())->m_wndStatusBar.SetPaneText(0, L"Could not find path!");		*******
	}



	bViewPointsChecked = TRUE;
}

void CA_starView::OnEditClassicala()
{
	m_dAlgorithm = ASTAR_CLASSIC;
}

void CA_starView::OnFileNew()
{
	// TODO: Add your command handler code here
	CRandomSel randsel;
	if (IDCANCEL == randsel.DoModal())
	{
		return;
	}

	if (GetDocument()->IsModified())
	{
		GetDocument()->SaveModified();
	}
	result.clear();
	nrInClosedList = -1;
	searches.clear();

	//we first allow to place blocks, nothing else.
	m_dPlace = PLACE_BLOCK;

	if (Map!= NULL)
	{
		delete[] Map;
	}

	dNrRounds = randsel.m_nrRounds;
	dNrColumns = randsel.m_nrColumns;
	dNrRandoms = randsel.m_rgen==TRUE ? randsel.m_nrRandom : -1;

	Map = new (BYTE[dNrRounds * dNrColumns]);
	memset(Map, 0, dNrRounds * dNrColumns);

	//placing random blocks:
	if (dNrRandoms > -1)
	{

		srand( (unsigned)time( NULL ) );
		int RANGE_MIN = 0;
		int RANGE_MAX = dNrRounds * dNrColumns;
		int nrs2fulfill = 0;
		int randRounds, i, j;
		while (nrs2fulfill < dNrRandoms)
		{
			randRounds = (((double) rand() / (double) RAND_MAX) * RANGE_MAX + RANGE_MIN);
			i = randRounds/dNrColumns;
			j = randRounds - i * dNrColumns;
			if (Map[i*dNrColumns + j] == 0)
			{
				Map[i*dNrColumns + j] = 1;
				nrs2fulfill++;
			}
		}
	}

	//initialize Source and Dest points, so that they are not on the map
	Source.x = -1; Source.y = -1;
	Dest.x = -1; Dest.y = -1;
//	this->InvalidateRect(0,0);
	this->m_ScrollPosition.x = 0;
	this->m_ScrollPosition.y = 0;
	this->RedrawWindow();

	GetDocument()->OnNewDocument();
	this->GetDocument()->SetModifiedFlag();
	this->GetParentFrame()->SetTitle(L"Untitled");
	GetDocument()->SetTitle(L"Untitled.map");
	//of course, no path found.
	bFoundPath = false;
}

void CA_starView::OnViewPointschecked()
{
	// TODO: Add your command handler code here
	bViewPointsChecked = !bViewPointsChecked;
	if (bFoundPath) InvalidateRect(0, 0);
}

void CA_starView::OnUpdateViewPointschecked(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(bFoundPath);
	pCmdUI->SetCheck(bViewPointsChecked);
}

void CA_starView::OnHeuristicManhattandistance()
{
	dHeuristic = MANHATTAN;
}

void CA_starView::OnUpdateHeuristicManhattandistance(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(dHeuristic == MANHATTAN);
}

void CA_starView::OnHeuristicEuclidiandistance()
{
	dHeuristic = EUCLIDIAN;
}

void CA_starView::OnUpdateHeuristicEuclidiandistance(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(dHeuristic == EUCLIDIAN);
}

void CA_starView::OnEditPlacestartpoint()
{
	m_dPlace = PLACE_START;
}

void CA_starView::OnEditPlaceendpoint()
{
	m_dPlace = PLACE_END;
}

void CA_starView::OnUpdateEditPlaceendpoint(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(dNrRounds != -1);
	pCmdUI->SetCheck(m_dPlace == PLACE_END);
}

void CA_starView::OnUpdateEditPlacestartpoint(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(dNrRounds != -1);
	pCmdUI->SetCheck(m_dPlace == PLACE_START);
}

void CA_starView::OnEditClearpath()
{
	result.clear();
	InvalidateRect(0,0);
}

void CA_starView::OnEditClearmap()
{
	memset(Map, 0, dNrRounds * dNrColumns);
	Source.x = Source.y = Dest.x = Dest.y = -1;
	result.clear();
	bFoundPath = FALSE;
	searches.clear();
	InvalidateRect(0, 0);
}

void CA_starView::OnUpdateEditClearpath(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!searches.empty());
}

void CA_starView::OnUpdateEditClearmap(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(dNrRounds > -1 || Source.x > -1 || Dest.x > -1 || !searches.empty());
}

void CA_starView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);

	if (nFlags & MK_LBUTTON && nFlags & MK_CONTROL)
	{
		if (m_dPlace == PLACE_BLOCK)
		{
			if (/*point.x > 20 && point.y > 20 && */
				point.x < /*20 + */dNrColumns * m_dSquareSize && point.y < /*20 + */dNrRounds * m_dSquareSize)
			{
				int i,j;
				i = (point.y/* - 20*/)/m_dSquareSize;
				j = (point.x/* - 20*/)/m_dSquareSize;

				Map[i*dNrColumns + j] = 1;//!Map[i*dNrColumns + j];
				if (i == Source.y && j == Source.x)
				{
					Source.x = -1; Source.y = -1;
				}

				if (i == Dest.y && j == Dest.x)
				{
					Dest.x = -1; Dest.y = -1;
				}

				InvalidateRect(0,0);
				this->GetDocument()->SetModifiedFlag();
				bFoundPath = false;
				bViewPointsChecked = FALSE;
				nrInClosedList = -1;
				result.clear();
				searches.clear();
			}
			return;
		}
	}

	else if (nFlags & MK_LBUTTON && m_dPlace == SCROLL_MODE)
	{
		//we always use scroll_to (never scroll_with).
		//we set the scroll position to current_pos - initial_pos.

		m_ScrollPosition.x -= point.x - m_LastPoint.x;
		m_ScrollPosition.y -= point.y - m_LastPoint.y;

		m_LastPoint = point;

		//we should limit the scroll position to its maximum value:
		if (m_ScrollPosition.x > m_MaxScrollPosition.x) m_ScrollPosition.x = m_MaxScrollPosition.x;
		if (m_ScrollPosition.y > m_MaxScrollPosition.y) m_ScrollPosition.y = m_MaxScrollPosition.y;
		
		//scroll position should always be positive (we don't have negative position on map)
		if (m_ScrollPosition.x < 0) m_ScrollPosition.x = 0;
		if (m_ScrollPosition.y < 0) m_ScrollPosition.y = 0;

		//we ask for a window redraw
		this->RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW/* | RDW_ERASENOW*/);
	}
}
void CA_starView::OnHeuristicDiagonalshortcut()
{
	dHeuristic = DIAGONAL_SHORTCUT;
}

void CA_starView::OnUpdateHeuristicDiagonalshortcut(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(dHeuristic == DIAGONAL_SHORTCUT);
}

void CA_starView::OnHeuristicNoheuristic()
{
	dHeuristic = NO_HEURISTIC;
}

void CA_starView::OnUpdateHeuristicNoheuristic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(dHeuristic == NO_HEURISTIC);
}

void CA_starView::OnViewOptions()
{
	COptionsDlg options(this);
	if (options.DoModal() == IDOK)
	{
		if (m_dSquareSize != options.m_dSquareSize)
			m_ScrollPosition.SetPoint(0,0);
		m_dSquareSize = options.m_dSquareSize;
		switch (options.m_dUserObjSize)
		{
			case 0: m_dUserObjSize = m_dSquareSize/2; break; 
			case 1: m_dUserObjSize = m_dSquareSize/2 + m_dSquareSize/4; break;
			case 2: m_dUserObjSize = m_dSquareSize; break;
		}
		m_dUserObjSizeOption = options.m_dUserObjSize;

		switch (options.m_dNeutObjSize)
		{
			case 0: m_dNeutObjSize = m_dSquareSize/2; break; 
			case 1: m_dNeutObjSize = m_dSquareSize/2 + m_dSquareSize/4; break;
			case 2: m_dNeutObjSize = m_dSquareSize; break;
		}
		m_dNeutObjSizeOption = options.m_dNeutObjSize;
		this->RedrawWindow();
	}
}

void CA_starView::OnUpdateEditScrollmode(CCmdUI *pCmdUI)
{
	CRect cl_size;
	GetClientRect(cl_size);

	//if the image is less than the view
	bool enable = m_dSquareSize * dNrRounds > cl_size.Height() || 
		m_dSquareSize * dNrColumns > cl_size.Width();

	pCmdUI->Enable(enable);
	pCmdUI->SetCheck(m_dPlace == SCROLL_MODE);

	if (!enable)
	{
		m_LastPoint.x = -1;
		m_LastPoint.y = -1;

		m_ScrollPosition.x = 0;
		m_ScrollPosition.y = 0;
	}
}
void CA_starView::OnEditScrollmode()
{
	m_dPlace = SCROLL_MODE;
	//we set the cursor!
}

BOOL CA_starView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	HCURSOR hCursor = m_dPlace == SCROLL_MODE ?  LoadCursor(0, IDC_HAND) : LoadCursor(0, IDC_ARROW);
	SetCursor(hCursor);

//	return CView::OnSetCursor(pWnd, nHitTest, message);
	return 0;
}

void CA_starView::OnAlgorithmVectors()
{
	m_dAlgorithm = ALG_VECTORS;
}

void CA_starView::OnUpdateEditClassicala(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_dAlgorithm == ASTAR_CLASSIC);
}

void CA_starView::OnUpdateAlgorithmVectors(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_dAlgorithm == ALG_VECTORS);
}
