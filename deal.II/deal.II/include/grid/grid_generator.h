//----------------------------  grid_generator.h  ---------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  grid_generator.h  ---------------------------
#ifndef __deal2__grid_generator_h
#define __deal2__grid_generator_h


#include <base/config.h>
#include <base/exceptions.h>
#include <base/point.h>
#include <map>


template <int dim> class Triangulation;
template <typename number> class Vector;
template <typename number> class SparseMatrix;


/**
 * This class offers triangulations of some standard domains such as hypercubes,
 * hyperball and the like. Following is a list of domains that can be generated
 * by the functions of this class:
 * @begin{itemize}
 *    @item Hypercube triangulations: a hypercube triangulation is a
 *       domain which is the tensor product of an interval $[a,b]$ in
 *       the given number of spatial dimensions. If you want to create such
 *       a domain, which is a common test case for model problems, call
 *       GridGenerator::hyper_cube(tria, a,b), which produces a
 *       hypercube domain triangulated with exactly one element. You
 *       can get tensor product meshes by successive refinement of
 *       this cell.
 *
 *       If you want the hypercube subdivided a certain number of
 *       times (and if this is not achievable by hierarchic
 *       refinement, for example 3 times), then the
 *       @p{subdivided_hyper_cube} function may be what you are
 *       looking for.
 *
 *    @item Rectangular coordinate-parallel domains as a generalization
 *      of hypercubes are generated by
 *      GridGenerator::hyper_rectangle (tria, p1, p2), with two
 *      opposite corner points @p{p1} and @p{p2}.
 *
 *    @item Rectangular coordinate-parallel domains with different numbers of
 *  	cells in each coordinate direction are generated by 
 *      GridGenerator::subdivided_hyper_rectangle
 *  	(tria, int[dim] repetitions, p1, p2, colorize), with two
 *      opposite corner points @p{p1} and @p{p2}.
 *
 *    @item Generalized L-shape domain:
 *      using the GridGenerator::hyper_L (tria, a,b) function produces
 *      the hypercube with the interval $[a,b]$ without the hypercube
 *      made out of the interval $[(a+b)/2,b]$. Let, for example, be $a=-1$
 *      and $b=1$, then the hpyer-L in two dimensions is the region
 *      $[-1,1]^2 - [0,1]^2$. To create a hyper-L in one dimension results in
 *      an error. The function is also implemented for three space dimensions.
 *
 *    @item Hyper ball:
 *      You get the circle or ball (or generalized: hyperball) around
 *      origin @p{p} and with radius @p{r} by calling
 *      GridGenerator::hyper_ball (tria, p, r). The circle is
 *      triangulated by five cells, the ball by seven cells. The
 *      diameter of the center cell is chosen so that the aspect ratio
 *      of the boundary cells after one refinement is minimized in
 *      some way. To create a hyperball in one dimension results in an
 *      error.
 *
 *      Do not forget to also attach a suitable boundary approximation object
 *      to the triangulation object you passed to this function if you later want
 *      the triangulation to be refined at the outer boundaries.
 *
 *    @item Half Hyper ball:
 *      You get half of the circle generated by Hyper ball.
 *      with center @p{p} and with radius @p{r} by calling
 *      GridGenerator::half_hyper_ball (tria, p, r). The half-circle is 
 *      triangulated by four cells. The diameter of the center cell is
 *      chosen to be the same as for the Hyper ball class. 
 *      To create a half-hyperball in one dimension results in
 *      an error.
 *
 *      Do not forget to also attach a suitable boundary approximation object
 *      to the triangulation object you passed to this function if you later want
 *      the triangulation to be refined at the outer boundaries. The class
 *      HalfHyperBallBoundary will provide a boundary object.
 *
 *    @item Hyper shell: A hyper shell is the region between two hyper
 *      sphere with the same origin. Therefore, it is a ring in two
 *      spatial dimensions. To triangulation it, call the function
 *      GridGenerator::hyper_shell (tria, origin, inner_radius, outer_radius, N),
 *      where the center of the spheres as well as
 *      the inner and outer radius of the two spheres are given as
 *      shown.
 *
 *      The parameter @p{N} denotes how many cells are to be used for
 *      this coarse triangulation. It defaults to zero, which tells
 *      the function to chose the number itself; this, then, is done
 *      such that the aspect ration of the resulting cells is as small
 *      as possible. However, it should be mentioned that this
 *      function does not work very well if the inner radius is much
 *      smaller than the outer radius since only one layer of cells is
 *      used in the radial direction.
 *
 *      You need to attach a boundary object to the triangulation. A
 *      suitable boundary class is provided as @ref{HyperSphereBoundary}
 *      in the library.
 *
 *    @item Half shells: This is a variant of the preceeding domain,
 *      where only one half of the shell is taken, defined by the first
 *      coordinate having non-negative values. This class is useful in
 *      computations with rotational symmetry, as then a 3d shell can be
 *      modeled by a 2d half shell.
 *
 *    @item Slit domain: The slit domain is a variant of the hyper cube
 *      domain. In two spatial dimensions, it is a square into which a slit
 *      is sawed; if the initial square is though to be composed of four
 *      smaller squares, then two of them are not connected even though
 *      they are neighboring each other. Analogously, into the cube in
 *      three spatial dimensions, a half-plane is sawed, disconnecting four
 *      of the eight child-cubes from one of their neighbors.
 * @end{itemize}
 *
 * Some of these functions receive a flag @p{colorize}. If this is
 * set, parts of the boundary receive different boundary numbers,
 * allowing them to be distinguished by application programs. See the
 * documentation of the functions for details.
 * 
 * Additionally this class provides a function
 * (@p{laplace_transformation}) that smoothly transforms a grid
 * according to given new boundary points. This can be used to
 * transform (simple-shaped) grids to a more complicated ones, like a
 * shell onto a grid of an airfoil, for example.
 *
 * @author Wolfgang Bangerth, Ralf Hartmann, Guido Kanschat, Stefan Nauber, Joerg Weimar, 1998, 1999, 2000, 2001, 2002, 2003.
 */
class GridGenerator
{
  public:
    				     /**
				      * Initialize the given
				      * triangulation with a hypercube
				      * (line in 1D, square in 2D,
				      * etc) consisting of exactly one
				      * cell. The hypercube volume is
				      * the tensor product of the
				      * intervall $[left,right]$ in
				      * the present number of
				      * dimensions, where the limits
				      * are given as arguments. They
				      * default to zero and unity,
				      * then producing the unit
				      * hypercube.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      */
    template <int dim>
    static void hyper_cube (Triangulation<dim> &tria,
			    const double        left = 0.,
			    const double        right= 1.);

    				     /**
				      * Same as @ref{hyper_cube}, but
				      * with the difference that not
				      * only one cell is created but
				      * each coordinate direction is
				      * subdivided into
				      * @p{repetitions} cells. Thus,
				      * the number of cells filling
				      * the given volume is
				      * @p{repetitions^d}.
				      */
    template <int dim>
    static void subdivided_hyper_cube (Triangulation<dim> &tria,
                                       const unsigned int  repetitions,
                                       const double        left = 0.,
                                       const double        right= 1.);

    				     /**
				      * Create a coordinate-parallel
				      * parallelepiped from the two
				      * diagonally opposite corner
				      * points @p{p1} and @p{p2}.
				      *
				      * If the @p{colorize} flag is
				      * set, the
				      * @p{boundary_indicator}s of the
				      * surfaces are assigned, such
				      * that the lower one in
				      * @p{x}-direction is 0, the
				      * upper one is 1. The indicators
				      * for the surfaces in
				      * @p{y}-direction are 2 and 3,
				      * the ones for @p{z} are 4 and
				      * 5.
				      * 
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      */
    template <int dim>
    static void hyper_rectangle (Triangulation<dim> &tria,
				 const Point<dim>   &p1,
				 const Point<dim>   &p2,
				 const bool          colorize = false);

				     /**
				      * Create a coordinate-parallel
				      * parallelepiped from the two
				      * diagonally opposite corner
				      * points @p{p1} and @p{p2}. In
				      * dimension @p{i},
				      * @p{repetitions[i]} cells are
				      * generated.
				      * 
				      * To get cells with an aspect
				      * ratio different from that of
				      * the recatangle, use different
				      * numbers of subdivisions in
				      * different coordinate
				      * directions. The minimum number
				      * of subdivisions in each
				      * direction is
				      * 1. @p{repetitions} is a list
				      * of integers denoting the
				      * number of subdivisions in each
				      * coordinate direction.
				      * 
				      * If the @p{colorize} flag is
				      * set, the
				      * @p{boundary_indicator}s of the
				      * surfaces are assigned, such
				      * that the lower one in
				      * @p{x}-direction is 0, the
				      * upper one is 1. The indicators
				      * for the surfaces in
				      * @p{y}-direction are 2 and 3,
				      * the ones for @p{z} are 4 and
				      * 5.
				      */
    template <int dim>
    static
    void
    subdivided_hyper_rectangle (Triangulation<dim>              &tria,
				const std::vector<unsigned int> &repetitions,
				const Point<dim>                &p1,
				const Point<dim>                &p2,
				const bool                       colorize=false);

				     /**
				      * Hypercube with a layer of
				      * hypercubes around it. The
				      * first two parameters give the
				      * lower and upper bound of the
				      * inner hypercube in all
				      * coordinate directions.
				      * @p{thickness} marks the size of
				      * the layer cells.
				      *
				      * If the flag colorize is set,
				      * the outer cells get material
				      * id's according tho the
				      * following scheme: extending
				      * over the inner cube in
				      * (+/-) x-direction: 1/2. In y-direction
				      * 4/8, in z-direction 16/32. The cells
				      * at corners and edges (3d) get
				      * these values bitwise or'd.
				      *
				      * Presently only available in 2d
				      * and 3d.
				      */
    static void enclosed_hyper_cube (Triangulation<2> &tria,
	 			     const double      left = 0.,
				     const double      right= 1.,
				     const double      thickness = 1.,
				     const bool        colorize = false);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void enclosed_hyper_cube (Triangulation<3> &tria,
	 			     const double      left = 0.,
				     const double      right= 1.,
				     const double      thickness = 1.,
				     const bool        colorize = false);

				     /**
				      * Initialize the given
				      * triangulation with a
				      * hyperball, i.e. a circle or a
				      * ball.  See the general
				      * documentation for a more
				      * concise description. The
				      * center of the hyperball
				      * default to the origin, the
				      * radius defaults to unity.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      *
				      * This function is declared to
				      * exist for triangulations of
				      * all space dimensions, but
				      * throws an error if called in
				      * 1d.
				      *
				      * This function is presently not
				      * implemented in 3d, since we
				      * fear that it might not be
				      * possible to represent a simple
				      * grid making up a ball with the
				      * requirements on the direction
				      * of lines and faces imposed by
				      * the triangulation classes. We
				      * hope that more complicated
				      * arrangements than the obvious
				      * one made up of 7 cells can
				      * cope with this situation, but
				      * this has not been implemented
				      * yet.
				      */    
    static void hyper_ball (Triangulation<1> &tria,
			    const Point<1>   &center = Point<1>(),
			    const double      radius = 1.);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void hyper_ball (Triangulation<2> &tria,
			    const Point<2>   &center = Point<2>(),
			    const double      radius = 1.);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void hyper_ball (Triangulation<3> &tria,
			    const Point<3>   &center = Point<3>(),
			    const double      radius = 1.);

				     /**
				      * Create a cylinder around the
				      * x-axis.  The cylinder extends
				      * from @p{x=-half_length} to
				      * @p{x=+half_length} and its
				      * projection into the
				      * @p{yz}-plane is a circle of
				      * radius @p{radius}.
				      *
				      * The boundaries are colored
				      * according to the following
				      * scheme: 0 for the hull of the
				      * cylinder, 1 for the left hand
				      * face and 2 for the right hand
				      * face.
				      */
    static void cylinder (Triangulation<3> &tria,
			  const double      radius = 1.,
			  const double      half_length = 1.);

				     /**
				      * Projection of the
				      * three-dimensional cylinder
				      * into the @p{xy}-plane.
				      * Therefore, this is simply a square.
				      *
				      * Coloring is like in 3D.
				      */
    static void cylinder (Triangulation<2> &tria,
			  const double      radius = 1.,
			  const double      half_length = 1.);

				     /**
				      * Non-implemented dummy for compilation purposes.
				      */
    static void cylinder (Triangulation<1> &tria,
			  const double      radius,
			  const double      half_length);



				     /**
				      * Initialize the given
				      * triangulation with a hyper-L
				      * consisting of exactly
				      * @p{2^dim-1} cells. See the
				      * general documentation for a
				      * description of the
				      * L-region. The limits default
				      * to minus unity and unity.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      *
				      * This function is declared to
				      * exist for triangulations of
				      * all space dimensions, but
				      * throws an error if called in
				      * 1d.
				      */
    static void hyper_L (Triangulation<1> &tria,
			 const double      left = -1.,
			 const double      right= 1.);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void hyper_L (Triangulation<2> &tria,
			 const double      left = -1.,
			 const double      right= 1.);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void hyper_L (Triangulation<3> &tria,
			 const double      left = -1.,
			 const double      right= 1.);
    
                                     /**
				      * Initialize the given
				      * Triangulation with a hypercube
				      * with a slit. The slit goes
				      * from @p{(x=0,y=-1)} to
				      * @p{(0,0)} in 2d.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      *
				      * This function is declared to
				      * exist for triangulations of
				      * all space dimensions, but
				      * throws an error if called in
				      * 1d. It is also not presently
				      * implemented in 3d.
				      */
    static void hyper_cube_slit (Triangulation<1> &tria,
				 const double      left = 0.,
				 const double      right= 1.,
				 const bool colorize = false);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      *
				      * If colorize is selected, then
				      * the two edges forming the slit
				      * carry numbers 1 and 2, while
				      * the outer boundary has number
				      * 0.
				      */
    static void hyper_cube_slit (Triangulation<2> &tria,
				 const double      left = 0.,
				 const double      right= 1.,
				 const bool colorize = false);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      *
				      * Colorization is not
				      * implemented in 3D.
				      */
    static void hyper_cube_slit (Triangulation<3> &tria,
				 const double      left = 0.,
				 const double      right= 1.,
				 const bool colorize = false);

				     /**
				      * Produce a hyper-shell,
				      * i.e. the space between two
				      * circles in two space
				      * dimensions and the region
				      * between two spheres in 3d,
				      * with given inner and outer
				      * radius and a given number of
				      * elements for this initial
				      * triangulation. If the number
				      * of initial cells is zero (as
				      * is the default), then it is
				      * computed adaptively such that
				      * the resulting elements have
				      * the least aspect ratio.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      *
				      * This function is declared to
				      * exist for triangulations of
				      * all space dimensions, but
				      * throws an error if called in
				      * 1d. It is also currently not
				      * implemented in 3d.
				      */
    static void hyper_shell (Triangulation<1>   &tria,
			     const Point<1>     &center,
			     const double        inner_radius,
			     const double        outer_radius,
			     const unsigned int  n_cells = 0);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void hyper_shell (Triangulation<2>   &tria,
			     const Point<2>     &center,
			     const double        inner_radius,
			     const double        outer_radius,
			     const unsigned int  n_cells = 0);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void hyper_shell (Triangulation<3>   &tria,
			     const Point<3>     &center,
			     const double        inner_radius,
			     const double        outer_radius,
			     const unsigned int  n_cells = 0);


				     /**
				      * This class produces a half hyper-ball,
				      * which contains four elements.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      *
				      * Currently, only a two-dimensional 
				      * version is implemented. The appropriate
				      * boundary class is 
				      * @ref{HalfHyperBallBoundary}.
				      */
    static void half_hyper_ball (Triangulation<2> &tria,
				 const Point<2>   &center = Point<2>(),
				 const double      radius = 1.);

				     /**
				      * Produce a half hyper-shell,
				      * i.e. the space between two
				      * circles in two space
				      * dimensions and the region
				      * between two spheres in 3d,
				      * with given inner and outer
				      * radius and a given number of
				      * elements for this initial
				      * triangulation.  However,
				      * opposed to the previous
				      * function, it does not produce
				      * a whole shell, but only one
				      * half of it, namely that part
				      * for which the first component
				      * is restricted to non-negative
				      * values. The purpose of this
				      * class is to enable
				      * computations for solutions
				      * which have rotational
				      * symmetry, in which case the
				      * half shell in 2d represents a
				      * shell in 3d.
				      *
				      * If the number of
				      * initial cells is zero (as is
				      * the default), then it is
				      * computed adaptively such that
				      * the resulting elements have
				      * the least aspect ratio.
				      *
				      * The triangulation needs to be
				      * void upon calling this
				      * function.
				      *
				      * At present, this function only
				      * exists in 2d.
				      */
    static void half_hyper_shell (Triangulation<2>   &tria,
				  const Point<2>     &center,
				  const double        inner_radius,
				  const double        outer_radius,
				  const unsigned int  n_cells = 0);


				     /**
				      * This function transformes the
				      * @p{Triangulation} @p{tria}
				      * smoothly to a domain that is
				      * described by the boundary
				      * points in the map
				      * @p{new_points}. This map maps
				      * the point indices to the
				      * boundary points in the
				      * transformed domain.
				      *
				      * Note, that the
				      * @p{Triangulation} is changed
				      * in-place, therefore you don't
				      * need to keep two
				      * triangulations, but the given
				      * triangulation is changed
				      * (overwritten).
				      *
				      * In 1d, this function is not
				      * currently implemented.
				      */
    template <int dim>
    static void laplace_transformation (Triangulation<dim> &tria,
					const std::map<unsigned int,Point<dim> > &new_points);

				     /**
				      * Declaration of same function
				      * for different space dimension.
				      */
    static void laplace_transformation (Triangulation<1> &tria,
					const std::map<unsigned int,Point<1> > &new_points);
    
				     /**
				      * Exception
				      */
    DeclException0 (ExcInvalidRadii);
                                     /**
                                      * Exception
                                      */
    DeclException1 (ExcInvalidRepetitions,
                    int,
                    << "The number of repetitions " << arg1
                    << " must be >=1.");
				     /**
				      * Exception
				      */
    DeclException1 (ExcInvalidRepetitionsDimension,
                    int,
                    << "The vector of repetitions  must have " 
		    << arg1 <<" elements.");

  private:
				     /**
				      * Perform the action specified
				      * by the @p{colorize} flag of
				      * the @ref{hyper_rectangle}
				      * function of this class.
				      */
    template <int dim>
    static void colorize_hyper_rectangle (Triangulation<dim> &tria);

				     /**
				      * Declaration of the above
				      * function for 1d.
				      */
    static void colorize_hyper_rectangle (Triangulation<1> &tria);
    
				     /**
				      * Perform the action specified
				      * by the @p{colorize} flag of
				      * the
				      * @ref{subdivided_hyper_rectangle}
				      * function of this class. This
				      * function is singled out
				      * because it is dimension
				      * specific.
				      */
    template <int dim>
    static
    void
    colorize_subdivided_hyper_rectangle (Triangulation<dim> &tria,
					 const Point<dim>   &p1,
					 const Point<dim>   &p2,
					 const double        epsilon);

				     /**
				      * Declaration of the above
				      * function for 1d.
				      */
    static
    void
    colorize_subdivided_hyper_rectangle (Triangulation<1> &tria,
					 const Point<1>   &p1,
					 const Point<1>   &p2,
					 const double      epsilon);

				     /**
				      * Solve the Laplace equation for
				      * @p{laplace_transformation}
				      * function for one of the
				      * @p{dim} space
				      * dimensions. Externalized into
				      * a function of its own in order
				      * to allow parallel execution.
				      */
    static
    void
    laplace_solve (const SparseMatrix<double>          &S,
		   const std::map<unsigned int,double> &m,
		   Vector<double>                      &u);
};



#endif
