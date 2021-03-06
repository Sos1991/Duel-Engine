//  [11/11/2013 OMEGA] created

#ifndef _DUELGUISTDBUTTON_H_
#define _DUELGUISTDBUTTON_H_

#include "DuelGUICommon.h"
#include "DuelGUICursor.h"

namespace Duel
{
	class DUELGUI_API DGStdCursorPictureLayer : public DRenderable
	{
		DUEL_DECLARE_RTTI(DGStdCursorPictureLayer)
	public:
		DGStdCursorPictureLayer(DGCursor* hostCursor);

		// override : DRenderable-----------------
		virtual DRenderTechnique*	getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
		virtual void	updateCustomGpuParameter(DShaderObject* so);

	protected:
		DGCursor*				mHostCursor;
		// renderable's required data.
		DVertexStreams			mVStream;
		DIndexBufferPtr			mIndices;
		DVertexDeclaration		mVDecl;
		DResourcePtr			mRenderEffect;
		// trans form data used in rendering.
		DMatrix4				mTransform;
		DResourcePtr			mTexture;
	};
	// 特化的东西 - - .
	class DUELGUI_API DGStdCursorRD : public DGRenderDelegate
	{
		DUEL_DECLARE_RTTI(DGStdCursorRD)
	public:
		DGStdCursorRD(DGCursor* hostCursor);
		~DGStdCursorRD();
		DGCursor*		getHostCursor() { return mHostCursor; }
		// override : DGRenderDelegate------------------
		virtual void	applyToRenderQueue( DRenderQueue* queue, uint32 groupID );

		virtual DGWidget* getParent();

	protected:
		DGCursor*		mHostCursor;
		DGStdCursorPictureLayer* mPicLayer;
	};

	class DUELGUI_API DGStdCursor : public DGCursor
	{
		DUEL_DECLARE_RTTI(DGStdCursor)
	public:
		DGStdCursor();
		~DGStdCursor();

	protected:


	private:
		// private render delegate.
		DGRenderDelegate*	mStdRenderDelegate;
	};
}


#endif