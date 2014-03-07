//  [1/15/2013 OMEGA] created

#ifndef _DUELINPUTEVENTQUEUE_H_
#define _DUELINPUTEVENTQUEUE_H_

#include "DuelCommon.h"
#include "DuelInputComponent.h"
#include "DuelInputMouse.h"
#include "DuelInputKeyboard.h"
#include "DuelInputJoyStick.h"
#include "DuelTimer.h"


namespace Duel
{

	class DUEL_API DInputEventQueue : public DObject
	{
	DUEL_DECLARE_RTTI(DInputEventQueue)
		// ������뷨������:
		// һ��ջ, ������¼���������, mouse/joystic/key, ���Ҽ�¼ʱ��stamp,����Ҫ��ȷ����.(������Ҫ��������timer)
		// 3��ջ, �ֱ������洢����ľ����¼�, 3��ջ���¼����͵�����ջ���¼���, �������Ը�����ջ���������ֱ𵯳���Ӧջ����Ϣ.
	public:
		DInputEventQueue();
		virtual  ~DInputEventQueue() {}
		// set the maximum event counts for the EventInfoQueue.
		static void	setMaxEventCount(uint16 size = 1024);

		uint16		getEventCount() const { return mEventCount; }
		// push an event to the event stack.
		void		pushEvent(const DKeyEvent& e);
		void		pushEvent(const DMouseEvent& e);
		void		pushEvent(const DJoyStickEvent& e);
		// push event with specified time stamp.
		void		pushEvent(const DKeyEvent& e, uint64 time);
		void		pushEvent(const DMouseEvent& e, uint64 time);
		void		pushEvent(const DJoyStickEvent& e, uint64 time);

		// a struct recording event information.
		struct EventInfo
		{
			EventInfo() {}
			EventInfo(InputDeviceType type, uint64 time) : 
				eventType(type), 
				timeStamp(time) {}
			InputDeviceType	eventType;
			uint64			timeStamp;
		};

		// pop a event info, use it as a key to retrieve different event.
		// if the stack is empty, a false will be returned.
		bool		popEventInfo(EventInfo& info);
		// if the stack is empty, a false will be returned.
		bool		popEvent(DKeyEvent& event);
		bool		popEvent(DMouseEvent& event);
		bool		popEvent(DJoyStickEvent& event);

		// clear all events
		void		clearEvents();
	protected:
		// internal value
		static	uint16	maxEventCount;

		typedef	std::queue<EventInfo>		EventInfoQueue;
		typedef	std::queue<DKeyEvent>		KeyEventQueue;
		typedef	std::queue<DMouseEvent>		MouseEventQueue;
		typedef	std::queue<DJoyStickEvent>	JoyStickEventQueue;
		EventInfoQueue		mEventInforQueue;
		KeyEventQueue		mKeyEventQueue;
		MouseEventQueue		mMouseEventQueue;
		JoyStickEventQueue	mJoyStickEventQueue;

		// local timer, used to record time stamp.
		DTimer				mTimer;
		uint16				mEventCount;
	};

}


#endif