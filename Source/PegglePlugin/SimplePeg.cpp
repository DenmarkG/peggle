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
#include "myComponent.h"
#include "SimplePeg.h"

#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

V_IMPLEMENT_SERIAL( SimplePeg, IVObjectComponent, 0, &g_myComponentModule);

SimplePeg_ComponentManager SimplePeg_ComponentManager::g_GlobalManager;

SimplePeg::SimplePeg()
{
	Vision::Message.reset();
	//Vision::Message.Add(1,"SimplePeg - Class Constructor");

	//to do initialize variables here
	hitCount = 0;
	scoreMultiplier = 1;
	pointValue = 100;
}

SimplePeg::~SimplePeg()
{

}

void SimplePeg::onStartup( VisTypedEngineObject_cl *pOwner )
{
	m_rigidBodyComponent = (vHavokRigidBody *)pOwner->Components().GetComponentOfType("vHavokRigidBody");
	
	//m_rigidBodyComponent->SetRestitution()
}


void SimplePeg::onRemove(  VisTypedEngineObject_cl *pOwner )
{
	//  do the component Removal code here.......
	//  [...]
	Vision::Message.reset();
	//Vision::Message.Add("SimplePeg - onRemove()");
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
		//Vision::Message.Add("myComponent - Component Instance created");
		onStartup( pOwner );
	}
	else
	{
		onRemove( pOwner );
		//Vision::Message.Add("myComponent - Removed Component Instance from component Manager");
		SimplePeg_ComponentManager::GlobalManager().Instances().SafeRemove(this);    
	}
}

bool SimplePeg::Hit() const
{
	return (hitCount > 0);
}

void SimplePeg::MessageFunction( int iID, INT_PTR iParamA, INT_PTR iParamB )
{
	if ( iID == VIS_MSG_HAVOK_ONCOLLISION )
	{
		vHavokCollisionInfo_t *collisionInfo = (vHavokCollisionInfo_t *)iParamA;

		if (m_rigidBodyComponent)
		{
			vHavokColliderInfo_t *info = &collisionInfo->m_Collider[0];

			if (info->m_pRigidBody == m_rigidBodyComponent)
				info = &collisionInfo->m_Collider[1];

			hkvVec3 newVelocity = collisionInfo->m_vNormal * -2.0f;
			info->m_pRigidBody->SetLinearVelocity(newVelocity);
		}

		if(m_rigidBodyComponent)
		{
			++hitCount;
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
    //  Load Data
    //ar >> boolDemo;
    //ar >> floatDemo;
    //ar >> intDemo;
    //ar >> colorDemo;    
  } 
  else
  {
    ar << iLocalVersion;
    
    //  Save Data
    //ar << boolDemo;
    //ar << floatDemo;
    //ar << intDemo; 
    //ar << colorDemo;   
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

DEFINE_VAR_INT  (SimplePeg, pointValue, "The point value of this peg", "100", 0, 0);
DEFINE_VAR_INT	(SimplePeg, scoreMultiplier, "The score multiplier when hit", "1", 0, 0);
DEFINE_VAR_INT	(SimplePeg, hitCount, "Hit Counter", "0", 0, 0);

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
