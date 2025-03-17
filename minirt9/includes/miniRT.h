/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 15:53:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
# define MINI_RT_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include "libft.h"
# include <mlx.h>
# include <math.h>

# include "structs.h"
# include "vector.h"
# include "render.h"
# include "parser.h"
# include "mlx_management.h"

# define LOGS 1
# define WIN_WIDTH 400
# define WIN_HEIGHT 225
# define NPIXELS WIN_HEIGHT * WIN_WIDTH

# define PI 3.1415926535
# define INFINITE_INT __INT_MAX__
# define INFINITE_DOUBLE __DBL_MAX__
# define MAX_RECURR_LVL 10

# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4
# define LIGHT 9

# define LAMBERTIAN 1
# define METAL 2

# define B_INTERSECTION 2
# define B_TEXTURES 4
# define B_COLORING 8
# define B_SHADOWS 16
extern int	g_log;

/*
= PLAN
+ implementer la conjonction entre
	\ recherche des lumieres -> coloration de base, hard shadows
	\ rayon aleatoire sur les lambertiens -> diffusion, soft shadows
+ implementer le modele de reflection de Phong (dont diffuse lighting)
+ incorporer ma version dans celle de Aleksei
+ implementer les lumieres colorees (dont alighting)
+ inclure les object cones (+ rectangles, parallelepipedes ?)
- implementer les surfaces miroir / fuzzy miroir
- implementer les textures + bumper maps
- petites corrections du code :
	\ typedef t_vec en t_color et t_point pour clarifier le code
	\ verifier les endroits ou on normalise pour eviter les redondances
	\ mettre une condition sur le diffuse lighting : ne se declenche qu'en dessous d'un seuil
	\ lister les valeurs fixees / parametres et leurs effets (ex epsilon de interval post-bounce)
	\ flag side/non-side dans la struct intersect pour pouvoir ne pas appliquer specular aux sides
	\ permettre des commentires plus faciles dans la scene
- tentatives d'optimisation :
	\ parallelisme
	\ hitbox hierarchisees en un arbre binaire
- features complexes :
	\ tone mapping
	\ refraction

= TESTS
- lumieres / camera a l'interieur des objets
- lumieres colorees
- changement de position et orientation de la camera
- soft shadows (en enlevant le diffuse lighting) :
	\ une sphere blanche collee a une sphere rouge
	\ une sphere blange dans l'ombre d'un rectangle
- sphere completement noire

= CHOSES BIZARRES A EXPLIQUER EN CORRECTION
- spheres deformees, d'autant plus quand place dans les coins du viewport
	-> phenomene normal lie a la perspective (corriger par notre cerveau quand l'oeuil percoit une image)
- pour rendre le alightning plus interessante que seulement "appliquer +0.1 partout",
	on fait a la place une moyenne entre alightning et lumiere cumulee jusqu'ici
		ce qui cree quand meme une sur-illumination sur les points proches de plusieurs objects
			car il y a bcp de bounces et donc bcp d'applications de cette moyenne
	(mais c'est moins visible que dans la version de base)

*/

#endif