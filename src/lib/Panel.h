#ifndef PANEL_H
#define PANEL_H


#include <vector>
#include <string>
#include "Constante.h"

using namespace std;


typedef void (*click_left_cb_t)(int,int);
typedef void (*release_left_cb_t)(int,int);
typedef void (*click_right_cb_t)(int,int);
typedef void (*release_right_cb_t)(int,int);

typedef void (*displayGL_cb_t)(void);

class Panel {
	public:
		//-----------------------------------------------------------------------------------
							Panel();
		//-----------------------------------------------------------------------------------
		void 				init();


		void				add( Panel* );
		void				sup( Panel* );
		
		virtual void		clickLeft( int, int);
		virtual void		motionLeft( int, int)                         {;};
		virtual void		releaseLeft( int, int);

		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);

		virtual void		clickRight( int, int);
		virtual void		motionRight( int, int)                         {;};
		virtual void		releaseRight( int, int);

		virtual void		clickMiddle( int, int)                          {;};
		virtual void		motionMiddle( int, int)                         {;};
		virtual void		releaseMiddle( int, int)                        {;};

		virtual void		wheelUp( int, int)                              {;};
		virtual void		wheelDown( int, int)                            {;};

		virtual Panel*		isMouseOver( int, int);
		virtual Panel*		isMouseOverBorder( int, int);

		virtual void		haveFocus() 	    							{ bFocus = true;}
		virtual void		lostFocus() 									{ bFocus = false;}

		virtual void		haveCapture()	    							{;}
		virtual void		lostCapture()									{;}

		virtual void		displayGL();
		virtual void		updatePos();
		virtual void		idle(float);//										{;};
		virtual void		cb_keyboard( unsigned char ) 					{;};
		virtual void		cb_keyboard_special( unsigned char )			{;};
		virtual void		cb_keyboard_special_up( unsigned char )			{;};
		virtual void		debug( bool )									{;};
		
		inline void			setParent( Panel* p )							{parent = p;};
		inline Panel*		getParent()										{return parent;};
		inline void			setPosAndSize(int x0, int y0, int dx0, int dy0)	{x=x0; y=y0; dx=dx0 ;dy=dy0;};
		inline void			setPos(int x0, int y0)							{x=x0; y=y0;};
		inline void			setSize(int dx0, int dy0)						{dx=dx0 ;dy=dy0;};
		inline int			getX()											{return x_raw;};
		inline int			getY()											{return y_raw;};
		inline int			getDX()											{return dx_raw;};
		inline int			getDY()											{return dy_raw;};
		inline void			setID(int id)									{ID = id;};
		inline int			getID()											{return ID;};
		
		inline void			setPosX(int i)									{x = i;};
		inline void			setPosY(int i)									{y = i;};
		inline void			setPosDX(int i)									{dx = i;};
		inline void			setPosDY(int i)									{dy = i;};
		inline int			getPosX()										{return x;};
		inline int			getPosY()										{return y;};
		inline int			getPosDX()										{return dx;};
		inline int			getPosDY()										{return dy;};
	
		inline bool			getVisible()									{return visible;};
		inline void			setVisible(bool b)								{visible=b;};
		
		inline bool			getCanMove()									{return canMove;};
		inline void			setCanMove(bool b)								{canMove=b;};
		

		inline void			setDisplayGL(displayGL_cb_t cb)                 {displayGL_cb=cb;};

		inline void			x2Screen(int & x)                               {x += x_raw;};
		inline void			y2Screen(int & y)                               {y += y_raw;};
		
		inline void			xy2Screen(int& x, int& y)                       {x += x_raw;y += y_raw;}
		
		inline int			Screen2x(int x)                                 {return x - x_raw;};
		inline int			Screen2y(int y)                                 {return y - y_raw;};
		
		inline 	std::vector<Panel*>& getChilds()							{return childs;};
		
		inline void			haveMove()                                      { bHaveMove = true;}
		inline bool			getHaveMove()                                   { return bHaveMove;}
		inline void			resetHaveMove()                                 { bHaveMove = false;}
		
		inline void			setClickLeft( click_left_cb_t cb)               { click_left_cb = cb;}
		inline void			setReleaseLeft( click_left_cb_t cb)             { release_left_cb = cb;}
		inline void			setClickRight( click_left_cb_t cb)              { click_right_cb = cb;}
		inline void			setReleaseRight( click_left_cb_t cb)            { release_right_cb = cb;}
		
		inline void			setPanelClickLeft( Panel* p )                   { panel_click_left   = p;}
		inline void			setPanelReleaseLeft( Panel* p )                 { panel_release_left = p;}
		inline void			setPanelClickRight( Panel* p  )                 { panel_click_right  = p;}
		inline void			setPanelReleaseRight( Panel* p )                { panel_release_right= p;}
		
		inline void			setScissor(bool b)                              { bScissor = b;}
		inline void			setFantome(bool b)                              { bFantome = b;}
		
		inline int 			getMouseOverBorder()                            { return mouseOverBorder;}

    	inline  void        setExtraString(string s)                        { sExtra = s; }
    	inline  string&     getExtraString()                                { return sExtra; }
	

		void				deleteChilds();
		

		//-----------------------------------------------------------------------------------
	protected:
		//-----------------------------------------------------------------------------------
		int					ID;
		int					TYPE;
		int					x_raw;
		int					y_raw;
		int					dx_raw;
		int					dy_raw;
	
		int					x;
		int					y;
		int					dx;
		int					dy;
	
		int					startX;
		int					startY;
	
		bool				visible;
		bool				canMove;
		bool				mouseVisible;
		int                 mouseOverBorder;
	
		Panel* 				parent;
		std::vector<Panel*> childs;
		
		bool				bDebug;
		
		displayGL_cb_t      displayGL_cb;
		click_left_cb_t     click_left_cb;
		release_left_cb_t   release_left_cb;
		click_right_cb_t    click_right_cb;
		release_right_cb_t  release_right_cb;
		
		bool                bHaveMove;
		bool                bScissor;
		bool                bFantome;
		bool                bFocus;
		bool                bCapture;

    	string              sExtra;

        Panel*              panel_click_left;
        Panel*              panel_release_left;
        Panel*              panel_click_right;
        Panel*              panel_release_right;

};


#endif

