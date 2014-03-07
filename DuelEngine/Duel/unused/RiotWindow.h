//  [7/15/2012 OMEGA]

#include "RiotCommon.h"
#include "RiotString.h"


#ifndef _RIOTWINDOW_H_
#define	_RIOTWINDOW_H_

namespace Riot{
	class Window;



	// �����: WndProc
	class Window
	{
	public:
		Window();
		~Window();

		//--------------------------------------------
		// һ��window��������ж��EventListentner
		// ����ʵ�ֻص�
		class EventListener
		{
		public:
			EventListener(){}
			virtual ~EventListener();

			virtual void windowActive(Window* win, bool flag){}
			virtual void windowMoved(Window* win){}
			virtual void windowResized(Window* win){}
			virtual bool windowClosing(Window* win){ return true;}
			virtual void windowClosed(Window* win){}
			// virtual����.
		};


		// Query function
		int32_t		getLeft();
		int32_t		getTop();
		uint32_t	getWidth();
		uint32_t	getHeight();
		HWND		getHWND();
		bool		isActivated();

		
		// operation function
		void creat(String const & name, String setting); // �����������ɴ�������. setting�������ļ�. �ű����ִ����
		void show(void);
		void addEventListentner(EventListener* e);	// ������ģʽ �㶮��.
		void deleteEventListener(EventListener* e);

		// CALLBACK function
		LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ��Ϣ����.
	
	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void		movedOrResized();
		void		setActive(bool flag);

		typedef std::vector<EventListener*> EventListenerList;
		EventListenerList *mListenerList;	// �¼�������ʱ��ͻ����ʹ��Listener�ĺ���.
		
	private:
		int32_t	 mLeft;
		int32_t	 mTop;
		uint32_t mWidth;
		uint32_t mHeight;

		bool mActive;
		WString	 mWname;
		HWND	 mhWnd;
	};

}

#endif