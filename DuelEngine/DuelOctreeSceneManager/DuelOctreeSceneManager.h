//  [1/18/2013 OMEGA] created

#ifndef _DUELOCTREESCENEMANAGER_H_
#define _DUELOCTREESCENEMANAGER_H_

#include "DuelOctreeCommon.h"
#include "DuelOctree.h"
#include "DuelSceneManager.h"
#include "DuelSceneManagerEnumerator.h"

namespace Duel
{

	class DUELOCTREE_API OctreeSceneManager : public DSceneManager
	{
	DUEL_DECLARE_RTTI(OctreeSceneManager)
	public:
		OctreeSceneManager(DSceneManagerFactory* creator, const DString& name);
		~OctreeSceneManager();

		// override : DSceneManger------------------------------
		void			initialize(DAxisAlignedBox region, DReal granularity);
		// resize the quadtree
		virtual void	resize(DAxisAlignedBox region);

		// get the octree
		Octant*			getOctree() { return mOctree; }

		// override : DSceneManager--------------------------
		void			updateSceneNode(DSceneNode* node);
		// override : DSceneManager---------------------------
		void			clearScene();

		// remove an OctreeSceneNode from scene octree, this method will no affect its children.
		// ��һ��OctreeSceneNode�ӳ����˲������Ƴ�, ����Ӱ�����ӽڵ�.
		INTERNAL void	removeOctreeSceneNode(OctreeSceneNode* n);
		// add an OctreeSceneNode to the scene octree recursivly, without affecting its children.
		// ������������һ��OctreeSceneNode��ָ���İ˲����ӽڵ���, ���һ�ݹ���Һ��ʵ��ӽڵ����, ����Ӱ��
		// OctreeSceneNode���ӽڵ�.
		INTERNAL void	addOctreeSceneNode(OctreeSceneNode* n, Octant* oct, uint32 depth );

		// override : DSceneManager--------------------------
		RayQuery*				createRayQuery();
		// override : DSceneManager--------------------------
		AxisAlignedBoxQuery*	createAABBQuery();
		// override : DSceneManager--------------------------
		SphereQuery*			createSphereQuery();
		// override : DSceneManager--------------------------
		IntersectionQuery*		createIntersectionQuery();
		// override : DSceneManager--------------------------
		void			destroyQuery(RayQuery* query);
		// override : DSceneManager--------------------------
		void			destroyQuery(AxisAlignedBoxQuery* query);
		// override : DSceneManager--------------------------
		void			destroyQuery(SphereQuery* query);
		// override : DSceneManager--------------------------
		void			destroyQuery(IntersectionQuery* query);
		
		// override : DSceneManager--------------------------
		void			populateLights(LightMap& outMap, DCamera* cam);
		// override : DSceneManager--------------------------
		void			applyToRenderQueue(DRenderQueue* queue, DCamera* cam);


	protected:
		// override : DSceneManager--------------------------
		DSceneNode*		createSceneNodeImpl();
		// override : DSceneManager--------------------------
		DSceneNode*		createSceneNodeImpl(const DString& name);
		// override : DSceneManager--------------------------
		void			destroySceneNodeImpl(DSceneNode* node);

		// internal method used to traverse the octree. and populate objects to 
		// the render queue.
		void			walkOctree(Octant* octant, DRenderQueue* queue, DCamera* cam);

		// shut down the scene manager, delete all octants
		void			shutdownOctree(Octant* octant);

		// root octree.
		Octant*			mOctree;
		// the box containts all the scene.
		DAxisAlignedBox	mSceneBox;
		// 
		DReal			mGranularity;
		// the maximun depth of octree.
		uint32			mMaxDepth;
		// name generater.
		DNameGenerator	mNameGen;
	};

	class DUELOCTREE_API OctreeSceneManagerFactory : public DSceneManagerFactory
	{
	DUEL_DECLARE_RTTI(OctreeSceneManagerFactory)
	public:
		OctreeSceneManagerFactory();
		// override:DSceneManagerFactory---------------------------
		DSceneManager*	createSceneManager(const DString& name);
		// override:DSceneManagerFactory---------------------------
		void			destroySceneManager(DSceneManager* sm);
	};

}

#endif