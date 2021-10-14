#ifndef PANEL_CHECKBOX_H
#define PANEL_CHECKBOX_H


#include "PanelSimple.h"
#include "PanelText.h"
#include "Texture2D.h"
#include "TextUtil.h"

class PanelCheckBox;

typedef void (* CALLBACK_TRUE)();
typedef void (* CALLBACK_FALSE)();
typedef void (* CALLBACK_CHECK)(PanelCheckBox*);




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
		void				setCallBackMouse( CALLBACK_CHECK p)		{ pCallBackMouse = p; }
		
		bool				getVal()								{ return bVal; }
		void				setPercent( float );
		inline void			setVal( bool b )						{ bVal = b; }
		inline void			setListener( bool* p)					{ pVal = p; }
		inline void			setUseInverse( bool p)					{ bUseInverse = p; }

	protected:
		_Texture2D*		m_pTexTrue;
		_Texture2D*		m_pTexFalse;

		CALLBACK_TRUE	pCallBackTrue;;
		CALLBACK_FALSE	pCallBackFalse;
		CALLBACK_CHECK  pCallBackMouse;
		
		bool			bVal;
		bool*			pVal;
		bool			bUseInverse;
		
		float			percent;
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

