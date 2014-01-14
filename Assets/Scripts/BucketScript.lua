-- new script file
function OnAfterSceneLoaded(self)
  self.left = Vision.hkvVec3(0,-1,0)
  self.right = Vision.hkvVec3(0,1,0)
  self.direction = self.left
end

function OnExpose(self)
  self.minPosition = -800
  self.maxPosition = 800
  self.moveSpeed = 8
  self.bonusPoints = 500
end

function OnThink(self)
  --local step = self.moveSpeed * Timer:GetTimeDiff()
	if self:GetPosition().y < self.minPosition then
    self.direction = self.right
   elseif self:GetPosition().y > self.maxPosition then
    self.direction = self.left
   end
   
   self:IncPosition(self.direction * self.moveSpeed)
end

function OnObjectEnter(self, object)
	if (object:GetKey() == "GameBall") then
		G.HideBall(object)
		G.ballsRemaining = G.ballsRemaining + 1
		G.gameScore = G.gameScore + self.bonusPoints
	end
end


