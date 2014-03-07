//  [3/19/2013 OMEGA] created

#include "pch.h"
#include "DuelGameDirector.h"
#include "NullState.h"
#include "DuelCore.h"

namespace Duel
{

	// �^����Ҫnew��2��GameDirector, �^�����e.
	DAGameDirector::DAGameDirector() : 
		m_dpi(-0.1f),
		mRenderWindow(NULL),
		mNULLState(NULL),
		mDemoState(NULL)
	{
		mEngineCore = new Duel::DCore("Assets\\CoreConfig.xml");
		mQuadtreePlugin = new Duel::QuadtreePlugin();
		mQuadtreePlugin->install();
		mQuadtreePlugin->initialize();
		
		mWPDXPlugin = new Duel::WPDXPlugin();
		mWPDXPlugin->install();
		mWPDXPlugin->initialize();


	}

	DAGameDirector::~DAGameDirector()
	{

		mEngineCore->getMovableManager()->unregisterMovableFactory(mActorFactory);
		delete	mActorFactory;

		if (mStateManager)
		{
			delete	mStateManager;
			mStateManager = NULL;
		}
		if (mNULLState)
		{
			delete	mNULLState;
			mNULLState = NULL;
		}
		if (mDemoState)
		{
			delete	mDemoState;
			mDemoState = NULL;
		}

		mQuadtreePlugin->shutdown();
		mQuadtreePlugin->uninstall();
		delete mQuadtreePlugin;
		mQuadtreePlugin = NULL;
		mWPDXPlugin->shutdown();
		mWPDXPlugin->uninstall();
		delete mWPDXPlugin;
		mWPDXPlugin = NULL;
	}

	void DAGameDirector::Initialize( Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi )
	{


		m_window = window;
		m_panel = panel;

		CreateDeviceResources();
		SetDpi(dpi);

		mActorFactory = new ActorFactory();
		mEngineCore->getMovableManager()->registerMovableFactory(mActorFactory);

		//////////////////////////////////////////////////////////////////////////
		// �YԴ�ļ��d���@��. �@�eֻ����һ���YԴ��. ��ʽ���YԴ�d��r�C���o����.
		Duel::FileStreamWP8* fs = new Duel::FileStreamWP8("Assets\\MeshResources.xml");
		Duel::ResourceParametersPtr resP = mEngineCore->getResourceGroupManager()->getResouceManager("Mesh")->parseParameter(Duel::DDataStreamPtr(fs));
		Duel::ResourceParameters::iterator i, iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}

		Duel::FileStreamWP8* gs = new Duel::FileStreamWP8("Assets\\GpuProgramResources.xml");
		resP = mEngineCore->getResourceGroupManager()->getResouceManager("GpuProgram")->parseParameter(Duel::DDataStreamPtr(gs));
		iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}

		Duel::FileStreamWP8* rs = new Duel::FileStreamWP8("Assets\\RenderEffectResources.xml");
		resP = mEngineCore->getResourceGroupManager()->getResouceManager("RenderEffect")->parseParameter(Duel::DDataStreamPtr(rs));
		iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}
		Duel::FileStreamWP8* ts = new Duel::FileStreamWP8("Assets\\TextureResources.xml");
		resP = mEngineCore->getResourceGroupManager()->getResouceManager("Texture")->parseParameter(Duel::DDataStreamPtr(ts));
		iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}
		mEngineCore->getResourceGroupManager()->createDeclaredResource("Test");
		mEngineCore->getResourceGroupManager()->createDeclaredResource("General");


		mStateManager = new DAStateManager(this);
		mNULLState = new DANullState();
		mDemoState = new DemoState();
		mStateManager->registerState(mNULLState);
		mStateManager->registerState(mDemoState);
		mStateManager->requestStateChange(mNULLState, "Null");
		mStateManager->requestStateChange(mDemoState, "DemoState");


		//////////////////////////////////////////////////////////////////////////
		// test////// �@��ͨ�^��.... �]���}-v-
		//////////////////////////////////////////////////////////////////////////

		Duel::DResourcePtr res = mEngineCore->getResourceGroupManager()->getResouceManager("Mesh")->getResource("box1box2.dm");
		res->load();
		//res = mCore->getResourceGroupManager()->getResouceManager("GpuProgram")->getResource("SimpleVertexShader.cso");
		//res->load();
		res = mEngineCore->getResourceGroupManager()->getResouceManager("RenderEffect")->getResource("testRE.dre");
		res->load();

		res = mEngineCore->getResourceGroupManager()->getResouceManager("Texture")->getResource("tree.dds");
		static_cast<DTexture*>(res.get())->load();

	}

	void DAGameDirector::HandleDeviceLost()
	{
		// ������Щ��Ա��������ȷ�� SetDpi ���´���������Դ��
		float dpi = m_dpi;
		m_dpi = -1.0f;
		m_windowBounds.Width = 0;
		m_windowBounds.Height = 0;
		mSwapChain = nullptr;

		CreateDeviceResources();
		SetDpi(dpi);
	}

	void DAGameDirector::CreateDeviceResources()
	{
		// �˱�־Ϊ�� API Ĭ�����þ��в�ͬ��ɫ����˳���ͼ��
		// ���֧�֡�Ҫ�� Direct2D ���ݣ����������Ҫ��
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
		// �����Ŀ���ڵ������ɹ����У���ͨ�����д˱�־�� SDK �����õ��ԡ�
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// �����鶨���Ӧ�ó���֧�ֵ� DirectX Ӳ�����ܼ����顣
		// ��ע�⣬Ӧ����˳��
		// �벻Ҫ������Ӧ�ó����˵���������������
		// ��͹��ܼ��𡣳�������˵��������ٶ�����Ӧ�ó����֧�� 9.1��
		D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// ���� Direct3D 11 API �豸����Ͷ�Ӧ�������ġ�
		ComPtr<ID3D11Device> d3dDevice;
		ComPtr<ID3D11DeviceContext> d3dContext;
		DX::ThrowIfFailed(
			D3D11CreateDevice(
			nullptr, // ָ�� nullptr ��ʹ��Ĭ����������
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags, // ���õ��Ժ� Direct2D �����Ա�־��
			featureLevels, // ��Ӧ�ó������֧�ֵĹ��ܼ�����б�
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, // ���� Windows Ӧ���̵�Ӧ�ó���ʼ�ս���ֵ����Ϊ D3D11_SDK_VERSION��
			&d3dDevice, // ���ش����� Direct3D �豸��
			&mD3DFeatureLevel, // �����������豸�Ĺ��ܼ���
			&d3dContext // �����豸�ļ�ʱ�����ġ�
			)
			);

		// ��ȡ Direct3D 11.1 API �豸�������Ľӿڡ�
		DX::ThrowIfFailed(
			d3dDevice.As(&mD3DDevice)
			);

		DX::ThrowIfFailed(
			d3dContext.As(&mD3DContext)
			);

		// ��������^���[�򴰿�.
		static_cast<WPDXRenderSystem*>(mEngineCore->getRenderSystem())->setDevice(mD3DDevice.Get());
		mRenderWindow = static_cast<WPDXRenderWindow*>(mEngineCore->getRenderSystem()->createRenderWindow("GameWindow"));
		
	}

	void DAGameDirector::UpdateForWindowSizeChange()
	{
		// ���û�й���� DPI ���ģ���������ڴ�С���ġ�
		if (m_dpi != DisplayProperties::LogicalDpi)
		{
			return;
		}

		if (m_window->Bounds.Width  != m_windowBounds.Width ||
			m_window->Bounds.Height != m_windowBounds.Height ||
			m_orientation != DisplayProperties::CurrentOrientation)
		{
			ID3D11RenderTargetView* nullViews[] = {nullptr};
			mD3DContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			m_d3dRenderTargetView = nullptr;
			mD3DContext->Flush();
			CreateWindowSizeDependentResources();
		}
	}

	void DAGameDirector::SetDpi( float dpi )
	{
		if (dpi != m_dpi)
		{
			// ������µ� DPI ֵ��
			m_dpi = dpi;


			// ͨ�� DPI ������ζ�Ŵ��ڴ�С���ġ�ĳЩ����£�Windows �ᷢ��
			// ��С�����¼��� DPI �����¼�������������£����ɵİ�
			// ��������ģ����Ҵ��ڵ�����С�����ִ��һ�Ρ�
			UpdateForWindowSizeChange();
		}
	}

	float DAGameDirector::ConvertDipsToPixels( float dips )
	{	
		static const float dipsPerInch = 96.0f;
		return floor(dips * m_dpi / dipsPerInch + 0.5f); // ���뵽��ӽ���������
	}

	void DAGameDirector::CreateWindowSizeDependentResources()
	{
		// �洢���ڰ󶨣��Ա��´����ǻ�ȡ SizeChanged �¼�ʱ���ɱ���
		// �ڴ�С��ͬ����������������������ݡ�
		m_windowBounds = m_window->Bounds;

		// �����Ҫ�Ľ�����������Ŀ���С(������Ϊ��λ)��
		float windowWidth = ConvertDipsToPixels(m_windowBounds.Width);
		float windowHeight = ConvertDipsToPixels(m_windowBounds.Height);

		// �������Ŀ�Ⱥ͸߶ȱ�����ڴ��ڵ�
		// �����Ⱥ͸߶ȡ��������������
		// ���������ʹ�ߴ練ת��
		m_orientation = DisplayProperties::CurrentOrientation;
		bool swapDimensions =
			m_orientation == DisplayOrientations::Portrait ||
			m_orientation == DisplayOrientations::PortraitFlipped;
		m_d3dRenderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
		m_d3dRenderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;



		if (mSwapChain != nullptr)
		{
			// ����������Ѵ��ڣ���������С��
			// ����. �[��͘�.
// 			HRESULT hr = mSwapChain->ResizeBuffers(
// 				2, // ˫���彻������
// 				static_cast<UINT>(m_d3dRenderTargetSize.Width),
// 				static_cast<UINT>(m_d3dRenderTargetSize.Height),
// 				DXGI_FORMAT_B8G8R8A8_UNORM,
// 				0
// 				);
// 
// 			if (hr == DXGI_ERROR_DEVICE_REMOVED)
// 			{
// 				HandleDeviceLost();
// 				return;
// 			}
// 			else
// 			{
// 				DX::ThrowIfFailed(hr);
// 			}
		}
		else
		{
			// ����ʹ�������� Direct3D �豸��ͬ���������½�һ����
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
			swapChainDesc.Width = static_cast<UINT>(m_d3dRenderTargetSize.Width); // ƥ�䴰�ڵĴ�С��
			swapChainDesc.Height = static_cast<UINT>(m_d3dRenderTargetSize.Height);
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ������õĽ�������ʽ��
			swapChainDesc.Stereo = false; 
			swapChainDesc.SampleDesc.Count = 1; // �벻Ҫʹ�ö������
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2; // ʹ��˫�������̶ȵؼ�С�ӳ١�
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // ���� Windows Ӧ���̵�Ӧ�ó��򶼱���ʹ�ô� SwapEffect��
			swapChainDesc.Flags = 0;

			ComPtr<IDXGIDevice1> dxgiDevice;
			DX::ThrowIfFailed(
				mD3DDevice.As(&dxgiDevice)
				);

			ComPtr<IDXGIAdapter> dxgiAdapter;
			DX::ThrowIfFailed(
				dxgiDevice->GetAdapter(&dxgiAdapter)
				);

			ComPtr<IDXGIFactory2> dxgiFactory;
			DX::ThrowIfFailed(
				dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
				);

			DX::ThrowIfFailed(
				dxgiFactory->CreateSwapChainForComposition(
				mD3DDevice.Get(),
				&swapChainDesc,
				nullptr,
				&mSwapChain
				)
				);

			// ���½������� SwapChainBackgroundPanel Ԫ�ع�����
			ComPtr<ISwapChainBackgroundPanelNative> panelNative;
			DX::ThrowIfFailed(
				reinterpret_cast<IUnknown*>(m_panel)->QueryInterface(IID_PPV_ARGS(&panelNative))
				);

			DX::ThrowIfFailed(
				panelNative->SetSwapChain(mSwapChain.Get())
				);

			// ȷ�� DXGI ����һ�ζԶ��֡�Ŷӡ������ȿ��Լ�С�ӳ٣�
			// �ֿ���ȷ��Ӧ�ó���ֻ��ÿ�� VSync ֮����֣��Ӷ����̶ȵؽ��͹������ġ�
			DX::ThrowIfFailed(
				dxgiDevice->SetMaximumFrameLatency(1)
				);
		}
		// Ϊ������������ȷ���򣬲����� 2D �� 3D ����
		// ת���Գ��ֵ���ת��������
		// ��ע�⣬���� 2D �� 3D ת������ת�ǶȲ�ͬ��
		// ����������ռ�Ĳ�������ġ����⣬
		// ��ʽָ�� 3D ������Ա���������
		DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
		switch (m_orientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			m_orientationTransform2D = D2D1::Matrix3x2F::Identity();
			m_orientationTransform3D = XMFLOAT4X4( // 0 �� Z ��ת
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			m_orientationTransform2D = 
				D2D1::Matrix3x2F::Rotation(270.0f) *
				D2D1::Matrix3x2F::Translation(0.0f, m_windowBounds.Width);
			m_orientationTransform3D = XMFLOAT4X4( // 90 �� Z ��ת
				0.0f, 1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			m_orientationTransform2D = 
				D2D1::Matrix3x2F::Rotation(180.0f) *
				D2D1::Matrix3x2F::Translation(m_windowBounds.Width, m_windowBounds.Height);
			m_orientationTransform3D = XMFLOAT4X4( // 180 �� Z ��ת
				-1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			m_orientationTransform2D = 
				D2D1::Matrix3x2F::Rotation(90.0f) *
				D2D1::Matrix3x2F::Translation(m_windowBounds.Height, 0.0f);
			m_orientationTransform3D = XMFLOAT4X4( // 270 �� Z ��ת
				0.0f, -1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		default:
			throw ref new Platform::FailureException();
		}

		DX::ThrowIfFailed(
			mSwapChain->SetRotation(rotation)
			);

		ComPtr<ID3D11Texture2D> backBuffer;
		DX::ThrowIfFailed(
			mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
			);

		DX::ThrowIfFailed(
			mD3DDevice->CreateRenderTargetView(
			backBuffer.Get(),
			nullptr,
			&m_d3dRenderTargetView
			)
			);

		// �� ������.
		mRenderWindow->updateRenderTargetView(m_d3dRenderTargetView.Get());

	}

	void DAGameDirector::ValidateDevice()
	{
		ComPtr<IDXGIDevice1> dxgiDevice;
		ComPtr<IDXGIAdapter> deviceAdapter;
		DXGI_ADAPTER_DESC deviceDesc;
		DX::ThrowIfFailed(mD3DDevice.As(&dxgiDevice));
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));
		DX::ThrowIfFailed(deviceAdapter->GetDesc(&deviceDesc));


		ComPtr<IDXGIFactory2> dxgiFactory;
		ComPtr<IDXGIAdapter1> currentAdapter;
		DXGI_ADAPTER_DESC currentDesc;

		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
		DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, &currentAdapter));
		DX::ThrowIfFailed(currentAdapter->GetDesc(&currentDesc));

		if (deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
			deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
			FAILED(mD3DDevice->GetDeviceRemovedReason()))
		{
			dxgiDevice = nullptr;
			deviceAdapter = nullptr;
			HandleDeviceLost();
		}
	}

	void DAGameDirector::update()
	{
		// hehe
		mStateManager->transactionStart();
		mStateManager->parseInput();
		mStateManager->update();

	}
	void DAGameDirector::render()
	{
		mStateManager->render();
	}


	void DAGameDirector::present()
	{
		// Ӧ�ó������ѡ��ָ�����Ѹ��¡��򡰹�����
		// ���Σ��ԸĽ��ض�������Ч�ʡ�
		DXGI_PRESENT_PARAMETERS parameters = {0};
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;

		// ��һ������ָʾ DXGI ������ֱֹ�� VSync����ʹӦ�ó���
		// ����һ�� VSync ǰ�������ߡ��⽫ȷ�����ǲ����˷��κ����ڳ��ֽ�
		// �Ӳ�������Ļ����ʾ��֡��
		HRESULT hr = mSwapChain->Present1(1, 0, &parameters);

		// ��������Ŀ������ݡ�
		// ���ǽ�����ȫ������������ʱ
		// ��Ч�Ĳ��������ʹ���Ѹ��»�������Σ���Ӧ��������ɾ����
		mD3DContext->DiscardView(m_d3dRenderTargetView.Get());

		// �������ģ�ߵ����ݡ�
		mD3DContext->DiscardView(mRenderWindow->getDepthStencilView());

		// ���ͨ���Ͽ����ӻ�����������������ķ���ɾ�����豸�������
		// �������´��������豸��Դ��
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			HandleDeviceLost();
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
		// �Y����݆ѭ�h.
		mStateManager->transactionFinish();
	}

}