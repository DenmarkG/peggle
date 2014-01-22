#include "PegglePluginPCH.h"
#include "SimplePeg.h"

#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>

const int SIMPLE_PEG_COMPONENT_VERSION_CURRENT = 1;

V_IMPLEMENT_SERIAL( SimplePeg, IVObjectComponent, 0, &g_peggleModule);

SimplePeg_ComponentManager SimplePeg_ComponentManager::g_GlobalManager;

SimplePeg::SimplePeg()
{
	Vision::Message.reset();

	// Initialize variables here
	m_hitCount = 0;
	m_scoreMultiplier = 1;
	m_pointValue = 100;
	m_hasHitLimit = false;
}

SimplePeg::~SimplePeg()
{
	//
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
		//if (m_rigidbodycomponent)
		//{
		//	vhavokcolliderinfo_t *info = &collisioninfo->m_collider[0];

		//	if (info->m_prigidbody == m_rigidbodycomponent)
		//		info = &collisioninfo->m_collider[1];

		//	//hkvvec3 newvelocity = collisioninfo->m_vnormal * -2.0f;
		//	//info->m_prigidbody->setlinearvelocity(newvelocity);
		//}

		if(m_rigidBodyComponent)
		{
			hkvLog::Info("HitCount Increased!");
			//update the hitCount OnCollision
			++m_hitCount;
		}
	}
}

void SimplePeg::Serialize( VArchive &ar )
{
	char iLocalVersion = SIMPLE_PEG_COMPONENT_VERSION_CURRENT ;
	IVObjectComponent::Serialize(ar);
	if (ar.IsLoading())
	{
		ar >> iLocalVersion;
		VASSERT_MSG(iLocalVersion == SIMPLE_PEG_COMPONENT_VERSION_CURRENT , "Invalid local version.");

		ar >> m_pointValue;
		ar >> m_scoreMultiplier;
		ar >> m_hitCount;
		ar >> m_hasHitLimit;
	} 
	else
	{
		ar << iLocalVersion;

		ar << m_pointValue;
		ar << m_scoreMultiplier;
		ar << m_hitCount;
		ar << m_hasHitLimit;
	}
}

START_VAR_TABLE(SimplePeg,IVObjectComponent,    "The basic peg component for all pegs in the Peggle mini-game", 
                          VVARIABLELIST_FLAGS_NONE, 
                          "SimplePeg" )
//these variables will be viewable in the inspector and in Lua scripts using _:GetProperty("_")
DEFINE_VAR_INT  (SimplePeg, m_pointValue, "The point value of this peg", "100", 0, 0);
DEFINE_VAR_INT	(SimplePeg, m_scoreMultiplier, "The score multiplier when hit", "1", 0, 0);
DEFINE_VAR_INT	(SimplePeg, m_hitCount, "Hit Counter", "0", 0, 0);
DEFINE_VAR_BOOL (SimplePeg, m_hasHitLimit, "If true, the peg will disappear during the round after 3 hits", "FALSE", 0, NULL);

END_VAR_TABLE