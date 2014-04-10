//  [1/6/2013 OMEGA] created

#ifndef _DUELAUTOGPUPARAMETER_H_
#define _DUELAUTOGPUPARAMETER_H_

#include "DuelCommon.h"

namespace Duel
{
	// �����GPU��������ʹ����Щ����(��gpu������ʹ��ı�����ͬ��ȥ��ǰ׺֮����Զ�����, ��������һ������ matrix Auto_WorldMatrix; (�ǵ���"Auto_"ǰ׺.)
	// ����������renderable�ĸ����Զ�����ʱȡ����Ӧ��ֵ.
	// use specified gpu variable name can make the shader update the values automatically,
	// these values are written here, you can specified variables in shader like "matrix Auto_WorldMatrix;" to use this machinism.
	enum AutoGpuParameterName
	{
		AGPN_WorldMatrix				= 0x0001,	// in shader: Auto_WorldMatrix
		AGPN_InvWorldMatrix				= 0x0002,	// in shader: Auto_InvWorldMatrix

		AGPN_ViewMatrix					= 0x0004,	// in shader: Auto_ViewMatrix
		AGPN_InvViewMatrix				= 0x0008,	// in shader: Auto_InvViewMatrix

		AGPN_ProjMatrix					= 0x0010,	// in shader: Auto_ProjMatrix
		AGPN_InvProjMatrix				= 0x0020,	// in shader: Auto_InvProjMatrix

		AGPN_ViewProjMatrix				= 0x0040,	// in shader: Auto_ViewProjMatrix
		AGPN_InvViewProjMatrix			= 0x0080,	// in shader: Auto_InvViewProjMatrix

		AGPN_WorldViewMatrix			= 0x0100,	// in shader: Auto_WorldViewMatrix
		AGPN_InvWorldViewMatrix			= 0x0200,	// in shader: Auto_InvWorldViewMatrix

		AGPN_WorldViewProjMatrix		= 0x0400,	// in shader: Auto_WorldViewProjMatrix
		AGPN_InvWorldViewProjMatrix		= 0x0800	// in shader: Auto_InvWorldViewProjMatrix
	};
	
	// base class for defining auto gpu parameter filling regulation
	class DUEL_API DAutoGpuParameterDelegate : public DObject
	{
	DUEL_DECLARE_RTTI(DAutoGpuParameterDelegate)
	public:
		DAutoGpuParameterDelegate() {}
		virtual ~DAutoGpuParameterDelegate() {}

		virtual DMatrix4	getViewMatrix() = 0;
		virtual DMatrix4	getProjectionMatrix() = 0;
		virtual DMatrix4	getInverseViewMatrix() { return getViewMatrix().getInverse(); }
		virtual DMatrix4	getInverseProjectionMatrix() { return getViewMatrix().getInverse(); }
	private:

	};

	class DUEL_API DNullAutoGpuParameterDelegate : public DAutoGpuParameterDelegate
	{
	DUEL_DECLARE_RTTI(DNullAutoGpuParameterDelegate)
	public:
		DNullAutoGpuParameterDelegate();
		virtual ~DNullAutoGpuParameterDelegate();

		DMatrix4	getViewMatrix();
		DMatrix4	getProjectionMatrix();
		DMatrix4	getInverseViewMatrix();
		DMatrix4	getInverseProjectionMatrix();

	private:
		DCamera*	mCamera;
	};

	 /** This utility class is used to hold the information used to generate the matrices
    and other information required to automatically populate GpuProgramParameters.
    @remarks
        This class exercises a lazy-update scheme in order to avoid having to update all
        the information a GpuProgramParameters class could possibly want all the time. 
        It relies on the SceneManager to update it when the base data has changed, and
        will calculate concatenated matrices etc only when required, passing back precalculated
        matrices when they are requested more than once when the underlying information has
        not altered.
    */
	// ����಻���������������еĲ���, ��Ӧ��SceneManagerӦ������������.
	class DUEL_API DAutoGpuParameter : public DSingleton<DAutoGpuParameter>
	{
	public:
		DAutoGpuParameter();
		virtual ~DAutoGpuParameter();
		// if specified delegate is NULL, then this object will use mNullDelegate as its delegate.
		void	setParameterDelegate(DAutoGpuParameterDelegate* dele);
		DAutoGpuParameterDelegate*	getCurrentParameterDelegate();
		DMatrix4	getViewMatrix();
		DMatrix4	getProjectionMatrix();
		DMatrix4	getInverseViewMatrix();
		DMatrix4	getInverseProjectionMatrix();

	protected:
		DAutoGpuParameterDelegate*		mCurDelegate;
		DNullAutoGpuParameterDelegate	mNullDelegate;

	};

}
#endif