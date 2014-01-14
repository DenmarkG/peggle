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
		--log update the global var to keep track of how many total hits this round
		G.pegsHit = G.pegsHit + 1
		
		
		--set the score multiplier
		if G.pegsHit == 3 then
			self.pegComp:SetProperty("m_scoreMultiplier", 2)
		elseif G.pegsHit == 4 then
			self.pegComp:SetProperty("m_scoreMultiplier", 3)
		elseif G.pegsHit == 5 then
			self.pegComp:SetProperty("m_scoreMultiplier", 4)
		elseif G.pegsHit >= 6 then
			self.pegComp:SetProperty("m_scoreMultiplier", 5)
		else
			self.pegComp:SetProperty("m_scoreMultiplier", 1)
		end
		
		--update the score
		G.gameScore = G.gameScore + (self.pegComp:GetProperty("m_pointValue") * self.pegComp:GetProperty("m_scoreMultiplier"))
	end
end
