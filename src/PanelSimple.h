#ifndef PANEL_SIMPLE_H
#define PANEL_SIMPLE_H


#include "Panel.h"
#include "Texture2D.h"
#include "TextUtil.h"

class PanelSimple : public Panel {
	public:
		PanelSimple();
		
		void				displayGL();
		void				updatePos();
		
	private:
		Texture2D*		m_pTexBackground;

		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;
};


#endif

