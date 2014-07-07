//  [5/23/2013 OMEGA] created

#ifndef _DUELMATERIAL_H_
#define _DUELMATERIAL_H_

#include "DuelCommon.h"
#include "DuelRenderState.h"

namespace Duel
{
	enum MaterialParameterType
	{
		MPT_Color,
		MPT_Texture,
		MPT_Sampler,
		MPT_Bool,
		MPT_Int1,
		MPT_Int2,
		MPT_Int3,
		MPT_Int4,
		MPT_Float1,
		MPT_Float2,
		MPT_Float3,
		MPT_Float4, 
		MPT_Matrix2x2,
		MPT_Matrix2x3,
		MPT_Matrix2x4,
		MPT_Matrix3x2,
		MPT_Matrix3x3,
		MPT_Matrix3x4,
		MPT_Matrix4x2,
		MPT_Matrix4x3,
		MPT_Matrix4x4
	};

	struct DMaterialParameter
	{
		MaterialParameterType	paramType;

		DString					paramName;
		// element size is the element count of specified parameter type, not the byte size.
		// for example, float4 has 4 elements, 
		uint32					elemSize;
		uint32					arraySize;	// for single parameter, this value also need to be 1.
		
		// physical index in the material, this value will be modified after added into material instance.
		uint32					physicalIndex;

		// what gpu param this param bound to
		DString					targetGpuParam;
		// what shader this parameter bound to.
		DString					targetGpuProgram;

		// whether this parameter is open to user.
		bool					bUserAccess;

		bool					isBool();
		static bool				isBool(MaterialParameterType type);
		bool					isInt();
		static bool				isInt(MaterialParameterType type);
		// 		bool					isBool();
		// 		static bool				isBool(MaterialParameterType type);
		bool					isFloat();
		static bool				isFloat(MaterialParameterType type);
		bool					isMatrix();
		static bool				isMatrix(MaterialParameterType type);
		bool					isColor();
		static bool				isColor(MaterialParameterType type);
		bool					isTexture();
		static bool				isTexture(MaterialParameterType type);
		bool					isSampler();
		static bool				isSampler(MaterialParameterType type);
		static uint32			getElementSize(MaterialParameterType type);
		const DString&			getParameterTypeName();
		// query the string for specified type.
		static const DString&	getParameterTypeName(MaterialParameterType type);
	protected:
		static std::map<MaterialParameterType, DString>	msParamNameMap;
	};

	// material instance is an access to specified material, since material has 
	// common actions, but their parameters can be varies according to the individual objects
	// so this class has the ability to preserve the parameters for each object,
	// objects can use DMaterialSystem::createMaterialInstance() to get a instance
	// with specified material name.
	class DUEL_API DMaterialInstance : public DObject
	{
		DUEL_DECLARE_RTTI(DMaterialInstance)
	public:
		DMaterialInstance(DMaterial* parent);

		DMaterial*	getParent() { return mParent; }
		typedef std::map<DString, DMaterialParameter>	ParameterMap;
		typedef MapIterator<ParameterMap>				ParameterIterator;
		ParameterIterator	getParameterIterator() { return ParameterIterator(mParamMap); }

		// for now, texture parameter only accept 1 element, meaning the arraySize will be automatically set to 1.
		// for non-texture parameter, the arraySize must be non-zero, otherwise it will be ignored.
		void	addParameter(const DString& name, const DString& gpuProgram, const DString& gpuParam,
			MaterialParameterType type,	uint32 arraySize = 1, bool openToUser = false);
		void	removeParameter(const DString& name);
		void	removeAllParameters();

		// set a bool constant to the program,
		void	setValue(const DString& name, bool val);
		// set a floating constant to the material,
		void	setValue(const DString& name, float val);
		// set a float array
		void	setValue(const DString& name, float* val, uint32 count);
		// set an int constant to the material,
		// this function also works for bool value.
		void	setValue(const DString& name, int val);
		// set an int array.
		// this function also works for bool value.
		void	setValue(const DString& name, int* val, uint32 count);
		// sets a DVector4 costant to the material.
		void	setValue(const DString& name, const DVector4& vec);
		// sets a DMatrix4 costant to the material.
		void	setValue(const DString& name, const DMatrix4& mat);
		// set a DMatrix4 array costant to the material.
		void	setValue(const DString& name, DMatrix4* mat, uint32 count);
		// set a color value constant to the material.
		void	setValue(const DString& name, const DColor& val);


		// set a GCT_Matrix2x2 value.
		void	setValue(const DString& name, const DVector2& m1_, const DVector2& m2_);
		// set a GCT_Matrix2x3 value.
		void	setValue(const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_);
		// set a GCT_Matrix2x4 value.
		void	setValue(const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_, const DVector2& m4_);

		// set a GCT_Matrix3x2 value.
		void	setValue(const DString& name, const DVector3& m1_, const DVector3& m2_);
		// set a GCT_Matrix3x3 value.
		void	setValue(const DString& name, const DMatrix3& mat);
		// set a GCT_Matrix3x4 value.
		void	setValue(const DString& name, const DVector3& m1_, const DVector3& m2_, const DVector3& m3_, const DVector3& m4_);

		// set a GCT_Matrix4x2 value.
		void	setValue(const DString& name, const DVector4& m1_, const DVector4& m2_);
		// set a GCT_Matrix4x3 value.
		void	setValue(const DString& name, const DVector4& m1_, const DVector4& m2_, const DVector4& m3_);

		// TextureConstant, use <groupName, resourcName> as pair.
		typedef std::pair<DString, DString>	TextureConstant;
		typedef std::pair<TextureConstant, DGpuTextureConstantPtr>	TextureConstantCache;
		// aserious of function overloading for setting values.
		void	setValue(const DString& paramName, TextureConstant tex);
		void	setValue(const DString& paramName, DTextureSampler samp);
		float*	getFloatValuePtr(uint32 physicalIndex);
		int32*	getIntValuePtr(uint32 physicalIndex);
		int32*	getBoolValuePtr(uint32 physicalIndex);
		// the constant cache allow you to speed up texture binding, 
		// when setting the texture value, this instance will query the resource manager
		// whether the resource is loaded, if so, the current loaded texture
		// will be cached, and the final texture used in shader will be that cached constant.
		TextureConstantCache	getTextureValue(const DString& paramName);
		typedef SetIterator<std::set<DLightSource*>>	LightIterator;
		DRenderTechnique*	getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li); 
		// use this function to map parameters into the gpu parameter.
		void	updateGpuParameter(DShaderObject* so);
		DTextureSamplerObjectPtr	getSamplerValue(const DString& paramName);
		// copy the parameter definitions and the values to another instance.
		// also the material parent will be reset to the current one.
		void	copyTo(DMaterialInstance* inst);

	protected:
		DMaterial*		mParent;
		ParameterMap	mParamMap;
		typedef	std::vector<float>	FloatConstantList;
		typedef	std::vector<int32>	IntConstantList;
		typedef std::vector<int32>	BoolConstantList;
		typedef std::map<DString, TextureConstantCache>	TextureConstantCacheMap;
		typedef std::map<DString, DTextureSamplerObjectPtr> SamplerConstantMap;
		FloatConstantList	mFloatConstants;
		IntConstantList		mIntConstants;
		BoolConstantList	mBoolConstants;
		TextureConstantCacheMap	mTextureMap;
		SamplerConstantMap	mSamplerMap;

	};   
	// ��ʵ��.. �������￴��, ����ϵͳ���Ƕ�Ӧһ��shader�ļ�, Ȼ��ָ��ʹ�õĲ����б��Լ���Ⱦ״̬,
	// Ȼ���ٰ󶨵�submesh��, �������ܽ�����������balabala������.
	// ���������м������˼·: һ��ʹ�ó��������������Ӳ���, Ȼ�����load��ʱ�򴴽��µĶ���,
	// ����ʹ�ó���Ĳ����б�, �����������ļ���Ϳ��Ա༭�øò���, Ȼ�󴫲ε�ʱ���ղ��������Ϳ�����.
	// д������Ļ�2���Ӻ�
	// �Ҹ�����ʹ�ú���...
	// д������Ļ�5���Ӻ�...
	// ��������ʹ�õ�һ��... ��Ϊ�ܶ���������Ӳ��ǿ��ļ����������ɵ�. �������ܰ����еĶ������������: ʲô��Ⱦ״̬ʲô�Ķ�Ҫ��.
	// fxxk.
	// ��������ʵʵʹ��factoryģʽ��... �ڻ���ֻҪ�������ò����ĽӿھͿ�����.

	/*
		<DMaterial 
		
	*/
	//  [2/5/2014 OMEGA]
	/*
		�ڿ�����defershading����Ⱦ�ܵ��Ժ�... �ҷ��ֲ����󶨵�����Ҫ����Ⱦ�������й�ϵ, ����˵������������pass�����Ŀ, ��Ҫ����
		pass�зֹ����ȵ�, ����������ʹ��Ӳ����ķ�ʽ��ʵ���������ϵͳ, ���Ҹ�ϵͳ��renderworkshopϢϢ���,
		��ô�õ�, ���ں˲���, ����ֻ�ṩע��/ע��material�ķ���, ���ҽ�material��Ϊ����, Ȼ����ʵ�ʵ���Ϸ�߼�������, ������
		�ڲ����, ʵ��һϵ�е�materialʵ��, ��ʵ�ְ󶨲����ĺ���, ����������һ��materialӦ�Բ�ͬ����Ⱦ����.
	*/
	// [2/6/2014 OMEGA]
	// ��ϸ������, material�Ķ�������Ҫ����Ⱦworkshop���, �����Ҫ��Ϊ����ṩ��renderworkshop�ṩ���п��õ�material 
	// base class of material, it can create MaterialInstance to hold the parameters
	// for individual object, and it knows which render technique should be used for 
	// different render context(implemented by sub-class).
	class DUEL_API DMaterial : public DObject
	{
		DUEL_DECLARE_RTTI(DMaterial)
	public:
		DMaterial(const DString& name);

		virtual ~DMaterial();

		const DString&		getMaterialName();

		// the texcoord index and its sampling targets.
		// for example, in shader we use texcoord1 for sampling diffuse color and normal
		// then the slot map will be {1, [diffuse,normal]}.
		// this feature provide information about input uv and its target shader property.
		typedef std::map<uint32, DStringVector>	UVSlotTargetMap;
		typedef MapIterator<UVSlotTargetMap>	UVSlotTargetIterator;
		UVSlotTargetIterator	getUVSlotIterator() { return UVSlotTargetIterator(mSlotMap); }

		void	addUVSlotTarget(uint32 index, const DString& slotName);
		void	setUVSlotTarget(uint32 index, const DStringVector& slotNames);
		const DStringVector&	getUVSlotTarget(uint32 index);
		void	removeUVSlotTarget(uint32 index);
		void	removeAllUVSlotTargets();

		// get technique with render context...�������������, defer/forward/reflect(water)�ȵ�...
		typedef SetIterator<std::set<DLightSource*>>	LightIterator;
		virtual DRenderTechnique*	getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li) = 0;

		// create an instance, which will be able to used in object.
		virtual DMaterialInstancePtr	createInstance();

		virtual	void		updateGpuParameter(DMaterialInstance* inst, DShaderObject* so);
	protected:
		DString				mMtlName;
		// copy of created instance.
		DMaterialInstance*	mPersistInstance;
		UVSlotTargetMap		mSlotMap;
	};

}

#endif