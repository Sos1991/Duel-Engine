//  [1/27/2013 OMEGA] created

#ifndef _DUELOCTREE_H_
#define _DUELOCTREE_H_

#include "DuelOctreeCommon.h"
#include "DuelOctreeSceneNode.h"

namespace Duel
{
	/** Octree datastructure for managing scene nodes.
	@remarks
	This is a loose octree implementation, meaning that each
	octant child of the octree actually overlaps it's siblings by a factor
	of .5.  This guarantees that any thing that is half the size of the parent will
	fit completely into a child, with no splitting necessary.
	*/
	// ��ɢ�˲���, ��ͨ�˲�����Ϊ���ô����紦�����嶨������(��Ϊ�˲�����"ճ��"), ����ƽ��ʹ�õ���
	// ��������Ƚϴ�(�����ԼΪԭ���ֿռ�1.5��)������, �������ֵܽڵ�֮������غϵ�����, ��Ȼ�ڻ��������Ͻ�����,
	// �����ܹ����ճ������.
	class DUELOCTREE_API Octant 
	{
	public:
		// if the Octree is the root, use NULL.
		// ��������һ�����ڵ�, parent����ΪNULL.
		Octant(Octant* parent);
		// this method will delete all children
		// ���������ɾ����������.
		~Octant();

		// add a DSceneNode, no check for node's region. usually called by OctreeSceneManager.
		// ����һ�������ڵ�, �������������ڵ�����������, ͨ����DSceneManager����.
		void		addSceneNode(OctreeSceneNode* node);
		// remove a DSceneNode from this octree node.
		// �Ƴ�һ���ڵ�.
		void		removeSceneNode(OctreeSceneNode* node);
		// remove all nodes
		void		removeAllSceneNodes();
		// get the count of node attached to this octree node, including children's node count.
		// �õ��ڵ�����, ���ֵ����������Ľڵ�����.
		size_t				getNodeCount() const { return mNodeCount; }
			
		// query whether the specified child exists.
		// ��ѯָ����ŵ������Ƿ����.
		bool				isOctantExists(uint32 x, uint32 y, uint32 z);
		// create a octant with specified index, if it already existed, current one will be returned.
		// ����һ������,����Ѿ�����, �򷵻ص�ǰ�Ѵ��ڵ�ֵ
		Octant*				createOctant(uint32 x, uint32 y, uint32 z);
		// get child octant.
		// ȡ��ĳ������.
		Octant*				getOctant(uint32 x, uint32 y, uint32 z)
		{
			assert(x < 2 && y < 2 && z < 2);
			return mChildren[x][y][z];
		}
		// delete an octant
		// ɾ��ĳ������
		void				removeOctant(uint32 x, uint32 y, uint32 z);

		// query whether this octant is leaf octant
		// ��ѯ����ڵ��Ƿ�ΪҶ�ڵ�
		bool				isTerminal() const { return mbTerminal; }

		// set the bounding region of this octant.
		// ������������Ķ�������. 
		INTERNAL void		setRegion(const DAxisAlignedBox& box);
		// get the bounding region of this octree node. 
		// ȡ����������Ķ�������.
		const DAxisAlignedBox&	getRegion() const;

		// this method is used to degermine whether the specified box is fit into a child node of this octant.
		// usually called by DSceneManager.
		// this method is the core of loose octree, which gives a 'loose' result of containment testing,
		// ������������жϸ�����AAB�Ƿ��ʺϷŽ�����ڵ��������, ͨ����DSceneManager����.
		// ��������ǰ˲���'��ɢ'�ĺ���, ���ܷ���һ����Ϊ���ɵİ����Բ��Խ��
		bool				isTwiceSize(const DAxisAlignedBox& box) const;

		// this method is used for querying which child should be traversed to insert specified box,
		// usually called by DSceneManager.
		// �����������ȡ�ø�����AAB���ڵ��������, ͨ����DSceneManager����.
		void				getChildIndex(const DAxisAlignedBox& box, uint32& x, uint32& y, uint32& z);

		// used to traverse children
		// ���ڱ�������.
		typedef	std::list<OctreeSceneNode*>			OctreeSceneNodeList;
		typedef	ListIterator<OctreeSceneNodeList>	OctreeSceneNodeIterator;
		OctreeSceneNodeIterator	getSceneNodeIterator() { return OctreeSceneNodeIterator(mNodeList.begin(), mNodeList.end()); }
	protected:
		// Increments the overall node count of this octree and all its parents
		// ���Ӱ󶨵��������Ľڵ���Ŀ.
		inline void			increaseNodeCount()
		{
			mNodeCount++;
			if ( mParent != NULL ) mParent -> increaseNodeCount();
		};

		// Decrements the overall node count of this octree and all its parents
		// ���ٰ󶨵��������Ľڵ���Ŀ.
		inline void			decreaseNodeCount()
		{
			mNodeCount--;
			if ( mParent != NULL ) mParent -> decreaseNodeCount();
		};
		// scene node storage.
		OctreeSceneNodeList	mNodeList;
		// parent pointer.
		Octant*				mParent;
		// seperate octree in x, y, z axis.
		Octant*				mChildren[2][2][2];
		// the bounding region of this octant.
		DAxisAlignedBox		mRegion;
		// record of node count
		size_t				mNodeCount;
		// indictate whether this octant is terminal.
		bool				mbTerminal;

	};
}

#endif