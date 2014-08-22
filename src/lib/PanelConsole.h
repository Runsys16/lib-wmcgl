#ifndef PANELCONSOLE_H
#define PANELCONSOLE_H

#include <vector>

#include "PanelSimple.h"
#include "PanelText.h"
#include "TextUtil.h"


typedef void (* CB_CMD)(std::string);

class PanelConsoleCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};


class PanelConsole : public PanelSimple	{
	public:

									PanelConsole( int );
		
		void						setPrompt( std::string );
		void						setCallBackCmd( CB_CMD );
		void						setCallBackCmd( PanelConsoleCallBack * );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		void						addLine();
		void						affiche( std::string * );
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
		void						moveCursor();
		
	private:
		std::vector<PanelText *>	texts;
		std::string					prompt;
		PanelText					cursor;
		CB_CMD						cb_cmd;
		PanelConsoleCallBack*		ppccb;
		
		int 						currentLine;
		int							currentPos;
		
		float						cursorTime;
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
};

#endif

