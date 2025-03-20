<entities> directory : files for the logic of scene object primitives, and the logic of texture mapping
Each entity has functions to do 3 things :
	- solve the the equation for the intersection between a ray and the entity
	- store the equation result and other information about the intersection
	  (normal, t, crosspoint...) in the <t_intersect> structure,
		and select the intersection with the smallest <t> if there are more than 1
	- if the entity has a texture : map the 3D crosspoint to a point in 2D,
		and also compute intersection's tangent and bitangent (will be used for bump mapping)
The texture mapping functions operate independantly of the object type :
	knowing the 2D point + tangent + bitangent, they retrieve the data in ppm textures
	to modify the intersection's color (albedo) and normal (= bump mapping)