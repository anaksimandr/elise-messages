/*
Elise Messages Plugin for Miranda IM

*/

#include "services.h"
#include "Elise.h"
#include "m_ieview.h"

int HandleEliseWindow(WPARAM wParam, LPARAM lParam) {
	IEVIEWWINDOW *window = (IEVIEWWINDOW *) lParam;
	if (window->iType == IEW_CREATE) {
		Elise * view = new Elise(window->parent, window->x, window->y, window->cx, window->cy);
		window->hwnd = view->getHWND();
	}
	else if (window->iType == IEW_SETPOS) {
		Elise * view = Elise::get(window->hwnd);
		if (view!=NULL) {
			view->setWindowPos(window->x, window->y, window->cx,window->cy);
		}
	}
	else if (window->iType == IEW_SCROLLBOTTOM) {
		Elise * view = Elise::get(window->hwnd);
		if (view!=NULL) {
			view->scrollToBottom();
		}
	}
	else if (window->iType == IEW_DESTROY) {
		Elise * view = Elise::get(window->hwnd);
		if (view!=NULL) {
			delete view;
		}
	}
	return 0;
}

int HandleEliseEvent(WPARAM wParam, LPARAM lParam) {
	IEVIEWEVENT* event = (IEVIEWEVENT *) lParam;
	Elise * view = Elise::get(event->hwnd);
	if (view != NULL) {
		if (event->iType == IEE_LOG_DB_EVENTS) {
			view->appendEventOld(event);
			//MessageBox(NULL, L"IEE_LOG_DB_EVENTS", L"Debug", MB_OK);
		}
		else if (event->iType == IEE_CLEAR_LOG) {
			view->clear(event);
		}
		else if (event->iType == IEE_GET_SELECTION) {
			return (int)view->getSelection(event);
		}
		else if (event->iType == IEE_SAVE_DOCUMENT) {
			view->saveDocument();
		}
		else if (event->iType == IEE_LOG_MEM_EVENTS) {
			view->appendEvent(event);
			//MessageBox(NULL, L"IEE_LOG_MEM_EVENTS", L"Debug", MB_OK);
		}
	}
	return 0;
}
