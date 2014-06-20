//  [1/18/2013 OMEGA] created

#ifndef _DUELOCTREESCENENODE_H_
#define _DUELOCTREESCENENODE_H_

#include "DuelOctreeCommon.h"
#include "DuelOctree.h"
#include "DuelSceneNode.h"

namespace Duel
{

	class DUELOCTREE_API OctreeSceneNode : public DSceneNode
	{
	DUEL_DECLARE_RTTI(OctreeSceneNode)
	public:
		OctreeSceneNode(DSceneManager* creator, const DString& name);
		~OctreeSceneNode();
		// set the octant this node belongs to.
		// ���������İ˲����ڵ�.
		INTERNAL void	setOctant(Octant* octant) { mOctant = octant; }
		// get the octant this node belongs to, if this node belongs to nothing,
		// a NULL will be returned.
		// ȡ�������İ˲����ڵ�.
		Octant*	getOctant() const { return mOctant; }
		// unbind octant this node attached to, the children will unbind, too.
		// �����˲����İ�, �ӽڵ�Ҳͬ������.
		INTERNAL void	unbindOctant();
		// query whether the center of this node is in the specified box.
		// ��ѯ���ڵ��Ƿ��ڰ�Χ����.
		bool	isIn(const DAxisAlignedBox& box);

		// override : DNode------------------------------
		// ����DNode�ĺ���
		DNode*	removeChild(uint32 index);
		// override : DNode------------------------------
		// ����DNode�ĺ���
		DNode*	removeChild(const DString& name);
		// override : DNode------------------------------
		// ����DNode�ĺ���
		DNode*	removeChild(DNode* n);
		// override : DNode------------------------------
		// ����DNode�ĺ���
		void	removeAllChildren();
		// override : DSceneNode------------------------
		// it will no longer calculate the size of children.
		// ����DSceneNode�ĺ���, ���ټ����ӽڵ�İ�Χ��.
		void	updateBound();

	protected:
		// octant pointer
		Octant*	mOctant;
	};
}

#endif