#ifndef PANEL_SIMPLE_H
#define PANEL_SIMPLE_H


#include "Panel.h"
#include "PanelText.h"
#include "Texture2D.h"
#include "TextUtil.h"

class PanelSimple : public Panel {
	public:
		PanelSimple();
		
		void				buildText();

		virtual void		displayGL();
		virtual void		updatePos();
		
		void				debug(bool);

	private:
		_Texture2D*		m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
};


#endif

