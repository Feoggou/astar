// A_starDoc.h : interface of the CA_starDoc class
//


#pragma once


class CA_starDoc : public CDocument
{
protected: // create from serialization only
	CA_starDoc();
	DECLARE_DYNCREATE(CA_starDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CA_starDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual void OnCloseDocument();
public:
//	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


