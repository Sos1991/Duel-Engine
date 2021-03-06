//  [4/2/2013 OMEGA] ֹͣʹ�� DRECodec_v1_0.
//  [12/31/2012 OMEGA] created


#ifndef _DUELDRECODEC_H_
#define _DUELDRECODEC_H_

#include "DuelCommon.h"
#include "DuelCodec.h"

namespace Duel
{

	// .dre = duel render effect
	// the file format is like:
	// values are case-sensitive
	//	<RenderEffect version="2.0">
	//		<Technique name="lalala">
	//			<Pass name="lalala">
	//				<RasterizerState>
	//					<PolygonMode>
	//						Points/Wireframe/Solid
	//					</PolygonMode>
	//					<ShadeMode>
	//						FLAT/GOURAUD/PHONG
	//					</ShadeMode>
	//					........
	//				</RasterizerState>
	//				<DepthStencilState>
	//					<DepthEnable>
	//						false/true
	//					</DepthEnable>
	//					........ if not specified, use default value.
	//					
	//				</DepthStencilState>
	//////////////////////////////////////////////////////////////////////////
	// v2.0: replace SceneBlendState with BlendState,
	/*
					<BlendState>
						<AlphaToCoverageEnable>
							false/true
						</AlphaToCoverageEnable>
						<RenderTargetBlendState	index = "0">
							<ColorBlendOpeartion>ADD</ColorBlendOpeartion>
							<ColorSrcBlend>ONE</ColorSrcBlend>
							........
						</RenderTargetBlendState>
						<RenderTargetBlendState	index = "1">
							<ColorBlendOpeartion>ADD</ColorBlendOpeartion>
							<ColorSrcBlend>ONE</ColorSrcBlend>
							........
						</RenderTargetBlendState>
						.....
					</BlendState>
	*/
	//////////////////////////////////////////////////////////////////////////
	//				<VertexProgram group="xxxx">
	//					lalalala (name, used for getting resource from GpuProgramManager)
	//				</VertexProgram>
	//				<PixelProgram group="xxxx">
	//					lalalala (name, used for getting resource from GpuProgramManager)
	//				</PixelProgram>
	//			</Pass>
	//			<Pass name="xxxxx">
	//				.......
	//			</Pass>
	//		</Technique>
	//		..... or more Techniques
	//	</RenderEffect>
	//	

	class DUEL_API DRECodec_v2_0 : public DResourceCodec
	{
	DUEL_DECLARE_RTTI(DRECodec_v2_0)
	public:
		// override: DRenderEffectCodec------------------
		void			decode(DDataStream* input, DResource* targetEffect);
		// override: DRenderEffectCodec------------------
		DDataStreamPtr	encode(DResource* targetEffect);
		// override: DRenderEffectCodec------------------
		void			encodeToFile(const DString& fileName, DResource* targetEffect);

		CodecType	getCodecType() { return CT_RenderEffect; }
	};

}


#endif