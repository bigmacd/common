#ifndef __STATICLINK_H__
#define __STATICLINK_H__

/////////////////////////////////////////////////////////////////////////////
// CStaticLink implementes a static control that's a hyperlink to any file on
// the desktop or web.  It can be used in dialog boxes to create hyperlinks
// to web sites.  When clicked, opens the file/URL.
//
class CStaticLink : public CStatic
{
public:
	CStaticLink();

	COLORREF
		m_ColorUnvisited,	// unvisited color
		m_ColorVisited;		// visited color

	BOOL
		m_bVisited;			// flag: whether visited or not

	// URL/filename for non-text controls (e.g., icon, bitmap) or when link is
	// different from window text. If you don't seet this, CStaticLink will use
	// GetWindowText to get the link.
	CString m_sLink;

protected:
	DECLARE_DYNAMIC(CStaticLink)

	CFont
		m_Font;				// underline font for text control

	// message handlers
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnClicked();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
}; // CStaticLink

#endif
