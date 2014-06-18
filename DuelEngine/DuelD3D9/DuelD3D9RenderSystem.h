//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9RENDERSYSTEM_H_
#define _DUELD3D9RENDERSYSTEM_H_

#include "DuelD3D9Common.h"
#include "DuelRenderSystem.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9RenderSystem : public DRenderSystem
	{
		DUEL_DECLARE_RTTI(D3D9RenderSystem)
	public:
		D3D9RenderSystem(D3D9RenderResourceFactory* fact);
		virtual const DString& getName() const;

		virtual void initialize();

		virtual void shutdown();

		virtual void bindFrameBuffer( DFrameBuffer* buf );

		virtual DFrameBuffer* getCurrentFrameBuffer();

		virtual void setRasterizerState( DRasterizerStateObject* rs );

		virtual void setDepthStencilState( DDepthStencilStateObject* dss, uint32 frontStencilRef = 0, uint32 backStencilRef = 0 );

		virtual void setBlendState( DBlendStateObject* sbs, const DColor& blendFactor );

		virtual DRasterizerStateObjectPtr getCurrentRaseterizerState();

		virtual DDepthStencilStateObjectPtr getCurrentDepthStencilStateObject();

		virtual DBlendStateObjectPtr getCurrentBlendState();

		virtual void bindShaderObject( DShaderObject* so );

		virtual void render( DRenderLayout* layout );

		virtual void fillDeviceCaps();

	protected:
		void			initRenderStates();
		// internal function used in binding parameters.
		void			setVertexShaderParameter(IDirect3DDevice9* dev, DGpuParameters* param);
		void			setPixelShaderParameter(IDirect3DDevice9* dev, DGpuParameters* param);
		void			setTextureSampler(IDirect3DDevice9* dev, uint32 logicalIdx, DGpuTextureConstantPtr tex);
		// a pointer to the device holder.
		D3D9RenderResourceFactory*	mFactory;
		// current frame buffer binded to the pipe line. on which all draw calls happen.
		DFrameBuffer*	mCurFrameBuffer;

		// use a pointer to indicate whether the current render states are available.
		DRasterizerState			mCurRasState;
		DDepthStencilState			mCurDepState;
		uint32						mCurFrontStencilRef;
		uint32						mCurBackStencilRef;
		DBlendState					mCurBlendState;
		DColor						mCurBlendFactor;

		IDirect3DDevice9*	mDevice;

		// used in bug tracing.
		DGpuProgram*		mVSProgram;
		DGpuProgram*		mPSProgram;
		// just a reference pointer.
		DGpuParameters*		mVSParams;
		DGpuParameters*		mPSParams;

		DString				mName;
	};

}

#endif