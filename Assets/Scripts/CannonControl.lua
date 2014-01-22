-- new script file
function OnAfterSceneLoaded(self)  
	if self == nil then
		Debug:Enable(true)

		--get the screen size
		G.width, G.height = Screen:GetViewportSize()
	  
		--set the font path
		G.fontPath = "Fonts/FineCollege_32.fnt"
	  
		--global function variables
		G.EndRound = EndTheRound
		G.HideBall = HideTheBall
		G.HidePeg = HideThePeg
		G.ShowPeg = ShowThePeg
		G.ResetBucket = ResetTheBucket
		
		--set some values for the gamestate
		G.playing = "PLAYING"
		G.lose = "LOSE"
		G.win = "WIN"
		G.gameState = G.playing
		
		G.ballHeaven = Game:GetEntity("BallHeaven"):GetPosition()
		G.bucket = Game:GetEntity("BallBucket")
		G.bucketStartPos = bucket:GetPosition()
	else
		--create the map
		self.map = Input:CreateMap("PlayerInputMap")
	  
		--map the triggers
		self.map:MapTrigger("RIGHT", "KEYBOARD", "CT_KB_RIGHT")
		self.map:MapTrigger("LEFT", "KEYBOARD", "CT_KB_LEFT")
		self.map:MapTrigger("FIRE01", "KEYBOARD", "CT_KB_SPACE", {onceperframe = true})
		self.map:MapTrigger("RESET", "KEYBOARD", "CT_KB_R", {once = true}) 
		
		--get and store the inital position
		self.startRot = self:GetOrientation()
		
		
		--to avoid issues, I'm going to take the necessary actions here, since there is no
		--ResetLevel(self)
		G.ballInPlay = false
		G.gameScore = 0
		G.pegsHit = 0
		G.ballsRemaining = self.initialBallCount
		G.goalScore = 1750
		G.gameState = G.playing
		
		self.on = true
	end
end

function OnBeforeSceneUnloaded(self)
	if self ~= nil then
		--destroy the map
		Input:DestroyMap(self.map)
		self.map = nil
	end
end

function OnExpose(self)
	self.RotationSpeed = 50
	self.impulseScalar = 3
	self.ballBounce = .75
	self.initialBallCount = 3
end

function OnThink(self)
	if (self.map:GetTrigger("RESET") > 0) then
		--ResetLevel(self)
		Toggle(self)
		ResetLevel(self)
	end

	--Show the 'UI'
	DisplayGameStats(self)
	
	
	if (G.ballsRemaining > 0) and (G.gameState == G.playing) then
		--create the variables for input
		local right = self.map:GetTrigger("RIGHT")
		local left = self.map:GetTrigger("LEFT")
		local fire = self.map:GetTrigger("FIRE01")
	  
		--check for input
		if (right~=0) or (left~=0) or (fire>0) then
			--fire the ball if the spacebar is pressed
			if (fire>0) and ( not G.ballInPlay) then
			  FireTheBall(self)
			end
			
			--create a local orientation variable to move cannon at the end of the frame
			local orientation = self:GetOrientation()
			
			--set the speed to move the cannon
			local step = self.RotationSpeed * Timer:GetTimeDiff()
			
			if(right>0) then
			  --move right
			  orientation.z = orientation.z + step
			elseif (left > 0) then
			  --move left
			  orientation.z = orientation.z - step
			end
			
			--set the new orientation
			orientation.z = LimitRotation(self, orientation)
			self:SetOrientation(orientation)
		end
	elseif (G.ballsRemaining == 0) and (G.gameState ~= G.playing)then
		DisplayStatusText(self)
		if(self.map:GetTrigger("FIRE01") > 0) then
			ResetLevel(self)
			G.gameState = G.playing
		end
	end

end

function LimitRotation(self, pOrientation)
  --limit the rotation to stay between 90 and -90
  local rotation = pOrientation.z
  if (rotation > 90) or (rotation < -90) then
    if (rotation > 90) then 
      rotation = 90
    elseif (rotation < -90) then
      rotation = -90
    end
  end
  return rotation
end

function FireTheBall(self)
  --find the spawn point
  local spawnObject = Game:GetEntity("BallSpawnPoint")
  
  --get the spawn position and the cannon position
  local spawnPos = spawnObject:GetPosition()
  
  --spawn the ball and add the rigidbody component
  self.gameBall = Game:CreateEntity(spawnPos, "VisBaseEntity_cl", "Models/MOD_GameBall.MODEL", "GameBall")
  local rigidBody = self.gameBall:AddComponentOfType("vHavokRigidBody")
  rigidBody:SetRestitution(self.ballBounce)

  --find the direction to apply the impulse
  local impulseVector = spawnPos - self:GetPosition()
  local length = 5000 - impulseVector:getLength()
  
  if(length < 1) then
    length = 1
  end
  
  rigidBody:SetActive(true)
  impulseVector:setLength(length)
  rigidBody:ApplyLinearImpulse(impulseVector * self.impulseScalar)
  
  G.ballsRemaining = G.ballsRemaining - 1
  G.ballInPlay = true
end

function EndTheRound()
	local pegParent = Game:GetEntity("PegParent")
	local numChildren = pegParent:GetNumChildren()
	for i = 0, pegParent:GetNumChildren() - 1, 1 do
		local peg = pegParent:GetChild(i)
		if peg ~= nil then
			local pegComp = peg:GetComponentOfType("SimplePeg")
			if pegComp ~= nil then
				local hasHitLimit = pegComp:GetProperty("m_hasHitLimit")
				local hitCount = pegComp:GetProperty("m_hitCount")
				if (not hasHitLimit) and (hitCount > 0) then
					HideThePeg(peg)
				end
			end
		end    
	end

  G.pegsHit = 0
  
  --if the number of balls left is = 0, then end the game,
	if G.ballsRemaining == 0 then
		if G.gameScore >= G.goalScore then
			G.gameState = G.win
		else
			G.gameState = G.lose
		end
	end
end 

function HideTheBall(ball)
  --get the rigid body on the ball, then set it to inactive
  local ballRB = ball:GetComponentOfType("vHavokRigidBody")
  ballRB:SetActive(false)
    
   --make the ball invisible
  ball:SetVisible(false)
    
  ball:SetPosition(G.ballHeaven)
  
  --set the global ball in play var to false
  G.ballInPlay = false
  G.EndRound()
end

function DisplayStatusText(self)
	if (G.gameState == G.win) then
		Debug:PrintAt( (G.width / 2) - 75, G.height / 2, "You Win!", Vision.V_RGBA_YELLOW, G.fontPath)
	elseif (G.gameState == G.lose) then
		Debug:PrintAt( (G.width / 2 - 75), G.height / 2, "You Lose!", Vision.V_RGBA_RED, G.fontPath)
	end
	
	Debug:PrintAt( (G.width / 2) - 200, G.height * 3 / 5, "Press Space to Play Again", Vision.V_RGBA_YELLOW, G.fontPath)
end

function DisplayGameStats(self)
	Debug:PrintAt(G.width * 1 / 10, G.height * 1 / 10, "Points: " .. G.gameScore, Vision.V_RGBA_YELLOW, G.fontPath)
	Debug:PrintAt(G.width * 1 / 10, G.height * 2 / 10, "Lives: " .. G.ballsRemaining, Vision.V_RGBA_YELLOW, G.fontPath)
	Debug:PrintAt(G.width * 2 / 3, G.height * 1 / 10, "Goal Score: " .. G.goalScore, Vision.V_RGBA_YELLOW, G.fontPath)
end

function HideThePeg(peg)
	peg:GetComponentOfType("vHavokRigidBody"):SetActive(false)
	peg:SetVisible(false)
end

function ShowThePeg(peg)
	peg:GetComponentOfType("vHavokRigidBody"):SetActive(true)
	peg:SetVisible(true)
	G.ResetPeg(peg)
end

function ResetLevel(self)
	--make all pegs visible, re-activate rigid bodies and Set Hit Count to 0
	local pegParent = Game:GetEntity("PegParent")
	
	for i = 0, pegParent:GetNumChildren() - 1, 1 do
		local peg = pegParent:GetChild(i)
		if peg ~= nil then
			ShowThePeg(peg)
		end
	end
	
	if (G.ballInPlay) then
		--reomove the ball from play
		local ball = Game:GetEntity("GameBall")
		HideTheBall(ball)
	end
	
	--reset the global game vars
	G.ballInPlay = false
	G.gameScore = 0
	G.pegsHit = 0
	G.ballsRemaining = self.initialBallCount
	G.goalScore = 1750
	G.gameState = G.playing
	
	G.ResetBucket()
	ResetCannon(self)
end

function ResetTheBucket()
	G.bucket:SetPosition(G.bucketStartPos)
end

function ResetCannon(self)
	self:SetOrientation(self.startRot)
end 



function Toggle(self)
	local ball = Game:GetEntity("TestPeg")
	ball:SetEffect(0, "Shaders/Library01", "NormalPeg.forward", "GlowSwitch=1")
--[[
	local pegParent = Game:GetEntity("PegParent")
	
	for i = 0, pegParent:GetNumChildren() - 1, 1 do
		local peg = pegParent:GetChild(i)
		if peg ~= nil then
			if self.on then
				peg:SetEffect(0, "Shaders/Library01", "NormalPeg.forward", "GlowSwitch=.5")
				self.on = false
			else
				peg:SetEffect(0, "Shaders/Library01", "NormalPeg.forward", "GlowSwitch=1")
				self.on = true
			end
		end
	end
	--]]
end