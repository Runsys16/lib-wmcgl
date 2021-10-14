#ifndef TEXT_OMBRE_H
#define TEXT_OMBRE_H

#include "Panel.h"
#include "PanelText.h"
#include "TextUtil.h"




class PanelTextOmbre : public PanelText	{

	public:

							PanelTextOmbre();
							PanelTextOmbre( std::string );
							PanelTextOmbre( std::string, FONT );
							PanelTextOmbre( std::string, FONT, int, int );
							PanelTextOmbre( std::string, FONT, int, int, unsigned int );
							PanelTextOmbre( char*, FONT, int, int );
							PanelTextOmbre( char*, FONT, int, int, unsigned int );

		void 				buildString();
		
		inline std::string	getText()										{return text;};
//		int					getTextLenght();
//		int					getTextLenght(int);

		void 				changeText( char* );
		void 				changeText( char*, FONT );
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		void 				changeText( std::string, bool );
		void 				changeText( std::string, FONT, bool );
		void 				eraseText( );
		void				setTabSize( int );
		void				setColor( unsigned long l);
		
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		
		//----------------- functions
//		std::string			strFont();		
		void				displayGLInternal();

	/*
		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		std::string			text;
		unsigned int		color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
		*/
	protected:
		PanelText			pTextOmbre;
};

#endif

