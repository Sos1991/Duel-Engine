//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREE_H_
#define _DUELQUADTREE_H_

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtreeSceneNode.h"

namespace Duel
{
	/** Quadtree datastructure for managing scene nodes.
	@remarks
	This is a loose Quadtree implementation, meaning that each
	quadrant child of the Quadtree actually overlaps it's siblings by a factor
	of .5.  This guarantees that any thing that is half the size of the parent will
	fit completely into a child, with no splitting necessary.
	*/
	// ��ɢ�Ĳ���, ��ͨ�Ĳ�����Ϊ���ô����紦�����嶨������(��Ϊ�Ĳ�����"ճ��"), ����ƽ��ʹ�õ���
	// ��������Ƚϴ�(�����ԼΪԭ���ֿռ�1.5��)������, �������ֵܽڵ�֮������غϵ�����, ��Ȼ�ڻ��������Ͻ�����,
	// �����ܹ����ճ������.
	class DUELQUADTREE_API Quadrant 
	{
	public:
		// if the Quadtree is the root, use NULL.
		// ��������һ�����ڵ�, parent����ΪNULL.
		Quadrant(Quadrant* parent);
		// this method will delete all children
		// ���������ɾ����������.
		~Quadrant();

		// add a SceneNode, no check for node's region. usually called by QuadtreeSceneManager.
		// ����һ�������ڵ�, �������������ڵ�����������, ͨ����SceneManager����.
		void		addSceneNode(QuadtreeSceneNode* node);
		// remove a SceneNode from this Quadtree node.
		// �Ƴ�һ���ڵ�.
		void		removeSceneNode(QuadtreeSceneNode* node);
		// get the count of node attached to this Quadtree node, including children's node count.
		// �õ��ڵ�����, ���ֵ����������Ľڵ�����.
		size_t				getNodeCount() const { return mNodeCount; }
			
		// query whether the specified child exists.
		// ��ѯָ����ŵ������Ƿ����.
		bool				isQuadrantExists(uint32 x, uint32 z);
		// create a Quadrant with specified index, if it already existed, current one will be returned.
		// ����һ������,����Ѿ�����, �򷵻ص�ǰ�Ѵ��ڵ�ֵ
		Quadrant*				createQuadrant(uint32 x, uint32 z);
		// get child Quadrant.
		// ȡ��ĳ������.
		Quadrant*				getQuadrant(uint32 x, uint32 z)
		{
			assert(x < 2 && z < 2);
			return mChildren[x][z];
		}
		// delete an quadrant
		// ɾ��ĳ������
		void				removeQuadrant(uint32 x, uint32 z);

		// query whether this Quadrant is leaf Quadrant
		// ��ѯ����ڵ��Ƿ�ΪҶ�ڵ�
		bool				isTerminal() const { return mbTerminal; }

		// set the bounding region of this Quadrant.
		// ������������Ķ�������. 
		INTERNAL void		setRegion(const DAxisAlignedBox& box);
// 		// get the bounding region of this Quadtree node. 
// 		// ȡ����������Ĳü���������.
// 		const DAxisAlignedBox&	getRegion() const;
// 		
		// ȡ��������������Ķ�������.
		const DAxisAlignedBox&	getRegion() const { return mRegion; }

		// this method is used to degermine whether the specified box is fit into a child node of this Quadrant.
		// usually called by SceneManager.
		// this method is the core of loose Quadtree, which gives a 'loose' result of containment testing,
		// ������������жϸ�����AAB�Ƿ��ʺϷŽ�����ڵ��������, ͨ����SceneManager����.
		// ����������Ĳ���'��ɢ'�ĺ���, ���ܷ���һ����Ϊ���ɵİ����Բ��Խ��
		bool				isTwiceSize(const DAxisAlignedBox& box) const;

		// this method is used for querying which child should be traversed to insert specified box,
		// usually called by SceneManager.
		// �����������ȡ�ø�����AAB���ڵ��������, ͨ����SceneManager����.
		void				getChildIndex(const DAxisAlignedBox& box, uint32& x, uint32& z);

		// used to traverse children
		// ���ڱ�������.
		typedef	std::list<QuadtreeSceneNode*>		QuadtreeSceneNodeList;
		typedef	ListIterator<QuadtreeSceneNodeList>	QuadtreeSceneNodeIterator;
		QuadtreeSceneNodeIterator	getSceneNodeIterator() { return QuadtreeSceneNodeIterator(mNodeList.begin(), mNodeList.end()); }
	protected:
		// Increments the overall node count of this Quadtree and all its parents
		// ���Ӱ󶨵��������Ľڵ���Ŀ.
		inline void			increaseNodeCount()
		{
			mNodeCount++;
			if ( mParent != NULL ) mParent -> increaseNodeCount();
		};

		// Decrements the overall node count of this Quadtree and all its parents
		// ���ٰ󶨵��������Ľڵ���Ŀ.
		inline void			decreaseNodeCount()
		{
			mNodeCount--;
			if ( mParent != NULL ) mParent -> decreaseNodeCount();
		};
		// scene node storage.
		QuadtreeSceneNodeList	mNodeList;
		// parent pointer.
		Quadrant*				mParent;
		// seperate Quadtree in x, z, z axis.
		Quadrant*				mChildren[2][2];
		// the bounding region of this Quadrant.
		DAxisAlignedBox		mRegion;


		// record of node count
		size_t				mNodeCount;
		// indictate whether this Quadrant is terminal.
		bool				mbTerminal;

	};

}

#endif // !_DUELQUADTREE_H_
