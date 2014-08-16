#ifndef PANELCONSOLE_H
#define PANELCONSOLE_H

#include <vector>

#include "PanelSimple.h"
#include "PanelText.h"
#include "TextUtil.h"



class PanelConsole : public PanelSimple	{

	public:

		PanelConsole( int );
		
		void						setPrompt( std::string );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		
		void						addLine();
		void						affiche( std::string * );
		void						addChar( char );
		void						supChar();
		
		void						incCursor();
		void						moveCursor();
		
	private:
		std::vector<PanelText *>	texts;
		std::string					prompt;
		PanelText					cursor;
		
		int 						currentLine;
		int							currentPos;
		
		float						cursorTime;
		bool						bIns;
	
};

#endif

