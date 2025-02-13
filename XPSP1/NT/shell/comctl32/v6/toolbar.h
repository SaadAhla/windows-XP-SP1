// common stuff for the toolbar control

#ifndef _TOOLBAR_H
#define _TOOLBAR_H

#define TBHIGHLIGHT_BACK
#define TBHIGHLIGHT_GLYPH

#include <uxtheme.h>

typedef struct 
{        /* info for recreating the bitmaps */
    int nButtons;
    HINSTANCE hInst;
    UINT_PTR wID;
} TBBMINFO, *PTBBMINFO;

typedef struct _TBBUTTONDATA 
{
    union
    {
        // Someone wanted to conserve space.  This is a union to make 
        // the code easier to read.
        int iBitmap;
        int cxySep;         // Used by separators
    }DUMMYUNIONNAME;
    int idCommand;
    BYTE fsState;
    BYTE fsStyle;
    WORD cx;
    DWORD_PTR dwData;
    INT_PTR iString;
    POINT pt;               // top left corner of this button
} TBBUTTONDATA, * LPTBBUTTONDATA;

#define HIML_NORMAL 0
#define HIML_HOT    1   // Image list for the hot-tracked image
#define HIML_DISABLED 2 // Image list for the hot-tracked image
#define HIML_MAX        2

typedef struct 
{
    HIMAGELIST himl[3];
} TBIMAGELISTS, *LPTBIMAGELISTS;

typedef struct 
{            /* instance data for toolbar window */
    CCONTROLINFO ci;
    DWORD dwStyleEx;
    HDC hdcMono;
    HBITMAP hbmMono;
    LPTBBUTTONDATA Buttons;     // Array of actual buttons
    LPTBBUTTONDATA pCaptureButton;
    POINT   ptCapture;
    HWND hwndToolTips;
    LPTSTR      pszTip;         // store current tooltip string.
    HWND hdlgCust;
    HFONT hfontIcon;
    int nBitmaps;
#ifdef GLYPHCACHE
    int nSelectedBM;            // currently selected pBitmaps index
#endif
    PTBBMINFO pBitmaps;
#ifdef FACECACHE
    HBITMAP hbmCache;
#endif
    PTSTR *pStrings;
    int nStrings;
    int nTextRows;              // # Rows of text per button
    UINT uStructSize;
    int iDxBitmap;
    int iDyBitmap;
    int iButWidth;
    int iButHeight;
    int iButMinWidth;           // The min and max width of the button. If the app does not
    int iButMaxWidth;           // have an opinion on what the min and max should be, these will be 0
    int iYPos;
    int iNumButtons;
    int dyIconFont;
    int dxDDArrowChar;
    int xFirstButton;
    int cxPad;
    int cyPad;
    int iListGap;               // space between icon and text on list-style buttons
    int iDropDownGap;           // padding after text on list-style drop-down buttons
    SIZE szCached;
    
#ifndef UNICODE
    BYTE bLeadByte;             // Save DBCS Lead Byte
#endif

    HDRAGPROXY hDragProxy;
    
    UINT uDrawText;
    UINT uDrawTextMask;

    COLORSCHEME clrsc;

    TBIMAGELISTS* pimgs;
    int cPimgs;
    
    int iHot;                   // Index of the currently Hot Tracked Button
    int iPressedDD;             // Index of the currently pressed dropdown button
    int iInsert;                // Index of the insertion mark, or -1 if none
    COLORREF    clrim;          // current insert mark color
    RECT rcInvalid;             // Saved invalid rectangle

    BITBOOL fHimlValid : 1;
    BITBOOL fHimlNative : 1;
    BITBOOL fFontCreated: 1;
    BITBOOL fNoStringPool :1;
    BITBOOL fTTNeedsFlush :1;

    BITBOOL fMouseTrack: 1;     // Are we currently tracking Mouse over this toolbar ?
    BITBOOL fActive: 1;
    BITBOOL fAnchorHighlight: 1;// TRUE: anchor the highlight to current position 
                                //       when mouse goes out of toolbar
    BITBOOL fRightDrag: 1;      // TRUE if current drag is right drag
    BITBOOL fDragOutNotify: 1;  // FALSE from start of drag until mouse leaves button
                                //       at which point it is TRUE until next drag
    BITBOOL fInsertAfter: 1;    // insert after (TRUE) or before (FALSE) button at iInsert?

    BITBOOL fRedrawOff : 1;     // did we get a WM_SETREDRAW = FALSE
    BITBOOL fInvalidate : 1;    // did we get any paint messages whilst we were fRedrawOff
    BITBOOL fRecalc : 1;        // did we try to call TBRecalc while we were fRedrawOff?
    
    BITBOOL fRequeryCapture :1; // app hack see comment on lbutton up
    BITBOOL fShowPrefix: 1;     // Show the underline of an item. Set with WM_KEYBOARDCUES

    BITBOOL fItemRectsValid:1;  // Are the cached button item rects valid?
    BITBOOL fAntiAlias: 1;    // Turn off AntiAliasing durning the create of a drag image.
    BITBOOL fForcedDoubleBuffer: 1; // Even though not explicitly set by external, we want to enable it. 
                                    // Double buffer has some bagage: It also means "FlickerFree/Efficient drawing"

    
    RECT rc;                    // cache rc of toolbar. (used only for TBSTYLE_EX_MULTICOL and TBSTYLE_EX_HIDECLIPPEDBUTTONS)
    SIZE sizeBound;             // largest bounding size in vertical multicolumn mode.

    HTHEME hTheme;

    int iTracking;              // Used for tooltips via keyboard (current item in focus for info display, >= 0 is tracking active)
    LPARAM lLastMMove;          // Filter out mouse move messages that didn't result in an actual move (for track tooltip canceling)
    
    int cxBarPad;
    int cyBarPad;
    int cxButtonSpacing;
    int cyButtonSpacing;
} TBSTATE, *PTBSTATE;

typedef struct {
/*REVIEW: index, command, flag words, resource ids should be UINT */
    int iBitmap;    /* index into bitmap of this button's picture */
    int idCommand;  /* WM_COMMAND menu ID that this button sends */
    BYTE fsState;   /* button's state */
    BYTE fsStyle;   /* button's style */
    int idsHelp;    /* string ID for button's status bar help */
} OLDTBBUTTON,* LPOLDTBBUTTON;


typedef struct _TBDRAWITEM
{
    TBSTATE * ptb;
    LPTBBUTTONDATA pbutton;

    UINT state;
    BOOL fHotTrack;

    // himl and image index
    int iIndex;
    int iImage;

    DWORD dwCustom;
    NMTBCUSTOMDRAW tbcd;
} TBDRAWITEM, * PTBDRAWITEM;


// toolbar keyboard tooltip tracking
#define TBKTT_NOTRACK           -1

#define TB_IsKbdTipTracking(ptb)  (ptb->iTracking != TBKTT_NOTRACK)

#define IDT_TRACKINGTIP         0

#ifdef __cplusplus
extern "C" {
#endif

HIMAGELIST TBGetImageList(PTBSTATE ptb, int iMode, int iIndex);
HIMAGELIST TBSetImageList(PTBSTATE ptb, int iMode, int iIndex, HIMAGELIST himl);
#define GET_HIML_INDEX GET_Y_LPARAM
#define GET_IMAGE_INDEX GET_X_LPARAM

HBITMAP SelectBM(HDC hDC, PTBSTATE pTBState, int nButton);
void DrawButton(HDC hdc, int x, int y, PTBSTATE pTBState, LPTBBUTTONDATA ptButton, BOOL fActive);
void DrawFace(HDC hdc, PRECT prc, int x, int y, int offx, int offy, int dxText, 
              int dyText, TBDRAWITEM * ptbdraw, int iListGap, PRECT prcText);
int  TBHitTest(PTBSTATE pTBState, int xPos, int yPos);
int  PositionFromID(PTBSTATE pTBState, LONG_PTR id);
void BuildButtonTemplates(void);
void TBInputStruct(PTBSTATE ptb, LPTBBUTTONDATA pButtonInt, LPTBBUTTON pButtonExt);
void TBOutputStruct(PTBSTATE ptb, LPTBBUTTONDATA pButtonInt, LPTBBUTTON pButtonExt);

BOOL SaveRestoreFromReg(PTBSTATE ptb, BOOL bWrite, HKEY hkr, LPCTSTR pszSubKey, LPCTSTR pszValueName);

void CustomizeTB(PTBSTATE pTBState, int iPos);
void MoveButton(PTBSTATE pTBState, int nSource);
BOOL DeleteButton(PTBSTATE ptb, UINT uIndex);
BOOL TBReallocButtons(PTBSTATE ptb, UINT uButtons);
BOOL TBInsertButtons(PTBSTATE ptb, UINT uWhere, UINT uButtons, LPTBBUTTON lpButtons, BOOL fNative);

LRESULT SendItemNotify(PTBSTATE ptb, int iItem, int code);
void TBInvalidateItemRects(PTBSTATE ptb);
void ReleaseMonoDC(PTBSTATE ptb);
void InitTBDrawItem(TBDRAWITEM * ptbdraw, PTBSTATE ptb, LPTBBUTTONDATA pbutton, 
                    UINT state, BOOL fHotTrack, int dxText, int dyText);
BOOL TBGetInfoTip(PTBSTATE ptb, LPTOOLTIPTEXT lpttt, LPTBBUTTONDATA pTBButton);
extern const int g_dxButtonSep;

BOOL TB_GetItemRect(PTBSTATE ptb, UINT uButton, LPRECT lpRect);

#ifdef __cplusplus
}
#endif

#endif // _TOOLBAR_H 
