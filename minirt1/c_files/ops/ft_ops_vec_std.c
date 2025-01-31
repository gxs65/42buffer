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

// Displays the given vector with or without a trailing newline
void	ft_vec_display(t_vec v, int newline)
{
	printf("(%f,%f,%f)", v.x, v.y, v.z);
	if (newline)
		printf("\n");
	else
		printf(" ");
}
