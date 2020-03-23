#ifndef PANEL_WINDOW_H
#define PANEL_WINDOW_H


#include "PanelSimple.h"

class PanelWindow : public PanelSimple {
	public:
		enum SKIN { STD, RED, BLACK };
	
		PanelWindow();
		
		void				loadSkin( std::string );
		void				loadSkin( SKIN );
		
		void				setColor( long );

		void				setPosAndSize(int, int, int, int);
		void				setPos(int, int);
		void				setdSize(int, int);

		inline void 		setBorderSize( int n )											{ borderSize = n; };
		inline int	 		getBorderSize()													{ return borderSize; };

		void				displayGLtex( _Texture2D* , float, float, float, float );

        Panel *             isMouseOverUpperLeft(int,int);
        Panel *             isMouseOverUpper(int,int);
        Panel *             isMouseOverUpperRight(int,int);
        Panel *             isMouseOverLeft(int,int);
        Panel *             isMouseOverRight(int,int);
        Panel *             isMouseOverBottomLeft(int,int);
        Panel *             isMouseOverBottom(int,int);
        Panel *             isMouseOverBottomRight(int,int);

        virtual Panel*      isMouseOverBorder(int, int);
        virtual Panel*      isMouseOver(int, int);
		virtual void		clickLeft( int, int);
		virtual void		releaseLeft( int, int);

		virtual void		displayGL();
		virtual void		updatePos();
		
	private:
		_Texture2D*			m_tex_tl;
		_Texture2D*			m_tex_tr;
		_Texture2D*			m_tex_bl;
		_Texture2D*			m_tex_br;
		_Texture2D*			m_tex_t;
		_Texture2D*			m_tex_b;
		_Texture2D*			m_tex_l;
		_Texture2D*			m_tex_r;
		
		int					borderSize;
};


#endif

