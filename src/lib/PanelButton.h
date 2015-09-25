#ifndef PANEL_BUTTON_H
#define PANEL_BUTTON_H


#include "PanelSimple.h"
#include "PanelText.h"
#include "Texture2D.h"
#include "TextUtil.h"


typedef void (* CALLBACK_OVER)();
typedef void (* CALLBACK_DOWN)();


class PanelButtonCallBack	{
	public :
	virtual void					callback_down()=0;
	virtual void					callback_over()					{};
};


class PanelButton : public PanelSimple {
	public:
		PanelButton();
		
		
		//void				buildText();

		virtual bool		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		void				debug(bool);
		void				setPush( char *);
		void				setDown( char *);
		void				setOver( char *);
		
		void				setCallBackOver( CALLBACK_OVER p)		{ pCallBackOver = p; }
		void				setCallBackDown( CALLBACK_DOWN p)		{ pCallBackDown = p; }

	protected:
		_Texture2D*		m_pTexPush;
		_Texture2D*		m_pTexDown;
		_Texture2D*		m_pTexOver;

		_Texture2D*		m_pTexCurrent;
		
		CALLBACK_DOWN	pCallBackDown;
		CALLBACK_DOWN	pCallBackOver;;
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

