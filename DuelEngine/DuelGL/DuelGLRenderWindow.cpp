//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include <GL/glew.h>
#include "DuelFrameBuffer.h"
#include "DuelGLRenderResourceFactory.h"
#include "DuelGLRenderWindow.h"
#include "DuelGLRenderSystem.h"
#include "DuelGLRenderView.h"
#include "DuelGLTranslator.h"
#include "DuelException.h"

// 实在是不能忍了. GLRenderWidow 第635行老给我报这个.
// 符号/无符号数不匹配
#pragma warning(disable : 4018)

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLRenderWindow, DRenderWindow);

	//////////////////////////////////////////////////////////////////////////
	GLRenderWindow::GLRenderWindow( DRenderResourceFactory* parent,/* GLRenderSystem* rSys, */const DString& name ) : 
		DRenderWindow(parent, name),
		//mTargetRSys(rSys),
		mName(name),
		mFBO(0),
		mHGLRC(NULL),
		mMainSurface(NULL),
		mMainDepthView(NULL),
		mCurDepthView(NULL)
	{
		for (int32 i = 0; i < 8; i++)
		{
			mViewList.push_back(NULL);
		}
		glGenFramebuffers(1, &mFBO);
		mCurViewport.reset(0,0, 100, 100);
	}

	GLRenderWindow::~GLRenderWindow()
	{
		detachAllRenderColorViews();
		detachRenderDepthStencilView();
		if (mFBO != 0)
		{
			glDeleteRenderbuffers(1, &mFBO);
			mFBO = 0;
		}
		if (mMainSurface != NULL)
		{
			DRenderResourceManager::getSingleton().destroyRenderColorView(mMainSurface);
		}
		if (mMainDepthView != NULL)
		{
			DRenderResourceManager::getSingleton().destroyRenderDepthStencilView(mMainDepthView);
		}
		wglMakeCurrent(mHDC, NULL);
		wglDeleteContext(mHGLRC);
		mCreator->getAs<GLRenderResourceFactory>()->resetResourceContext();
	}

	void GLRenderWindow::create( const DString& name, const RenderWindowSetting& setting, uint32 winHandle )
	{
		DFrameBuffer::resize(setting.width, setting.height);
		mCurViewport.reset(0,0, setting.width, setting.height);
		mColorBits = DPixelFormatTool::getFormatBits(setting.surfaceFormat);
#ifdef DUEL_PLATFORM_WINDOWS
		// 
		mHWND = (HWND)winHandle;
		mHDC = GetDC(mHWND);

		uint32 sufaceFormatBit = DPixelFormatTool::getFormatBits(setting.surfaceFormat);
		uint32 depthFormatBit = 24; // D24S8
		uint32 stencilFormatBit = 8;
		// there is no guarantee that the contents of the stack that become
		// the pfd are zeroed, therefore _make sure_ to clear these bits.
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize		= sizeof(pfd);
		pfd.nVersion	= 1;
		pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType	= PFD_TYPE_RGBA;
		pfd.cColorBits	= static_cast<BYTE>(sufaceFormatBit);
		pfd.cDepthBits	= static_cast<BYTE>(depthFormatBit);
		pfd.cStencilBits = static_cast<BYTE>(stencilFormatBit);
		pfd.iLayerType	= PFD_MAIN_PLANE;
		bool try_srgb = false;
		if (setting.useGamma && ((PF_A8B8G8R8 == setting.surfaceFormat) || (PF_A8R8G8B8 == setting.surfaceFormat)))
		{
			try_srgb = true;
		}

		// here is the trick, create a temp HWND to create glew evironment.

 		int pixfmt = ChoosePixelFormat(mHDC, &pfd);
 		assert(pixfmt != 0);
// 

		uint32 smpCount = setting.samplerCount;

		if ((smpCount > 1) || try_srgb)
		{
			UINT num_formats;
			float float_attrs[] = { 0, 0 };
			BOOL valid;
			do
			{
				int int_attrs[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_COLOR_BITS_ARB, sufaceFormatBit,
					WGL_DEPTH_BITS_ARB, depthFormatBit,
					WGL_STENCIL_BITS_ARB, stencilFormatBit,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_SAMPLES_ARB, smpCount,
					try_srgb ? WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB : 0, try_srgb,
					0, 0
				};

				valid = wglChoosePixelFormatARB(mHDC, int_attrs, float_attrs, 1, &pixfmt, &num_formats);
				if (!valid || (num_formats < 1))
				{
					--smpCount;
				}
			} while ((smpCount > 1) && (!valid || (num_formats < 1)));

			if (valid && ((smpCount > 1) || try_srgb))
			{
				BOOL result = SetPixelFormat(mHDC, pixfmt, &pfd);	//每个窗口只能设置一次 
				assert(result == TRUE);
			}
		}
		else
		{
			BOOL result = SetPixelFormat(mHDC, pixfmt, &pfd);//每个窗口只能设置一次  
			assert(result == TRUE);
		}

		mHGLRC = wglCreateContext(mHDC);
		wglMakeCurrent(mHDC,mHGLRC); 

		

		int flags = 0;//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		#ifdef DUEL_DEBUG
			flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
		#endif
		int versions[7][2] =
		{
			{ 4, 2 },
			{ 4, 1 },
			{ 4, 0 },
			{ 3, 3 },
			{ 3, 2 },
			{ 3, 1 },
			{ 3, 0 },
		};
		int attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 4, WGL_CONTEXT_MINOR_VERSION_ARB, 2, WGL_CONTEXT_FLAGS_ARB, flags,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0 };
		for (int i = 0; i < 7; ++ i)
		{
			attribs[1] = versions[i][0];
			attribs[3] = versions[i][1];
			HGLRC hRC_new = wglCreateContextAttribsARB(mHDC,
				mCreator->getAs<GLRenderResourceFactory>()->getResourceGLRC(), attribs);
			if (hRC_new != NULL)
			{
				wglMakeCurrent(mHDC, NULL);
				wglDeleteContext(mHGLRC);
				mHGLRC = hRC_new;
				wglMakeCurrent(mHDC, mHGLRC);
				break;
			}
		}
		 	



		glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
		glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
		glClampColor(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);


		wglSwapIntervalEXT(setting.verticalSync);

#endif // DUEL_PLATFORM_WINDOWS

		mCreator->getAs<GLRenderResourceFactory>()->resetResourceContext();

		if (try_srgb)
		{
			glEnable(GL_FRAMEBUFFER_SRGB);

		}

		if (mFBO)
		{
			glDeleteFramebuffers(1, &mFBO);
		}
		glGenFramebuffers(1, &mFBO);


		resize(mWidth, mHeight);
		if (mMainSurface != NULL)
		{
			DRenderResourceManager::getSingleton().destroyRenderColorView(mMainSurface);
		}
		mMainSurface = DRenderResourceManager::getSingleton().
			createRenderColorView(setting.surfaceFormat)->getAs<GLRenderColorView>();
		// there isn't a render color view, use our main surface view instead.
		if (mViewList[0] == NULL)
		{
			attachRenderColorView(EA_Color0, mMainSurface);
		}
		if(mMainDepthView != NULL)
		{
			DRenderResourceManager::getSingleton().destroyRenderDepthStencilView(mMainDepthView);
		}
		mMainDepthView = DRenderResourceManager::getSingleton().
			createRenderDepthStencilView()->getAs<GLRenderDepthStencilView>();
		// there isn't a render depth view, use our main depth view view instead.
		if (getRenderDepthStencilView() == NULL)
		{
			attachRenderDepthStencilView(mMainDepthView);
		}
	}

	void GLRenderWindow::setFullScreen( bool flag, const DisplayMode& fullScreenMode )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool GLRenderWindow::isFullScreen() const
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void GLRenderWindow::reposition( uint32 top, uint32 left )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::uint32 GLRenderWindow::getTop()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::uint32 GLRenderWindow::getLeft()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void GLRenderWindow::attachRenderColorView( ElementAttachment elem, DRenderColorView* v )
	{
		if (v == NULL)
		{
			assert(false);// "Can't attach an empty render color view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);// "Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		// the case specified v is attached to another channel
		if (getRenderColorView(elem) != NULL)
		{
			assert(false);//"Specified attachement is already in use, you must detach it firstly"
			return;
		}
		// if this view has been attached to this frame buffer, change its channel.
		if (v->getAttachedFrameBuffer() == this)
		{
			detachRenderColorView(v->getAttachPoint());
		}

		// chech format compatibility
		if (DPixelFormatTool::getFormatBits(v->getFormat()) != mColorBits)
		{
			return;
		}
		GLRenderColorView* rv = v->getAs<GLRenderColorView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		rv->resize(mWidth, mHeight);
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);			
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, rv->getTextureID(), 0);
		mViewList[(uint32)(elem)] = rv;
		rv->setAttachFrameBuffer(this);
		rv->setAttachElement(elem);
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	void GLRenderWindow::detachRenderColorView( ElementAttachment elem )
	{
		if (mViewList[(uint32)(elem)] != NULL)
		{
			GLuint	oldFbo = cacheFBO();
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
			mViewList[(uint32)(elem)]->setAttachFrameBuffer(NULL);
			mViewList[(uint32)(elem)]->setAttachElement(EA_Color0);
			mViewList[(uint32)(elem)] = NULL;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, 0, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
		}

	}

	void GLRenderWindow::detachAllRenderColorViews()
	{
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		for (uint32 i = 0; i < 8; ++i)
		{
			mViewList[i] = NULL;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	void GLRenderWindow::attachRenderDepthStencilView( DRenderDepthStencilView* v )
	{		
		if (v == NULL)
		{
			assert(false);//"Can't attach an empty depth stencil view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);//"Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		GLRenderDepthStencilView* rv = v->getAs<GLRenderDepthStencilView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		detachRenderDepthStencilView();
		rv->resize(mWidth, mHeight);
		mCurDepthView = rv;
		mCurDepthView->setAttachFrameBuffer(this);
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rv->getRBO());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rv->getRBO());
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	void GLRenderWindow::detachRenderDepthStencilView()
	{
		if (mCurDepthView == NULL)
		{
			return;
		}
		mCurDepthView->setAttachFrameBuffer(NULL);
		mCurDepthView = NULL;
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);

	}

	DRenderColorView* GLRenderWindow::getRenderColorView( ElementAttachment elem )
	{
		assert(elem < mViewList.size());
		return mViewList[elem];
	}

	void GLRenderWindow::resize( uint32 width, uint32 height )
	{
		// from wiki:
		/*
			EVER EVER MAKE A STENCIL buffer. All GPUs and all drivers do not support an independent stencil buffer. 
			If you need a stencil buffer, then you need to make a Depth=24, Stencil=8 buffer, also called D24S8. 
			Please search for the example about GL_EXT_packed_depth_stencil on this page.
		*/
		if (width == mWidth && height == mHeight)
		{
			return;
		}
		assert(width != 0 && height != 0);
		mWidth = width, mHeight = height;
		DFrameBuffer::resize(width, height);
		RenderColorViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->resize(width, height);
			}
		}
		if(mCurDepthView != NULL)
		{
			mCurDepthView->resize(width, height);
		}
		// cache the old fbo. so that we can make no effect on the pipe line.
		GLuint	oldFbo = cacheFBO();

		// set the rendering destination to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		// bind valid and enabled render view.
		uint32 j; 
		for (i = mViewList.begin(), j = 0; i != iend; ++i, ++j)
		{
			if ((*i) != NULL)
			{	
				// attach a texture to FBO color attachement point
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, (*i)->getTextureID(), 0);
			}
			else
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, 0, 0);
			}
		}
		if (mCurDepthView != NULL)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mCurDepthView->getRBO());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mCurDepthView->getRBO());
		}
		else
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);

		}
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);

		// emit signals.
		// TODO: do window resize?
		signalResized(this);

	}

	void GLRenderWindow::clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil )
	{		
		GLuint	oldFbo = cacheFBO();

		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		GLbitfield ogl_flags = 0;
		if (flags & CBM_Color)
		{
			ogl_flags |= GL_COLOR_BUFFER_BIT;
			glClearColor(clr.r, clr.g, clr.b, clr.a);
		}
		if (flags & CBM_Depth)
		{
			ogl_flags |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(depth);
		}
		if (flags & CBM_Stencil)
		{
			ogl_flags |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(stencil);
		}
		glClear(ogl_flags);

		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	void GLRenderWindow::update()
	{
		if (mViewList[0] == NULL)
		{
			return;
		}
		GLuint oldFBO = cacheFBO();
		wglMakeCurrent(mHDC, mHGLRC);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//脱离绑定

		glClearColor(0,0,0,0);
		glClearDepth(1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST); 

		// gl的视口是左下角为(0,0)
		glViewport(0, 0, mWidth, mHeight);
		glMatrixMode(GL_PROJECTION);//转入正投影
		glPushMatrix();
		glLoadIdentity();
		int32 halfW = mWidth/2;
		int32 halfH = mHeight/2;
		glOrtho( -halfW , halfW, -halfH, halfH, -1, 1 );//屏幕大小
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D); // 只画第一块咯.
		glBindTexture(GL_TEXTURE_2D, mViewList[0]->getTextureID());

		glBegin(GL_QUADS);
		/*
		//前面
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,-1.0f,1.0f);//纹理和四边形的左下
		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);//右下
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);//右上
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,1.0f,1.0f);//左上


		//后面
		glTexCoord2f(1.0f,0.0f);glVertex3f(-1.0f,-1.0f,-1.0f);//左下
		glTexCoord2f(1.0f,1.0f);glVertex3f(-1.0f,1.0f,-1.0f);//左上
		glTexCoord2f(0.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);//右上
		glTexCoord2f(0.0f,0.0f);glVertex3f(1.0f,-1.0f,-1.0f);//右下



		//顶面
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,1.0f,-1.0f);//左上
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,1.0f,1.0f);//左下

		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,1.0f,1.0f);//右下
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);//右上


		//底面
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,-1.0f,-1.0f);//右上
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,-1.0f,-1.0f);//左上
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,-1.0f,1.0f);//左下
		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);//右下


		//右面
		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,-1.0f,-1.0f);//左下
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);//左上
		glTexCoord2f(0.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);//右上
		glTexCoord2f(0.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);//右下


		//左面
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,1.0f,1.0f);//右上
		glTexCoord2f(1.0f,0.0f);glVertex3f(-1.0f,1.0f,-1.0f);//左上
		glTexCoord2f(1.0f,1.0f);glVertex3f(-1.0f,-1.0f,-1.0f);//左下
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,-1.0f,1.0f);//右下*/
		glTexCoord2f(0.0f, 0.0f);   glVertex3i(-halfW,  -halfH, 0);
		glTexCoord2f(1.0f, 0.0f);   glVertex3i( halfW,  -halfH, 0);
		glTexCoord2f(1.0f, 1.0f);   glVertex3i( halfW, halfH, 0);
		glTexCoord2f(0.0f, 1.0f);   glVertex3i(-halfW, halfH, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);


		glMatrixMode( GL_PROJECTION );//回来
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );   
		glPopMatrix();    

		glEnable(GL_DEPTH_TEST);
		glPopAttrib();

		glFlush();
		SwapBuffers(mHDC);

#ifdef DUEL_PLATFORM_WINDOWS
		mCreator->getAs<GLRenderResourceFactory>()->resetResourceContext();
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
#endif // DUEL_PLATFORM_WINDOWS

	}

	Duel::uint32 GLRenderWindow::getWindowHandle()
	{
		return (uint32)(mHWND);
	}

	GLuint GLRenderWindow::cacheFBO()
	{
		GLint	oldFbo = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);/*= mTargetRSys->getCurGLFBO();*/
		return (GLuint)oldFbo;
	}

	DRenderDepthStencilView* GLRenderWindow::getRenderDepthStencilView()
	{
		return mCurDepthView;
	}


}

#pragma warning(default : 4018)