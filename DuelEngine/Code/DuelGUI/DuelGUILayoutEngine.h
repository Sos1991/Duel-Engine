//  [7/13/2013 OMEGA] created

#ifndef _DUELGUILAYOUTENGINE_H_
#define _DUELGUILAYOUTENGINE_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelGUISize.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUILayoutItem.h"

namespace Duel
{
	// stretch is the factor take effect on multi items, for example,
	// there are three items in one layout i1, i2, i3, with stretch factor
	// 1, 3, 2, then the i2 has more space than i3, i3 has more space than i1.
	struct DGLayoutStruct
	{
		inline void		init(uint32 stretchFactor = 0, DReal minSize = 0.0f) {
			stretch = stretchFactor;
			minimumSize = sizeHint = minSize;
			maximumSize = DGLAYOUTSIZE_MAX;
			expansive = false;
			empty = true;
			spacing = 0;
		}
		// ������һ������ȡֵ�Ľӿ�.- -
		DReal	smartSizeHint() 
		{
			return (stretch > 0) ? minimumSize : sizeHint;
		}
		uint32	effectiveSpacer(uint32 uniformSpacer) const 
		{
			assert(uniformSpacer >= 0 || spacing >= 0);
			return (uniformSpacer >= 0) ? uniformSpacer : spacing;
		}
		// properties
		uint32	stretch;
		DReal	sizeHint;
		DReal	maximumSize;
		DReal	minimumSize;
		bool	expansive;	// ����: ��չ��
		bool	empty;
		uint32	spacing;	// ����󷽵ļ����С.

		// temporary storage
		bool	done;

		// result
		DReal	pos;
		DReal	size;
	};

	// just a collection of static functions, used in calculation of item size.
	class DUELGUI_API DGLayoutEngine
	{
	public:
		// used to generate a proper size.
		static DGSize	smartMinSize(DGWidgetItem* item);
		static DGSize	smartMinSize(DGWidget* w);
		static DGSize	smartMinSize(const DGSize& sizeHint,
			const DGSize& minSize, const DGSize& maxSize,
			const DGSizePolicy& sizePolicy);
		static DGSize	smartMaxSize(DGWidgetItem* item, DGAlignment alignment);
		static DGSize	smartMaxSize(DGWidget* w, DGAlignment alignment);
		static DGSize	smartMaxSize(const DGSize& sizeHint,
			const DGSize& minSize, const DGSize& maxSize,
			const DGSizePolicy& sizePolicy, DGAlignment alignment);

		
		/*
		  This is the main workhorse of the DGGridLayout. It portions out
		  available space to the chain's children.

		  The calculation is done in fixed point: "fixed" variables are
		  scaled by a factor of 256.

		  If the layout runs "backwards" (i.e. RightToLeft or Up) the layout
		  is computed mirror-reversed, and it's the caller's responsibility
		  do reverse the values before use.

		  chain contains input and output parameters describing the geometry.
		  count is the count of items in the chain; pos and space give the
		  interval (relative to parentWidget topLeft).
		*/
		/*	comment:
			qGeomCalc()�����С��Ҫ��Ϊ������������еģ�������Ҳ���������Ŀ��ƹ�������������£�
			1����������ܿռ��С < �����ӿؼ���minimumSize�ܺ�
			��ʱ������ݿؼ�size�ɴ�С��˳�򣬼�С�ؼ���size��
			2����������ܿռ��С < �����ӿؼ���smartSizeHint�ܺ�
			��ʱ�����ȱ�֤����̶���С�Ŀؼ���minimumSize��Ȼ����ͨ����һ��ƽ���ؼ�С���ؼ���smartSizeHint()���õ�sizeֵ��
			3����������ܿռ��С > �����ӿؼ���smartSizeHint���ܺͣ�������Ҫ��չ�ӿؼ���С��
			��ʱ�� �������úù̶���С���ӿؼ���Ȼ����ƽ���ط���ʣ�µ��ӿؼ��Ĵ�С���ٿ�ƽ�������С����ӿؼ��Ƿ��ܹ���������(����smartSizeHint ��С��maximumSize)���ҽ��е���

			����˵��, chain��Ϊ������, start, count ��Ϊ��ʼ��ͼ���������, posΪ�������Կռ��е�, space�ǿ��ó���, spacerΪ������

		*/
		static void		calculateGeometry(std::vector<DGLayoutStruct> &chain, uint32 start, uint32 count,
					   DReal pos, DReal space, uint32 spacer);
		
		/*
		  Modify total maximum (max), total expansion (exp), and total empty
		  when adding boxmax/boxexp.

		  Expansive boxes win over non-expansive boxes.
		  Non-empty boxes win over empty boxes.
		  // ������ڼ������չ������, �����max�ǵ�ǰ���, boxmax�����ڶԱȵ����, ���boxmax���ڵ�ǰ���,
		  // �򽫵�ǰ���Ŵ�.
		*/
		static inline void	maxExpandingCalculation(DReal & max, bool &exp, bool &empty,
			DReal boxmax, bool boxexp, bool boxempty)
		{
			if (exp) 
			{
				if (boxexp)
				{
					max = DMath::Max<DReal>(max, boxmax);
				}
			}
			else 
			{
				if (boxexp || (empty && (!boxempty || DMath::realEqual(max, 0.0f))))
				{
					max = boxmax;
				}
				else if (empty == boxempty)
				{
					max = DMath::Min<DReal>(max, boxmax);
				}
			}
			exp = exp || boxexp;
			empty = empty && boxempty;
		}

	};

}


#endif