//  [12/16/2012 OMEGA] created

#ifndef _DUELRENDERQUEUE_H_
#define _DUELRENDERQUEUE_H_

#include "DuelCommon.h"
#include "DuelRenderable.h"
#include "DuelRenderGroup.h"

namespace Duel
{

	// pre-defined ID, also act as the priority of the group,
	// self-defined group ID MUST avoid these values, and insert into the gaps 
	enum RenderGroupID
	{
		RG_Background		= 0,
		RG_Sky				= 10000,
		RG_Terrain			= 50000,
		RG_Main_Opaque		= 100000,
		RG_Main_Transparent	= 200000,
		RG_FX				= 300000,
		RG_Reflactable		= 400000,
		RG_GuiBack			= 500000,	// reserved for gui rendering.
		RG_Overlay			= 600000,
		RG_GuiFront			= 700000,	// reserved for gui rendering.
		RG_Max				= 1000000, // do not exceed it.
		RG_FORCE_UINT		= 0xffffffffUL // never used
	};

	// DRenderQueue is sorted by priority group, meaning the group with smaller
	// priority will be rendered first, no matter how far the renderables in the
	// group from the camera, sorting renderables by the distance to the camera
	// should be in individual group.
	class DUEL_API DRenderQueue : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderQueue)
	public:
		DRenderQueue();
		virtual ~DRenderQueue();
		typedef	std::map<uint32, DRenderGroup*>	RenderGroupMap;
		typedef	MapIterator<RenderGroupMap>		RenderGroupIterator;
		RenderGroupIterator		getRenderGroupIterator() { return RenderGroupIterator(mGroupMap.begin(), mGroupMap.end()); }

		// used to traverse all lights.
		typedef	std::vector<DLightSource*>		Lights;
		typedef	VectorIterator<Lights>	LightIterator;
		LightIterator	getLightIterator() { return LightIterator(mLights.begin(), mLights.end()); }
		// register a light for rendering current group, only used when shadow is enabled.
		// dont add duplicated light, it will slow down the speed of rendering.
		void			addLight(DLightSource* light);
		// clear all lights, preparing for next rendering.
		void			clearAllLights();

		// if specified groupID does not exist, the queue will create one.
		void			addRenderale(uint32 groupID, DRenderable* rend);
//		void					addRenderale(uint32 groupID, DRenderable* rend, DRenderTechnique* tech);
		// create a new render group with specified id, if there was an existed group, it will be returned.
		DRenderGroup*	createRenderGroup(uint32 groupID);
		// get the group by ID, a NULL pointer will be returned if specified
		// group does not exist
		DRenderGroup*	getRenderGroup(uint32 groupID);
		// destroy a render group by ID.
		void			destroyRenderGroup(uint32 groupID);
		// destroy all render groups.
		void			destroyAllRenderGroups();

		typedef	std::vector<DPostEffectInstancePtr>	PostEffectList;
		typedef	VectorIterator<PostEffectList>		PostEffectIterator;
		PostEffectIterator	getPostEffectIterator() { return PostEffectIterator(mPostEffectList); }
		// you can add same post effect for more than once.
		void			addPostEffect(DPostEffectInstancePtr pe);
		// if there are more than one post effect found, this method will only remove once.
		void			removePostEffect(DPostEffectInstancePtr pe);
		void			removeAllPostEffects();

		// shortcut for clear all groups
		void			clear();

	protected:
		// we rely on std::map because in its inner implement, the sorting is
		// based on std::less method, and we can only rely on its ascending sorting.
		RenderGroupMap			mGroupMap;
		// DLight storage.
		Lights			mLights;
		PostEffectList	mPostEffectList;
	};

}


#endif