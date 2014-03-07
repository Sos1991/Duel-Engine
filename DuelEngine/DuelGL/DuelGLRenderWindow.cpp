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

// ʵ���ǲ�������. GLRenderWidow ��635���ϸ��ұ����.
// ����/�޷�������ƥ��
#pragma warning(disable : 4018)

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLRenderWindow, DRenderWindow);

// #ifdef DUEL_PLATFORM_WINDOWS
// 	HWND	GLRenderWindow::msEnviHWND = NULL;
// 	HGLRC	GLRenderWindow::msEnviHGLRC = NULL;
// 	HDC		GLRenderWindow::msEnviHDC = NULL;

// 	void GLRenderWindow::initStaticEvironment()
// 	{
// 		if (msEnviHWND == NULL)
// 		{
// 			HINSTANCE hInst = GetModuleHandle(NULL);
// 			WNDCLASSEX wc;
// 
// 			wc.cbSize			= sizeof(wc);
// 			wc.style			= CS_HREDRAW | CS_VREDRAW;
// 			wc.lpfnWndProc		= _TMPGL_DefaultWndProc;
// 			wc.cbClsExtra		= 0;
// 			wc.cbWndExtra		= 0;
// 			wc.hInstance		= hInst;
// 			wc.hIcon			= NULL;
// 			wc.hCursor			= NULL;
// 			wc.hbrBackground	= NULL;
// 			wc.lpszMenuName		= NULL;
// 			// TODO: from config
// 			wc.lpszClassName	= "__TmpGLWindow";
// 			wc.hIconSm			= NULL;
// 
// 			DWORD	styleWord;
// 			styleWord = WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
// 			RECT rc = { 0, 0, 50, 50 };
// 			AdjustWindowRectEx( &rc, styleWord, FALSE, 0 );
// 			RegisterClassEx(&wc);
// 			msEnviHWND = CreateWindowEx(0, "__TmpGLWindow", "__TmpGLWindow", 
// 				styleWord, 0, 0,  rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);
// 
// 			msEnviHDC = GetDC(msEnviHWND);
// 			uint32 sufaceFormatBit = 32;
// 			uint32 depthFormatBit = 24; // D24S8
// 			uint32 stencilFormatBit = 8;
// 			// there is no guarantee that the contents of the stack that become
// 			// the pfd are zeroed, therefore _make sure_ to clear these bits.
// 			PIXELFORMATDESCRIPTOR pfd;
// 			memset(&pfd, 0, sizeof(pfd));
// 			pfd.nSize		= sizeof(pfd);
// 			pfd.nVersion	= 1;
// 			pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
// 			pfd.iPixelType	= PFD_TYPE_RGBA;
// 			pfd.cColorBits	= static_cast<BYTE>(sufaceFormatBit);
// 			pfd.cDepthBits	= static_cast<BYTE>(depthFormatBit);
// 			pfd.cStencilBits = static_cast<BYTE>(stencilFormatBit);
// 			pfd.iLayerType	= PFD_MAIN_PLANE;
// 
// 			// here is the trick, create a temp HWND to create glew evironment.
// 
// 			int pixfmt = ChoosePixelFormat(msEnviHDC, &pfd);
// 			assert(pixfmt != 0);
// 
// 			SetPixelFormat(msEnviHDC, pixfmt, &pfd);//ÿ������ֻ������һ��  
// 
// 			msEnviHGLRC = wglCreateContext(msEnviHDC);
// 			wglMakeCurrent(msEnviHDC,msEnviHGLRC); 
// 			glewInit();
// 		}
// 	}
// 
// #endif
	//////////////////////////////////////////////////////////////////////////
	GLRenderWindow::GLRenderWindow( DRenderResourceFactory* parent,/* GLRenderSystem* rSys, */const DString& name ) : 
		DRenderWindow(parent, name),
		//mTargetRSys(rSys),
		mName(name),
		mFBO(0),
		mHGLRC(NULL),
		mDepthStencilBuffer(0)
	{
		for (int32 i = 0; i < 8; i++)
		{
			mViewList.push_back(NULL);
		}
	}

	GLRenderWindow::~GLRenderWindow()
	{
		RenderViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				delete (*i);
			}
		}

		if (mFBO != 0)
		{
			glDeleteRenderbuffers(1, &mFBO);
			mFBO = 0;
		}
		if (mDepthStencilBuffer != 0)
		{
			glDeleteRenderbuffers(1, &mDepthStencilBuffer);
		}
		wglMakeCurrent(mHDC, NULL);
		wglDeleteContext(mHGLRC);
		mParent->getAs<GLRenderResourceFactory>()->resetResourceContext();
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
				BOOL result = SetPixelFormat(mHDC, pixfmt, &pfd);	//ÿ������ֻ������һ�� 
				assert(result == TRUE);
			}
		}
		else
		{
			BOOL result = SetPixelFormat(mHDC, pixfmt, &pfd);//ÿ������ֻ������һ��  
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
				mParent->getAs<GLRenderResourceFactory>()->getResourceGLRC(), attribs);
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

		mParent->getAs<GLRenderResourceFactory>()->resetResourceContext();

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
		enableElement(EA_Color0, setting.surfaceFormat);


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

	void GLRenderWindow::enableElement( ElementAttachment elem, DPixelFormat format )
	{
		if (DPixelFormatTool::getFormatBits(format) != mColorBits)
		{
			return;
		}
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		uint32 index = (uint32)elem;
		GLRenderView* rv = NULL;
		if (mViewList[(uint32)(elem)] == NULL)
		{
			rv = new GLRenderView(this, elem, format);
			mViewList[index] = rv;
			rv->resize(mWidth, mHeight);
			rv->setEnable(true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, rv->getTextureID(), 0);
		}
		else
		{
			if (mViewList[(uint32)elem]->getFormat() != format)
			{
				delete mViewList[(uint32)elem];
				GLRenderView* rv = new GLRenderView(this, elem, format);
				rv->resize(mWidth, mHeight);
				mViewList[(uint32)(elem)] = rv;
			}
			mViewList[(uint32)(elem)]->setEnable(true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, rv->getTextureID(), 0);
		}

		// check if everything is OK
// 		GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
// 		switch (e) {
// 
// 		case GL_FRAMEBUFFER_UNDEFINED:
// 			printf("FBO Undefined\n");
// 			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
// 			printf("FBO Incomplete Attachment\n");
// 			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
// 			printf("FBO Missing Attachment\n");
// 			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
// 			printf("FBO Incomplete Draw Buffer\n");
// 			break;
// 		case GL_FRAMEBUFFER_UNSUPPORTED :
// 			printf("FBO Unsupported\n");
// 			break;
// 		case GL_FRAMEBUFFER_COMPLETE:
// 			printf("FBO OK\n");
// 			break;
// 		default:
// 			printf("FBO Problem?\n");
// 		}
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	void GLRenderWindow::disableElement( ElementAttachment elem )
	{
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		if (mViewList[(uint32)(elem)] != NULL)
		{
			mViewList[(uint32)(elem)]->setEnable(false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, 0, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);

	}

	DRenderView* GLRenderWindow::getRenderView( ElementAttachment elem )
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
		DFrameBuffer::resize(width, height);
		RenderViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->resize(width, height);
			}
		}
		// cache the old fbo. so that we can make no effect on the pipe line.
		GLuint	oldFbo = cacheFBO();


		// set the rendering destination to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		// for depth buffer and stencil buffer.
		if (mDepthStencilBuffer != 0)
		{
			glDeleteRenderbuffers(1, &mDepthStencilBuffer);
		}
		glGenRenderbuffers(1, &mDepthStencilBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		// bind valid and enabled render view.
		uint32 j; 
		for (i = mViewList.begin(), j = 0; i != iend; ++i, ++j)
		{
			if ((*i) != NULL)
			{
				if ((*i)->isEnabled())
				{			
					// attach a texture to FBO color attachement point
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, (*i)->getTextureID(), 0);
				}
			}
		}


		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer);

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer);

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
		GLuint oldFBO = cacheFBO();
		wglMakeCurrent(mHDC, mHGLRC);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//�����

		glClearColor(0,0,0,0);
		glClearDepth(1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST); 

		// gl���ӿ������½�Ϊ(0,0)
		glViewport(0, 0, mWidth, mHeight);
		glMatrixMode(GL_PROJECTION);//ת����ͶӰ
		glPushMatrix();
		glLoadIdentity();
		int32 halfW = mWidth/2;
		int32 halfH = mHeight/2;
		glOrtho( -halfW , halfW, -halfH, halfH, -1, 1 );//��Ļ��С
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D); // ֻ����һ�鿩.
		glBindTexture(GL_TEXTURE_2D, mViewList[0]->getTextureID());

		glBegin(GL_QUADS);
		/*
		//ǰ��
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,-1.0f,1.0f);//������ı��ε�����
		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);//����
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);//����
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,1.0f,1.0f);//����


		//����
		glTexCoord2f(1.0f,0.0f);glVertex3f(-1.0f,-1.0f,-1.0f);//����
		glTexCoord2f(1.0f,1.0f);glVertex3f(-1.0f,1.0f,-1.0f);//����
		glTexCoord2f(0.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);//����
		glTexCoord2f(0.0f,0.0f);glVertex3f(1.0f,-1.0f,-1.0f);//����



		//����
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,1.0f,-1.0f);//����
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,1.0f,1.0f);//����

		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,1.0f,1.0f);//����
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);//����


		//����
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,-1.0f,-1.0f);//����
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,-1.0f,-1.0f);//����
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,-1.0f,1.0f);//����
		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);//����


		//����
		glTexCoord2f(1.0f,0.0f);glVertex3f(1.0f,-1.0f,-1.0f);//����
		glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);//����
		glTexCoord2f(0.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);//����
		glTexCoord2f(0.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);//����


		//����
		glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,1.0f,1.0f);//����
		glTexCoord2f(1.0f,0.0f);glVertex3f(-1.0f,1.0f,-1.0f);//����
		glTexCoord2f(1.0f,1.0f);glVertex3f(-1.0f,-1.0f,-1.0f);//����
		glTexCoord2f(0.0f,1.0f);glVertex3f(-1.0f,-1.0f,1.0f);//����*/
		glTexCoord2f(0.0f, 0.0f);   glVertex3i(-halfW,  -halfH, 0);
		glTexCoord2f(1.0f, 0.0f);   glVertex3i( halfW,  -halfH, 0);
		glTexCoord2f(1.0f, 1.0f);   glVertex3i( halfW, halfH, 0);
		glTexCoord2f(0.0f, 1.0f);   glVertex3i(-halfW, halfH, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);


		glMatrixMode( GL_PROJECTION );//����
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );   
		glPopMatrix();    

		glEnable(GL_DEPTH_TEST);
		glPopAttrib();

		glFlush();
		SwapBuffers(mHDC);

#ifdef DUEL_PLATFORM_WINDOWS
		mParent->getAs<GLRenderResourceFactory>()->resetResourceContext();
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


}

#pragma warning(default : 4018)