// A_starDoc.cpp : implementation of the CA_starDoc class
//

#include "stdafx.h"
#include "A_star.h"

#include "A_starDoc.h"
#include <deque>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL bFoundPath;
extern deque<PATHPOINT> result;
extern deque<SEEKPOINT> searches;
extern BOOL bViewPointsChecked;
extern int nrInClosedList;

// CA_starDoc

IMPLEMENT_DYNCREATE(CA_starDoc, CDocument)

BEGIN_MESSAGE_MAP(CA_starDoc, CDocument)
END_MESSAGE_MAP()

extern CPoint Source, Dest;
extern BYTE* Map;
extern int dNrRounds, dNrColumns;

// CA_starDoc construction/destruction

CA_starDoc::CA_starDoc()
{
	// TODO: add one-time construction code here

}

CA_starDoc::~CA_starDoc()
{
}

BOOL CA_starDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CA_starDoc serialization

void CA_starDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here = saving
		ar<<dNrRounds;
		ar<<dNrColumns;
		ar<<Source;
		ar<<Dest;
		ar.Write(Map, dNrRounds * dNrColumns);
	}
	else
	{
		// TODO: add loading code here = opening
		ar>>dNrRounds;
		ar>>dNrColumns;
		ar>>Source;
		ar>>Dest;
		Map = new (BYTE[dNrRounds * dNrColumns]);
		ar.Read(Map, dNrRounds * dNrColumns);
		this->UpdateAllViews(0);

		bFoundPath = FALSE;
		//dNrRounds = -1;			nu avem nevoie de el totusi
		nrInClosedList = -1;

		result.clear();
		searches.clear();
	}
}


// CA_starDoc diagnostics

#ifdef _DEBUG
void CA_starDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CA_starDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG