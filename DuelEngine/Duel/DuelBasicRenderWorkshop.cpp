//  [6/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelBasicRenderWorkshop.h"
#include "DuelRenderWorkshop.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI(DABasicRenderWorkshop, DRenderWorkshop);


	void DABasicRenderWorkshop::renderDeferStage()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void DABasicRenderWorkshop::renderForwardStage()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void DABasicRenderWorkshop::setOptions( const RenderOption& option )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DABasicRenderWorkshop::DABasicRenderWorkshop()
	{
		// ����4��ds�õ�texture.
	}

	void DABasicRenderWorkshop::setRenderTarget( DRenderTarget* target )
	{
		// ����ֻ�ǰ�������ȾĿ���ݴ�����. ������Ⱦ��ʱ������Ƿ���Ⱦds����ȷ��.
	}



}