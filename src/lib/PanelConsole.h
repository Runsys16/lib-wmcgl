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

									PanelConsole();
									PanelConsole( int, int );
		
		void						setPrompt( std::string );
		void						setCallBackCmd( CB_CMD );
		void						setCallBackCmd( PanelConsoleCallBack * );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		void						addCmd(std::string cmd);
		
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		void						moveCursor();
		void						scroll();
		void						rotateBuffer();
		void						addLine();
		void						eraseTexts();
		void						clear();
		void						affiche( std::string * );
		void						affiche( std::string );
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
		
	private:
		int							maxCmd;
		std::vector<std::string*> 	cmds;
		std::vector<PanelText *>	texts;
		std::string					prompt;
		PanelText					cursor;
		CB_CMD						cb_cmd;
		PanelConsoleCallBack*		ppccb;
		
		int							heightLine;
		int							tabSize;
		
		int 						currentLine;
		int							currentPos;
		int 						currentCmd;
		std::string					svgCmd;
		
		float						cursorTime;
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		long                        color;
};

#endif

