#ifndef TEXT_H
#define TEXT_H

#include "wm.h"
#include "Panel.h"
#include "TextUtil.h"
#include "TrueType.h"




class PanelText : public Panel	{

	public:
		enum FONT { 	ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO,\
					 	NORMAL_FONT, SMALL_FONT, LARGE_FONT, \
					 	FREE_TYPE };
		enum ALIGN { LEFT, RIGHT, CENTER };


							PanelText();
							PanelText( std::string );
							PanelText( std::string, FONT );
							PanelText( std::string, FONT, int, int );
							PanelText( std::string, FONT, int, int, unsigned int );
							PanelText( char*, FONT, int, int );
							PanelText( char*, FONT, int, int, unsigned int );
							PanelText( char*, char*, int, int, uint32_t, uint32_t );
							PanelText( char*, char*, int, int, uint32_t, uint32_t, bool );

		void 				init();
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
		int					getTabSize()									{return tabSize;}
		void				setColor( int l);
inline	void				setChangeColor(bool b)							{ bColor = b; }
	
		virtual Panel*		isMouseOver( int, int);
		void				displayGLfreetype();
		void				displayGLInternal();
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		inline void 		setChangeText(bool b )							{bChange = b;};
		inline void 		setAlpha(float a )								{ alpha = a;};
		
		//----------------- functions
		std::string			strFont();		

	protected:
		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		TrueType*			pTrueType;
		std::string			sFreeTypeName;
		std::string			text;
		uint32_t            color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
		bool                bStatic;
		bool				bColor;
		
		float				alpha;
	
};

#endif

