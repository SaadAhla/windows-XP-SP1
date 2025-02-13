// KBMAIN.H      KEYBOARD MAIN HEADER FILE


/**************************************************************************/
/* GLOBAL DEFINES  */
/**************************************************************************/
// WINDOW TYPES
#define KB_MAIN_WND    	1
#define KB_KEY_WND	  	2
#define KB_MODIFIER_WND 3
#define KB_DEAD_WND		4
#define KB_NUMBASE_WND 	5
#define KB_NUMKEY_WND  	6
#define DESKTOP_ACCESSDENIED 0
#define DESKTOP_DEFAULT      1
#define DESKTOP_SCREENSAVER  2
#define DESKTOP_WINLOGON     3
#define DESKTOP_TESTDISPLAY  4
#define DESKTOP_OTHER        5

// KEYBOARD MARGINS
//The smallest width (pixel) you can get for KB
//#define KB_SMALLRMARGIN  152//137     //for width(smallKB).  152 - Block, 137 - Actual  (Moved to kbmain, WM_SIZE) 
#define KB_LARGERMARGIN  202     //for width
#define KB_CHARBMARGIN   57        //for height


#define KB_NUMRMARGIN
#define KB_NUMBMARGIN

// KEYBOARD NUMBERKEY
#define KB_HOWMANYKEY	 130    // IT HAS TO INCLUDE THE NUMERIC KEYBOARD 

// SOUND TYPES
#define SND_UP 		1
#define SND_DOWN 		2

// TIMERS
#define TIMER_HELPTOOLTIP 1014

#include "door.h"

LRESULT WINAPI kbMainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI kbKeyWndProc(HWND hwndKey, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeybdInputProc(WPARAM  wParam, LPARAM  lParam);
void RelocateDialog(HWND hDlg);

#define CAPLOCK_SCANCODE 0x3A
#define LSHIFT_SCANCODE  0x2A
#define RSHIFT_SCANCODE  0x36
#define TAB_SCANCODE     0x0f
#define BREAK_SCANCODE   0xE1
#define LMENU_SCANCODE   0x38
#define RMENU_SCANCODE   0x38 // extended 0xE0
#define CONTROL_SCANCODE 0x1D

#define RGB_KEYBGHIGHLIGHT COLOR_HIGHLIGHT /*RGB(0,0,0)*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#endif

/**************************************************************************/
/* GLOBAL VARIABLES  */
/**************************************************************************/

extern LPKBPREFINFO lpkbPref;				// Pointer to KB structure
extern LPKBPREFINFO lpkbDefault;			// ditto Default
extern HINSTANCE 	hInst;					// iinstance of keyboard process
extern HWND   	   *lpkeyhwnd;				// ptr to array of HWND
extern HWND 		numBasehwnd;			// HWND to the num base window
extern HWND			g_hwndOSK;				// HWND to the kbmain window
extern int 			lenKBkey;				// How Many Keys?
extern int          scrCY;					// Screen Height
extern int 			scrCX;					// Screen Width
extern int 			captionCY;				// Caption Bar Height
extern HHOOK		hkJRec;					// HHOOK to the Journal Record
extern int 			g_margin;         		// Margin between rows and columns
extern BOOL			smallKb;				// TRUE when working with Small Keyboard
extern COLORREF	PrefTextKeyColor;   		// Prefered Color for text in keys
extern COLORREF 	PrefCharKeyColor; 		// ditto normal key
extern COLORREF 	PrefModifierKeyColor;	// ditto modifier key
extern COLORREF 	PrefDeadKeyColor;       // ditto dead key
extern COLORREF	PrefBackgroundColor;        // ditto Keyboard backgraund
extern BOOL			PrefAlwaysontop;			// Always on Top control
extern int			PrefDeltakeysize;			// Preference increment in key size
extern BOOL			PrefshowActivekey;		// Show cap letters in keys
extern int			KBLayout;				// 101, 102, 106, KB layout
extern BOOL			Prefusesound;				// Use click sound
extern BOOL			newFont;						// Font is changed
extern LOGFONT		*plf;            			// pointer to the actual char font
extern HGDIOBJ 	oldFontHdle;     			// Old object handle
extern COLORREF	InvertTextColor; 			//Font color on inversion
extern COLORREF	InvertBKGColor;			//BKG color on inversion
extern BOOL			Prefhilitekey;				// True for hilite eky under cursor

// Dwelling time control variables
extern BOOL			PrefDwellinkey;			// use dwelling system
extern UINT			PrefDwellTime;		    // Dwell time preference

extern BOOL         PrefScanning;           //use scanning
extern UINT         PrefScanTime;           //Prefer scan time

extern BOOL		    g_fShowWarningAgain;		// Show initial warning dialog again

extern HWND			Dwellwindow;	  		// dwelling window

extern int	 		stopPaint;				// stop the bucket paint
extern 				UINT_PTR timerK1;       // timer id for keyboard
extern 				UINT_PTR timerK2;		// timer for print line on key
/*****************************************************************************/

extern BOOL g_fDrawCapital;
extern BOOL g_fCapsLockOn;
extern BOOL g_fShiftKeyDn;
extern BOOL g_fRAltKey;			// TRUE if the RALT key is down
extern BOOL	g_fLAltKey;			// TRUE if the left ALT key is down
extern BOOL g_fLCtlKey;         // TRUE if the left CTRL key is donw
extern HWND g_hwndInputFocus;   // the window we are inputting to


//
// These are the different 'modified' states a key can have. Note that shifted,
// caps-lock, and shifted-caps-lock are all different states, since alphabetic
// numbers and symbols behave differently in these states.
//
typedef enum {
    KEYMOD_NORMAL,
    KEYMOD_SHIFTED,
    KEYMOD_CAPSLOCK,
    KEYMOD_SHIFTEDCAPSLOCK,
    KEYMOD_ALTGR,
    KEYMOD_STATES
} KEYMODSTATE;



__inline BOOL AltGrKeyPressed()	
{ 
    // this check is relaxed because the OS sets LCTRL toggled when LALT key is pressed
    return (g_fRAltKey || (g_fLAltKey && g_fLCtlKey))? TRUE:FALSE; 
}

__inline BOOL CapsLockIsOn() 
{ 
	return (g_fCapsLockOn)?TRUE:FALSE; 
}

__inline KEYMODSTATE GetModifierState()
{
	// The order of keyboard state checking is important; ALTGR takes
	// precedence over the shift/capslock variants, which takes precedence
    // over normal.
    if( AltGrKeyPressed() )
        return KEYMOD_ALTGR;

    if( g_fCapsLockOn && g_fShiftKeyDn )
        return KEYMOD_SHIFTEDCAPSLOCK;

    if( g_fCapsLockOn )
        return KEYMOD_CAPSLOCK;

    if( g_fShiftKeyDn )
        return KEYMOD_SHIFTED;

    return KEYMOD_NORMAL;
}


__inline HWND OurInputTarget() 
{ 
	if (!IsWindow(g_hwndInputFocus))
	{
		g_hwndInputFocus = NULL;
	}
	return g_hwndInputFocus; 
}

__inline HKL GetCurrentHKL()
{
    DWORD dwProcessId;
    HWND hwnd = OurInputTarget();
	return GetKeyboardLayout(GetWindowThreadProcessId((hwnd)?hwnd:g_hwndOSK, &dwProcessId));
}

__inline void SetBackgroundColor(HWND hwnd, unsigned long ulColor)
{
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)(ulColor+1));
}

BOOL OSKRunSecure();

