#ifndef PANEL_SCROLL_H
#define PANEL_SCROLL_H


#include "PanelSimple.h"
#include "PanelText.h"
#include "Texture2D.h"
#include "TextUtil.h"

class PanelScrollY : public PanelSimple {
	public:
		PanelScrollY();
		
		int 				computeDY();
		
		inline void			setDelta( int d )				{ y_delta = d; }
		//void				buildText();
		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);
		virtual Panel*		isMouseOver( int, int);


		virtual void		wheelUp( int, int);
		virtual void		wheelDown( int, int);

		virtual void		displayGL();
		virtual void		updatePos();

//	private:
	protected:
		int					dy_scroll;
		int					y_scroll;
		int					y_delta;
		
};


#endif

