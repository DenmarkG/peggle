-- new script file

function OnAfterSceneLoaded(self)
	--get the vHavokRigidBody attached to this component
  self.rigidBody = self:GetComponentOfType("vHavokRigidBody")
  Vision.Assert(self.rigidBody ~= nil, "Rigid Body is Missing")
  
  self.pegComp = self:GetComponentOfType("SimplePeg")
  Vision.Assert(self.pegComp ~= nil, "Peg Component is Missing")
end

-- info fields: HitPoint, HitNormal, Force, RelativeVelocity,
--              ColliderType, ColliderObject (maybe nil)
function OnCollision(self, info)
  --check for the double hit
  
  if self.lastCollider == info.ColliderObject and Timer:GetTime() - self.lastCollision < 0.1 then
	return
  end
  
  --remember the collision time
  self.lastCollider = info.ColliderObject
  self.lastCollision = Timer:GetTime()
  
  local otherObj = info.ColliderObject
	if otherObj:GetKey() == "GameBall" then
		G.gameScore = G.gameScore + self.pegComp:GetProperty("pointValue")
	end
end
