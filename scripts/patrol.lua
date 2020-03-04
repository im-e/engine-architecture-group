patrolState = "backward"

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