//  [8/28/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelRenderLayout.h"
#include "DuelGLRenderLayout.h"
#include "DuelGLVertexBuffer.h"
#include "DuelGLTranslator.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLRenderLayout, DRenderLayout);

	GLRenderLayout::GLRenderLayout(DRenderResourceFactory* creator) :
		DRenderLayout(creator),
		mVAO(0)
	{

	}

	GLRenderLayout::~GLRenderLayout()
	{
		if (mVAO)
		{
			glDeleteVertexArrays(1, &mVAO);
		}
	}



	void GLRenderLayout::seal()
	{
		if (mVAO)
		{
			glDeleteVertexArrays(1, &mVAO);
		}
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);

		//////////////////////////////////////////////////////////////////////////
		// bind input layout.
		DVertexDeclaration vDecl = getVertexData().getDeclaration();
		DVertexDeclaration::VertexElementIterator vi = vDecl.getVertexElementIterator();
		// give an invalid source to begin with.
		//  [8/4/2013 OMEGA] ticket:
		// VAO ������layout��ʵ�ָ��ߵ�Ч��. ��������ʱ�����Ż�.
		//glBindVertexArray(mVAO);
		int32 curSrc = -1;
		size_t	vertexStride = 0;
		// http://stackoverflow.com/questions/14249634/opengl-vaos-and-multiple-buffers
		// ps:
		/*
			This is still a very new extension, 
			and only NVIDIA really provides support for it at this point.
			And even that's still early. So you can't rely on it.
		*/
		std::vector<int32>	attrStack;
		while (vi.hasMoreElements())
		{
			char*	offsetP = 0;
			DVertexElement vd = vi.getNext();
			if (vd.getSource() != curSrc)
			{
				curSrc = vd.getSource();
				vertexStride = vDecl.getVertexSize(curSrc);
				DVertexStreams vs = getVertexData().getBufferStreams();
				DVertexBufferPtr vb = vs.getStream(curSrc);
				if (vb == NULL)
				{
					// collapsed streams. stop processing.
					break;
				}
				GLVertexBuffer*	glVB = vb->getAs<GLVertexBuffer>();
				glBindBuffer(GL_ARRAY_BUFFER, glVB->getGLVertexBufferID());
			}
			VertexElementSemantic sm = vd.getSemantic();
			// �������²�һ��. �ҵ��Կ�֧�����29��attribute, ��!��!, position ��������0��λ����. �������Ժ�����.. 
			int32 smIndex = (int32)sm;
			if (sm == VES_TexCoord)
			{
				smIndex += vd.getIndex();
			}
			GLuint	size = 0;
			GLenum	type = 0;
			GLboolean	bNormalize = GL_FALSE;
			// store the index we've enabled. 
			attrStack.push_back(smIndex);
			GLTranslator::getGLVertexFormat(size, type, bNormalize, vd.getElementType());
			char*	offset = 0;
			// bind stream. - -ah..
			glVertexAttribPointer(
				smIndex, size, type, bNormalize, vertexStride, (offset + vd.getOffset()));
			glEnableVertexAttribArray(smIndex);
		}
		//  [8/27/2013 OMEGA] ati��֧������??? 
		// ����VAO�����̾������, �Ժ��ٶ�ÿһ��layout�����Ż�. ��layout����һ��sealed����, �����sealed�����ɶ�Ӧ
		// VAO, ��������ֱ�����VAO�İ󶨽��л���, ����Ҫ�ٽ������ϴ�ʩ,
		// �����е�ĳ��������vertexbuffer/vertexdeclaration/indexbuffer�ı�ʱ����sealed���Ըı�, ����������
		// ��Ӧ��VAO
		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, NULL);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		//////////////////////////////////////////////////////////////////////////
		//glBindVertexArray(mVAO);  

	}

}