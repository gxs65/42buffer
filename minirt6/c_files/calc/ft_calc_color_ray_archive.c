
// Color for rays hitting nothing
	/* Via comparison between camera direction and ray direction
	if (ft_vec_sproduct(ray.dir, data->camera.dvec) < 0)
		ray.dir = ft_vec_neg(ray.dir);
	sproduct = 0.5 * (ft_vec_sproduct(ray.dir, data->camera.dvec) / 3) + 0.5;
	return (ft_vec_init(sproduct, sproduct, sproduct));
	*/
	/* Constant color
	(void)data;
	(void)ray;
	(void)sproduct;
	return (ft_vec_init(0.9, 0.9, 0.9));
	*/


/* Coloring sphere based on normal vector (pretty)
// Colors a ray crossing a sphere according to the normal vector
// 		/!\ the ray may cross the sphere from inside or outside,
// 			these cases may be distinguished by the sign of the dot product
// 				between the normal vector and the ray direction vector
t_vec	ft_color_ray_at_sphere(t_data *data, t_ray ray, t_object sphere, t_vec crosspoint)
{
	t_vec	normal_vec;

	(void)data;
	(void)ray;
	normal_vec = ft_vec_setnorm(ft_vec_substr(crosspoint, sphere.start), 1);
	if (ft_vec_sproduct(normal_vec, ray.dir) < 0)
		return (ft_vec_init(0.5 * (normal_vec.x + 1),
							0.5 * (normal_vec.y + 1),
							0.5 * (normal_vec.z + 1)));
	else
		return (ft_vec_init(1 - 0.5 * (normal_vec.x + 1),
							1 - 0.5 * (normal_vec.y + 1),
							1 - 0.5 * (normal_vec.z + 1)));
}
*/

// Lambertian sphere diffusing ray by uniform diffusion
	/*
	// Method 1 : uniform diffusion (diffusion vector randomly taken on the hemisphere around crosspoint)
	diffusion_vec = ft_vec_init_random_unit();
	if (ft_vec_sproduct(normal_vec, diffusion_vec) < 0)
		diffusion_vec = ft_vec_neg(diffusion_vec);
	*/