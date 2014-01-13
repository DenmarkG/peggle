-- new script file
function OnAfterSceneLoaded(self)
	--create the map
  self.map = Input:CreateMap("PlayerInputMap")
  
  --map the triggers
  self.map:MapTrigger("RIGHT", "KEYBOARD", "CT_KB_RIGHT")
  self.map:MapTrigger("LEFT", "KEYBOARD", "CT_KB_LEFT")
  self.map:MapTrigger("FIRE01", "KEYBOARD", "CT_KB_SPACE", {once = true})
  
  --set the global variables
  G.ballInPlay = false
  G.EndRound = EndTheRound
  G.HideBall = HideTheBall
  G.gameScore = 0
  G.pegsHit = 0
  G.ballsRemaining = 3
  G.ballHeaven = Game:GetEntity("BallHeaven"):GetPosition()
end

function OnExpose(self)
  self.RotationSpeed = 50
  self.impulseScalar = 3
  self.ballBounce = 1.5
end

function OnBeforeSceneUnloaded(self)
	--destroy the map
  Input:DestroyMap(self.map);
end

function OnThink(self)
   if not (G.ballsRemaining > 0) then 
    return
  end

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
  
   --Show the score and balls remaining  
   Debug:PrintAt(0, 50, "Points: " .. G.gameScore, Vision.V_RGBA_YELLOW)
   Debug:PrintAt(0, 60, "Lives: " .. G.ballsRemaining, Vision.V_RGBA_YELLOW)
   Debug:PrintAt(0, 70, "NumPegsHit: " .. G.pegsHit, Vision.V_RGBA_YELLOW)
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
  pegParent = Game:GetEntity("PegParent")
  local numChildren = pegParent:GetNumChildren()
  for i = 0, pegParent:GetNumChildren() - 1, 1 do
    local peg = pegParent:GetChild(i)
    if peg ~= nil then
      local pegComp = peg:GetComponentOfType("SimplePeg")
	  if pegComp ~= nil then
		if pegComp:GetProperty("hitCount") > 0 then
			peg:GetComponentOfType("vHavokRigidBody"):SetActive(false)
			peg:SetVisible(false)
			G.pegsHit = G.pegsHit + 1
			--[[ can't get this part to work, using a workaround instead
			peg:DetachFromParent()
			--]]
		end
	  end
    end    
  end
  
  --if the number of balls left is = 0, then end the game,
  if G.pegsHit == numChildren - 1 then
	WinLevel()
  elseif G.ballsRemaining == 0 and not (G.pegsHit == numChildren) then
    LoseLevel()
  end
end 

function HideTheBall(ball)
  --get the rigid body on the ball, then set it to inactive
  local ballRB = ball:GetComponentOfType("vHavokRigidBody")
  ballRB:SetActive(false)
    
   --make the ball invisible
  --ball:SetVisible(false)
    
  ball:SetPosition(G.ballHeaven)
  
  --set the global ball in play var to false
  G.ballInPlay = false
  G.EndRound()
end

function WinLevel()
  --win if all pegs cleared
  Debug:PrintLine("You Win")
end

function LoseLevel()
	--lose if not all pegs cleared
	Debug:PrintLine("You Lose")
end

function ResetLevel(self)
	--make all pegs visible
	--reactivate the rigid bodies
	--set hit count back to 0
end