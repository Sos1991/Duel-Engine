//  [3/19/2013 OMEGA] created

#pragma once
#include "pch.h"
#include "DuelStateMachine.h"
#include "NullState.h"
#include "DemoState.h"
#include "DuelCore.h"
#include "DuelWPDXPlugin.h"
#include "DuelQuadtreePlugin.h"
#include "ActorFactory.h"


namespace Duel
{

	class DAGameDirector : public Duel::DSingleton<DAGameDirector>
	{
	public:
		// �@�e����D3DX�YԴ�K���ṩ�o��Ⱦϵ�yʹ��.
		DAGameDirector();
		~DAGameDirector();

	public:
		//////////////////////////////////////////////////////////////////////////
		// �@�e�����cXAML��������Ҫ�Ľӿ�. �����ԃ�[���B��.֮�֮�.





		// �������[���������P��. ����Ҫ֪��.
		//////////////////////////////////////////////////////////////////////////
		// ��ģ��������.
		virtual	void		Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
		virtual void		HandleDeviceLost();
		virtual void		CreateDeviceResources();
		virtual void		UpdateForWindowSizeChange();
		virtual	void		SetDpi(float dpi);	// dpi��ë��?		
		virtual float		ConvertDipsToPixels(float dips);
		virtual void		CreateWindowSizeDependentResources();
		void				ValidateDevice();

		// - - �{��render����.
		void				parseInput(){ mStateManager->parseInput(); }
		void				update();
		void				render();

		// ��Ⱦ�ꮅ. չʾ����.
		void				present();

		// get/set ȡ���[��Ġ�B����Ϣ, �@�e�ṩ�ӿں����ͨ��.
		Duel::DCore*			getEngineCore() { return mEngineCore; }
		DAStateManager*		getStateManager() { return mStateManager; }
		Duel::DRenderWindow*	getGameWindow() { return (Duel::DRenderWindow*)mRenderWindow; }

	protected:
		// ��ֻ�ǳ���..
		Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
		Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ m_panel;

		// d3d �YԴ, �@�eֻ��һ����������, �����ڌ��H�\���^����ʹ��, ���wʹ�õĕr�C���oWPDX��Ⱦϵ�y.
		Microsoft::WRL::ComPtr<ID3D11Device1>			mD3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1>	mD3DContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>			mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_d3dRenderTargetView;

		D3D_FEATURE_LEVEL		mD3DFeatureLevel;

		// ������ʾ�����ת����
		D2D1::Matrix3x2F m_orientationTransform2D;
		DirectX::XMFLOAT4X4 m_orientationTransform3D;

		// �������.
		Duel::DCore*				mEngineCore;	
		Duel::QuadtreePlugin*	mQuadtreePlugin;
		Duel::WPDXPlugin*		mWPDXPlugin;
		Duel::WPDXRenderWindow*	mRenderWindow;

		// ����ĳ��������ԡ�
		// �@��ֻ�ǳ���.
		Windows::Foundation::Size m_d3dRenderTargetSize;
		Windows::Graphics::Display::DisplayOrientations m_orientation;
		Windows::Foundation::Rect m_windowBounds;
		float m_dpi;

		// ��B����.
		DAStateManager*			mStateManager;
		// �@�e����ʾ��, �d��һ���ՠ�B.
		DANullState*				mNULLState;
		DemoState*				mDemoState;
		// ������Ľ�ɫ. �Uչ֮������.
		ActorFactory*			mActorFactory;

	};

}