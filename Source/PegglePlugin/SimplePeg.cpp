/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//  Basic Component Template
//  


#include "PegglePluginPCH.h"
#include "SimplePeg.h"

#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

const int MYCOMPONENT_VERSION_CURRENT = 1;

V_IMPLEMENT_SERIAL( SimplePeg, IVObjectComponent, 0, &g_myComponentModule);

SimplePeg_ComponentManager SimplePeg_ComponentManager::g_GlobalManager;

SimplePeg::SimplePeg()
{
	Vision::Message.reset();

	//initialize variables here
	m_hitCount = 0;
	m_scoreMultiplier = 1;
	m_pointValue = 100;
	m_hasHitLimit = false;
}

SimplePeg::~SimplePeg()
{
	//Empty Constructor
}

void SimplePeg::onStartup( VisTypedEngineObject_cl *pOwner )
{
	m_rigidBodyComponent = (vHavokRigidBody *)pOwner->Components().GetComponentOfType("vHavokRigidBody");
}


void SimplePeg::onRemove(  VisTypedEngineObject_cl *pOwner )
{
	//  do the component Removal code here.......
	//  [...]
	Vision::Message.reset();
}


void SimplePeg::onFrameUpdate()
{
	if (!GetOwner())return;
}


void SimplePeg::SetOwner( VisTypedEngineObject_cl *pOwner )
{
	IVObjectComponent::SetOwner( pOwner );
	if (pOwner!=NULL)
	{
		SimplePeg_ComponentManager::GlobalManager().Instances().AddUnique(this);
		onStartup( pOwner );
	}
	else
	{
		onRemove( pOwner );
		SimplePeg_ComponentManager::GlobalManager().Instances().SafeRemove(this);    
	}
}

bool SimplePeg::Hit() const
{
	return (m_hitCount > 0);
}

void SimplePeg::MessageFunction( int iID, INT_PTR iParamA, INT_PTR iParamB )
{
	if ( iID == VIS_MSG_HAVOK_ONCOLLISION )
	{
		vHavokCollisionInfo_t *collisionInfo = (vHavokCollisionInfo_t *)iParamA;

		//example of how to interact with the collider components
		//if (m_rigidBodyComponent)
		//{
		//	vHavokColliderInfo_t *info = &collisionInfo->m_Collider[0];

		//	if (info->m_pRigidBody == m_rigidBodyComponent)
		//		info = &collisionInfo->m_Collider[1];

		//	hkvVec3 newVelocity = collisionInfo->m_vNormal * -2.0f;
		//	info->m_pRigidBody->SetLinearVelocity(newVelocity);
		//}

		if(m_rigidBodyComponent)
		{
			//update the hitCount OnCollision
			++m_hitCount;
		}
	}
}

void SimplePeg::Serialize( VArchive &ar )
{
  char iLocalVersion = MYCOMPONENT_VERSION_CURRENT ;
  IVObjectComponent::Serialize(ar);
  if (ar.IsLoading())
  {
    ar >> iLocalVersion;
    VASSERT_MSG(iLocalVersion == MYCOMPONENT_VERSION_CURRENT , "Invalid local version.");

    //  add your property variables here
	//[...]
  } 
  else
  {
    ar << iLocalVersion;
    
    //  Save Data
	//[...]
  }
}


//============================================================================================================
//  Variable Table - Property variables can be exposed by the programmer 
//  and edited per instance by the artist  
//============================================================================================================
//
START_VAR_TABLE(SimplePeg,IVObjectComponent,    "The basic peg component for all pegs in the Peggle mini-game", 
                          VVARIABLELIST_FLAGS_NONE, 
                          "SimplePeg" )
//these variables will be viewable in the inspector and in Lua scripts using _:GetProperty("_")
DEFINE_VAR_INT  (SimplePeg, m_pointValue, "The point value of this peg", "100", 0, 0);
DEFINE_VAR_INT	(SimplePeg, m_scoreMultiplier, "The score multiplier when hit", "1", 0, 0);
DEFINE_VAR_INT	(SimplePeg, m_hitCount, "Hit Counter", "0", 0, 0);
DEFINE_VAR_BOOL (SimplePeg, m_hasHitLimit, "If true, the peg will disappear during the round after 3 hits", "FALSE", 0, NULL);

END_VAR_TABLE



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
