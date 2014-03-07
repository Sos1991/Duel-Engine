//  [4/2/2013 OMEGA] ֹͣʹ�� DRECodec_v1_0, ��Ĭ��codec���� DRECodec_v2_0
//  [12/2/2012 OMEGA] created

#ifndef _DUELRENDEREFFECTMANAGER_H_
#define _DUELRENDEREFFECTMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"

namespace Duel
{

	class DUEL_API DRenderEffectDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DRenderEffectDescription)
	public:
		// type name: RenderEffect
		DRenderEffectDescription(const DString& name, const DString& groupName);
		
		// format that used to identify the codec for loading. 
		// now only dre supported.
		DString	fileFormat;
		// dadada...
	};

#define DEFAULT_RENDEREFFECT_CODEC	DRECodec_v2_0

	// RenderEffect relies on GpuProgramManager.
	class DUEL_API DRenderEffectManager : public DResourceManager
	{
	DUEL_DECLARE_RTTI(DRenderEffectManager)
	public:
		DRenderEffectManager();
		virtual ~DRenderEffectManager();
		//RenderEffectManager()
		// TODO:
		// ����default������(setDefaultXXX...), ���½���effect��δ��ʽ�����Ĳ�������Щֵ���
		// TODO:�����ű�, �㶮��.
		// <RenderEffectResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat>
		// </RenderEffectResourceParameter>
		ResourceDescriptionList	parseParameter(DDataStream* data);


	protected:
		DResource*				createImpl(DResourceDescription* createParam);

		// default codec for RenderEffects.
		DResourceCodec*		mCommonCodec;

	};

}

#endif