#include <afxwin.h>
#include "StaticLink.h"

IMPLEMENT_DYNAMIC(CStaticLink, CStatic)

BEGIN_MESSAGE_MAP(CStaticLink, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

CStaticLink::CStaticLink() : m_bVisited(FALSE)
// Sets default colors: blue/purple.
{
  m_ColorUnvisited = RGB(0, 0, 255);	// blue
  m_ColorVisited = RGB(128, 0, 128);	// purple
} // CStaticLink::CStaticLink

//-----------------------------------------------------------------------------
HBRUSH 
CStaticLink::CtlColor(CDC* pDC, UINT nCtlColor)
// Handles reflected WM_CTLCOLOR to set custom control color. For a text contol,
// use visited/unvisited colors and uinderline font. For non-text controls, do
// nothing. Also ensures SS_NOTIFY is on.
{
  if (nCtlColor != CTLCOLOR_STATIC)
    return NULL;


  DWORD dwStyle = GetStyle();
  if (!(dwStyle & SS_NOTIFY))
  {
    // turn on notify flag to get mouse messages and STN_CLICKED
    // this flag can also be turned on in AppStudio (property|style)
    ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);
  }
  
  HBRUSH hbr = NULL;
  
  // if this is a text control: setup font and colors
  if ((dwStyle & 0xFF) <= SS_RIGHT)
  {
    if (!(HFONT)m_Font)
    {
      // first time init: create font
      LOGFONT lf;
      GetFont()->GetObject(sizeof(lf), &lf);
      lf.lfUnderline = TRUE;
      m_Font.CreateFontIndirect(&lf);
    }
    
    // use underline font and visited/unvisited colors
    pDC->SelectObject(&m_Font);
    pDC->SetTextColor(m_bVisited? m_ColorVisited: m_ColorUnvisited);
    pDC->SetBkMode(TRANSPARENT);
    
    // return hollow brush to preserve parent background color
    hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
  }
  
  return hbr;
} // CStaticLink::CtlColor

void 
CStaticLink::OnClicked()
// Handles mouse click: opens URL/file.
{
  CWaitCursor wc;
  
  // if URL/filename not set
  if (m_sLink.IsEmpty())
  {
    // get it from window text
    GetWindowText(m_sLink);
  }
  
  // call ShellExecute to run the file, for an URL, this means opening it in
  // the browser
  HINSTANCE h = ShellExecute(NULL, _T("open"), m_sLink, NULL, NULL, SW_SHOWNORMAL);
  if (UINT(h) > 32)
  {
    m_bVisited = TRUE;	// not really - might not have found the link
    Invalidate();		// repaint to show visited color
  }
} // CStaticLink::OnClicked


BOOL 
CStaticLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
// Sets the hand cursor when the mouse point is over the control.
{
  HCURSOR hcursor = LoadCursor(NULL, MAKEINTRESOURCE(32649));
  if (!hcursor)
    return FALSE;

  ::SetCursor(hcursor);
  return TRUE;
} // CStaticLink::OnSetCursor

