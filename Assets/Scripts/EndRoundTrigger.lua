-- new script file

function OnObjectEnter(self, object)
  if (object:GetKey() == "GameBall") then
		G.HideBall(object)
    end
end 