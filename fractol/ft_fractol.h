#ifndef FT_FRACTOL_H
# define FT_FRACTOL_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <mlx.h>
# include <math.h>
# include <X11/keysym.h>
# include <X11/X.h>

# define LOGS 1
# define WIN_WIDTH 20
# define WIN_HEIGHT 20
# define NPIXELS WIN_WIDTH * WIN_HEIGHT
# define MAX_ITER 100

// Representation of a point on the plane, can be interpreted
// 		- classically : eg. compute distance between 1 points (<dist> function)
// 		- in the complex plane : <x> real part, <y> imaginary part
typedef struct	s_point
{
	double	x;
	double	y;
}				t_point;

// Conveyer of data for MLX images dedicated to drawings on complex plane
// 	MLX IMAGES
// 		- img_ptr : a pointer to the image not being currently displayed
// 		- img_caches : the pointers to both images (displayed and modifiable)
// 		- which_img : the index of the inage not being currently displayed
// 	PIXEL REPRESENTATION
// 		- addr : the pointers to the addresses
// 				 of the first pixel in each of the 2 MLX image
// 		- bpp/opp : in how much bits/bytes a pixel is encoded
// 		- width/height : dimensions in pixels
// 		- line_length : space taken in memory by the data of pixels of a line
// 						= width * opp	(-> unit is bytes)
// 		- endian : here, ordering of the 4 bytes of ARGB
// 				   in the 4-bytes integers describing colors to MLX functions
// 	POSITION OF IMAGE ON COMPLEX PLANE
// 		- nw_coords : where the upperleft corner of the image should be
// 					  on the COMPLEX plane
//		- scale : how many units in the complex plane the image width covers
// 				  (eg. if set to 2 with width = 700pix, a segment of length 1
// 				   on the plane will be represented by 350 pixels)
// 	PARAMETERS FOR DRAWING FRACTALS
// 		- ftype : flag for wether the image is Julia (0) or Mandelbrot (1) set
// 		- depth : number of iterations before end of divergence check
// 		- threshold_sq : threshold to stay under to remain convergent,
// 						 already squared (to avoid sqrt when finding module)
// 		- c : constant parameter of Julia set
typedef struct	s_img {
	void				*img_ptr;
	void				*img_caches[2];
	int					which_img;
	char				*addr[2];
	int					height;
	int					width;
	int					bpp;
	int					opp;
	int					line_length;
	int					endian;
	t_point				nw_coords;
	int					ftype;
	double				scale;
	int					depth;
	double				threshold_sq;
	t_point				c;
	struct s_mlxdata	*data;
}				t_img;

// Conveyer of data for MLX window instances
typedef struct	s_mlxdata
{
    void	*mlx_ptr;
    void	*win_ptr;
	t_img	*img;
}				t_mlxdata;

// Cyclical linked list with double direction :
// 		- <prev> and <next> : links to other elements
// 		- <n> : number stored, <bin> : its form in base 2
typedef struct s_rlist
{
	struct s_rlist	*prev;
	struct s_rlist	*next;
	int				x;
	int				y;
}				t_rlist;


// Management of MLX images with intermediary <t_img>
void	new_image(void *mlx, t_img *img, int width, int height);
void	display_changed_image(t_mlxdata *data, t_img *img);
void	change_pixel(t_img *img, int x, int y, int color);
void	draw_black(t_img *img);

// MLX events
int		handle_destroy(t_mlxdata *data);
int		handle_keyrelease(int keysym, t_mlxdata *data);
int		handle_mouse(int button, int x, int y, t_mlxdata *data);

// Utils
int		ft_printf(int mode, const char *s, ...);
void	log_imgdata(t_img *img);
int		assemble_color(int a, int r, int g, int b);
char	**allocate_chararray_2dim(int width, int height);
int		is_on_edge(t_img *img, int x, int y);
void	draw(t_mlxdata *data);

// Fractal drawings
void	draw_mandelbrot1(t_img *img);
void	draw_mandelbrot2(t_img *img);
void	draw_mandelbrot3(t_img *img);

// Geometry on the plane and complex operations
double	dist(t_point *pa, t_point *pb);
void	add_complex(t_point *a, t_point *b, t_point *res);
void	multiply_complex(t_point *a, t_point *b, t_point *res);
void	apply_complex_qmap_inplace(t_point *p, t_point *c);
int		time_to_diverge(t_point *p, t_point *c, double threshold_sq, int max_iter);

// Cyclical linked list used to emulate a queue
t_rlist	*ft_lstnew(int x, int y);
void	ft_lstadd_front(t_rlist **lst, t_rlist *newelem);
void	ft_lstadd_back(t_rlist **lst, t_rlist *newelem);
void	ft_lstdelone(t_rlist *lst);
void	ft_lstclear(t_rlist *lst);
int		ft_lstsize(t_rlist *elem, t_rlist *orig, int passed_first);

#endif


/* TODO

= ROBUSTESSE ET RESPECT DES INSTRUCTIONS
- verifier proportionnalite et scale au moment du zoom
	\ tester avec fenetre carree
	\ tester avec fenetre rectangulaire w>h et h>w
- fonctions de libft a inclure :
	\ atoi
- valgrind
	\ protection et liberation des mallocs
	\ proteger les appels aux fonctions MLX (if (!pointer))
	\ free les pointeurs renvoyes par fonctions MLX

= EVENEMENT
+ gerer l'evenement fermeture de fenetre
+ gerer l'evenement deplacement par keyboard	(/!\ -> conservation en memoire)
+ gerer l'evenement "zoom"
+ gerer l'evenement clic (avoir les coordonnees du complex clique)
+ gerer l'evenement "augmenter depth" (pas automatique, pour des raisons de perf)
- gerer l'evenement "afficher log"
- gerer l'evenement "revenir a situation initiale"
- gerer l'evenement "changer les parametres de julia"

// COLORISATION
- choix entre
	\ coloration en modulo
		-> un point du plan complexe garde toujours la meme couleur qqe soit le niveau de zoom
	\ coloration normalisee sur le min-max d'iterations du niveau de zoom considere
		-> pas de repetition des couleurs pour differents niveaus d'iterations
- coloration par histogram -> independance au niveau de profondeur

// OPTIMISATION
- optimisation par la conservation en memoire avant deplacement
- optimisation par l'algo de plotting
	+ exploration recursive des pixels (possible sur les sets connexes et pleins)
	+ safeguard cet algorithme en parcourant au moins tous les pixels du bord de l'image
	+ reduction des operations "*" pendant application iteree de la fonction
	+ retirer utilisation de "pow" de matlib
	\ derivative bailout ?
	\ border following AUSSI pour les bandes hors du set ?
	\ interpolation pour des niveaux de zoom intermediaires ?

// FONCTIONNALITES BONUS
- selon la suggestion du post zenn, pour garantir de toujours repondre aux evenements utilisateurs,
	\ ompter le nombre d'operations realisees et arreter le calcul des que ca depasse
		un montant correspondant a un temps de X dixiemes de secondes
			(avec les resultats temporaires gardes en stock pour continuer au prochain tour de mlx_loop)
	\ permettant aussi, une fois une image completement calculee,
			de recommencer SEULEMENT sur les pixels noirs (pas divergents) avec une profondeur plus elevee
					<=> on a un rendu provisoire puis plus precis
- coloration sans bande de couleur
	(comme ca requiert de ne pas utiliser le border rendering, adapter <draw>
	pour qu'elle utilise la bonne fonction de <draw_mandelbrot>)
- option pour explorer Mandelbrot et afficher le Julia correspondant
- autre fractale (burning ship ?)

// PRE-COMMIT
- Norminette
- retirer minilibx
*/

/* AUTRES

- commentaires interessants sur le salon discord Fractol :
	\ 23/07/2024
	\ 15/08/2024
	\ 09/09/2023
	\ 08/03/2024 13:03, sur le salon General (erreurs dans la Minilibx)

- a discuter :
	\ remainder de minitalk : devanagari, lecture droite a gauche, unicode
	\ methode recursive et stackoverflow
	\ methode recursive de trace complet des frontieres

*/
