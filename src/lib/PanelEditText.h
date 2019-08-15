#ifndef PANELEDITTEXT_H
#define PANELEDITTEXT_H

#include <vector>

#include "PanelSimple.h"
#include "PanelText.h"
#include "TextUtil.h"


typedef void (* CB_CMD)(std::string);

class PanelEditTextCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};


class PanelEditText : public PanelSimple	{
	public:

									PanelEditText();
									//PanelEditText( int, int );
		
		void						setPrompt( std::string );
		void						setCallBackCmd( CB_CMD );
		void						setCallBackCmd( PanelEditTextCallBack * );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		void						addCmd(std::string cmd);
		
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		void						moveCursor();
		void						addChar( char );
		void						supChar();
		void						delChar();
		void						supWord();
		void						delWord();
		
		void						incCursor();
		void						decCursor();
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );
		void                        setColor(long);

		inline std::string	        getText()					{ return text->getText(); };
		inline void      	        changeText(std::string s)   { text->changeText(s, PanelText::NORMAL_FONT, true ); };
		
	private:
		PanelText *                 text;
		std::string					prompt;
		PanelText					cursor;
		CB_CMD						cb_cmd;
		PanelEditTextCallBack*		ppccb;
		
		int							heightLine;
		int							tabSize;
		
		//int 						currentLine;
		int							currentPos;
		//int 						currentCmd;
		//std::string					svgCmd;
		
		float						cursorTime;
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		long                        color;
};

#endif

