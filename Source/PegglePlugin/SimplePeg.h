/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */


//  Basic Component Template
//  look in the EngineSamples Examples90.sln -> VisionEnginePlugin90 -> components -> VFollowPathCamera.h
//
//  18/01/2011
//  PEGGLE_PLUGIN_IMPEXP prefix added + include to new MyEnginePluginModule.h
//

#ifndef SIMPLEPEG_H_INCLUDED
#define SIMPLEPEG_H_INCLUDED

#include "PegglePluginModule.h"

// Versions
#define SIMPLEPEG_VERSION_0          0     // Initial version
#define SIMPLEPEG_VERSION_CURRENT    1     // Current version

class vHavokRigidBody;

//=====================================================================================
//SIMPLE PEG CLASS
//=====================================================================================
class SimplePeg : public IVObjectComponent
{
public:
	V_DECLARE_SERIAL  ( SimplePeg, PEGGLE_PLUGIN_IMPEXP ); // for RTTI
	V_DECLARE_VARTABLE( SimplePeg, PEGGLE_PLUGIN_IMPEXP );

	PEGGLE_PLUGIN_IMPEXP SimplePeg();
	PEGGLE_PLUGIN_IMPEXP ~SimplePeg();

	PEGGLE_PLUGIN_IMPEXP VOVERRIDE void SetOwner( VisTypedEngineObject_cl *pOwner );

	PEGGLE_PLUGIN_IMPEXP VOVERRIDE BOOL CanAttachToObject( VisTypedEngineObject_cl *pObject, VString &sErrorMsgOut )
	{
		if(!IVObjectComponent::CanAttachToObject( pObject, sErrorMsgOut)) return FALSE;

		//Define criteria here that allows the editor to attach this component to 
		//  the passed object.In this example, the object should be derived from 
		//  VisObject3D_cl eg it can be positionable. 
		if(!pObject->IsOfType( V_RUNTIME_CLASS( VisObject3D_cl )))
		{
			sErrorMsgOut = "Component can only be added to instances of VisObject3D_cl or derived classes.";
			return FALSE;
		}

		return TRUE;
	}

	PEGGLE_PLUGIN_IMPEXP VOVERRIDE void OnVariableValueChanged(VisVariable_cl *pVar, const char * value){}

	//override the serialize function
	PEGGLE_PLUGIN_IMPEXP VOVERRIDE void Serialize( VArchive &ar );

	PEGGLE_PLUGIN_IMPEXP void onFrameUpdate();
	PEGGLE_PLUGIN_IMPEXP void onStartup( VisTypedEngineObject_cl *pOwner );
	PEGGLE_PLUGIN_IMPEXP void onRemove(  VisTypedEngineObject_cl *pOwner );
	
	PEGGLE_PLUGIN_IMPEXP virtual void MessageFunction(int iID, INT_PTR iParamA, INT_PTR iParamB) HKV_OVERRIDE;

	PEGGLE_PLUGIN_IMPEXP bool Hit() const;

protected:
	//define the variables to use as properties
	//Exposed to vForge:
	int m_pointValue;
	int m_hitCount;
	int m_scoreMultiplier;

	vHavokRigidBody *m_rigidBodyComponent;
};


//  Collection for handling playable character component
class SimplePeg_Collection : public VRefCountedCollection<SimplePeg> {};

//============================================================================================================
//  MyComponent_ComponentManager Class
//============================================================================================================
/// This manager class has a list of all available MyComponent instances
/// and takes care of calling their MyComponent::PerFrameUpdate function
/// on each frame.
class SimplePeg_ComponentManager : public IVisCallbackHandler_cl
{
public:
	//gets the singleton of the manager
	static SimplePeg_ComponentManager &GlobalManager(){ return g_GlobalManager; }

	void OneTimeInit(){  Vision::Callbacks.OnUpdateSceneFinished += this;} // listen to this callback

	void OneTimeDeInit(){ Vision::Callbacks.OnUpdateSceneFinished -= this;} // de-register

	VOVERRIDE void OnHandleCallback( IVisCallbackDataObject_cl *pData )
	{
		VASSERT( pData->m_pSender==&Vision::Callbacks.OnUpdateSceneFinished );
		// call update function on every component
		const int iCount = m_Components.Count();
		for (int i=0;i<iCount;i++){ m_Components.GetAt(i)->onFrameUpdate(); }
	}

	inline SimplePeg_Collection &Instances() { return m_Components; }

protected:
	/// Holds the collection of all instances of MyComponent
	SimplePeg_Collection m_Components;

	/// One global instance of our manager
	static SimplePeg_ComponentManager g_GlobalManager;
};


#endif
/*
 * Havok SDK - Base file, BUILD(#20131218)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
