#ifndef PANEL_H
#define PANEL_H


#include <vector>


class Panel {
	public:
		//-----------------------------------------------------------------------------------
							Panel();
		//-----------------------------------------------------------------------------------
		void 				init();
		void				add( Panel* );
		void				sup( Panel* );
		
		virtual void		clickLeft( int, int)							{;}
		virtual void		releaseLeft( int, int)							{;}

		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);

		virtual void		clickRight( int, int)							{;}
		virtual void		releaseRight( int, int)							{;}

		virtual Panel*		isMouseOver( int, int);
		virtual void		haveFocus()										{;}
		virtual void		lostFocus()										{;}
		virtual void		displayGL();
		virtual void		updatePos();
		virtual void		idle(float)										{;};
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
		
		
		inline 	std::vector<Panel*> getChilds()								{return childs;};
		
		
		//-----------------------------------------------------------------------------------
	protected:
		//-----------------------------------------------------------------------------------
		int					ID;
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
	
		Panel* 				parent;
		std::vector<Panel*> childs;
		
		bool				bDebug;
};


#endif

