//  [3/15/2013 OMEGA] created

#ifndef _DUELSTATEMACHINE_H_
#define	_DUELSTATEMACHINE_H_

#include "pch.h"
#include "Duel.h"

// ״̬������, ������Ϸ״̬������̳�.����ͬʱ����һ��״̬��������. ʹ�õ���ʵ��?
namespace Duel
{
	class DAStateBase;
	class DAStateBase : public DObject
	{
	DUEL_DECLARE_RTTI(DAStateBase)
	public:
		DAStateBase(const DString& stateName) : mName(stateName) {}
		~DAStateBase() {}

		const DString&	getName() { return mName; }

		// ��ʼ���õĺ���. �^��!! ��Ҫ���@�������Y����Ո��B�D�Q!!
		virtual	void	initialize() {}
		// ת��������״̬֮ǰ�ĺ���. �^��!! ��Ҫ���@�������Y����Ո��B�D�Q!!
		virtual	void	release() {}

		// ����Ĳ���������Ҫ��ȡ��. �@�e�ą������ѽ������õ��ǂ���ʽ.
		virtual	void	parseInput() {}
		// �����������Ҫ��������Ҫ���ƵĶ���ȫ���ŵ�renderqueue���沢����������ÿ�����sort()����.
		virtual	void	update() {}
		// �����������ʹ��renderqueue��render�������л���.
		virtual	void	render() {}
		// �������ʹ������ϵĽ��չʾ����.
		virtual	void	present() {}

		// ״̬ת����ʱ����Ҫ�����õĺ���: ����true��ʾ����ת��, stateManager������һ֡�л�״̬.
		virtual	bool	stateChangingFrom(DAStateBase* preState) { return true; }

	protected:
		// ����Ψһ��ʾ���״̬������, ֮����ʹ��string������enum�ǳ�����չ�ԵĿ���.
		DString	mName;

	};

	class DAStateBase;
	class DAGameDirector;
	class DApplication;
	class DANullState;
	class DAStateManager
	{
	public:
		DAStateManager(DApplication* app);
		~DAStateManager();

		//////////////////////////////////////////////////////////////////////////
		// signals block
		DSignal<void(DAStateManager* sm)>
			signalTransactionStart;
		DSignal<void(DAStateManager* sm, DAStateBase* before, DAStateBase* after)>
			signalStateChanged;
		DSignal<void(DAStateManager* sm)>
			signalTransactionFinish;
		//////////////////////////////////////////////////////////////////////////

		// ��Ҫ����ֵ. ע��/ע��һ��״̬, ʹ���ܹ���������.
		void		registerState(DAStateBase* state);
		// ע��һ��state ���ǲ���ɾ����.ע��. ���Ҳ��܄h����ǰ��B������һ����B.
		void		unregisterState(DAStateBase* state);
		void		shutdown()
		{
			if (mCurState != NULL)
			{
				mCurState->release();
				mCurState = NULL;
				mNextState = NULL;
			}
			
		}

		// ��һ������ʼǰִ�еĺ���, һ�����eventListener�Ϳ�����.
		void		transactionStart()
		{
			// ����һ��.
			if (mCurState != NULL) // ̎���״μ����B��������r.
			{
				mNextState = mCurState;
			}
			else
			{
				if (mNextState != NULL)
				{
					mNextState->initialize();
				}
				mCurState = mNextState;
			}
			signalTransactionStart(this);
		}

		// ��Ҫת��״̬��ʱ��ʹ���������. �����ֱ��Ƿ��������״̬��ָ�����Ҫת���״̬����.
		void		requestStateChange(DAStateBase* requestingState, const Duel::DString& targetState)
		{
			// ȡ����Ӧ��state ���������, ת��ʧ��.
			StateMap::iterator i = mStateMap.find(targetState);
			if (i != mStateMap.end())
			{
				if(i->second->stateChangingFrom(requestingState))
				{
					signalStateChanged(this, requestingState, i->second);
					mNextState = i->second;
				}
			}
		}

		void		parseInput()
		{ 
			if(mCurState != NULL) 
			{
				mCurState->parseInput();
			}
		}
		void		update()
		{
			if(mCurState != NULL) 
			{
				mCurState->update(); 
			}
		}
		void		render()
		{
			if(mCurState != NULL) 
			{
				mCurState->render(); 
			}
		}

		void		present()
		{
			if(mCurState != NULL) 
			{
				mCurState->present(); 
			}
		}

		// 
		DApplication*	getParentApplication() { return mParentApp; }

		DAStateBase*	getCurrentState() { return mCurState; }
		DAStateBase*	getNextState() { return mNextState; }

		// ��һ��������ɺ���õĺ���, ��������״̬�л��Լ���������ĺ���.
		void		transactionFinish()
		{
			signalTransactionFinish(this);
			if (mNextState != mCurState)
			{
				// ��ʼ��һЩ��Դ. ���ͷŵ�ǰ״̬����Դ.
				if (mCurState != NULL)
				{
					mCurState->release();
				}
				if (mNextState != NULL)
				{
					mNextState->initialize();
				}
			}
			mCurState = mNextState;
		}

	protected:

		// ���������Ѿ�ע���״̬.
		typedef	std::map<DString, DAStateBase*>	StateMap;
		StateMap			mStateMap;
		// ��ǰ״̬.
		DAStateBase*		mCurState;
		// ��һ״̬.
		DAStateBase*		mNextState;

		DApplication*		mParentApp;
		static DANullState	msNullState;
	};

	// ����ʾ��: ʲ?�����ֵ�һ����B.
	class DANullState : public DAStateBase
	{
	DUEL_DECLARE_RTTI(DANullState)
	public:
		DANullState() : DAStateBase("Null") {}
		~DANullState() {}

	private:

	};
}

#endif