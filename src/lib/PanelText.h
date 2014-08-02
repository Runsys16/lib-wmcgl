#ifndef TEXT_H
#define TEXT_H

#include "Panel.h"
#include "TextUtil.h"




class PanelText : public Panel	{

	public:
		enum FONT { ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO, NORMAL_FONT, SMALL_FONT, LARGE_FONT };


		PanelText();
		PanelText( std::string );
		PanelText( std::string, FONT );
		PanelText( std::string, FONT, int, int );

		void 				buildString();
		
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		
		void				displayGL();
		void				updatePos();
		
	private:
		void				displayGLInternal();
		std::string			strFont();		

		TextUtil*		textUtil;
		FONT			typeFont;
		std::string		text;
		bool			bChange;
		
		void*			pTextGL;
	
};

#endif

