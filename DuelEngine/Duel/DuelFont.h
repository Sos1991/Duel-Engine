//  [8/20/2013 OMEGA] created

#ifndef _DUELFONT_H_
#define _DUELFONT_H_

#include "DuelCommon.h"
#include "DuelResource.h"
#include "DuelResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


namespace Duel
{
	/*
		����ʹ�õļ������:
		����ʹ�õ������ļ�(��������ֻʹ��ttf��ʽ��)���������κ������漸������:
		���δ�С(glyphSize), �ֱ���(glyphResolution),width/height, hBearingX, hBeartingY,
		vBeartingX(����), vBeartingY(����), hAdvance, vAdvance(����), 
		�������:
		http://www.unixresources.net/linux/clf/kylix/archive/00/00/59/21/592188.html
		(��ʱ�����ų����������, ȥ�����ŵ�֧��). ��Щ���������������Ű�, ��������ֻ��ȡ����Щ���ֵ���Ϣ,
		�Լ�����������ͼ�е�uv����(�Լ������������),�Ű湤�������ϲ�Ӧ�ó���. Ϊ��ʵ������������
		�����ֿ��֧��, ���ǲ�����һ���԰������ַ�д��������, ֻ��һ�δ�������, ������δ�С�ͷֱ���
		һ��load�Ժ�Ͳ��ɸı�, ������Ҳ����ÿ��ȡ�����ֵ�ʱ������д����Ӧ��������.
		�û�����Ҫ֪��д������ʱ������ʲô, ��������Ϊ���պ󷽱�, �һ�����΢��¼һ��.
		����ʹ��һ��Ĵ�����, ���ϵ��µļ�¼���ֻ���ķ�ʽ, ÿ������ռһ��С��, �����������ο��С��һ,
		�������ǲ�֪���û��´�ȡ�õ����ĸ���,�������û��Ԥ�����ֵ�ռ�ÿռ�, ʹ��ͳһ�ռ��������˷�,
		����Ҳ��õķ�ʽ����������иߵ�����, ��¼ÿһ����ߵ����εĸ�, �����������û�и߳�����и�,
		�������������, ������θ߳�, ������и߼�¼Ϊ��ǰ���εĸ�, ת����һ��ʱ��������и����¶�λ,
		ֱ��������ǰ����߶�(һ����������߶�����д��������, ��������ʣ��λ��, ���α�֤�¼����������Զ����
		���һ��������).
	*/
	// font resource, 
	class DUEL_API DFont : public DResource
	{
	DUEL_DECLARE_RTTI(DFont)
	public:
		DFont(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		typedef	uint32	CodePoint;
		typedef	FRect	UVRect;
		/// Information about the position and size of a glyph in a texture
		struct GlyphInfo 
		{
			CodePoint	codePoint;
			UVRect		uvRect;
			uint32		textureIndex;	
			// the index of this glyph located in the font file.
			uint32		hAdvance;
			uint32		hBearingX;
			uint32		hBearingY;
			uint32		width;
			uint32		height;
			GlyphInfo() : codePoint(0), textureIndex(0), hAdvance(0), hBearingY(0), hBearingX(0), width(0), height(0),
				uvRect(0.0f, 0.0f, 0.0f, 0.0f) {}
		};
		/// A range of code points, inclusive on both ends
		/// Map from unicode code point to texture coordinates
		typedef std::map<CodePoint, GlyphInfo>	CodePointMap;
		typedef	std::vector<DResourcePtr>		GlyphTextures;
		typedef	MapIterator<CodePointMap>		CodePointIterator;
		typedef	VectorIterator<GlyphTextures>	GlyphTextureIterator;
		CodePointIterator	getCodePointIterator() { return CodePointIterator(mCodePointMap); }
		GlyphTextureIterator	getGlyphTextureIterator() { return GlyphTextureIterator(mGlyphTextures); }
       
		/** Sets the size of a truetype font (only required for FT_TRUETYPE). 
        @param ttfSize The size of the font in points. Note that the
            size of the font does not affect how big it is on the screen, just how large it is in
            the texture and thus how detailed it is.
			resolution of single glyph, no more than 128px, and it must be 2^n.
			it will not be modified if the font is already loaded.
        */
        void			setGlyphSize(uint32 size);

        /** Gets the point size of the font used to generate the texture.
        @remarks
            Only applicable for FT_TRUETYPE Font objects.
            Note that the size of the font does not affect how big it is on the screen, 
            just how large it is in the texture and thus how detailed it is.            
        */
        uint32			getGlyphSize(void) const;

		// set the size of the texture holding the glyph, ranges from 256 to 1024, and must be 2^n
		// this function will force the size to fit the requirement.
		// it will not be modified if the font is already loaded.
		void			setTextureSize(uint32 size);
		uint32			getTextureSize();

		/** Gets the maximum baseline distance of all glyphs used in the texture.
		@remarks
			Only applicable for FT_TRUETYPE Font objects.
			The baseline is the vertical origin of horizontal based glyphs.  The bearingY
			attribute is the distance from the baseline (origin) to the top of the glyph's 
			bounding box.
		@note
			This value is only available after the font has been loaded.
		*/
		int32			getTrueTypeMaxBearingY() const;

		/** Sets whether or not the colour of this font is antialiased as it is generated
            from a true type font.
        @remarks
        	This is valid only for a FT_TRUETYPE font. If you are planning on using 
            alpha blending to draw your font, then it is a good idea to set this to
            false (which is the default), otherwise the darkening of the font will combine
            with the fading out of the alpha around the edges and make your font look thinner
            than it should. However, if you intend to blend your font using a colour blending
            mode (add or modulate for example) then it's a good idea to set this to true, in
            order to soften your font edges.
        */
		void			setAntialiasColour(bool enabled);

		/** Gets whether or not the colour of this font is antialiased as it is generated
		from a true type font.
		*/
        bool			getAntialiasColour(void) const;

		// use this method to get glyph for rendering.
		GlyphInfo		getGlyphInfo(uint32 codePoint);
		
		// get the texture storing the glyphs.
		DResourcePtr		getGlyphTexture(uint32 idx);

	protected:
		// override : DResource-------------------------------
		virtual void		loadImpl() ;
		virtual void		unloadImpl();
		virtual size_t		calculateSize();
		//////////////////////////////////////////////////////////////////////////
		// internal method, return a new pace for the new glyph, returned value is texture index.
		uint32				getNextGlyphSpace(uint32 w, uint32 h, uint32& outX, uint32& outY);
		void				appendNewTexture();

		CodePointMap	mCodePointMap;

		/// Size of the truetype font, in points
		uint32			mGlyphSize;
		// width(height) of the texture.
		uint32			mTexureSize;
		/// Max distance to baseline of this (truetype) font
		int32			mTtfMaxBearingY;

		/// for TRUE_TYPE font only
		bool			mAntialiasColour;

		GlyphTextures	mGlyphTextures;
		
		// recording the position the new glyph should be placed.
		uint32			mCurRowMaxHeight;
		uint32			mCurLocX;
		uint32			mCurLocY;
		// internal object used for reading glyphs.
		FT_Library		mFtLib;
		FT_Face			mFtFace;
	};

}

#endif