#ifndef WM_H
#define WM_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <string>


#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include <typeinfo>


#include <ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <freetype2/freetype/ftglyph.h>
#include <freetype2/freetype/ftoutln.h>
#include <freetype2/freetype/fttrigon.h>



#include "Panel.h"
//#include "Font.h"
#include "TextUtil.h"
#include "ImageTools.h"
#include "Texture2D.h"

//#define DEBUG_CONST
//#define DEBUG_WM
//#define DEBUG

#include "Mathlib.h"
#include "Constante.h"


#ifndef WM_CPP
	extern void                log( char*  );
	extern void                logf(char *, ...);
    extern void                log_tab( bool );
#else
	void                log( char*  );
	void                logf(char *, ...);
    void                log_tab( bool );
#endif



class WindowsManager {

public:


	void				init();

/*
static	void			log( char*  );
static	void            logf(char *, ...);
static	void            log_tab( bool );
*/

	void				setScreenSize(int, int);
	void				add(Panel *);
	void				supCapture(Panel *);
	void				supFocus(Panel *);
	void				sup(Panel *);
	void				onTop(Panel *);
	void				onBottom(Panel *);
	void				supByID(int);
	Panel*				getByID(int);
	int					getFreeID();
	Panel*				getParentScroll(Panel*);
	Panel*				getParentRoot(Panel*);
	Panel*				findPanelMouseOver(int, int);
	int					getOrder(Panel*);
	void				movePanel( int, int);
	void				movePanel( int, int, Panel* );
	void				swapVisible();
	void				debug(bool);
	void				debug();
	void			    changeCapture(Panel *);
	void			    changeFocus(Panel *);
	
	/*
	void			setWidth( int);
	int				getWidth();
	void			setHeight( int );
	int				getHeight();
	Font*			getFonts();
	int				getOffsetX();
	int				getOffsetY();
	*/


	inline int 			getNbPanel()	     			{return childs.size();}
	inline void			setDebug( bool b )				{debug(b);}
	inline int			getDebug()						{return bDebug;}
	inline void			setWidth( int w)				{width=w;}
	inline int			getWidth()						{return width;}
	inline void			setHeight( int h )				{height=h;}
	inline int			getHeight()						{return height;}
//	inline _Font*		getFonts()						{return fonts;}
	inline int			getOffsetX()					{return 0;}
	inline int			getOffsetY()					{return 0;}
	inline TextUtil*	getTextUtil()					{return &textUtil;}
	inline void			setSlot( int s )				{slot=s;}
	inline int			getSlot()						{return slot;}
	inline Panel *  	getCapture()    				{return panelCapture;}
	inline Panel *  	getFocus()      				{return panelFocus;}
	std::vector<Panel*>&	getChilds()					{return childs;}

	void				ChangeViewport(int, int, int, int, int, int);

	void				idleGL(float);
	void				displayGL();
	void				clearBufferGL();
	void				clearBufferGL( GLbitfield );
		
	void				passiveMotionFunc(int, int);
    bool                isPanelFocus(Panel *);
    bool                isPanelCapture(Panel *);
	void				motionFunc(int, int);
	void				mouseFunc(int, int, int, int);
	void				keyboardUpFunc( unsigned char, int, int );
	void				keyboardFunc( unsigned char, int, int );
	void				keyboardSpecialUpFunc( unsigned char, int, int );
	void				keyboardSpecialFunc( unsigned char, int, int );

	void				call_back_keyboard( Panel * );
	void				sup_call_back_keyboard( Panel * );
	bool				is_call_back_keyboard( Panel * );

	inline void         startKeyboard()                 { bStopKeyboard = false; }
	inline void         stopKeyboard()                  { bStopKeyboard = true; }
	inline float        getMouseX()                     { return mouseX; }
	inline float        getMouseY()                     { return mouseY; }
	inline int          getMouseState(int but)          { return iMouseButton[but]; }

	int					getNbTextures()					{ return _Texture2D::getNbTextures(); }
	
static GLubyte*         OpenImage( const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d);


static void             loadResourceImage( const std::string& );


	inline static WindowsManager&	getInstance()			{ if (!instance) instance = new WindowsManager();return *instance;}
	inline static void				Destroy()				{ if (instance) delete instance;instance=0;}

	static void				        genereMipMap(bool b);

	WindowsManager();
	WindowsManager(int, int);
	~WindowsManager();

	int					iTest;

				
private:
	static WindowsManager*	instance;
	
	int 						width;
	int							height;
	std::vector<Panel*>			childs;
	//std::vector<Panel*>		panels;
	//_Font*						fonts;
	
	int							xm_old;
	int							ym_old;
				
	void * 						cTextObj;
	TextUtil					textUtil;
	std::string					str[10];

	bool						bMovePanel;
	bool						bResize;
	bool						bMotionMiddle;
	bool						bMotionLeft;
	bool						bMotionRight;
	
	Panel*						panelMove;
	Panel*						panelCapture;
	Panel*						panelFocus;
	Panel*						panelResize;
	Panel*						panelMotionMiddle;
	Panel*						panelMotionLeft;
	Panel*						panelMotionRight;
	std::vector<Panel *>		panelCallBackKeys;
	
	bool						bDebug;
	int							slot;

	bool						bStopKeyboard;
        
    //string                      sTab;
    //int                         nb_tab;
    
    float                       mouseX;
    float                       mouseY;
    int                         iMouseButton[10];
	
};

#ifdef WM_CPP
WindowsManager* WindowsManager::instance = 0;
#endif


#endif

