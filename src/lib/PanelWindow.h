#ifndef PANEL_WINDOW_H
#define PANEL_WINDOW_H


#include "PanelSimple.h"

class PanelWindow : public PanelSimple {
	public:
		enum SKIN { STD, RED, BLACK };
	
		PanelWindow();
		
		void				loadSkin( std::string );
		void				loadSkin( SKIN );
		
		void				setPosAndSize(int, int, int, int);
		void				setPos(int, int);
		void				setdSize(int, int);

		inline void 		setBorderSize( int n )											{ borderSize = n; };
		inline int	 		getBorderSize()													{ return borderSize; };

		void				displayGLtex( _Texture2D* , float, float, float, float );

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

