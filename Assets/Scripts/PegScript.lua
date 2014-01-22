
function OnAfterSceneLoaded(self)
	--get the vHavokRigidBody attached to this component
	self.rigidBody = self:GetComponentOfType("vHavokRigidBody")
	Vision.Assert(self.rigidBody ~= nil, "Rigid Body is Missing")
  
	self.pegComp = self:GetComponentOfType("SimplePeg")
	Vision.Assert(self.pegComp ~= nil, "Peg Component is Missing")
    
	self.maxHitCount = 3
	self.isActive = true
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
		--elseif (self.isActive) then
		--	Deactivate(self)
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
	--[[this section does not work
	local paramChanged = self:GetMesh():SetEffect("SurfaceTexture", "Shaders\Library01.ShaderLib", "GlowSwitch", "GlowSwitch=0")
	local mesh = self:GetMesh()
	local name = mesh:GetName()
	local surface = mesh:GetSurface(name)
	local wasParamChanged = mesh:SetEffect(name, "Shaders\Library01.ShaderLib", "GlowSwitch", "GlowSwitch=0")
	if(paramChanged) then
		Debug:PrintLine("changed")
	end
	--]]
	self.active = false
end