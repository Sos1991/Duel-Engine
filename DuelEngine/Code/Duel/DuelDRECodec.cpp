//  [1/1/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelDRECodec.h"
#include "DuelGpuProgramManager.h"
#include "DuelRenderEffect.h"
#include "DuelRenderTechnique.h"
#include "DuelRenderPass.h"
#include "DuelMemoryStream.h"
#include "DuelFileStream.h"

namespace Duel
{
	//DUEL_IMPLEMENT_RTTI_1(DRECodec_v1_0, DRenderEffectCodec);
	DUEL_IMPLEMENT_RTTI_1(DRECodec_v2_0, DResourceCodec);

	// from DString value to native value------------------
	PolygonMode	getPolygonMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("POINTS" == value)
		{
			return PM_Points;
		}
		else if ("WIREFRAME" == value)
		{
			return PM_Wireframe;
		}
		else if ("SOLID" == value)
		{
			return PM_Solid;
		}
		return PM_Solid;
	}
	ShadeMode	getShadeMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("FLAT" == value)
		{
			return SM_Flat;
		}
		else if ("GOURAUD" == value)
		{
			return SM_Gouraud;
		}
		else if ("PHONG" == value)
		{
			return SM_Phong;
		}
		return SM_Gouraud;
	}
	CullMode	getCullMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("NONE" == value)
		{
			return CM_None;
		}
		else if ("FRONT" == value)
		{
			return CM_Front;
		}
		else if ("BACK" == value)
		{
			return CM_Back;
		}
		return CM_Back;
	}
	bool		getBool(const DString& val)
	{
		DString value(val);
		DStringTool::toLowerCase(value);
		if ("false" == value)
		{
			return false;
		}
		else if ("true" == value)
		{
			return true;
		}
		return false;
	}
	CompareFunction		getCompareFunction(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("ALWAYSFAIL" == value)
		{
			return CF_AlwaysFail;
		}
		else if ("ALWAYSPASS" == value)
		{
			return CF_AlwaysPass;
		}
		else if ("LESS" == value)
		{
			return CF_Less;
		}
		else if ("LESSEQUAL" == value)
		{
			return CF_LessEqual;
		}
		else if ("EQUAL" == value)
		{
			return CF_Equal;
		}
		else if ("NOTEQUAL" == value)
		{
			return CF_NotEqual;
		}
		else if ("GRATEREQUAL" == value)
		{
			return CF_GreaterEqual;
		}
		else if ("GRATER" == value)
		{
			return CF_Greater;
		}
		return CF_AlwaysPass;
	}
	StencilOperation	getStencilOperation(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("KEEP" == value)
		{
			return SO_Keep;
		}
		else if ("ZERO" == value)
		{
			return SO_Zero;
		}
		else if ("REPLACE" == value)
		{
			return SO_Replace;
		}
		else if ("INCREMENT" == value)
		{
			return SO_Increment;
		}
		else if ("DECREMENT" == value)
		{
			return SO_Decrement;
		}
		else if ("INCREMENTWRAP" == value)
		{
			return SO_DecrementWrap;
		}
		else if ("DECREMENTWRAP" == value)
		{
			return SO_DecrementWrap;
		}
		else if ("INVERT" == value)
		{
			return SO_Invert;
		}
		return SO_Keep;
	}
	BlendOperation	getSceneBlendOperation(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("ADD" == value)
		{
			return BO_Add;
		}
		else if ("SUB" == value)
		{
			return BO_Sub;
		}
		else if ("REVSUB" == value)
		{
			return BO_RevSub;
		}
		else if ("MIN" == value)
		{
			return BO_Min;
		}
		else if ("MAX" == value)
		{
			return BO_Max;
		}
		return BO_Add;
	}
	BlendFactor	getSceneBlendFactor(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("ZERO" == value)
		{
			return BF_Zero;
		}
		else if ("ONE" == value)
		{
			return BF_One;
		}
		else if ("SRCALPHA" == value)
		{
			return BF_SrcAlpha;
		}
		else if ("DSTALPHA" == value)
		{
			return BF_DstAlpha;
		}
		else if ("INVSRCALPHA" == value)
		{
			return BF_InvSrcAlpha;
		}
		else if ("INVDSTALPHA" == value)
		{
			return BF_InvDstAlpha;
		}
		else if ("SRCCOLOR" == value)
		{
			return BF_SrcColor;
		}
		else if ("DSTCOLOR" == value)
		{
			return BF_DstColor;
		}
		else if ("INVSRCCOLOR" == value)
		{
			return BF_InvSrcColor;
		}
		else if ("INVDSTCOLOR" == value)
		{
			return BF_InvDstColor;
		}
		else if ("SRCALPHASAT" == value)
		{
			return BF_SrcAlphaSat;
		}
		return BF_Zero;
	}
	TextureAddressMode	getTextureAddressMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("WRAP" == value)
		{
			return TAM_Wrap;
		}
		else if ("MIRROR" == value)
		{
			return TAM_Mirror;
		}
		else if ("CLAMP" == value)
		{
			return TAM_Clamp;
		}
		else if ("BORDER" == value)
		{
			return TAM_Border;
		}
		return TAM_Clamp;
	}
	FilterOption	getFilterOption(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("NONE" == value)
		{
			return FO_None;
		}
		else if ("POINT" == value)
		{
			return FO_Point;
		}
		else if ("LINEAR" == value)
		{
			return FO_Linear;
		}
		else if ("ANISOTROPIC" == value)
		{
			return FO_Anistropic;
		}
		return FO_Anistropic;
	}


	uint8	readColorMask(const DString& value)
	{

		DString val(value);
		DStringTool::toUpperCase(val);
		if (val == "RED")
		{
			return CWM_Red;
		}
		else if(val == "GREEN")
		{
			return CWM_Green;
		}
		else if(val == "BLUE")
		{
			return CWM_Blue;
		}
		else if(val == "ALPHA")
		{
			return CWM_Alpha;
		}
		else if(val == "ALL")
		{
			return CWM_All;
		}
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	// v2.0
	void	parseSamplerState(const DString& name, const DString& val, DTextureSampler& sampState)
	{
		if ("AddressU" == name)
		{
			sampState.addressU = getTextureAddressMode(val);
		}
		else if ( "AddressV" == name )
		{
			sampState.addressV = getTextureAddressMode(val);
		}
		else if ( "AddressW" == name )
		{
			sampState.addressW = getTextureAddressMode(val);
		}
		else if ( "MinFilter" == name )
		{
			sampState.minFilter = getFilterOption(val);
		}
		else if ( "MagFilter" == name )
		{
			sampState.magFilter = getFilterOption(val);
		}
		else if ( "MipFilter" == name )
		{
			sampState.mipFilter = getFilterOption(val);
		}
		else if ( "SamplerComparison" == name )
		{
			sampState.samplerComparison = getCompareFunction(val);
		}
		else if ( "BorderColor" == name )
		{
			DVector4 c;
			if (val.find_first_of(',') != DString::npos)
			{
				sampState.borderColor = DColor(DStringTool::parseVector4(val, c, ","));
			}
			else
			{
				sampState.borderColor = DColor(DStringTool::parseVector4(val, c));
			}
		}

		else if ( "MinLod" == name )
		{
			DReal lod;
			DStringTool::parseString(val, lod);
			sampState.minLod = lod;
		}
		else if ( "MaxLod" == name )
		{
			DReal lod;
			DStringTool::parseString(val, lod);
			sampState.maxLod = lod;
		}
		else if ( "MaxAnisotropy" == name )
		{
			uint32 anis;
			DStringTool::parseString(val, anis);
			sampState.maxAnisotropy = anis;
		}
		else if ( "MipLodBias" == name )
		{
			DReal bias;
			DStringTool::parseString(val, bias);
			sampState.mipLodBias = bias;
		}
	}
	
	// from native val to DString val-----------------------
	DString	parsePolygonMode(PolygonMode val)
	{
		if (PM_Points == val)
		{
			return "Points";
		}
		else if ( PM_Wireframe == val)
		{
			return "Wireframe";
		}
		else if (PM_Solid == val)
		{
			return "Solid";
		}
		return "Solid";
	}
	DString	parseShadeMode(ShadeMode val)
	{
		if (SM_Flat == val)
		{
			return "Flat";
		}
		else if (SM_Gouraud == val)
		{
			return "Gouraoud";
		}
		else if (SM_Phong == val)
		{
			return "Phong";
		}
		return "Gouraoud";
	}
	DString	parseCullMode(CullMode val)
	{
		if (CM_None == val)
		{
			return "None";
		}
		else if (CM_Front == val)
		{
			return "Front";
		}
		else if (CM_Back == val)
		{
			return "Back";
		}
		return "Back";
	}
	DString	parseBool(bool val)
	{
		if (false == val)
		{
			return "false";
		}
		else if (true == val)
		{
			return "true";
		}
		return "false";
	}
	DString	parseCompareFunction(CompareFunction val)
	{
		if (CF_AlwaysFail == val)
		{
			return "AlwaysFail";
		}
		else if (CF_AlwaysPass == val)
		{
			return "AlwaysPass";
		}
		else if (CF_Less == val)
		{
			return "Less";
		}
		else if (CF_LessEqual == val)
		{
			return "LessEqual";
		}
		else if (CF_Equal == val)
		{
			return "Equal";
		}
		else if (CF_NotEqual == val)
		{
			return "NotEqual";
		}
		else if (CF_GreaterEqual == val)
		{
			return "GreaterEqual";
		}
		else if (CF_Greater == val)
		{
			return "Greater";
		}
		return "AlwaysPass";
	}
	DString	parseStencilOperation(StencilOperation val)
	{
		if (SO_Keep == val)
		{
			return "Keep";
		}
		else if (SO_Zero == val)
		{
			return "Zero";
		}
		else if (SO_Replace == val)
		{
			return "Replace";
		}
		else if (SO_Increment == val)
		{
			return "Increment";
		}
		else if (SO_Decrement == val)
		{
			return "Decrement";
		}
		else if (SO_IncrementWrap == val)
		{
			return "IncrementWrap";
		}
		else if (SO_DecrementWrap == val)
		{
			return "DecrementWrap";
		}
		else if (SO_Invert == val)
		{
			return "Invert";
		}
		return "Keep";
	}
	DString	parseSceneBlendOperation(BlendOperation val)
	{
		if (BO_Add == val)
		{
			return "Add";
		}
		else if (BO_Sub == val)
		{
			return "Sub";
		}
		else if (BO_RevSub == val)
		{
			return "RevSub";
		}
		else if (BO_Min == val)
		{
			return "Min";
		}
		else if (BO_Max == val)
		{
			return "Max";
		}
		return "Add";
	}
	DString	parseSceneBlendFactor(BlendFactor val)
	{
		if (BF_Zero == val)
		{
			return "Zero";
		}
		else if (BF_One == val)
		{
			return "One";
		}
		else if (BF_SrcAlpha == val)
		{
			return "SrcAlpha";
		}
		else if (BF_DstAlpha == val)
		{
			return "DstAlpha";
		}
		else if (BF_InvSrcAlpha == val)
		{
			return "InvSrcAlpha";
		}
		else if (BF_InvDstAlpha == val)
		{
			return "InvDstAlpha";
		}
		else if (BF_SrcColor == val)
		{
			return "SrcColor";
		}
		else if (BF_DstColor == val)
		{
			return "DstColor";
		}
		else if (BF_InvSrcColor == val)
		{
			return "InvSrcColor";
		}
		else if (BF_InvDstColor == val)
		{
			return "InvDstColor";
		}
		else if (BF_SrcAlphaSat == val)
		{
			return "SrcAlphaSat";
		}
		return "Zero";
	}
	DString	parseTextureAddressMode(TextureAddressMode val)
	{
		if (TAM_Wrap == val)
		{
			return "Wrap";
		}
		else if (TAM_Mirror == val)
		{
			return "Mirror";
		}
		else if (TAM_Clamp == val)
		{
			return "Clamp";
		}
		else if (TAM_Border == val)
		{
			return "Border";
		}
		return "Clamp";
	}
	DString	parseFilterOption(FilterOption val)
	{
		if (FO_None == val)
		{
			return "None";
		}
		else if (FO_Point == val)
		{
			return "Point";
		}
		else if (FO_Linear == val)
		{
			return "Linear";
		}
		else if (FO_Anistropic == val)
		{
			return "Anistropic";
		}
		return "Anistropic";
	}

	void DRECodec_v2_0::decode( DDataStream* input, DResource* targetRes )
	{
		DRenderEffect* targetEffect = targetRes->getAs<DRenderEffect>(false);
		if (targetEffect == NULL)
		{
			return;
		}
		DXMLDoc	doc;
		// because the xml parsing is destructive, we use 
		// embeded DXMLDoc's memory pool to handle memory.
		char*	xmlContent;
		size_t	size = input->getSize();
		// spare 1 char for the last '\0'
		xmlContent = doc.allocate_string(0, size+1);
		input->read(xmlContent, size);
		xmlContent[size] = '\0';

		doc.parse<0>(xmlContent);
		DXMLNode* effectNode = doc.first_node("RenderEffect");
		if (effectNode == NULL)
		{
			// failed to parse xml, is anything wrong with this file?
#ifdef DUEL_DEBUG
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"Error in parsing XML",
				"Error in parsing XML, no node named: 'GroupDescription' found",
				"Duel::DRECodec_v2_0::decodeImpl")
#endif
				return;
		}
		DXMLAttribute*	effectAtt = effectNode->first_attribute("version");
		if (effectAtt == NULL)
		{
			// failed to parse xml, is anything wrong with this file?
			DString	attVal = DXMLTool::readValue(effectAtt);
			if (attVal != "2.0")
			{
#ifdef DUEL_DEBUG
				DUEL_EXCEPT(DException::ET_ItemNotFound,
					"Error in parsing XML",
					"Error in parsing XML, version not correct(only version 2.0 available)",
					"Duel::DRECodec_v2_0::decodeImpl")
#endif
			}
			return;
		}

		// set name for the Effect
		//targetEffect->setName(effectNode->first_attribute("name")->value());
		DXMLNode* techniqueNode = effectNode->first_node("Technique");
		DXMLNode* passNode;
		DRenderTechniquePtr newTech;
		DRenderPassPtr newPass;
		// clear before adding
		targetEffect->removeAllTechniques();
		//Technique---------------------------
		while(techniqueNode)
		{
			newTech = targetEffect->addTechnique(DXMLTool::readValue(techniqueNode->first_attribute("name")));
			passNode = techniqueNode->first_node("Pass");
			//Pass-------------------------------------
			while(passNode)
			{
				newPass = newTech->addPass(DXMLTool::readValue(passNode->first_attribute("name")));
				DXMLNode* stateNode;
				DXMLNode* rasterizerNode = passNode->first_node("RasterizerState");
				//RasterizerState------------------------------
				if (rasterizerNode)
				{
					stateNode = rasterizerNode->first_node("PolygonMode");
					if (stateNode)
					{
						newPass->rasterizeState.polygonMode = getPolygonMode(DXMLTool::readValue(stateNode));
					}
					stateNode = rasterizerNode->first_node("ShadeMode");
					if (stateNode)
					{
						newPass->rasterizeState.shadeMode = getShadeMode(DXMLTool::readValue(stateNode));
					}
					stateNode = rasterizerNode->first_node("CullMode");
					if (stateNode)
					{
						newPass->rasterizeState.cullMode = getCullMode(DXMLTool::readValue(stateNode));
					}
					stateNode = rasterizerNode->first_node("PolygonOffsetFactor");
					if (stateNode)
					{
						DStringTool::parseString(DXMLTool::readValue(stateNode), newPass->rasterizeState.polygonOffsetFactor);
					}
					stateNode = rasterizerNode->first_node("PolygonOffsetUnit");
					if (stateNode)
					{
						DStringTool::parseString(DXMLTool::readValue(stateNode), newPass->rasterizeState.polygonOffsetUnit);
					}
					stateNode = rasterizerNode->first_node("ScissorEnable");
					if (stateNode)
					{
						newPass->rasterizeState.scissorEnable = getBool(DXMLTool::readValue(stateNode));
					}
					stateNode = rasterizerNode->first_node("MultisampleEnable");
					if (stateNode)
					{
						newPass->rasterizeState.multisampleEnable = getBool(DXMLTool::readValue(stateNode));
					}
				}
				//DepthStencil------------------------------------
				DXMLNode* depthStencilNode = passNode->first_node("DepthStencilState");
				if (depthStencilNode)
				{
					stateNode = depthStencilNode->first_node("DepthEnable");
					if (stateNode)
					{
						newPass->depthStencilState.depthEnable = getBool(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("DepthWriteEnable");
					if (stateNode)
					{
						newPass->depthStencilState.depthWriteEnable = getBool(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("DepthComparison");
					if (stateNode)
					{
						newPass->depthStencilState.depthComparison = getCompareFunction(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("FrontStencilEnable");
					if (stateNode)
					{
						newPass->depthStencilState.frontStencilEnable = getBool(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("FrontStencilComparison");
					if (stateNode)
					{
						newPass->depthStencilState.frontStencilComparison = getCompareFunction(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("FrontStencilReadMask");
					if (stateNode)
					{
						DStringTool::parseString(DXMLTool::readValue(stateNode), newPass->depthStencilState.frontStencilReadMask);
					}
					stateNode = depthStencilNode->first_node("FrontStencilWriteMask");
					if (stateNode)
					{
						DStringTool::parseString(DXMLTool::readValue(stateNode), newPass->depthStencilState.frontStencilWriteMask);
					}
					stateNode = depthStencilNode->first_node("FrontStencilFail");
					if (stateNode)
					{
						newPass->depthStencilState.frontStencilFail = getStencilOperation(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("FrontStencilDepthFail");
					if (stateNode)
					{
						newPass->depthStencilState.frontStencilDepthFail = getStencilOperation(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("FrontStencilPass");
					if (stateNode)
					{
						newPass->depthStencilState.frontStencilPass = getStencilOperation(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("BackStencilEnable");
					if (stateNode)
					{
						newPass->depthStencilState.backStencilEnable = getBool(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("BackStencilComparison");
					if (stateNode)
					{
						newPass->depthStencilState.backStencilComparison = getCompareFunction(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("BackStencilReadMask");
					if (stateNode)
					{
						DStringTool::parseString(DXMLTool::readValue(stateNode), newPass->depthStencilState.backStencilReadMask);
					}
					stateNode = depthStencilNode->first_node("BackStencilWriteMask");
					if (stateNode)
					{
						DStringTool::parseString(DXMLTool::readValue(stateNode), newPass->depthStencilState.backStencilWriteMask);
					}
					stateNode = depthStencilNode->first_node("BackStencilFail");
					if (stateNode)
					{
						newPass->depthStencilState.backStencilFail = getStencilOperation(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("BackStencilDepthFail");
					if (stateNode)
					{
						newPass->depthStencilState.backStencilDepthFail = getStencilOperation(DXMLTool::readValue(stateNode));
					}
					stateNode = depthStencilNode->first_node("BackStencilPass");
					if (stateNode)
					{
						newPass->depthStencilState.backStencilPass = getStencilOperation(DXMLTool::readValue(stateNode));
					}
				}
				

				//////////////////////////////////////////////////////////////////////////
				DXMLNode* blendNode = passNode->first_node("BlendState");
				if (blendNode)
				{

					DXMLNode* alphaCov = blendNode->first_node("AlphaToCoverageEnable");
					if (alphaCov)
					{
						newPass->blendState.alphaToCoverageEnable = getBool(DXMLTool::readValue(alphaCov));
					}
					DXMLNode* rtBlendState = blendNode->first_node("RenderTargetBlendState");
					while (rtBlendState)
					{
						DXMLAttribute* rtBSAtt = rtBlendState->first_attribute("index");
						uint32 rtIndx = 0;
						if (rtBSAtt)
						{
							if(DStringTool::parseString(DXMLTool::readValue(rtBSAtt), rtIndx))
							{
								// 为了适应图形API的现状, 限定这个序号小于8, 
								if (rtIndx < DUEL_MAX_BLEND_LAYERS)
								{
									DRenderTargetBlendState	tmpState;
									stateNode = rtBlendState->first_node("BlendEnable");
									if (stateNode)
									{
										tmpState.blendEnable = getBool(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("ColorBlendOpeartion");
									if (stateNode)
									{
										tmpState.colorBlendOpeartion = getSceneBlendOperation(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("ColorSrcFactor");
									if (stateNode)
									{
										tmpState.colorSrcFactor = getSceneBlendFactor(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("ColorDstFactor");
									if (stateNode)
									{
										tmpState.colorDstFactor = getSceneBlendFactor(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("AlphaBlendOpeartion");
									if (stateNode)
									{
										tmpState.alphaBlendOpeartion = getSceneBlendOperation(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("AlphaSrcFactor");
									if (stateNode)
									{
										tmpState.alphaSrcFactor = getSceneBlendFactor(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("AlphaDstFactor");
									if (stateNode)
									{
										tmpState.alphaDstFactor = getSceneBlendFactor(DXMLTool::readValue(stateNode));
									}
									stateNode = rtBlendState->first_node("ColorWriteMask");
									if (stateNode)
									{
										uint8 msk = 0;
										DStringVector maskValue = DStringTool::split(DXMLTool::readValue(stateNode), "|");
										size_t len = maskValue.size();
										for (uint32 i = 0; i < len; ++i)
										{
											msk |= readColorMask(maskValue[i]);
										}
										tmpState.colorWriteMask = msk;
									}
									newPass->blendState.targetStates[rtIndx] = tmpState;
								}
							}
						}
						rtBlendState = rtBlendState->next_sibling("RenderTargetBlendState");
					}
				}
				DXMLNode* vsNode = passNode->first_node("VertexProgram");
				if (vsNode)
				{
					DResourcePtr vs;
					DString vsName = DXMLTool::readValue(vsNode);
					DString vsGroup = DXMLTool::readValue(vsNode->first_attribute("group"));
					vs = DGpuProgramManager::getSingleton().getResource( 
						vsGroup,vsName);
					newPass->setVertexProgram(vs);
				}
				//PixelShader------------------------------------
				DXMLNode* psNode = passNode->first_node("PixelProgram");
				if (psNode)
				{
					DResourcePtr ps;
					DString psName = DXMLTool::readValue(psNode);
					DString psGroup = DXMLTool::readValue(psNode->first_attribute("group"));
					ps = DGpuProgramManager::getSingleton().getResource( 
						psGroup, psName);
					newPass->setPixelProgram(ps);
				}
				passNode = passNode->next_sibling("Pass");
			}

			techniqueNode = techniqueNode->next_sibling("Technique");
		}
	}

	Duel::DDataStreamPtr DRECodec_v2_0::encode( DResource* targetRes )
	{
		DRenderEffect* targetEffect = targetRes->getAs<DRenderEffect>(false);
		if (targetEffect == NULL)
		{
			return DDataStreamPtr();
		}
		DXMLDoc doc;
		DRenderEffect::RenderTechniqueIterator ti =  targetEffect->getRenderTechniqueIterator();

		DXMLNode* root = doc.allocate_node(DUEL_XML::node_pi, doc.allocate_string("xml version='1.0'"));
		doc.append_node(root);
		DXMLNode* effectNode = doc.allocate_node(DUEL_XML::node_element, "RenderEffect");
		effectNode->append_attribute(doc.allocate_attribute("name", targetEffect->getName().c_str()));
		effectNode->append_attribute(doc.allocate_attribute("version", "2.0"));
		root->append_node(effectNode);

 		while(ti.hasMoreElements())
 		{
 			DXMLNode* techniqueNode = doc.allocate_node(DUEL_XML::node_element, "RenderTechnique");
 			techniqueNode->append_attribute(doc.allocate_attribute("name", ti.current()->get()->getName().c_str()));
 			effectNode->append_node(techniqueNode);
 
 			DRenderTechnique::RenderPassIterator pi = ti.current()->get()->getRenderPassIterator();
 			while(pi.hasMoreElements())
 			{
 				DRenderPass* curPass = pi.current()->get();
				DXMLNode* passNode = doc.allocate_node(DUEL_XML::node_element, "RenderPass");
 				passNode->append_attribute(doc.allocate_attribute("name", curPass->getName().c_str()));
 				techniqueNode->append_node(passNode);
 
 				DXMLNode* stateNode; 
 				DXMLNode* rasterizerNode = doc.allocate_node(DUEL_XML::node_element, "RaRasterizerState");
 				passNode->append_node(rasterizerNode);
				//RasterizerState--------------------------------------
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "PolygonMode", parsePolygonMode(curPass->rasterizeState.polygonMode).c_str());
 				rasterizerNode->append_node(stateNode);
 
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "ShadeMode", parseShadeMode(curPass->rasterizeState.shadeMode).c_str());
 				rasterizerNode->append_node(stateNode);
 
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "CullMode", parseCullMode(curPass->rasterizeState.cullMode).c_str());
 				rasterizerNode->append_node(stateNode);
 
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "PolygonOffsetFactor", DStringTool::toString(curPass->rasterizeState.polygonOffsetFactor).c_str());
 				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "PolygonOffsetUnit", DStringTool::toString(curPass->rasterizeState.polygonOffsetUnit).c_str());
				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "ScissorEnable", parseBool(curPass->rasterizeState.scissorEnable).c_str());
				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "MultisampleEnable", parseBool(curPass->rasterizeState.multisampleEnable).c_str());
				rasterizerNode->append_node(stateNode);
 
 
 				DXMLNode* depthStencilNode = doc.allocate_node(DUEL_XML::node_element, "DepthStencilState");
				passNode->append_node(depthStencilNode);
				//DepthStencilState-----------------------------------
				stateNode = doc.allocate_node(DUEL_XML::node_comment, "DepthEnable", parseBool(curPass->depthStencilState.depthEnable).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "DepthWriteEnable", parseBool(curPass->depthStencilState.depthWriteEnable).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "DepthComparison", parseCompareFunction(curPass->depthStencilState.depthComparison).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilEnable", parseBool(curPass->depthStencilState.frontStencilEnable).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilComparison", parseCompareFunction(curPass->depthStencilState.frontStencilComparison).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilReadMask", DStringTool::toString(curPass->depthStencilState.frontStencilReadMask).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilWriteMask", DStringTool::toString(curPass->depthStencilState.frontStencilWriteMask).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilFail", parseStencilOperation(curPass->depthStencilState.frontStencilFail).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilDepthFail", parseStencilOperation(curPass->depthStencilState.frontStencilDepthFail).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "FrontStencilPass", parseStencilOperation(curPass->depthStencilState.frontStencilPass).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilEnable", parseBool(curPass->depthStencilState.backStencilEnable).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilComparison", parseCompareFunction(curPass->depthStencilState.backStencilComparison).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilReadMask", DStringTool::toString(curPass->depthStencilState.backStencilReadMask).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilWriteMask", DStringTool::toString(curPass->depthStencilState.backStencilWriteMask).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilFail", parseStencilOperation(curPass->depthStencilState.backStencilFail).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilDepthFail", parseStencilOperation(curPass->depthStencilState.backStencilDepthFail).c_str());
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "BackStencilPass", parseStencilOperation(curPass->depthStencilState.backStencilPass).c_str());
				depthStencilNode->append_node(stateNode);


				//////////////////////////////////////////////////////////////////////////
				// v2.0
				DXMLNode* blendNode = doc.allocate_node(DUEL_XML::node_element, "BlendState");
				stateNode = doc.allocate_node(DUEL_XML::node_comment, "AlphaToCoverageEnable", parseBool(curPass->blendState.alphaToCoverageEnable).c_str());
				blendNode->append_node(stateNode);

				// we only encode those states that are not the default value.
				DBlendState defaultState;
				if ( curPass->blendState != defaultState)
				{
					for (uint32 i = 0; i < DUEL_MAX_BLEND_LAYERS; ++i)
					{
						if (curPass->blendState.targetStates[i] != defaultState.targetStates[i])
						{
							DXMLNode* rtbsNode = doc.allocate_node(DUEL_XML::node_comment, "RenderTargetBlendState");
							rtbsNode->append_attribute(doc.allocate_attribute("index", DStringTool::toString(i).c_str()));

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "BlendEnable", parseBool(curPass->blendState.targetStates[i].blendEnable).c_str());
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "ColorBlendOpeartion", parseSceneBlendOperation(curPass->blendState.targetStates[i].colorBlendOpeartion).c_str());	
							rtbsNode->append_node(stateNode);


							stateNode = doc.allocate_node(DUEL_XML::node_comment, "ColorSrcFactor", parseSceneBlendFactor(curPass->blendState.targetStates[i].colorSrcFactor).c_str());
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "ColorDstFactor", parseSceneBlendFactor(curPass->blendState.targetStates[i].colorDstFactor).c_str());
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "AlphaBlendOpeartion", parseSceneBlendOperation(curPass->blendState.targetStates[i].alphaBlendOpeartion).c_str());
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "AlphaSrcFactor", parseSceneBlendFactor(curPass->blendState.targetStates[i].alphaSrcFactor).c_str());
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "AlphaDstFactor", parseSceneBlendFactor(curPass->blendState.targetStates[i].alphaDstFactor).c_str());
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "ColorWriteMask", DStringTool::toString(curPass->blendState.targetStates[i].colorWriteMask).c_str());
							rtbsNode->append_node(stateNode);

							blendNode->append_node(rtbsNode);

						}
					}
				}
				passNode->append_node(blendNode);


				// vertex shader------------------------------------------------
				if (curPass->getVertexProgram() != NULL)
				{
					DXMLNode* vsNode = doc.allocate_node(DUEL_XML::node_element, "VertexProgram", curPass->getVertexProgram()->getName().c_str());
					vsNode->append_attribute(doc.allocate_attribute("group",curPass->getVertexProgram()->getGroupName().c_str()));
					passNode->append_node(vsNode);
				}
				else
				{
					DXMLNode* vsNode = doc.allocate_node(DUEL_XML::node_element, "VertexProgram", "");
					vsNode->append_attribute(doc.allocate_attribute("group", ""));
					passNode->append_node(vsNode);
				}
				// pixel shader--------------------------------------------------
				if (curPass->getPixelProgram() != NULL)
				{
					DXMLNode* psNode = doc.allocate_node(DUEL_XML::node_element, "PixelProgram", curPass->getPixelProgram()->getName().c_str());
					psNode->append_attribute(doc.allocate_attribute("group",curPass->getPixelProgram()->getGroupName().c_str()));
					passNode->append_node(psNode);
				}
				else
				{
					DXMLNode* psNode = doc.allocate_node(DUEL_XML::node_element, "PixelProgram", "");
					psNode->append_attribute(doc.allocate_attribute("group", ""));
					passNode->append_node(psNode);
				}

				pi.moveNext();
			}
			ti.moveNext();
 		}
// 
// 		// output 
		DString result;
		DUEL_XML::print(std::back_inserter(result), doc);

		char* buf = new char[result.size()];
		DMemoryStream* ret = new DMemoryStream((void*)buf, result.size(), DA_Write, true); 
		ret->write(result.c_str(), result.size());

		return DMemoryStreamPtr(ret);
	}

	void DRECodec_v2_0::encodeToFile( const DString& fileName, DResource* targetRes )
	{
		DRenderEffect* targetEffect = targetRes->getAs<DRenderEffect>(false);
		if (targetEffect == NULL)
		{
			return;
		}
		DMemoryStream* memData = static_cast<DMemoryStream*>(encode(targetEffect).get());
		// output 
		std::ofstream output(fileName);
		output.write(memData->getChunkPtr(), memData->getSize());
		output.flush();
	}

}