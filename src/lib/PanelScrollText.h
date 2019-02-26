#ifndef PanelScrollText_H
#define PanelScrollText_H

#include <vector>

#include "PanelSimple.h"
#include "PanelText.h"
#include "TextUtil.h"


/*
typedef void (* CB_CMD)(std::string);

class PanelScrollTextCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};
*/

class PanelScrollText : public PanelSimple	{
	public:

									PanelScrollText();
									PanelScrollText( int, int );
		
		void						setPrompt( std::string );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		
		
		void						scroll();
		void						rotateBuffer();
		void						addLine();
		void						eraseTexts();
		void						clear();
		void						affiche( std::string * );
		void						affiche( std::string );
		void						addChar( char );
		
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );

		void						setColor( long );
		
	private:
		int							maxCmd;
		std::vector<PanelText *>	texts;
		std::string					prompt;
		
		int							heightLine;
		int							tabSize;
		
		int                         color;
		
		int 						currentLine;
		int							currentPos;
		int 						currentCmd;
		std::string					svgCmd;
		
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		
};

#endif

