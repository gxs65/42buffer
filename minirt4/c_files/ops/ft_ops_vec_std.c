#include "../../h_files/ft_minirt.h"

// Functions performing operations on vectors (3 dimensions) :
// vectors are used both taken is parameters as value, and returned as value

// Returns a copy of the given vector
t_vec	ft_vec_copy(t_vec v)
{
	t_vec	res;

	res.x = v.x;
	res.y = v.y;
	res.z = v.z;
	return (res);
}

// Returns the <t_vec> instance with members defined by params
t_vec	ft_vec_init(double x, double y, double z)
{
	t_vec	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

// Returns the vector resulting from adding <v2> to <v1>
t_vec	ft_vec_add(t_vec v1, t_vec v2)
{
	t_vec	res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

// Returns the vector resulting from multiplying <v1> by -1
t_vec	ft_vec_neg(t_vec v)
{
	t_vec	res;

	res.x = -1 * v.x;
	res.y = -1 * v.y;
	res.z = -1 * v.z;
	return (res);
}

// Returns the vector resulting from substracting <v2> to <v1>
t_vec	ft_vec_substr(t_vec v1, t_vec v2)
{
	return (ft_vec_add(v1, ft_vec_neg(v2)));
}

// Returns the vector resulting from multiplying <v1> by scalar <a>
t_vec	ft_vec_scale(t_vec v, double a)
{
	t_vec	res;

	res.x = a * v.x;
	res.y = a * v.y;
	res.z = a * v.z;
	return (res);
}

t_vec	ft_vec_scale_by_vec(t_vec v1, t_vec v2)
{
	t_vec	res;

	res.x = v1.x * v2.x;
	res.y = v1.y * v2.y;
	res.z = v1.z * v2.z;
	return (res);
}

// Returns the scalar product of two vectors
double	ft_vec_sproduct(t_vec v1, t_vec v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// Returns the vector resulting from vector multiplications <v2> x <v1>
// (which is a vector orthogonal to both <v1> and <v2>)
t_vec	ft_vec_vproduct(t_vec v1, t_vec v2)
{
	t_vec	res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return (res);
}

// Returns the norm of a vector (its "length" for our purpose) squared
// (ie. before the square root was applied)
double	ft_vec_norm_sq(t_vec v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

// Returns the norm of a vector (its "length" for our purpose)
double	ft_vec_norm(t_vec v)
{
	return (sqrt(ft_vec_norm_sq(v)));
}

// Scales a vector so that its norm is now equal to <new_norm>
// /!\ normalizing a vector <=> setting its norm to 1 with this function
// /!\ reminder : ||scalar * v|| == scalar * ||v||
t_vec	ft_vec_setnorm(t_vec v, double new_norm)
{
	t_vec	res;
	double	old_norm;

	old_norm = ft_vec_norm(v);
	res = ft_vec_scale(v, new_norm / old_norm);
	return (res);
}

// In the case of an incident vector on a surface with normal vector <normal>,
// returns the vector reflected by the surface, ie. symmetric to <incident>
// if <normal> were the axis
// 		computed thanks to formula `reflected = incident - 2 * (normal . incident) * normal`
t_vec	ft_vec_symmetric_by_normal(t_vec incident, t_vec normal)
{
	return (ft_vec_substr(incident, ft_vec_scale(normal, ft_vec_sproduct(incident, normal) * 2)));
}

// Initiates a vector with the 3 coordinates set to random values
// within the interval [min, max] (max excluded)
t_vec	ft_vec_init_random_within(double min, double max)
{
	t_vec	res;

	res = ft_vec_init(ft_rand_double_within(min, max),
		ft_rand_double_within(min, max),
		ft_rand_double_within(min, max));
	return (res);
}

// Generates random vectors in the unit cube (all coordinates within [-1, 1])
// until it finds one that is within the unit sphere,
// 		this vector is scales to be a unit vector and then returned
// /!\ Check that <norm_squared> is sufficiently big, to avoid 0 division error 
t_vec	ft_vec_init_random_unit(void)
{
	t_vec	res;
	double	norm_squared;

	while (1)
	{
		res = ft_vec_init_random_within(-1, 1);
		norm_squared = ft_vec_norm_sq(res);
		if (norm_squared > 1e-20 && norm_squared < 1)
			return (ft_vec_scale(res, 1 / sqrt(norm_squared)));
	}
}

// Checks if the given vector is too small to be handled correctly
int	ft_vec_is_infinitesimal(t_vec v)
{
	if (v.x < 1e-20 && v.y < 1e-20 && v.z < 1e-20)
		return (1);
	else
		return (0);
}

// Displays the given vector with or without a trailing newline
void	ft_vec_display(t_vec v, int newline)
{
	printf("(%f,%f,%f)", v.x, v.y, v.z);
	if (newline)
		printf("\n");
	else
		printf(" ");
}

// Stores in <vec_dest> the vector described by string <s> of the form `x,y,z`
int	ft_vec_from_str(t_vec *vec_dest, char *s)
{
	char	**components;
	int		nb_components;

	components = ft_split(s, ',');
	if (!components)
		return (1);
	nb_components = 0;
	while (components[nb_components])
		nb_components++;
	if (nb_components != 3)
		return (1);
	vec_dest->x = ft_atof(components[0]);
	vec_dest->y = ft_atof(components[1]);
	vec_dest->z = ft_atof(components[2]);
	return (0);
}
