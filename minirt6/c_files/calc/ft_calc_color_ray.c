#include "../../h_files/ft_minirt.h"

// NO OBJECT INTERSECTED
t_vec	ft_color_ray_no_intersect(t_data *data, t_ray ray)
{
	double	sproduct;
	
	sproduct = 0.5 * (ft_vec_sproduct(ray.dir, data->camera.dvec) / 3 + 1);
	return (ft_vec_init(sproduct, sproduct, sproduct));
}

// INTERSECTED SPHERE

t_vec	ft_color_ray_at_sphere_lambertian(t_data *data, t_ray ray, t_object *sphere, t_vec crosspoint)
{
	t_vec	color_components;
	t_vec	normal_vec;
	t_vec	diffusion_vec;

	normal_vec = ft_vec_setnorm(ft_vec_substr(crosspoint, sphere->start), 1); // on a sphere, the normal vector depends on the crosspoint
	if (ft_vec_sproduct(ray.dir, normal_vec) > 0) // check if the normal vector and the ray direction point in the same way, which means the ray hits the spheres's inside
	{
		if (g_log)
			printf("\t\tRay intersects inside of sphere, normal inverted\n");
		normal_vec = ft_vec_neg(normal_vec);
	}
	
	// Method 2 : diffusion skewed towards normal (diffusion vector randomly taken on the unit sphere, then offset from crosspoint by normal)
	diffusion_vec = ft_vec_add(normal_vec, ft_vec_init_random_unit());
	if (ft_vec_is_infinitesimal(diffusion_vec)) // to avoid having normal_vec and random_unit_vector canceling one another and giving a null diffusion vector	
	{
		diffusion_vec = ft_vec_copy(normal_vec);
	}
	
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, diffusion_vec)), sphere->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_sphere_metal(t_data *data, t_ray ray, t_object *sphere, t_vec crosspoint)
{
	t_vec	color_components;
	t_vec	normal_vec;
	t_vec	reflection_vec;

	normal_vec = ft_vec_setnorm(ft_vec_substr(crosspoint, sphere->start), 1);
	if (ft_vec_sproduct(ray.dir, normal_vec) > 0) // check if the normal vector and the ray direction point in the same way, which means the ray hits the spheres's inside
	{
		if (g_log)
			printf("\t\tRay intersects inside of sphere, normal inverted\n");
		normal_vec = ft_vec_neg(normal_vec);
	}
	reflection_vec = ft_vec_symmetric_by_normal(ray.dir, normal_vec);
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, reflection_vec)), sphere->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_sphere(t_data *data, t_ray ray, t_object *sphere, t_vec crosspoint)
{
	t_vec	color_components;

	if (sphere->material == LAMBERTIAN)
		color_components = ft_color_ray_at_sphere_lambertian(data, ray, sphere, crosspoint);
	else
		color_components = ft_color_ray_at_sphere_metal(data, ray, sphere, crosspoint);
	if (g_log)
	{
		printf("\tColor return value of <color_ray_at_sphere> : ");
		ft_vec_display(color_components, 1);
	}
	return (color_components);
}

// INTERSECTED PLANE

t_vec	ft_color_ray_at_plane_lambertian(t_data *data, t_ray ray, t_object *plane, t_vec crosspoint)
{
	t_vec	color_components;
	t_vec	normal_vec;
	t_vec	diffusion_vec;

	normal_vec = plane->normal; // on a plane, the normal vector is always the same
	normal_vec = ft_vec_setnorm(normal_vec, 1);
	if (ft_vec_sproduct(ray.dir, normal_vec) > 0) // check if the normal vector and ray direction point in the same way, in which case we put normal vector in opposition
	{
		normal_vec = ft_vec_neg(normal_vec);
	}
	
	// Method 2 : diffusion skewed towards normal (diffusion vector randomly taken on the unit sphere, then offset from crosspoint by normal)
	diffusion_vec = ft_vec_add(normal_vec, ft_vec_init_random_unit());
	if (ft_vec_is_infinitesimal(diffusion_vec)) // to avoid having normal_vec and random_unit_vector canceling one another and giving a null diffusion vector	
	{
		diffusion_vec = ft_vec_copy(normal_vec);
	}
	
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, diffusion_vec)), plane->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_plane_metal(t_data *data, t_ray ray, t_object *plane, t_vec crosspoint)
{
	t_vec	color_components;
	t_vec	normal_vec;
	t_vec	reflection_vec;

	normal_vec = plane->normal;
	normal_vec = ft_vec_setnorm(normal_vec, 1);
	if (ft_vec_sproduct(ray.dir, normal_vec) > 0) // check if the normal vector and ray direction point in the same way, in which case we put normal vector in opposition
	{
		normal_vec = ft_vec_neg(normal_vec);
	}
	reflection_vec = ft_vec_symmetric_by_normal(ray.dir, normal_vec);
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, reflection_vec)), plane->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_plane(t_data *data, t_ray ray, t_object *plane, t_vec crosspoint)
{
	t_vec	color_components;

	if (plane->material == LAMBERTIAN)
		color_components = ft_color_ray_at_plane_lambertian(data, ray, plane, crosspoint);
	else
		color_components = ft_color_ray_at_plane_metal(data, ray, plane, crosspoint);
	if (g_log)
	{
		printf("\tColor return value of <color_ray_at_plane : ");
		ft_vec_display(color_components, 1);
	}
	return (color_components);
}

// INTERSECTED CYLINDER

t_vec	ft_color_ray_at_cylinder_lambertian(t_data *data, t_ray ray, t_object *cylinder, t_vec crosspoint)
{
	t_vec	color_components;
	double	axis_scalar;
	t_vec	axis_point;
	t_vec	normal_vec;
	t_vec	diffusion_vec;

	axis_scalar = ft_vec_sproduct(cylinder->normal, crosspoint) - ft_vec_sproduct(cylinder->normal, cylinder->start);
	axis_point = ft_vec_add(cylinder->start, ft_vec_scale(cylinder->normal, axis_scalar));
	normal_vec = ft_vec_substr(crosspoint, axis_point); // on a cylinder, the normal goes from axis to crosspoint
	normal_vec = ft_vec_setnorm(normal_vec, 1);
	if (ft_vec_sproduct(ray.dir, normal_vec) > 0) // check if the normal vector and the ray direction point in the same way, which means the ray hits the cy;inder's inside
	{
		normal_vec = ft_vec_neg(normal_vec);
	}
	
	// Method 2 : diffusion skewed towards normal (diffusion vector randomly taken on the unit sphere, then offset from crosspoint by normal)
	diffusion_vec = ft_vec_add(normal_vec, ft_vec_init_random_unit());
	if (ft_vec_is_infinitesimal(diffusion_vec)) // to avoid having normal_vec and random_unit_vector canceling one another and giving a null diffusion vector	
	{
		diffusion_vec = ft_vec_copy(normal_vec);
	}
	
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, diffusion_vec)), cylinder->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_cylinder_metal(t_data *data, t_ray ray, t_object *cylinder, t_vec crosspoint)
{
	t_vec	color_components;
	double	axis_scalar;
	t_vec	axis_point;
	t_vec	normal_vec;
	t_vec	reflection_vec;


	axis_scalar = ft_vec_sproduct(cylinder->normal, crosspoint) - ft_vec_sproduct(cylinder->normal, cylinder->start);
	axis_point = ft_vec_add(cylinder->start, ft_vec_scale(cylinder->normal, axis_scalar));
	normal_vec = ft_vec_substr(crosspoint, axis_point); // on a cylinder, the normal goes from axis to crosspoint
	normal_vec = ft_vec_setnorm(normal_vec, 1);
	if (ft_vec_sproduct(ray.dir, normal_vec) > 0) // check if the normal vector and the ray direction point in the same way, which means the ray hits the cy;inder's inside
	{
		normal_vec = ft_vec_neg(normal_vec);
	}
	reflection_vec = ft_vec_symmetric_by_normal(ray.dir, normal_vec);
	if (g_log)
	{
		printf("\t\tMetal cylinder at axis scalar %f -> incident vec, normal vec, reflection vec :\n", axis_scalar);
		ft_vec_display(ray.dir, 1);
		ft_vec_display(normal_vec, 1);
		ft_vec_display(reflection_vec, 1);
	}
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, ft_ray_init_by_dir(crosspoint, reflection_vec)), cylinder->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_cylinder(t_data *data, t_ray ray, t_object *plane, t_vec crosspoint)
{
	t_vec	color_components;

	if (plane->material == LAMBERTIAN)
		color_components = ft_color_ray_at_cylinder_lambertian(data, ray, plane, crosspoint);
	else
		color_components = ft_color_ray_at_cylinder_metal(data, ray, plane, crosspoint);
	if (g_log)
	{
		printf("\tColor return value of <color_ray_at_plane : ");
		ft_vec_display(color_components, 1);
	}
	return (color_components);
}

// DETERMINING WHAT TYPE OF OBJECT WAS INTERSECTED

// Calculates the color of the intersection point between ray and object
// being given reference to the first object the ray intersects
// 		and the ray scalar <t> at which this intersection happens
// <object> at NULL means the ray intersects no object,
// 		in that case the given color is only determined by the ray direction
t_vec	ft_color_ray_at(t_data *data, t_object *object, t_ray ray, double t)
{
	t_vec		crosspoint;

	if (!object)
		return (ft_color_ray_no_intersect(data, ray));
	crosspoint = ft_ray_at(ray, t);
	if (object->type == SPHERE)
		return (ft_color_ray_at_sphere(data, ray, object, crosspoint));
	if (object->type == PLANE)
		return (ft_color_ray_at_plane(data, ray, object, crosspoint));
	if (object->type == CYLINDER)
		return (ft_color_ray_at_cylinder(data, ray, object, crosspoint));
	else
		return (ft_vec_init(0.5, 0.5, 0.5));
}
