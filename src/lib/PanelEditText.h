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
		void						setBackgroundInverse(char*);
		
		void						moveCursor();

		void						addCmd(std::string cmd);

		void						setTabSize( int );
		void                        setColor(long);

		void						addChar( char );
		void						delChar();
		void						supChar();

		void						incCursor();
		void						decCursor();
		void						resetCursor();

		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						supWord();
		void						delWord();
		void						majuscule();
		void						minuscule();

		bool						isAlpha( unsigned char );
		bool						isNum( unsigned char );
		bool						isAlphaNum( unsigned char );
		bool						isAlphaMaj( unsigned char );
		bool						isAlphaMin( unsigned char );

		virtual Panel*				isMouseOver( int, int);
		virtual void				haveFocus();
		virtual void				lostFocus();

		virtual void				clickLeft( int, int);
		
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		virtual void				idle(float);
		virtual void				updatePos();
		virtual void				displayGL();

		inline std::string	        getText()					{ return text->getText(); };
		inline void      	        changeText(std::string s)   { text->changeText(s, PanelText::NORMAL_FONT, true ); currentPos=0;  };
		inline void					hideCursor()				{ bHideCursor = true; }
		inline void					showCursor()				{ bHideCursor = false; }
		inline	void				setConstLarg(bool b)		{ bDxStr = b; }
		inline	bool				getConstLarg()				{ return bDxStr; }
		
	private:
		PanelText *                 text;
		std::string					prompt;
		PanelText					cursor;
		PanelSimple                 cursorInverse;
		CB_CMD						cb_cmd;
		PanelEditTextCallBack*		ppccb;
		
		int							heightLine;
		int							tabSize;
		
		//int 						currentLine;
		int							currentPos;
		//int 						currentCmd;
		//std::string					svgCmd;
		
		float						cursorTime;
		bool						bCursor;
		
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		bool						bHideCursor;
		long                        color;
		bool						bDxStr;
		_Texture2D*					TextureInverse;//
};

#endif

