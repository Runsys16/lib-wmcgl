#ifndef WM_H
#define WM_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <string>

#include "Panel.h"
#include "Font.h"
#include "TextUtil.h"

//#define DEBUG_WM

class WindowsManager {

public:
	void				init();
	void				setScreenSize(int, int);
	void				add(Panel *);
	void				sup(Panel *);
	void				supByID(int);
	int					getFreeID();
	Panel *				findPanelMouseOver(int, int);
	void				movePanel( int, int);
	void				swapVisible();
	
	/*
	void			setWidth( int);
	int				getWidth();
	void			setHeight( int );
	int				getHeight();
	Font*			getFonts();
	int				getOffsetX();
	int				getOffsetY();
	*/


	inline void			setWidth( int w)				{width=w;}
	inline int			getWidth()						{return width;}
	inline void			setHeight( int h )				{height=h;}
	inline int			getHeight()						{return height;}
	inline Font*		getFonts()						{return fonts;}
	inline int			getOffsetX()					{return 0;}
	inline int			getOffsetY()					{return 0;}
	inline TextUtil*	getTextUtil()					{return &textUtil;}


	void				idleGL();
	void				displayGL();
	void				clearBufferGL();
	void				clearBufferGL( GLbitfield );
		
	void				passiveMotionFunc(int, int);
	void				motionFunc(int, int);
	void				mouseFunc(int, int, int, int);
	void				keyboardUpFunc( unsigned char, int, int );
	void				keyboardFunc( unsigned char, int, int );
	void				keyboardSpecialUpFunc( unsigned char, int, int );
	void				keyboardSpecialFunc( unsigned char, int, int );
	
	inline static WindowsManager&	getInstance()			{ if (!instance) instance = new WindowsManager();return *instance;}
	inline static void				Destroy()				{ if (instance) delete instance;instance=0;}
	
	WindowsManager();
	WindowsManager(int, int);
	~WindowsManager();

	int			iTest;

private:
	static WindowsManager*	instance ;
	
	int 					width;
	int						height;
	std::vector<Panel*>		childs;
	//std::vector<Panel*>		panels;
	Font*					fonts;
	
	int						xm_old;
	int						ym_old;
	
	void * 					cTextObj;
	TextUtil				textUtil;
	std::string				str[10];


};

//WindowsManager* WindowsManager::instance = 0;


#endif

