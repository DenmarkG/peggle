
function OnAfterSceneLoaded(self)
	--get the vHavokRigidBody attached to this component
	self.rigidBody = self:GetComponentOfType("vHavokRigidBody")
	Vision.Assert(self.rigidBody ~= nil, "Rigid Body is Missing")
  
	self.pegComp = self:GetComponentOfType("SimplePeg")
	Vision.Assert(self.pegComp ~= nil, "Peg Component is Missing")
    
	self.maxHitCount = 3
	
	G.ResetPeg = Reset
end

-- info fields: HitPoint, HitNormal, Force, RelativeVelocity,
--              ColliderType, ColliderObject (maybe nil)
function OnCollision(self, info)
	local time = Timer:GetTime()
	local otherObj = info.ColliderObject

	--check for the double hit
	if self.lastCollider == otherObj and time - self.lastCollision < 0.1 then
		return
	end

	--remember the collision time
	self.lastCollider = otherObj
	self.lastCollision = time

	if otherObj:GetKey() == "GameBall" then
		--log update the global var to keep track of how many total hits this round
		G.pegsHit = G.pegsHit + 1
		
        local hitCount = self.pegComp:GetProperty("m_hitCount")
        local hasHitLimit = self.pegComp:GetProperty("m_hasHitLimit")
		if hasHitLimit and (hitCount >= self.maxHitCount) then
			G.HidePeg(self)
		elseif (not hasHitLimit) then
			Deactivate(self)
		end
		
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

function Deactivate(self)
	--turn off the glow material to show it's been hit
	self:SetEffect(0, "Shaders/Library01", "NormalPeg.forward", "GlowSwitch=.5")
end

function Activate(peg)
	peg:SetEffect(0, "Shaders/Library01", "NormalPeg.forward", "GlowSwitch=1")
end

function Reset(peg)
	local pegComponent = peg:GetComponentOfType("SimplePeg")
	pegComponent:SetProperty("m_scoreMultiplier", 1)
	pegComponent:SetProperty("m_hitCount", 0)
	peg:SetTechnique(0, "Shaders/Library01", "NormalPeg.forward", "GlowSwitch=1")
end