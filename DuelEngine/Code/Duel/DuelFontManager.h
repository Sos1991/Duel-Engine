//  [8/20/2013 OMEGA] created

#ifndef _DUELFONTMANAGER_H_
#define _DUELFONTMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"

namespace Duel
{

	class DUEL_API DFontDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DFontDescription)
	public:
		DFontDescription(const DString& name, const DString& groupName);

		// resolution of single glyph, no more than 128px, and it must be 2^n. 
		uint32		glyphSize;
		// the size of single block of texture, its width is equal to the height,
		// ranges from 256px to 1024px, must be 2^n.
		uint32		textureSize;
	};

	class DUEL_API DFontManager : public DResourceManager
	{
	DUEL_DECLARE_RTTI(DFontManager)
	public:
		DFontManager();
		

		/* ����ʹ��xml�ļ���¼�����ļ���Ϣ,
		 �������Ŀ���д:
				<FontResourceParameter>
		 			<Name>xxxxxx</Name>
					<Group>xx</Group>
		 			<GlyphSize>32</GlyphSize>
		 			<TextureSize>1024</TextureSize>
				</FontResourceParameter>
				<FontResourceParameter>
					.......
					.......
				</FontResourceParameter>
		 ����֪����ϸ�������������.
		 */
		ResourceDescriptionList parseParameter(DDataStream* data);

	protected:
		DResource*	createImpl(DResourceDescription* createParam);
	};
}

#endif