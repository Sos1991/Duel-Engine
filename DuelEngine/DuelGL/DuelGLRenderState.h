//  [7/27/2013 OMEGA] created

#ifndef _DUELGLRENDERSTATE_H_
#define _DUELGLRENDERSTATE_H_

#include "Duel.h"
#include "DuelGLCommon.h"
#include "DuelRenderState.h"
#include "DuelGLRenderSystem.h"

namespace Duel
{
	class DUELGL_API GLRasterizerStateObject : public DRasterizerStateObject
	{
	DUEL_DECLARE_RTTI(GLRasterizerStateObject)
	public:
		GLRasterizerStateObject(const DRasterizerState& state);
		// override : DRasterizerStateObject--------------------
		virtual void getRasterizerState( DRasterizerState& outState );

	protected:
		GLenum	mGLPolygonMode;
		GLenum	mGLShadeMode;
		GLenum	mGLFrontFace;
		bool	mbCulling;
		// just a copy.
		DRasterizerState	mState;
		// for quick lookup.
		friend	class GLRenderSystem;
	};

	class DUELGL_API GLDepthStencilStateObject : public DDepthStencilStateObject
	{
	DUEL_DECLARE_RTTI(GLDepthStencilStateObject)
	public:
		GLDepthStencilStateObject(const DDepthStencilState& state);
		// override : DDepthStencilStateObject -------------------------------
		virtual void getDepthStencilState( DDepthStencilState& outState );

	protected:
		GLboolean	mGLDepthWriteMask;
		GLenum		mGLDepthFunc;
		GLenum		mGLFrontStencilFunc;
		GLenum		mGLFrontStencilFail;
		GLenum		mGLFrontStencilDepthFail;
		GLenum		mGLFrontStencilPass;
		GLenum		mGLBackStencilFunc;
		GLenum		mGLBackStencilFail;
		GLenum		mGLBackStencilDepthFail;
		GLenum		mGLBackStencilPass;
		DDepthStencilState	mState;
		// for quick lookup.
		friend	class GLRenderSystem;
	};

	class DUELGL_API GLBlendStateObject : public DBlendStateObject
	{
	DUEL_DECLARE_RTTI(GLBlendStateObject)
	public:
		GLBlendStateObject(const DBlendState& state);
		// override : DBlendStateObject -------------------------------
		virtual void getBlendState( DBlendState& outState );

	protected:
		GLenum	mGLColorBlendOp[DUEL_MAX_BLEND_LAYERS];
		GLenum	mGLAlphaBlendOp[DUEL_MAX_BLEND_LAYERS];
		GLenum	mGLColorSrcBlend[DUEL_MAX_BLEND_LAYERS];
		GLenum	mGLColorDstBlend[DUEL_MAX_BLEND_LAYERS];
		GLenum	mGLAlphaSrcBlend[DUEL_MAX_BLEND_LAYERS];
		GLenum	mGLAlphaDstBlend[DUEL_MAX_BLEND_LAYERS];
		DBlendState		mState;
		// for quick lookup.
		friend	class GLRenderSystem;
	};

// 	class DUELGL_API GLTextureSamplerStateObject : public DTextureSamplerStateObject
// 	{
// 	DUEL_DECLARE_RTTI(GLTextureSamplerStateObject);
// 	public:
// 		GLTextureSamplerStateObject(const DTextureSamplerState& state);
// 		// override : DTextureSamplerStateObject -------------------------------
// 		virtual void getTextureSamplerState( DTextureSamplerState& outState );
// 	protected:
// 		GLenum	mGLAddressModeU;
// 		GLenum	mGLAddressModeV;
// 		GLenum	mGLAddressModeW;
// 		GLenum	mGLMinFilter;
// 		GLenum	mGLMagFilter;
// 		DTextureSamplerState	mState;
// 		// for quick lookup.
// 		friend	class GLRenderSystem;
// 	};
}

#endif