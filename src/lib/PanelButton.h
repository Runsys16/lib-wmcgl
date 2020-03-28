#ifndef PANEL_BUTTON_H
#define PANEL_BUTTON_H


#include "PanelSimple.h"
#include "PanelText.h"
#include "Texture2D.h"
#include "TextUtil.h"


class PanelButton;

typedef void (* CALLBACK_MOUSE)(PanelButton*);
//typedef void (* CALLBACK_DOWN)(PanelButton*);

class PanelButtonCallBack	{
	public :
	virtual void					cb_button_mouse_over(PanelButton*)				{;};
	virtual void					cb_button_mouse_down(PanelButton*)				{;};
	virtual void					cb_button_mouse_up(PanelButton*)				{;};
};


class PanelButton : public PanelSimple {
	public:
		PanelButton();
		PanelButton( PanelButtonCallBack* );
		void init();
		
		
		virtual void		haveCapture();
		virtual void		lostCapture();
		virtual void		clickLeft( int, int);
		virtual void		releaseLeft( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();

		void				debug(bool);

		void				texUp();
		void				texDown();
		void				texOver();
		
		void				setUp( char *);
		void				setDown( char *);
		void				setOver( char *);
		
		void				setCallBackOver( CALLBACK_MOUSE p)		{ pCallBackOver = p; }
		void				setCallBackUp( CALLBACK_MOUSE p)		{ pCallBackUp = p; }
		void				setCallBackDown( CALLBACK_MOUSE p)		{ pCallBackDown = p; }

	protected:
		PanelButtonCallBack*	pPanelButtonCallBack;
		_Texture2D*				m_pTexUp;
		_Texture2D*				m_pTexDown;
		_Texture2D*				m_pTexOver;

		_Texture2D*				m_pTexCurrent;
		
		CALLBACK_MOUSE			pCallBackDown;
		CALLBACK_MOUSE			pCallBackUp;
		CALLBACK_MOUSE			pCallBackOver;;
};


#endif

