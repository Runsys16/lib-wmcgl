#ifndef TEXT_H
#define TEXT_H

#include "Panel.h"
#include "TextUtil.h"




class PanelText : public Panel	{

	public:
		enum FONT { ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO, NORMAL_FONT, SMALL_FONT, LARGE_FONT };
		enum ALIGN { LEFT, RIGHT, CENTER };


							PanelText();
							PanelText( std::string );
							PanelText( std::string, FONT );
							PanelText( std::string, FONT, int, int );
							PanelText( std::string, FONT, int, int, unsigned int );
							PanelText( char*, FONT, int, int );
							PanelText( char*, FONT, int, int, unsigned int );

		void 				buildString();
		
		inline std::string	getText()										{return text;};
		int					getTextLenght();
		int					getTextLenght(int);

		void 				changeText( char* );
		void 				changeText( char*, FONT );
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		void 				changeText( std::string, bool );
		void 				changeText( std::string, FONT, bool );
		void 				eraseText( );
		void				setTabSize( int );
		void				setColor( int l);
		
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		
		//----------------- functions
		std::string			strFont();		
		void				displayGLInternal();

	protected:
		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		std::string			text;
		int    	            color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
		bool                bStatic;
	
};

#endif

