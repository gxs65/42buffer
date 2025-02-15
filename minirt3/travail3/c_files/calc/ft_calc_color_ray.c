#include "../../h_files/ft_minirt.h"

/*
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


t_vec	ft_color_ray_no_intersect(t_data *data, t_ray ray)
{
	double	sproduct;

	/*
	if (ft_vec_sproduct(ray.dir, data->camera.dvec) < 0)
		ray.dir = ft_vec_neg(ray.dir);
	sproduct = 0.5 * (ft_vec_sproduct(ray.dir, data->camera.dvec) / 3) + 0.5;
	return (ft_vec_init(sproduct, sproduct, sproduct));
	*/


	sproduct = 0.5 * (ft_vec_sproduct(ray.dir, data->camera.dvec) / 3 + 1);
	return (ft_vec_init(sproduct, sproduct, sproduct));


	/*
	(void)data;
	(void)ray;
	(void)sproduct;
	return (ft_vec_init(0.9, 0.9, 0.9));
	*/
}

t_vec	ft_color_ray_at_sphere_lambertian(t_data *data, t_ray ray, t_object sphere, t_vec crosspoint)
{
	t_vec	color_components;
	t_vec	normal_vec;
	t_vec	diffusion_vec;

	(void)ray;
	normal_vec = ft_vec_setnorm(ft_vec_substr(crosspoint, sphere.start), 1);
	/* Method 1 : uniform diffusion (diffusion vector randomly taken on the hemisphere around crosspoint)
	diffusion_vec = ft_vec_init_random_unit();
	if (ft_vec_sproduct(normal_vec, diffusion_vec) < 0)
		diffusion_vec = ft_vec_neg(diffusion_vec);
	*/
	/* Method 2 : diffusion skewed towards normal (diffusion vector randomly taken on the unit sphere offset from crosspoint by normal)*/
	diffusion_vec = ft_vec_add(normal_vec, ft_vec_init_random_unit());
	if (ft_vec_is_infinitesimal(diffusion_vec)) // to avoid having normal_vec and random_unit_vector canceling one another and giving a null diffusion vector
		diffusion_vec = ft_vec_copy(normal_vec);
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, diffusion_vec)), sphere.albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_sphere_metal(t_data *data, t_ray ray, t_object sphere, t_vec crosspoint)
{
	t_vec	color_components;
	t_vec	normal_vec;
	t_vec	reflection_vec;

	normal_vec = ft_vec_setnorm(ft_vec_substr(crosspoint, sphere.start), 1);
	reflection_vec = ft_vec_symmetric_by_normal(ray.dir, normal_vec);
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, reflection_vec)), sphere.albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_sphere(t_data *data, t_ray ray, t_object sphere, t_vec crosspoint)
{
	t_vec	color_components;

	if (sphere.material == LAMBERTIAN)
		color_components = ft_color_ray_at_sphere_lambertian(data, ray, sphere, crosspoint);
	else
		color_components = ft_color_ray_at_sphere_metal(data, ray, sphere, crosspoint);
	return (color_components);
}


// Calculates the color of the intersection point between ray and object
// being given the index of the the first object the ray intersects
// 		and the ray scalar <t> at which this intersection happens
// <ind> at -1 means the ray intersects no object,
// 		in that case the given color is only determined by the ray direction
t_vec	ft_color_ray_at(t_data *data, int ind, t_ray ray, double t)
{
	t_vec		crosspoint;
	t_object	object;

	//printf("coloring ray : ");
	//ft_ray_display(ray, 1);
	if (ind == -1)
		return (ft_color_ray_no_intersect(data, ray));
	object = data->objects[ind];
	crosspoint = ft_ray_at(ray, t);
	if (object.type == SPHERE)
		return (ft_color_ray_at_sphere(data, ray, object, crosspoint));
	else
		return (ft_vec_init(0.5, 0.5, 0.5));
}
