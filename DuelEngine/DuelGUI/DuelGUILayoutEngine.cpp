//  [7/13/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUISize.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUILayoutEngine.h"

namespace Duel
{


	DGSize DGLayoutEngine::smartMinSize( DGWidgetItem* item )
	{
		return smartMinSize(item->getWidget());
	}

	DGSize DGLayoutEngine::smartMinSize( DGWidget* w )
	{
		return smartMinSize(w->getSizeHint(), w->getMinimumSize(),
			w->getMaximumSize(), w->getSizePolicy());
	}

	DGSize DGLayoutEngine::smartMinSize( const DGSize& sizeHint, const DGSize& minSize, const DGSize& maxSize, const DGSizePolicy& sizePolicy )
	{
		DGSize s(0, 0);

		if (sizePolicy.getHorizontalPolicy() != SP_Ignored)
		{
			if (sizePolicy.getHorizontalPolicy() & SPF_Shrink)
				s.setWidth(minSize.getWidth());
			else
				s.setWidth(DMath::Max<DReal>(sizeHint.getWidth(), minSize.getWidth()));
		}

		if (sizePolicy.getVerticalPolicy() != SP_Ignored) {
			if (sizePolicy.getVerticalPolicy() & SPF_Shrink) {
				s.setHeight(minSize.getHeight());
			} else {
				s.setHeight(DMath::Max<DReal>(sizeHint.getHeight(), minSize.getHeight()));
			}
		}

		s = s.boundedTo(maxSize);
		
		return s.expandedTo(DGSize(0.0f,0.0f));
	}

	DGSize DGLayoutEngine::smartMaxSize( DGWidgetItem* item, DGAlignment alignment )
	{
		return smartMaxSize(item->getWidget(), alignment);
	}

	DGSize DGLayoutEngine::smartMaxSize( DGWidget* w, DGAlignment alignment )
	{
		return smartMaxSize(w->getSizeHint(), 
			w->getMinimumSize(), w->getMaximumSize(),
			w->getSizePolicy(), alignment);
	}

	DGSize DGLayoutEngine::smartMaxSize( const DGSize& sizeHint, const DGSize& minSize, const DGSize& maxSize,
		const DGSizePolicy& sizePolicy, DGAlignment align )
	{
		if (align & AF_HorizontalMask && align & AF_VerticalMask)
			return DGSize(DGLAYOUTSIZE_MAX, DGLAYOUTSIZE_MAX);
		DGSize s = maxSize;
		DGSize hint = sizeHint.expandedTo(minSize);
		if (s.getWidth() == DGLAYOUTSIZE_MAX && !(align & AF_HorizontalMask))
			if (!(sizePolicy.getHorizontalPolicy() & SPF_Grow))
				s.setWidth(hint.getWidth());

		if (s.getHeight() == DGLAYOUTSIZE_MAX && !(align & AF_VerticalMask))
			if (!(sizePolicy.getVerticalPolicy() & SPF_Grow))
				s.setHeight(hint.getHeight());

		if (align & AF_HorizontalMask)
			s.setWidth(DGLAYOUTSIZE_MAX);
		if (align & AF_VerticalMask)
			s.setHeight(DGLAYOUTSIZE_MAX);
		return s;
	}

	void DGLayoutEngine::calculateGeometry(std::vector<DGLayoutStruct> &chain, uint32 start, uint32 count,
		DReal pos, DReal space, uint32 spacer)
	{
		//  [11/5/2013 OMEGA]
		// �ð���δ���ֻ����������... 

		// c = cumulative �ۼ�
		DReal cHint = 0.0f;
		DReal cMin = 0.0f;
		DReal cMax = 0.0f;
		uint32 sumStretch = 0;
		// sumSpacing�����ļ���ܺ�
		uint32 sumSpacing = 0;

		bool wannaGrow = false; // anyone who really wants to grow?
		//    bool canShrink = false; // anyone who could be persuaded to shrink?

		bool allEmptyNonstretch = true;
		// pendingSpacing��Ϊ����Ǹ�����ĺ�ʹ�õ�spacer, ���ڻ�ͷ��ȥ.
		int32 pendingSpacing = -1;
		uint32 spacerCount = 0;
		uint32 i;

		for (i = start; i < start + count; i++) 
		{
			DGLayoutStruct *data = &chain[i];

			// �����������done��Ϊ��ʼֵ �Ա��������.
			data->done = false;
			// �ۼӸ��ֳ����ܺ�.
			cHint += data->smartSizeHint();
			cMin += data->minimumSize;
			cMax += data->maximumSize;
			sumStretch += data->stretch;
			if (!data->empty) 
			{
				/*
				Using pendingSpacing, we ensure that the spacing for the last
				(non-empty) item is ignored.
				������ַ���, ��¼�������spacer, �ڲ�����һ�����ʱ����, �����ͱ�֤���һ��
				���ᱻ����.
				*/
				if (pendingSpacing >= 0)
				{
					sumSpacing += pendingSpacing;
					++spacerCount;
				}
				pendingSpacing = data->effectiveSpacer(spacer);
			}
			// ���grow��, һ������һ��Ԫ������չ��, ��ô����grow���ǳ�����.
			wannaGrow = wannaGrow || data->expansive || data->stretch > 0;
			// ����������嶼Ϊ��, ����������Ϊ��.
			allEmptyNonstretch = allEmptyNonstretch && !wannaGrow && data->empty;
		}

		// ����ռ�?
		DReal extraspace = 0.0f;

		// ���� ��һ�����, ������ռ䲻�㹻�����������.�����.
		if (space < cMin + sumSpacing) 
		{
			/*
			Less space than minimumSize; take from the biggest first
			��仰����˼�Ǵ�����Ԫ������ٳ���.
			*/

			DReal minSize = cMin + sumSpacing;

			// shrink the spacers proportionally
			// ��Сspacer
			if (spacer >= 0.0f) 
			{
				spacer = minSize > 0.0f ? (uint32)(spacer * space / minSize) : 0;
				sumSpacing = spacer * spacerCount;
			}
			// �洢��С���ȵ��б�.
			std::vector<DReal> list;

			for (i = start; i < start + count; i++)
			{
				list.push_back(chain[i].minimumSize);
			}
			// ��С��������, 
			std::stable_sort(list.begin(),list.end());

			// ʣ����ÿռ�, ���ܹ��Ŀռ��м�ȥspacer���ܳ�
			DReal space_left = space - sumSpacing;

			// ��С�����ʼ����, ������ʹ�ö��ٿռ�.
			DReal sum = 0.0f;
			uint32 idx = 0;
			DReal space_used = 0.0f;	// ���Ǹ��ݴ�ֵ.
			DReal current = 0;	// ��ǰ���Ե��������.
			while (idx < count && space_used < space_left)
			{
				current = list.at(idx);
				// ���Խ������С������ɽ�ȥ.
				space_used = sum + current * (count - idx);
				sum += current;
				++idx;
			}
			// ��id���ص����һ�������.
			--idx;
			// �����ֵ.
			DReal deficit = space_used - space_left;
			// ����ʹ�õ������.
			uint32 items = count - idx;
			/*
			* If we truncate all items to "current", we would get "deficit" too many pixels. Therefore, we have to remove
			* deficit/items from each item bigger than maxval. The actual value to remove is deficitPerItem + remainder/items
			* "rest" is the accumulated error from using integer arithmetic.
			
			��仰����˼��, �������������������� "��ǰ���" ���Ǹ�ֵ, �������ػ᲻����,
			�������Ǳ���ȥ����Щ����"maxval"�����, ʵ����Ҫ�Ƴ����� deficitPerItem + remainder/items
			���������õ��Ǹ�����, �����deficitPerItem �Ѿ����㹻�˵�.
			*/
			DReal deficitPerItem = deficit/items;
			//DReal remainder = deficit % items;	// �������Ҫ��. �ڸ������������, deficitPerItem�Ѿ���ÿ�����Ҫ����������.
			// �������ʹ�õ����ֵ.
			DReal maxval = current - deficitPerItem;

//			DReal rest = 0.0f;
			for (i = start; i < start + count; i++)
			{
				// ����������Ƕ�������һ��trick, ����ȡ��ʣ�µ�����������䵽ÿ��Ԫ����
// 				DReal maxv = maxval;
// 				rest += remainder;
// 				if (rest >= items) 
// 				{
// 					maxv--;
// 					rest-=items;
// 				}
				DGLayoutStruct *data = &chain[i];
				// ���������ֵ�����������maxval��.
				data->size = DMath::Min<DReal>(data->minimumSize, maxval);
				data->done = true;
			}
		} 
		// �ڶ������, �ռ�С���������ռ�. ��������Сֵ.
		else if (space < cHint + sumSpacing) 
		{
			/*
			Less space than smartSizeHint(), but more than minimumSize.
			Currently take space equally from each, ��ÿ�������ȡ����ͬ�Ŀռ�.
			Commented-out lines will give more space to stretchier
			items.
			*/
			// ���䲻����������
			uint32 n = count;
			DReal space_left = space - sumSpacing;
			// ͸֧�Ĳ���. 
			DReal overdraft = cHint - space_left;

			// first give to the fixed ones:
			for (i = start; i < start + count; i++) 
			{
				DGLayoutStruct *data = &chain[i];
				if (!data->done
					&& data->minimumSize >= data->smartSizeHint())
				{
						data->size = data->smartSizeHint();
						data->done = true;
						space_left -= data->smartSizeHint();
						n--;
				}
			}
			bool finished = n == 0;
			while (!finished)
			{
				finished = true;
				DReal deficitPerItem = 0.0f;

				// �����С�����𽥳����µ���������(fp_over/n)
				for (i = start; i < start+count; i++) 
				{
					DGLayoutStruct *data = &chain[i];
					if (data->done)
					{
						continue;
					}
					// ����ʹ��DReal��, ����ֻҪ��ÿ��������ٵĿռ�����������.
					// ���ﲻ�ᷢ����0�Ľ��.. ��Ϊһ���������һ��, ��Ȼ����n==1�����,
					// �ñ����ܹ�����overdraft/n��ֵ(�������, �򲻻��������minSize<space<sizeHint�����.
					n = n == 0 ? n : 1;
					deficitPerItem = overdraft / n; 

					data->size = data->smartSizeHint() - deficitPerItem;
					// ��������ʵ�ڲ�������(��СС����Сֵ), �����������.
					if (data->size < data->minimumSize) 
					{
						data->done = true;
						data->size = data->minimumSize;
						finished = false;
						// ��overdraft�м�ȥ���������������Ӱ��.
						overdraft -= data->smartSizeHint() - data->minimumSize;
						n--;
						break; // ���´�ͷ����.
					}
				}
			}
		}
		// ���������, �ռ�����������ռ�, ��ʱչ�����������չ��..
		else 
		{ 
			// extra space
			uint32 n = count;
			DReal space_left = space - sumSpacing;
			// first give to the fixed ones, and handle non-expansiveness
			// ��Ū����Щ�̶���󳤶ȵļһ�.
			for (i = start; i < start + count; i++) 
			{
				DGLayoutStruct *data = &chain[i];
				if (!data->done
					&& (data->maximumSize <= data->smartSizeHint()
					|| (wannaGrow && !data->expansive && data->stretch == 0)
					|| (!allEmptyNonstretch && data->empty &&
					!data->expansive && data->stretch == 0))) 
				{
					data->size = data->smartSizeHint();
					data->done = true;
					space_left -= data->size;
					sumStretch -= data->stretch;
					n--;
				}
			}
			extraspace = space_left;

			/*
			Do a trial distribution and calculate how much it is off.
			If there are more deficit pixels than surplus pixels, give
			the minimum size items what they need, and repeat.
			Otherwise give to the maximum size items, and repeat.

			Paul Olav Tvete has a wonderful mathematical proof of the
			correctness of this principle, but unfortunately this
			comment is too small to contain it.
			
			�����Ǿ仰����˼���ȷ���һ��, �������ʣ��Ĳ������ش��ڳ���������, ��minimumSize��
			������źÿռ�, Ȼ���ظ�, �������ЩmaximumSize�����, Ȼ���ظ�.
			Paul Olav Tvete�Դ�������ѧ��֤��, ��������İ���̫С��. д����ȥ.- -....
			*/
			
			DReal surplus, deficit;
			do {
				surplus = deficit = 0.0f;
// 				DReal fp_space = toFixed(space_left);
				DReal fp_w = 0.0f;
				for (i = start; i < start + count; i++) 
				{
					DGLayoutStruct *data = &chain[i];
					if (data->done)
					{
						continue;
					}
					extraspace = 0.0f;
					if (sumStretch = 0)
					{
						fp_w = space_left / n;
					}
					else
					{
						// ���������stretch���ӵ�����Ч��, �����stretch��Ϊ0,
						// ��ô������õ��Ŀռ伴Ϊ��stretch����stretch�е�Ȩ��.
						fp_w = (space_left * data->stretch) / sumStretch;
					}
//					DReal w = fRound(fp_w);
					data->size = fp_w;
// 					fp_w -= toFixed(w); // give the difference to the next
					if (fp_w < data->smartSizeHint()) 
					{
						deficit +=  data->smartSizeHint() - fp_w;
					} 
					else if (fp_w > data->maximumSize) 
					{
						surplus += fp_w - data->maximumSize;
					}
				}
				if (deficit > 0.0f && surplus <= deficit)
				{
					// give to the ones that have too little
					for (i = start; i < start+count; i++) 
					{
						DGLayoutStruct *data = &chain[i];
						if (!data->done && data->size < data->smartSizeHint()) 
						{
							data->size = data->smartSizeHint();
							data->done = true;
							space_left -= data->smartSizeHint();
							sumStretch -= data->stretch;
							n--;
						}
					}
				}
				if (surplus > 0.0f && surplus >= deficit) 
				{
					// take from the ones that have too much
					for (i = start; i < start + count; i++)
					{
						DGLayoutStruct *data = &chain[i];
						if (!data->done && data->size > data->maximumSize)
						{
							data->size = data->maximumSize;
							data->done = true;
							space_left -= data->maximumSize;
							sumStretch -= data->stretch;
							n--;
						}
					}
				}
			} while (n > 0 && surplus != deficit);

			if (n == 0)
			{
				extraspace = space_left;
			}
		}

		/*
		As a last resort, we distribute the unwanted space equally
		among the spacers (counting the start and end of the chain). We
		could, but don't, attempt a sub-pixel allocation of the extra
		space.
		// ������Ҫת������ϵ, ʹ���е㶨λ�����
		*/
		DReal extra = extraspace / (spacerCount + 2);
		DReal p = pos + extra - space/2;
		for (i = start; i < start+count; i++) 
		{
			DGLayoutStruct *data = &chain[i];
			p += data->size/2;
			data->pos = p;
			p += data->size/2;
			if (!data->empty)
			{ 
				// skip the empty box.
				p += data->effectiveSpacer(spacer) + extra;
			}
		}
	}

}