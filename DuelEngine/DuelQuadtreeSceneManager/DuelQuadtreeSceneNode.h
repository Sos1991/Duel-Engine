//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREESCENENODE_H_
#define _DUELQUADTREESCENENODE_H_

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtree.h"
#include "DuelSceneNode.h"

namespace Duel
{

	class DUELQUADTREE_API QuadtreeSceneNode : public DSceneNode
	{
	DUEL_DECLARE_RTTI(QuadtreeSceneNode)
	public:
		QuadtreeSceneNode(QuadtreeSceneManager* creator, const DString& name);
		~QuadtreeSceneNode();
		// set the quadrant this node belongs to.
		// �����������Ĳ����ڵ�.
		INTERNAL void	setQuadrant(Quadrant* quadrant) { mQuadrant = quadrant; }
		// get the quadrant this node belongs to, if this node belongs to nothing,
		// a NULL will be returned.
		// ȡ���������Ĳ����ڵ�.
		Quadrant*	getQuadrant() const { return mQuadrant; }
		// unbind quadrant this node attached to, the children will unbind, too.
		// ������Ĳ����İ�, �ӽڵ�Ҳͬ������.
		INTERNAL void	unbindQuadrant();
		// query whether the center of this node is in the specified box. since we are loose, only check the center.
		// ��ѯ���ڵ��Ƿ��ڰ�Χ����.��Ϊ����ɢ��, ����ֻ�������.
		bool	isIn(const DAxisAlignedBox& box);

		// override : DNode------------------------------
		// ����Node�ĺ���
		DNode*	removeChild(uint32 index);
		// override : DNode------------------------------
		// ����Node�ĺ���
		DNode*	removeChild(const DString& name);
		// override : DNode------------------------------
		// ����Node�ĺ���
		DNode*	removeChild(DNode* n);
		// override : DNode------------------------------
		// ����Node�ĺ���
		void	removeAllChildren();
		// override : DSceneNode------------------------
		// it will no longer calculate the size of children.
		// ����SceneNode�ĺ���, ���ټ����ӽڵ�İ�Χ��.
		void	updateBound();

	protected:
		// quadrant pointer
		Quadrant*	mQuadrant;
	};
}


#endif // !_DUELQUADTREESCENENODE_H_
