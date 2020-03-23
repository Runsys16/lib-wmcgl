#ifndef PANEL_SIMPLE_H
#define PANEL_SIMPLE_H


#include <GL/glew.h>


#include "Panel.h"
#include "PanelText.h"
#include "PanelTextOmbre.h"
#include "Texture2D.h"
#include "TextUtil.h"

class PanelSimple : public Panel {
	public:
		PanelSimple();
		
		//void				buildText();

		void                setColor(long);

		virtual void		displayGL();
		virtual void		updatePos();
		virtual void        debug(bool);
		
		void				setBackground( char *);
		void				setBackground( GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d );
		void				deleteBackground();


//	private:
	protected:
		_Texture2D*		                m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
		
		unsigned long                   c;
};


#endif

