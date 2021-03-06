//  [6/7/2014 OMEGA] created

#ifndef _DUELD3D9GPUPROGRAMMANAGER_H_
#define _DUELD3D9GPUPROGRAMMANAGER_H_

#include "DuelD3D9Common.h"
#include "DuelGpuProgramManager.h"

namespace Duel
{
	class D3D9GpuProgramManger : public DGpuProgramManager
	{
		DUEL_DECLARE_RTTI(D3D9GpuProgramManger)
	protected:

		virtual DResource* createImpl( DResourceDescription* createParam );


	};

}


#endif