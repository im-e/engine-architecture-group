patrolState = "backward"
pathState = "forward"

Path = {
	update = function(comp)
		if comp:numWaypoints() == 0 then
			index = comp:currentPathNum()
			if index == 0 then
				pathState = "forward"
			elseif index == comp:numPath() - 1 then
				pathState = "backward"
			end

			if pathState == "forward" then
				x = comp:pathPosX(index + 1)
				y = comp:pathPosY(index + 1)
				z = comp:pathPosZ(index + 1)
				comp:addWaypoint(x, y, z)
			end
	
			if pathState == "backward" then
				x = comp:pathPosX(index - 1)
				y = comp:pathPosY(index - 1)
				z = comp:pathPosZ(index - 1)
				comp:addWaypoint(x, y, z)
			end
		end
	end
}

Patrol = {
	update = function(comp)
		if comp:numWaypoints() == 0 then
			if patrolState == "backward" then
				patrolState = "forward"
				comp:addWaypoint(0.0, 0.0, 5.0)
			elseif patrolState == "forward" then
				patrolState = "backward"
				comp:addWaypoint(0.0, 0.0, -5.0)
			end
		end
	end
}

Wander = {
	update = function(comp)
		if comp:numWaypoints() == 0 then
			x = math.random(-5.0, 5.0)
			y = math.random(-5.0, 5.0)
			z = math.random(-5.0, 5.0)
			comp:addWaypoint(x, y, z)
		end
	end
}