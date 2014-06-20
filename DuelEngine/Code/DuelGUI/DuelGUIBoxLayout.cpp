//  [8/7/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelGUIBoxLayout.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGBoxLayout, DGLayout);
	DUEL_IMPLEMENT_RTTI_1(DGVBoxLayout, DGBoxLayout);
	DUEL_IMPLEMENT_RTTI_1(DGHBoxLayout, DGBoxLayout);

	bool	isHorizontal(DGBoxLayout::Direction dir)
	{
		return dir == DGBoxLayout::D_LeftToRight || dir == DGBoxLayout::D_RightToLeft;
	}


	DGBoxLayout::DGBoxLayout( Direction dir, DGWidget* w /*= NULL*/ ) : 
		mDirection(dir),
		DGLayout(w),
		mbHasHFW(true),
		mbGeoDirty(true)
	{

	}

	void DGBoxLayout::setDirection( DGBoxLayout::Direction dir )
	{
		if (dir == mDirection)
		{
			return;
		}
		if (isHorizontal(mDirection) != isHorizontal(dir))
		{
			//swap around the spacers (the "magic" bits)
			//#### a bit yucky, knows too much.
			//#### probably best to add access functions to spacerItem
			//#### or even a QSpacerItem::flip()
			ItemList::iterator i, iend = mItemList.end();
			for (i = mItemList.begin(); i != iend; ++i) 
			{
				DGBoxLayoutItem *box = (*i);
				if (box->magic) 
				{
					DGSpacerItem* sp = box->item->getAs<DGSpacerItem>(false);
					if (sp) 
					{
						if (sp->getExpandingDirections() == GO_None /*No Direction*/) 
						{
							//spacing or strut
							DGSize s = sp->getSizeHint();
							sp->changeSize(s.getHeight(), s.getWidth(),
								isHorizontal(dir) ? SP_Fixed : SP_Minimum,
								isHorizontal(dir) ? SP_Minimum : SP_Fixed);

						}
						else
						{
							//stretch
							if (isHorizontal(dir))
								sp->changeSize(0, 0, SP_Expanding,
								SP_Minimum);
							else
								sp->changeSize(0, 0, SP_Minimum,
								SP_Expanding);
						}
					}
				}
			}
		}
		mDirection = dir;
		invalidate();
	}

	DGBoxLayout::Direction DGBoxLayout::getDirection() const
	{
		return mDirection;
	}

	void DGBoxLayout::addLayout( DGLayout* layout, uint32 stretch )
	{
		insertLayout(-1, layout, stretch);
	}

	void DGBoxLayout::addSpacerItem( DGSpacerItem* item )
	{
		insertSpacerItem(-1, item);
	}

	void DGBoxLayout::addStrut( DReal size )
	{
		DGLayoutItem *b;
		if (isHorizontal(mDirection))
		{
			b = new DGSpacerItem( 0.0f, size, SP_Fixed, SP_Minimum);
		}
		else
		{
			b = new DGSpacerItem( size, 0.0f, SP_Minimum, SP_Fixed);
		}

		DGBoxLayoutItem *it = new DGBoxLayoutItem(b);
		it->magic = true;
		mItemList.push_back(it);
		invalidate();
	}

	void DGBoxLayout::addWidget( DGWidget* w, int32 stretch /*= 0*/, DGAlignment align/* = AF_None*/ )
	{
		insertWidget(-1, w, stretch, align);
	}

	void DGBoxLayout::insertLayout( int32 index, DGLayout* layout, uint32 stretch /*= 0*/ )
	{
		addChildLayout(layout);
		ItemList::iterator ii = mItemList.begin();
		if (index < 0)
		{
			index = mItemList.size();
			ii = mItemList.end();
		}
		else
		{
			for (int32 i = 0; i < index; ++i)
			{
				if(ii == mItemList.end())
				{
					break;
				}
				++ii;
			}
		}
		DGBoxLayoutItem* box = new DGBoxLayoutItem(layout, stretch);
		mItemList.insert(ii, box);
		invalidate();
	}

	void DGBoxLayout::insertSpacerItem( int32 index, DGSpacerItem* item )
	{
		ItemList::iterator ii = mItemList.begin();
		if (index < 0)
		{
			index = mItemList.size();
			ii = mItemList.end();
		}
		else
		{
			for (int32 i = 0; i < index; ++i)
			{
				if(ii == mItemList.end())
				{
					break;
				}
				++ii;
			}
		}
		DGBoxLayoutItem* box = new DGBoxLayoutItem(item);
		box->magic = true;
		mItemList.insert(ii, box);
		invalidate();
	}


	void DGBoxLayout::insertWidget( int32 index, DGWidget* w, uint32 stretch /*= 0*/, DGAlignment align/* = AF_None*/ )
	{
		if (w == NULL)
		{
			return;
		}
		addChildWidget(w);
		ItemList::iterator ii = mItemList.begin();
		if (index < 0)
		{
			index = mItemList.size();
			ii = mItemList.end();
		}
		else
		{
			for (int32 i = 0; i < index; ++i)
			{
				if(ii == mItemList.end())
				{
					break;
				}
				++ii;
			}
		}
		DGWidgetItem* b = new DGWidgetItem(w);
		b->setAlignment(align);

		DGBoxLayoutItem *it = new DGBoxLayoutItem(b);
		mItemList.insert(ii, it);
		invalidate();
	}

	void DGBoxLayout::addItem( DGLayoutItem* item )
	{
		DGBoxLayoutItem *it = new DGBoxLayoutItem(item);
		mItemList.push_back(it);
		invalidate();
	}

	DGLayoutItem* DGBoxLayout::getItemAt( uint32 index ) const
	{
		if (index < 0 || index >= mItemList.size())
		{
			return NULL;
		}
		ItemList::const_iterator ii = mItemList.begin();
		for (uint32 i = 0; i < index; ++i)
		{
			++ii;
		}
		return (*ii)->item;
	}

	DGLayoutItem* DGBoxLayout::takeItemAt( uint32 index )
	{
		if (index < 0 || index >= mItemList.size())
		{
			return NULL;
		}
		ItemList::iterator ii = mItemList.begin();
		for (uint32 i = 0; i < index; ++i)
		{
			++ii;
		}
		DGLayoutItem* ret = (*ii)->item;
		delete *ii;
		mItemList.erase(ii);
		invalidate();
		return ret;
	}

	uint32 DGBoxLayout::getItemCount() const
	{
		return mItemList.size();
	}

	void DGBoxLayout::removeItem( DGLayoutItem* item )
	{
		ItemList::iterator i = mItemList.begin();
		for (; i != mItemList.end(); ++i)
		{
			if ((*i)->item == item)
			{
				delete (*i);
				mItemList.erase(i);
				invalidate();
			}
		}
	}

	void DGBoxLayout::removeWidget( DGWidget* w )
	{
		ItemList::iterator i = mItemList.begin();
		for (; i != mItemList.end(); ++i)
		{
			if ((*i)->item->getWidget() == w)
			{
				delete (*i);
				mItemList.erase(i);
				invalidate();
			}
		}
	}

	void DGBoxLayout::setGeometry( const DGGeometry& g )
	{
		if (mbDirty || mGeometry != g)
		{
			mGeometry = g;
			invalidate();
		}
	}

	GUIOrientation DGBoxLayout::getExpandingDirections() 
	{
		if (mbGeoDirty)
		{
			setupGeometry();
		}
		return mExpanding;
	}

	bool DGBoxLayout::hasHeightForWidth()
	{
		if (mbGeoDirty)
		{
			setupGeometry();
		}
		return mbHasHFW;
	}

	DGSize DGBoxLayout::getMinimumSize()
	{
		if (mbGeoDirty)
		{
			setupGeometry();
		}
		return mMinSize;
	}

	DGSize DGBoxLayout::getMaximumSize()
	{
		if (mbGeoDirty)
		{
			setupGeometry();
		}
		return mMaxSize;
	}

	DGSize DGBoxLayout::getSizeHint()
	{
		if(mbGeoDirty)
		{
			setupGeometry();
		}
		return mSizeHint;
	}


	void DGBoxLayout::deleteAllItems()
	{
		ItemList::iterator i, iend = mItemList.end();
		for (i = mItemList.begin(); i != iend; ++i)
		{
			delete *(i);
		}
		mItemList.clear();
	}

	void DGBoxLayout::invalidate()
	{
		if (mParentWidget && !mbDirty)
		{
			mParentWidget->requestLayoutUpdate();
		}
		mbDirty = true;
		mbGeoDirty = true;
	}

	/*
	Initializes the data structure needed by qGeomCalc and
	recalculates max/min and size hint.
	*/
	void DGBoxLayout::setupGeometry()
	{
		// �ۼƵ�����/�߶�. ���ݷ���ͬ���ò�ͬ�ĳ�ʼֵ. 
		DReal maxw = isHorizontal(mDirection) ? 0.0f : DGLAYOUTSIZE_MAX;
		DReal maxh = isHorizontal(mDirection) ? DGLAYOUTSIZE_MAX : 0.0f;
		// �ۼƵ���С��/�߶�, ���ݹ������������ۼ�
		DReal minw = 0.0f;
		DReal minh = 0.0f;
		// �ۼƵ���ѿ�/�߶�, ���ݹ������������ۼ�
		DReal hintw = 0.0f;
		DReal hinth = 0.0f;
		// �������չ, һ�����������ĳ��������չ, ��ô���layout�ͻ���չ.
		bool horexp = false;
		bool verexp = false;
		//
		mbHasHFW = false;
		uint32 n = mItemList.size();
		mGeoArray.clear();
		for (uint32 t = 0; t < n; ++t)
		{
			mGeoArray.push_back(DGLayoutStruct());
		}
		
		// widget��̶������С.
		uint32 fixedSpacing = getSpacing();
		int32 previousNonEmptyIndex = -1;

		// �ð��Ҿ�����ϸע��һ����δ���, ��Ȼ�Ժ�������
		// ����, ���setupGeometry�������ڳ�ʼ������������, ͨ������layout��������䶯��ʱ��
		// (�����и�����, ������ĳ��widget��Ϊ���ɼ���ʱ����ô��,�Ƿ���Ҫ���¼���?ʹ���ź�?)
		// ���ɵ�GeoArray��ʹ��LayoutEngine�ĺ������м���. �ó����Ľ��.
		// ���ڿؼ��Ĵ�С�����ǴӸ����ӵ�, ��˸�������������Ӷ���ʹ�õĿռ��С, ���Ƕ���һ����Layout
		// ������һ����Widget,ͬʱʹ����һ����Layout(ʹ��Layout���涨Widget��С��Constrain)�����,
		// ����Layout��Geometry���ܻᱻ��Layout����Ϊ�޷���������Widget���ݵĴ�С, ���ʱ����Layout
		// ���Ծܾ�ʹ�������������Geometry, �Ӷ�����"�ſ�"���ӵ�����, ��������������.
		ItemList::iterator itI = mItemList.begin();
		for (uint32 i = 0; i < n; ++i)
		{
			DGBoxLayoutItem* box = (*itI);
			DGSize max = box->item->getMaximumSize();
			DGSize min = box->item->getMinimumSize();
			DGSize hint = box->item->getSizeHint();
			GUIOrientation exp = box->item->getExpandingDirections();
			bool empty = box->item->isEmpty();

			// ���ֵ������¼ɶ? .. ����ֵ
			uint32 spacing = 0;
			if (!empty)
			{
				spacing = (previousNonEmptyIndex >= 0) ? fixedSpacing : 0;

				// �������ƶ� spacingӦ��ָ����󷽵Ŀռ��С. ���ĳ�����֮������(��ǰ��i)�ǿ�,
				// �򽫸������spacing��Ϊĳ���̶�spacing
				if (previousNonEmptyIndex >= 0)
				{
					mGeoArray[previousNonEmptyIndex].spacing = spacing;
				}
				previousNonEmptyIndex = i;
			}
			bool ignore = empty && box->item->getWidget(); // ignore hidden widgets
			bool dummy = true;

			// ���ݲ�ͬ�ĳ�������ͬ��ʽ�Ĵ���.
			if (isHorizontal(mDirection))
			{
				// ��չ��, ���õ�ÿһ��struct��,
				bool expand = (exp & GO_Horizontal || box->stretch > 0);
				horexp = horexp || expand;	// ����һ�������expand�����, ����Ŀ�ܶ���expand
				maxw += spacing + max.getWidth();	// �ۼ������. ��spacing��������.
				minw += spacing + min.getWidth();	// ��С���ͬ��.
				hintw += spacing + hint.getWidth();	// ��ѿ��ͬ��.

				// ��������ܿ��. 
				if (!ignore)
				{
					bool boxexp = (exp & GO_Vertical) != 0; // ��������չ�����.
					DGLayoutEngine::maxExpandingCalculation(
						maxh, verexp, dummy,
						max.getHeight(), boxexp, box->item->isEmpty());
				}
				// ������С�ܿ�Ⱥ�����ܿ��,�ܹ������������.
				minh = DMath::Max<DReal>(minh, min.getHeight());
				hinth = DMath::Max<DReal>(hinth, hint.getHeight());
			
				mGeoArray[i].sizeHint = hint.getWidth();
				mGeoArray[i].maximumSize = max.getWidth();
				mGeoArray[i].minimumSize = min.getWidth();
				mGeoArray[i].expansive = expand;
				mGeoArray[i].stretch = box->stretch ? box->stretch : box->hStretch();
			}
			else
			{
				// ����ͬ��.
				bool expand = (exp & GO_Vertical || box->stretch > 0);
				verexp = verexp || expand;
				maxh += spacing + max.getHeight();
				maxh += spacing + min.getHeight();
				hinth += spacing + hint.getHeight();
				if (!ignore)
				{
					bool boxexp = (exp & GO_Horizontal) != 0;
					DGLayoutEngine::maxExpandingCalculation(
						maxw, verexp, dummy,
						max.getWidth(), boxexp, box->item->isEmpty());
				}
				minw = DMath::Max<DReal>(minw, min.getWidth());
				hintw = DMath::Max<DReal>(hintw, hint.getWidth());

				mGeoArray[i].sizeHint = hint.getHeight();
				mGeoArray[i].maximumSize = max.getHeight();
				mGeoArray[i].minimumSize = min.getHeight();
				mGeoArray[i].expansive = expand;
				mGeoArray[i].stretch = box->stretch ? box->stretch : box->vStretch();
			}
			mGeoArray[i].empty = empty;
			mGeoArray[i].spacing = 0;   // might be be initialized with a non-zero value in a later iteration
			// ������Ե��׸�ʲô��..
			mbHasHFW = mbHasHFW || box->item->hasHeightForWidth();
		}
		// �����չ��.
		if (horexp && verexp)
		{
			mExpanding = GO_Both;
		}
		else if (!horexp && !verexp)
		{
			mExpanding = GO_None;
		}
		else
		{
			mExpanding = horexp ? GO_Horizontal : GO_Vertical;
		}

		mMinSize = DGSize(minw, minh);
		mMaxSize = DGSize(maxw, maxh).expandedTo(mMinSize);
		mSizeHint = DGSize(hintw, hinth).expandedTo(mMinSize).boundedTo(mMaxSize);

		DGSize extra((DReal)mMargin.getLeft() + (DReal)mMargin.getRight(), (DReal)mMargin.getTop() + (DReal)mMargin.getBottom());

		// �������յĴ�С.
		mMinSize += extra;
		mMaxSize += extra;
		mSizeHint += extra;
		mbGeoDirty = false;
	}

	void DGBoxLayout::update()
	{
		if (!mbEnabled)
		{
			return;
		}
		if (mbGeoDirty)
		{
			setupGeometry();
		}
		if (mbDirty)
		{
			// layoutEngine::CalculateGeometry.
			// ����layoutConstrain�Ĳ�ͬ����parentWidget���,
			// center�϶��ǲ�����, ֱ������Ϊ����center.
			DVector2 c = mGeometry.getCenter();
			DGSize containerSize = getSizeHint();
			DReal xOrigin;
			DReal yOrigin;
			DReal space = 0.0f;
			DReal wMax, hMax;
			if (mParentWidget)
			{
				switch(getSizeConstrain())
				{
				case LSC_Default:
					{
						DGSize minWidSize = mParentWidget->getMinimumSize();
						if (!(minWidSize.getWidth() > containerSize.getWidth() &&
							minWidSize.getHeight() > containerSize.getHeight()))
						{
							// the widget doesn't have a min size.
							// use widget's min size instead.
							minWidSize = getMinimumSize();
						}
						// calculate the final size.
						containerSize = containerSize.expandedTo(minWidSize).boundedTo(getMaximumSize());
						// make full use of available size.
						containerSize = containerSize.expandedTo(mGeometry.getSize());
					}
					break;
				case LSC_NoConstrain:
					containerSize = mGeometry.getSize();
					break;
				case LSC_Minimum:
					containerSize = containerSize.expandedTo(getMinimumSize());
					// make full use of available size.
					containerSize = containerSize.expandedTo(mGeometry.getSize());
					break;
				case LSC_Fixed:
					containerSize = getSizeHint();
					break;
				case LSC_Maximum:
					// make full use of available size.
					containerSize = containerSize.expandedTo(mGeometry.getSize());
					containerSize = containerSize.boundedTo(getMaximumSize());
					break;
				case LSC_MinimumMaximum:
					containerSize = containerSize.expandedTo(getMinimumSize()).boundedTo(getMaximumSize());
					break;
				}
				// �ڷŸ����.
				mParentWidget->setGeometry(DGGeometry(c, containerSize));
				yOrigin = (DReal)(mMargin.getBottom() - mMargin.getTop()) * 0.5f;
				xOrigin = (DReal)(mMargin.getLeft() - mMargin.getRight()) * 0.5f;
			}
			else	// û��parentWidget�����. ֻ��Ҫ��x,yԭ���Լ�space�ı�һ�¾ͺ�.
			{
				containerSize = mGeometry.getSize();
				yOrigin = (DReal)(mMargin.getBottom() - mMargin.getTop()) * 0.5f + mGeometry.getCenter().x;
				xOrigin = (DReal)(mMargin.getLeft() - mMargin.getRight()) * 0.5f + mGeometry.getCenter().y;
			}

			// ���������λ��.
			if (isHorizontal(mDirection))
			{
				wMax = space = containerSize.getWidth() - mMargin.getLeft() - mMargin.getRight();
				hMax = containerSize.getHeight() - mMargin.getTop() - mMargin.getBottom();
				DGLayoutEngine::calculateGeometry(mGeoArray, 0, mGeoArray.size(),
					xOrigin, space, mSpacing);
			}
			else
			{
				hMax = space = containerSize.getHeight() - mMargin.getTop() - mMargin.getBottom();
				wMax = containerSize.getWidth() - mMargin.getLeft() - mMargin.getRight();
				DGLayoutEngine::calculateGeometry(mGeoArray, 0, mGeoArray.size(),
					yOrigin, space, mSpacing);
			}

			// �������..
			ItemList::iterator ii, iiend = mItemList.end();
			GeometryArray::iterator gi, giend = mGeoArray.end();
			for (ii = mItemList.begin(), gi = mGeoArray.end();
				ii != iiend && gi != giend; ++ii, ++gi)
			{
				DReal w = 0.0f;
				DReal h = 0.0f;
				DReal x = xOrigin;
				DReal y = yOrigin;
				GUIOrientation exp = (*ii)->item->getExpandingDirections();
				switch (mDirection)
				{
				case Duel::DGBoxLayout::D_LeftToRight:
					{
						x = gi->pos;
						w = gi->size;
						h = (exp & GO_Vertical ? (*ii)->item->getSizeHint().getHeight() : hMax);
						h = DMath::Min<DReal>(h, hMax);
					}
					break;
				case Duel::DGBoxLayout::D_RightToLeft:
					{
						x = xOrigin - gi->pos;
						w = gi->size;
						h = (exp & GO_Vertical ? (*ii)->item->getSizeHint().getHeight() : hMax);
						h = DMath::Min<DReal>(h, hMax);
					}
					break;
				case Duel::DGBoxLayout::D_TopToBottom:
					{
						y = yOrigin - gi->pos;
						h = gi->size;
						w = (exp & GO_Horizontal ? (*ii)->item->getSizeHint().getWidth() : wMax);
						w = DMath::Min<DReal>(w, wMax);
					}
					break;
				case Duel::DGBoxLayout::D_BottomToTop:
					{
						y = gi->pos;
						h = gi->size;
						h = (exp & GO_Horizontal ? (*ii)->item->getSizeHint().getWidth() : wMax);
						w = DMath::Min<DReal>(w, wMax);
					}
					break;
				default:
					break;
				}
				(*ii)->item->setGeometry(DGGeometry(DVector2(x, y), DGSize(w, h)));
			}
		}
		mbDirty = false;
	}


	DGVBoxLayout::DGVBoxLayout( DGWidget* w /*= NULL*/ ) :
		DGBoxLayout(DGBoxLayout::D_TopToBottom, w)
	{

	}


	DGHBoxLayout::DGHBoxLayout( DGWidget* w /*= NULL*/ ) :
		DGBoxLayout(DGBoxLayout::D_LeftToRight, w)
	{

	}

}
