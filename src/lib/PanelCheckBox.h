#ifndef PANEL_CHECKBOX_H
#define PANEL_CHECKBOX_H


#include "PanelSimple.h"
#include "PanelText.h"
#include "Texture2D.h"
#include "TextUtil.h"


typedef void (* CALLBACK_TRUE)();
typedef void (* CALLBACK_FALSE)();




class PanelCheckBox : public PanelSimple {
	public:
		PanelCheckBox();
		
		
		//void				buildText();

		virtual void		clickLeft( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		void				debug(bool);
		void				setFalse( char *);
		void				setTrue( char *);
		
		void				setCallBackTrue( CALLBACK_TRUE p)		{ pCallBackTrue  = p; }
		void				setCallBackFalse( CALLBACK_FALSE p)		{ pCallBackFalse = p; }
		
		bool				getVal()								{ return bVal; }
		inline void			setVal( bool b )						{ bVal = b; }

	protected:
		_Texture2D*		m_pTexTrue;
		_Texture2D*		m_pTexFalse;

		CALLBACK_TRUE	pCallBackTrue;;
		CALLBACK_FALSE	pCallBackFalse;
		
		bool			bVal;
//	private:
		/*	
	protected:
		_Texture2D*		m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
		*/
};


#endif

