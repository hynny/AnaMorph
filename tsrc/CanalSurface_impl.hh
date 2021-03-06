/*
 * This file is part of
 *
 * AnaMorph: a framework for geometric modelling, consistency analysis and surface
 * mesh generation of anatomically reconstructed neuron morphologies.
 * 
 * Copyright (c) 2013-2017: G-CSC, Goethe University Frankfurt - Queisser group
 * Author: Konstantin Mörschel
 * 
 * AnaMorph is free software: Redistribution and use in source and binary forms,
 * with or without modification, are permitted under the terms of the
 * GNU Lesser General Public License version 3 (as published by the
 * Free Software Foundation) with the following additional attribution
 * requirements (according to LGPL/GPL v3 §7):
 * 
 * (1) The following notice must be displayed in the Appropriate Legal Notices
 * of covered and combined works:
 * "Based on AnaMorph (https://github.com/NeuroBox3D/AnaMorph)."
 * 
 * (2) The following notice must be displayed at a prominent place in the
 * terminal output of covered works:
 * "Based on AnaMorph (https://github.com/NeuroBox3D/AnaMorph)."
 *
 * (3) Neither the name "AnaMorph" nor the names of its contributors may be
 * used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * (4) The following bibliography is recommended for citation and must be
 * preserved in all covered files:
 * "Mörschel K, Breit M, Queisser G. Generating neuron geometries for detailed
 *   three-dimensional simulations using AnaMorph. Neuroinformatics (2017)"
 * "Grein S, Stepniewski M, Reiter S, Knodel MM, Queisser G.
 *   1D-3D hybrid modelling – from multi-compartment models to full resolution
 *   models in space and time. Frontiers in Neuroinformatics 8, 68 (2014)"
 * "Breit M, Stepniewski M, Grein S, Gottmann P, Reinhardt L, Queisser G.
 *   Anatomically detailed and large-scale simulations studying synapse loss
 *   and synchrony using NeuroBox. Frontiers in Neuroanatomy 10 (2016)"
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ------------------------------------------------------------------------------------------------------------------ *
 *                                                                                                                    *
 *                        CanalSurface implementation..                                                               *
 *                                                                                                                    *
 * ------------------------------------------------------------------------------------------------------------------ */
#include "PolyAlgorithms.hh"

/* default constructor: implicitly default constructs spine curve and radius functor.  these are
 * required to behave in a consistent way and default-init to trivial zero-valued parametric curve /
 * radius functor */
template <typename C2F, typename RadF, typename R>
CanalSurface<C2F, RadF, R>::CanalSurface()
{
}

/* ctor with spine curve and radius functor. domain [t0, t1] is extracted from given spine curve */
template <typename C2F, typename RadF, typename R>
CanalSurface<C2F, RadF, R>::CanalSurface(
    SpaceCurveReal<C2F, R> const   &spine_curve,
    RadF const                     &radius_functor)
{
    this->spine_curve       = spine_curve;
    /*
    this->spine_curve_d     = spine_curve.getDerivative();
    this->spine_curve_d2    = this->spine_curve_d.getDerivative();
    */

    this->radius_functor    = radius_functor;

    /* obtain domain from given spine curve and set as domain of canal surface */
    auto domain             = spine_curve.getDomain();
    this->t0                = domain.first;
    this->t1                = domain.second;
}

/* ctor taking array of three component functors, the radius functor and the domain */
template <typename C2F, typename RadF, typename R>
CanalSurface<C2F, RadF, R>::CanalSurface(
    std::array<C2F, 3> const       &component_functors,
    RadF const                     &radius_functor,
    R const                        &t0,
    R const                        &t1)
{
    this->spine_curve       = SpaceCurveReal<C2F, R>(component_functors, t0, t1);
    /*
    this->spine_curve_d     = spine_curve.getDerivative();
    this->spine_curve_d2    = this->spine_curve_d.getDerivative();
    */

    this->radius_functor    = radius_functor;

    this->t0                = t0;
    this->t1                = t1;
}

/* copy ctor, assignment operator */
template <typename C2F, typename RadF, typename R>
CanalSurface<C2F, RadF, R>::CanalSurface(const CanalSurface<C2F, RadF, R> &delta) 
{
    this->spine_curve       = delta.spine_curve;
    /*
    this->spine_curve_d     = delta.spine_curve_d;
    this->spine_curve_d2    = delta.spine_curve_d2;
    */

    this->radius_functor    = delta.radius_functor;

    this->t0                = delta.t0;
    this->t1                = delta.t1;
}

/* copy ctor, assignment operator */
template <typename C2F, typename RadF, typename R>
CanalSurface<C2F, RadF, R> &
CanalSurface<C2F, RadF, R>::operator=(const CanalSurface<C2F, RadF, R> &delta) 
{
    this->spine_curve       = delta.spine_curve;
    /*
    this->spine_curve_d     = delta.spine_curve_d;
    this->spine_curve_d2    = delta.spine_curve_d2;
    */

    this->radius_functor    = delta.radius_functor;

    this->t0                = delta.t0;
    this->t1                = delta.t1;

    return (*this);
}

/* virtual dtor */
template <typename C2F, typename RadF, typename R>
CanalSurface<C2F, RadF, R>::~CanalSurface()
{
}

/* virtual spine curve / radius evaluation. delegators in effect.. */
template <typename C2F, typename RadF, typename R>
Vec3<R>
CanalSurface<C2F, RadF, R>::spineCurveEval(R const &t) const
{
    return (this->spine_curve.eval(t));
}

template <typename C2F, typename RadF, typename R>
Vec3<R>
CanalSurface<C2F, RadF, R>::spineCurveEval_d(R const &t) const
{
    return (this->spine_curve.eval_d(t));
}

template <typename C2F, typename RadF, typename R>
Vec3<R>
CanalSurface<C2F, RadF, R>::spineCurveEval_d2(R const &t) const
{
    return (this->spine_curve.eval_d2(t));
}

template <typename C2F, typename RadF, typename R>
R
CanalSurface<C2F, RadF, R>::radiusEval(R const &t) const
{
    return (this->radius_functor(t, this->spine_curve));
}

template <typename C2F, typename RadF, typename R>
void
CanalSurface<C2F, RadF, R>::spineCurveGetRenderFrame(
    R const    &t,
    Vec3<R>     rvec,
    Vec3<R>    &x,
    Vec3<R>    &y,
    Vec3<R>    &z) const
{
    this->spine_curve.getRenderFrame(t, rvec, x, y, z);
}

template <typename C2F, typename RadF, typename R>
void
CanalSurface<C2F, RadF, R>::spineCurveGetFrenetFrame(
    R const    &t,
    Vec3<R>    &x,
    Vec3<R>    &y,
    Vec3<R>    &z) const
{
    this->spine_curve.getFrenetFrame(t, x, y, z);
}

template <typename C2F, typename RadF, typename R>
R
CanalSurface<C2F, RadF, R>::spineCurveApproxArcLength(
    R const &tstart, 
    R const &tend,
    R const &dt) const
{
    return (this->spine_curve.approxArcLength(tstart, tend, dt));
}

template <typename C2F, typename RadF, typename R>
std::pair<R, R>
CanalSurface<C2F, RadF, R>::getDomain() const
{
    return (std::pair<R, R>(this->t0, this->t1));
}

/*
template <typename C2F, typename RadF, typename R>
void
CanalSurface<C2F, RadF, R>::setDomain(
    R const &t0, 
    R const &t1)
{
    this->t0    = t0;
    this->t1    = t1;
}
*/

template <typename C2F, typename RadF, typename R>
SpaceCurveReal<C2F, R> const &
CanalSurface<C2F, RadF, R>::getSpineCurve() const
{
    return (this->spine_curve);
}

template <typename C2F, typename RadF, typename R>
void
CanalSurface<C2F, RadF, R>::setSpineCurve(SpaceCurveReal<C2F, R> const &gamma)
{
    this->spine_curve = gamma;
}

template <typename C2F, typename RadF, typename R>
RadF
CanalSurface<C2F, RadF, R>::getRadiusFunctor() const
{
    return (this->radius_functor);
}

template <typename C2F, typename RadF, typename R>
void
CanalSurface<C2F, RadF, R>::setRadiusFunctor(RadF const &radius_functor)
{
    this->radius_functor = radius_functor;
}

template <typename C2F, typename RadF, typename R>
template <typename Tm, typename Tv, typename Tf>
void
CanalSurface<C2F, RadF, R>::generateMesh(
    Mesh<Tm, Tv, Tf, R>                                    &M,
    uint32_t                                                n_phi_segments,
    R                                                       triangle_height_factor,
    Vec3<R>                                                 rvec,
    R const                                                &phi_0,
    R const                                                &arclen_dt,
    bool                                                    start_circle_offset,
    std::vector<
            typename Mesh<Tm, Tv, Tf, R>::vertex_iterator
        >                                                  *start_circle_its,
    bool                                                   *end_circle_offset,
    std::vector<
            typename Mesh<Tm, Tv, Tf, R>::vertex_iterator
        >                                                  *end_circle_its,
    typename Mesh<Tm, Tv, Tf, R>::vertex_iterator          *closing_vertex_it,
	bool                                                    preserve_crease_edges) const
{
    debugl(1, "CanalSurface::generateMesh().\n");
    debugTabInc();

    uint32_t    i, j, ntsegments;
    R           t, r, phi, dphi, phi_offset;
    Vec3<R>     c, p, n, last_p, a, px, py, pz, vpos;

    dphi = Common::twopi / (R)n_phi_segments;

    /* compute t values: start at t = 0.0 and compute an increment based on the radius. if the
     * radius is larger, we want to take a larger step than if it is smaller. the step is computed
     * in the following way: with n_phi_segments given, the length of a line segment on the
     * approximated circle is 
     *
     * c = r*sqrt(2*(1-cos(dphi)))
     *
     * and since we want equilateral triangles and the next circle is "offset" with dphi / 2.0, we
     * want to choose the step in to such that the height of the triangles is approx
     *
     * h = c/2 *sqrt(3)
     *
     * for a unit-length cylinder, this will result in only equilateral triangles. in practive, this
     * strategy results in close to equilateral triangles also for canal surfaces */
    R               l, h, t_new, tlast_slack;

    /* get arc length of spine curve */
    R               arclength  = this->spineCurveApproxArcLength(this->t0, this->t1, arclen_dt);

    std::vector<R>  t_values = { this->t0 };
    t = this->t0;
    while (t < this->t1) {
        /* compute and push next t-value */
        r       = this->radiusEval(t);
        l       = r*sqrt(2.0*(1.0 - cos(dphi))); 
        if (!preserve_crease_edges)
        	h   = triangle_height_factor*sqrt(3.0)*l / 2.0;
        else
        	h   = triangle_height_factor*l;

        /* delta_t depends on the arc length: we want to advance a step of h in arc length, which
         * approximately amounts to an advance of h / l in t (for close to constant parametric
         * speed). */
        t_new   = t + h / arclength;
        t_values.push_back(t_new);

        /* and shift */
        t       = t_new;
    }

    /* the last value will be larger than 1.0, pop it, but first compute tlast_slack, which is the
     * last computed value - this->t1. the last two circles will have a distance that is too small =>
     * distribute tlast_slack uniformly among the other t values by setting t(i) = t(i) -
     * i*tlast_slack / ntsegments */
    tlast_slack = t_values.back() - this->t1;
    t_values.pop_back();

    /* the final value t1 is not yet present in the vector of t values right now, therefore
     * ntsegments is the size of the vector, instead of size() - 1 */
    ntsegments   = t_values.size();

    /* offset to make room for last to circles */
    for (i = 1; i < t_values.size(); i++) {
        t_values[i] -= (R)i*tlast_slack/ntsegments;
    }

    /* append t1 to finish the list */
    t_values.push_back(this->t1);

    /* vectors storing vertices of current and last circle */
    std::vector<typename Mesh<Tm, Tv, Tf, R>::vertex_iterator>  last_circle;
    std::vector<typename Mesh<Tm, Tv, Tf, R>::vertex_iterator>  current_circle;

    /* resize vector */
    current_circle.resize(n_phi_segments);
    last_circle.resize(n_phi_segments);

    /* if start_circle_its == NULL, generate initial circle and close it with triangles */
    if (start_circle_its == NULL) {
        /* get starting point and radius */
        p   = this->spineCurveEval(this->t0);
        r   = this->radiusEval(this->t0);

        /* get curve base for t0 */
        this->spineCurveGetRenderFrame(this->t0, rvec, px, py, pz);

        /* frenet base is available to demonstrate its disadvantageous nature for meshing, which is due
         * to strong spinning of the frenet trihedron, which is quite bad for discretization, since
         * quads get twisted then.. */
        //this->getFrenetFrame(this->t0, px, py, pz);

        debugl(1, "start_circle_its == NULL => generating initial circle.\n"); 

        typename Mesh<Tm, Tv, Tf, R>::vertex_iterator   start_closing_vertex_it;

        /* start circle is never offset, overwrite wrong argument if necessary */
        phi_offset          = phi_0;
        start_circle_offset = false;

        /* generate starting circle vertices and save iterators */
        for (j = 0; j < n_phi_segments; j++) {
            phi                         = ( (R)j * Common::twopi) / (R)n_phi_segments;
            vpos                        = p + py*(r*cos(phi + phi_offset)) + pz*(r*sin(phi + phi_offset));
            current_circle[j]           = M.vertices.insert(vpos);
        }

        /* get centroid of initial circle */
        start_closing_vertex_it     = M.vertices.insert(p);

        /* closing triangles */
        M.faces.insert(start_closing_vertex_it, current_circle[0], current_circle[n_phi_segments - 1]);
        for (j = 0; j < n_phi_segments - 1; j++) {
            M.faces.insert(start_closing_vertex_it, current_circle[j+1], current_circle[j]);
        }
    }
    /* otherwise, this canal surface is not the start canal segment and the previous canal surface
     * has already generated the vertices of the start circle, which is the end circle of the
     * previous segment. the caller needs to take care to reopen the mesh around the end circle of the previous segment
     * by deleting the closing vertex. */
    else {
        /* NOTE: n_phi_segments has to match along the whole path for this to work properly.. */
        if (start_circle_its->size() == n_phi_segments) {
            current_circle = *start_circle_its;
            debugl(1, "start_circle_its != NULL => deleting closing vertex and taking given initial circle vertex ids..\n"); 
        }
        else {
            throw("CanalSurface::generateMesh(): supplied start_circle_its does not consist of exactly n_phi_segments vertices => invalid arguments supplied by the caller.");
        }
    }

    /* now the last two circles will, in general, have a distance that is too small compared to the
     * radii => compute the difference to the approximate good value and distribute this difference
     * equally among all the other segments */
    debugl(1, "rendering inner circles..\n");
    for (i = 1; i < ntsegments; i++) {
        /* if start is offset, then even values for i are not offset, odd values are offset.
         * otherwise, vice versa */
        if (start_circle_offset)
        {
            if (!preserve_crease_edges && i % 2 == 0)
                phi_offset = phi_0 + dphi / 2.0;
            else
                phi_offset = phi_0;
        }
        else {
            if (preserve_crease_edges || i % 2 == 0)
                phi_offset = phi_0;
            else
                phi_offset = phi_0 + dphi / 2.0;
        }

        //t = (R)i / (R)ntsegments;
        t = t_values[i];

        /* shift current_circle to last_circle. same for p and last_p */
        last_circle = current_circle;
        last_p      = p;

        /* calculate new current point and radius */
        p           = this->spineCurveEval(t);
        r           = this->radiusEval(t);

        this->spineCurveGetRenderFrame(t, rvec, px, py, pz);
        //this->spineCurveGetFrenetFrame(t, px, py, pz);

        /* generate current circle vertices and save iterators */
        for (j = 0; j < n_phi_segments; j++) {
            phi                 = ( (R)j * Common::twopi) / (R)n_phi_segments;
            current_circle[j]   = M.vertices.insert( p + py*(r*cos(phi + phi_offset)) + pz*(r*sin(phi + phi_offset)) );
        }

        /* generate quad faces between last_circle and current_circle */
        for (j = 0; j < n_phi_segments - 1; j++) {
            M.faces.insert(last_circle[j], last_circle[j+1], current_circle[j+1], current_circle[j]);
        }

        /* closing quad at index wrap-around */
        M.faces.insert(last_circle[n_phi_segments - 1], last_circle[0], current_circle[0], current_circle[n_phi_segments - 1]);
    }

    debugl(1, "rendering last circle..\n");
    /* and the same procedure for the endpoint, i.e. t = t1 */
    last_circle = current_circle;
    last_p      = p;
    p           = this->spineCurveEval(this->t1);
    r           = this->radiusEval(this->t1);

    this->spineCurveGetRenderFrame(this->t1, rvec, px, py, pz);
    //this->spineCurveGetFrenetFrame(this->t1, px, py, pz);
    
    /* last circle has index ntsegments, since we got (ntsegments + 1) circles */
    if (start_circle_offset)
    {
        if (!preserve_crease_edges && ntsegments % 2 == 0)
            phi_offset = phi_0 + dphi / 2.0;
        else
            phi_offset = phi_0;
    }
    else
    {
        if (preserve_crease_edges || ntsegments % 2 == 0)
            phi_offset = phi_0;
        else
            phi_offset = phi_0 + dphi / 2.0;
    }

    px.print_debugl(1);
    py.print_debugl(1);
    pz.print_debugl(1);
    debugTabInc();
    for (j = 0; j < n_phi_segments; j++) {
        phi                     = ( (R)j * Common::twopi) / (R)n_phi_segments;
        vpos                    = p + py*(r*cos(phi + phi_offset)) + pz*(r*sin(phi + phi_offset));
        current_circle[j]       = M.vertices.insert(vpos);
    }
    debugTabDec();

    /* get centroid of last circle vertices and place closing vertex in the middle */
    Vec3<R> end_closing_vertex_pos = p;
    typename Mesh<Tm, Tv, Tf, R>::vertex_iterator end_closing_vertex_it = M.vertices.insert(end_closing_vertex_pos);

    /* generate quad faces between last_circle and current_circle */
    for (j = 0; j < n_phi_segments - 1; j++) {
        M.faces.insert(last_circle[j], last_circle[j+1], current_circle[j+1], current_circle[j]);
    }

    /* closing quad at index warp around */
    M.faces.insert(last_circle[n_phi_segments - 1], last_circle[0], current_circle[0], current_circle[n_phi_segments - 1]);

    /* add closing triangles at the end */
    M.faces.insert(end_closing_vertex_it, current_circle[n_phi_segments - 1], current_circle[0]);
    for (j = 0; j < n_phi_segments - 1; j++) {
        M.faces.insert(end_closing_vertex_it, current_circle[j], current_circle[j + 1] );
    }

    /* if desired by the caller, write out information to referenced data */
    if (end_circle_its) {
        *end_circle_its = current_circle;
    }

    if (closing_vertex_it) {
        *closing_vertex_it = end_closing_vertex_it;
    }

    /* we got ntsegments + 1 circles with indices 0..ntsegments in total. so if ncircles is even,
     * then the offset bit will swap, otherwise it remains the same => XOR with ncircles_even */
    if (end_circle_offset) {
        *end_circle_offset   = Aux::Logic::lxor(start_circle_offset, (ntsegments + 1) % 2 == 0);
        //printf("start_circle_offset = %d, ntsegments: %5d, ncircles_even: %5d, neven: %d => end_circle_offset = %d\n", start_circle_offset, ntsegments, ncircles, ncircles_even, end_circle_offset);
    }

    debugTabDec();
    debugl(1, "CanalSurface::generateMesh(): done.\n");
}

/* ------------------------------------------------------------------------------------------------------------------ *
 *                                                                                                                    *
 *                        BezierCanalSurface implementation..                                                         *
 *                                                                                                                    *
 * ------------------------------------------------------------------------------------------------------------------ */
template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>::BezierCanalSurface()
: CanalSurface<BernsteinPolynomial<degree, R, R>, RadF, R>(),
  bb_set(false)
{
    /* domain is always [0,1] for BezierCurves and hence also for BezierCanalSurface */
    this->t0 = 0;
    this->t1 = 1;
}

template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>::BezierCanalSurface
(
	const BezierCurve<degree, R>& spine_curve,
    const RadF& radius_functor
)
: CanalSurface<BernsteinPolynomial<degree, R, R>, RadF, R>(spine_curve, radius_functor), bb_set(false)
{
    /* CanalSurface constructor sets [t0, t1] = [0,1] through domain of BezierCurve parameter spine_curve */
    this->spine_curve = spine_curve;
}

template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>::BezierCanalSurface
(
    const std::array<BernsteinPolynomial<degree, R, R>, 3>& component_functors,
    const RadF& radius_functor
)
    /* in-line constructed BezierCurve from component_functors arrays is passed to base class CanalSurface ctor. */
: CanalSurface<BernsteinPolynomial<degree, R, R>, RadF, R>
  (BezierCurve<degree, R>(component_functors), radius_functor),
  bb_set(false)
{
    this->spine_curve = BezierCurve<degree, R>(component_functors);
}

/* same as previous constructor above for control-point based construction of BezierCurves */
template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>::BezierCanalSurface
(
	const std::vector<Vec3<R> >& control_points,
    const RadF& radius_functor
)
    /* in-line constructed BezierCurve from component_functors arrays is passed to base class CanalSurface ctor. */
: CanalSurface<BernsteinPolynomial<degree, R, R>, RadF, R>
  (BezierCurve<degree, R>(control_points), radius_functor),
  bb_set(false)
{
    this->spine_curve = BezierCurve<degree, R>(control_points);
}

template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>::BezierCanalSurface(BezierCanalSurface const &delta)
: CanalSurface<BernsteinPolynomial<degree, R, R>, RadF, R>(delta),
  bb_set(false)
{
    this->spine_curve   = delta.spine_curve;
    this->bb_set        = delta.bb_set;
    this->bb            = delta.bb;
}

template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>&
BezierCanalSurface<degree, RadF, R>::operator=(const this_type& delta)
{
    CanalSurface<BernsteinPolynomial<degree, R, R>, RadF, R>::operator=(delta);
    this->spine_curve   = delta.spine_curve;
    this->bb_set        = delta.bb_set;
    this->bb            = delta.bb;

    return *this;
}

template <uint32_t degree, typename RadF, typename R>
BezierCanalSurface<degree, RadF, R>::~BezierCanalSurface()
{}

/* since spine_curve has been overridden with more specific type BezierCurve in BezierCanalSurface, also override
 * getSpineCurve() */
template <uint32_t degree, typename RadF, typename R>
BezierCurve<degree, R>
BezierCanalSurface<degree, RadF, R>::getSpineCurve() const
{
    return this->spine_curve;
}

template <uint32_t degree, typename RadF, typename R>
void
BezierCanalSurface<degree, RadF, R>::clipToInterval(const R& t0, const R& t1)
{
    if (t0 <= t1) {
        /* clip radius functor BEFORE clipping spine curve, since the old domain is needed to get radii */
        this->radius_functor.clipToInterval(t0, t1, this->spine_curve);

        /* clip this->spine_curve, which is a BezierCurve<R>. the spine_curve members from the base class
         * CanalSurface< BernsteinPolynomial<degree, R, R>, RadF, R> has been overridden in BezierCanalSurface, which
         * necessitates the update below. */
        this->spine_curve.clipToInterval(t0, t1, &(this->spine_curve));

        /* update spine curve object in base class. note that BezierCanalSurface::spine_curve is a BezierCurve, which is
         * a derived class of SpaceCurveReal< BernsteinPolynomial<degree, R, R>, R>, which is in turn the spine curve type of
         * the base class CanalSurface< BernsteinPolynomial<degree, R, R>, RadF, R>.
         *
         * after clipping the BezierCurve in (this) BezierCanalSurface, use the resulting clipped BezierCurve to update
         * the base class member CanalSurface< BernsteinPolynomial<degree, R, R>, RadF, R>::spine_curve. the following update
         * assignment "slices" the assigned BezierCurve, since the specifics of BezierCurve<R> are lost when assigning
         * to an object of the base class type SpaceCurveReal< BernsteinPolynomial<degree, R, R>, R>.  this slicing is
         * unproblematic here however, since the CanalSurface<..> base class of BezierCanalSurface only require the
         * functionality at the "sliced" base level of abstraction. */
        CanalSurface< BernsteinPolynomial<degree, R, R>, RadF, R>::spine_curve = this->spine_curve;
    }
    else {
        throw("BezierCanalSurface::clipToInterval(): malformed interval [t0, t1]: t0 > t1.");
    }
}

template <uint32_t degree, typename RadF, typename R>
void
BezierCanalSurface<degree, RadF, R>::updateBoundingBox(uint32_t spine_curve_subdivision_depth)
{
    using Aux::VecMat::onesVec3;
    using Aux::VecMat::fabsVec3;
    using Aux::VecMat::maxVec3;

    this->bb        = this->spine_curve.getBoundingBox(spine_curve_subdivision_depth);
    
    /* extend bounding box of spine curve with maximum radius */
    R rmax          = this->radius_functor.getMaxRadius();
    Vec3<R> offset  = onesVec3<R>() * rmax;
    this->bb.extend(0, offset);

    /* extend resulting bounding box by 2,5%, but no less than 1E-3, in every component. */
    this->bb.extend(0.025, Vec3<R>(1E-3, 1E-3, 1E-3));
    this->bb_set    = true;
}

template <uint32_t degree, typename RadF, typename R>
BoundingBox<R>
BezierCanalSurface<degree, RadF, R>::getBoundingBox() const
{
    if (this->bb_set) {
        return (this->bb);
    }
    else {
        throw("BezierCanalSurface::getBoundingBox(): bounding box has not been updated. use updateBoundingBox() first. this is a const method.");
    }
}

template <uint32_t degree, typename RadF, typename R>
R
BezierCanalSurface<degree, RadF, R>::checkRenderVector(Vec3<R> const &r) const
{
    debugl(3, "BezierCanalSurface::checkRenderVector():\n");
    debugTabInc();

    BezierCurve<degree, R> const       &gamma   = this->spine_curve;
    BezierCurve<derivDeg, R>            dgamma  = gamma.getDerivative();

    BernsteinPolynomial<derivDeg, R, R>   r_cross_dgamma[3];
    BernsteinPolynomial<2*derivDeg, R, R> p, q;
    const int zDeg = 2*derivDeg + ((2*derivDeg>0) ? 2*derivDeg-1 : 0);
    BernsteinPolynomial<zDeg, R, R> z;

    r_cross_dgamma[0]   = dgamma[2]*r[1] - dgamma[1]*r[2];
    r_cross_dgamma[1]   = dgamma[0]*r[2] - dgamma[2]*r[0];
    r_cross_dgamma[2]   = dgamma[1]*r[0] - dgamma[0]*r[1];
    p                   = r_cross_dgamma[0].square() + r_cross_dgamma[1].square() + r_cross_dgamma[2].square();
    q                   = dgamma[0].square() + dgamma[1].square() + dgamma[2].square();

    /* target polynomial, numerator of rational function describing || r \times t(t) || */
    z                   = (p.getDerivative()).multiply(q) - p.multiply(q.getDerivative());

    /* locate roots with bezier clipping */
    std::vector<PolyAlg::RealInterval<R> > z_roots;
    PolyAlg::BezClip_roots<zDeg, R>(z, 0.0, 1.0, 1E-6, z_roots);

    R f_min = Aux::Numbers::inf<R>();

    /* boundary values */
    f_min = std::min(f_min, p.eval(0.0) / q.eval(0.0));
    f_min = std::min(f_min, p.eval(1.0) / q.eval(1.0));


    /* check all all roots in zroots */
    R root;
    debugTabInc();
    for (auto root_interval : z_roots) {
        root    = (root_interval.t0 + root_interval.t1) / 2.0;
        f_min   = std::min(f_min, p.eval(root) / q.eval(root));
        debugl(3, "function value of rational target function for root %5.4f of numerator polynomial: %5.4f\n",
            root, p.eval(root) / q.eval(root));
    }
    debugTabDec();
    debugl(3, "minimum target function value fmin: %5.4f\n", f_min);

    debugTabDec();
    debugl(3, "BezierCanalSurface::checkRenderVector(): done. f_min: %5.4f\n", f_min);

    return f_min;
}

/* ------------------------------------------------------------------------------------------------------------------ *
 *                                                                                                                    *
 *                        BLRCanalSurface implementation.                                                             *
 *                                                                                                                    *
 * ------------------------------------------------------------------------------------------------------------------ */
template <uint32_t degree, typename R>
BLRCanalSurface<degree, R>::BLRCanalSurface()
{
    initGlobalSelfIntersectionData();
}

template <uint32_t degree, typename R>
BLRCanalSurface<degree, R>::BLRCanalSurface(const BezierCurve<degree, R>& spine_curve, const R& r0, const R& r1)
: base_type(spine_curve, LinearRadiusInterpolatorArcLen<BernsteinPolynomial<degree, R, R>, R>(r0, r1))
{
    initGlobalSelfIntersectionData();
}

template <uint32_t degree, typename R>
BLRCanalSurface<degree, R>::BLRCanalSurface
(
    const std::array<BernsteinPolynomial<degree, R, R>, 3>& component_functors,
    const R& r0,
    const R& r1
)
: base_type(component_functors, LinearRadiusInterpolatorArcLen<BernsteinPolynomial<degree, R, R>, R>(r0, r1))
{
    initGlobalSelfIntersectionData();
}

/* same as previous constructor above for control-point based construction of BezierCurves */
template <uint32_t degree, typename R>
BLRCanalSurface<degree, R>::BLRCanalSurface(const std::vector<Vec3<R> >& control_points, const R& r0, const R& r1)
: base_type(control_points, LinearRadiusInterpolatorArcLen<BernsteinPolynomial<degree, R, R>, R>(r0, r1))
{
    initGlobalSelfIntersectionData();
}

template <uint32_t degree, typename R>
BLRCanalSurface<degree, R>::BLRCanalSurface(const this_type& delta)
: base_type(delta)
{
    initGlobalSelfIntersectionData();
}

template <uint32_t degree, typename R>
BLRCanalSurface<degree, R> &
BLRCanalSurface<degree, R>::operator=(BLRCanalSurface const &delta)
{
    base_type::operator=(delta);
    return *this;
}

template <uint32_t degree, typename R>
void
BLRCanalSurface<degree, R>::initGlobalSelfIntersectionData()
{
    uint32_t i, j, k, m;
    std::vector<PowerPolynomial<degree, R, R> >  B_n_pow;
    StaticMatrix<degree, degree, R> F_i_n_powercoeff;

    // helper polynomials
    StaticVector<degree+1, BiBernsteinPolynomial<degree-1, degree-1, R, R> > F;

    // compute the monomial representation of each i-th Bernstein basis polynom B_i^n(t)
    B_n_pow.resize(degree+1);
    for (i = 0; i < degree + 1; ++i)
        PolyAlg::convertBasis<degree, R>(B_n_pow[i], PolyAlg::computeBernsteinBasisPoly<degree, R, R>(i));

    // compute all bivariate polynomials F_i^n(x,y), i = 0..n
    for (i = 0; i < degree + 1; ++i)
    {
        // first, compute coefficients of F_i^n in M(n-1, n-1) => nxn coefficient matrix
        // init F_i_n_powercoeff to zerod (n, n) matrix
        F_i_n_powercoeff.fill(0.0);

        for (k = 1; k < degree + 1; ++k)
            for (m = 0; m < k; ++m)
                F_i_n_powercoeff(m, k - 1 - m) += B_n_pow[i][k];

        // now generate F_i^n from the power coefficient matrix F_i_n_powercoeff.
        F[i].convertFromPowerBasis(F_i_n_powercoeff);
    }

    // compute all bivariate polynomials G_{ij}^n, i, j = 0..n
    BiBernsteinPolynomial<degree, degree, R, R> B_in_y, B_jn_y;
    for (i = 0; i < degree + 1; ++i)
    {
        for (j = 0; j < degree + 1; ++j)
        {
            B_in_y = PolyAlg::BernsteinConvertToBiPoly<degree, degree, false>::get
                     (PolyAlg::computeBernsteinBasisPoly<degree, R, R>(i));
            B_jn_y = PolyAlg::BernsteinConvertToBiPoly<degree, degree, false>::get
                     (PolyAlg::computeBernsteinBasisPoly<degree, R, R>(j));

            // compute coefficient G(i, j)
            G(i, j) = B_jn_y.multiply(F(i)) - B_in_y.multiply(F(j));
        }
    }
}

template <uint32_t degree, typename R>
BLRCanalSurface<degree, R>::~BLRCanalSurface()
{
}

template <uint32_t degree, typename R>
std::pair<R, R>
BLRCanalSurface<degree, R>::getRadii() const
{
    return (this->radius_functor.getRadii());
}

template <uint32_t degree, typename R>
R
BLRCanalSurface<degree, R>::getMinRadius() const
{
    auto rpair = this->radius_functor.getRadii();
    return (std::min(rpair.first, rpair.second));
}

template <uint32_t degree, typename R>
R
BLRCanalSurface<degree, R>::getMaxRadius() const
{
    auto rpair = this->radius_functor.getRadii();
    return (std::max(rpair.first, rpair.second));
}

template <uint32_t degree, typename R>
void
BLRCanalSurface<degree, R>::spineCurveComputeRegularityPolynomial(BernsteinPolynomial<2*derivDeg, R, R> &p_reg) const
{
    this->spine_curve.computeRegularityPolynomial(p_reg);
}

template <uint32_t degree, typename R>
void
BLRCanalSurface<degree, R>::spineCurveComputeStationaryPointDistPoly(
    Vec3<R> const              &x,
    BernsteinPolynomial<degree+derivDeg, R, R>  &p) const
{
    this->spine_curve.computeStationaryPointDistPoly(x, p);
}

template <uint32_t degree, typename R>
void
BLRCanalSurface<degree, R>::computeLocalSelfIntersectionPolynomial(BernsteinPolynomial<6*derivDeg, R, R> &p_lsi) const
{
    debugl(3, "BLRCanalSurface::computeLocalSelfIntersectionPolynomial()\n");
    debugTabInc();

    /* get degree, maximum radius, reference gamma to (this->spine_curve), first two derivatives dgamma and d2gamma of
     * spine_curve. */
    R const                     rmax    = this->getMaxRadius();
    BezierCurve<degree, R> const       &gamma   = this->spine_curve;
    BezierCurve<derivDeg, R> const        dgamma  = gamma.getDerivative();
    BezierCurve<deriv2Deg, R> const        d2gamma = dgamma.getDerivative();

    /* three components of the cross product (dgamma \cdot d2gamma). every summand is distinct
     * and has to be computed exactly once. */
    BernsteinPolynomial<derivDeg+deriv2Deg, R, R>   d_d2_yz_minus_zy, d_d2_zx_minus_xz,d_d2_xy_minus_yx;
    BernsteinPolynomial<2*(derivDeg+deriv2Deg), R, R> crossprod_square;
    BernsteinPolynomial<2*derivDeg, R, R>   dgamma_square;
    BernsteinPolynomial<6*derivDeg, R, R>   dgamma_sqcube;

    /* the derivative dgamma is represented in BB(n-1), d2gamma in BB(n-2), no degree elevation was performed!
     * multiplying works nonetheless and yields a the summands of the cross product in in BB(2n-3) */
    d_d2_yz_minus_zy    = dgamma[1].multiply(d2gamma[2]) - dgamma[2].multiply(d2gamma[1]);
    d_d2_zx_minus_xz    = dgamma[2].multiply(d2gamma[0]) - dgamma[0].multiply(d2gamma[2]);
    d_d2_xy_minus_yx    = dgamma[0].multiply(d2gamma[1]) - dgamma[1].multiply(d2gamma[0]);

    /* the square of the cross product (that is, the square of the norm, inner product with itself) can be computed by
     * squaring the above terms and adding up. this yields a polynomial in BB(4n-6), which is then directly multiplied
     * by rmax^2 */
    crossprod_square    = (
            (d_d2_yz_minus_zy.multiply(d_d2_yz_minus_zy)) +
            (d_d2_zx_minus_xz.multiply(d_d2_zx_minus_xz)) + 
            (d_d2_xy_minus_yx.multiply(d_d2_xy_minus_yx))
        );

    crossprod_square    = crossprod_square * (rmax * rmax);

    /* the other term we need is (dgamma_x^2 + dgamma_y^2 + dgamma_z^2)^3, i.e.  the third power of the inner product of
     * the tangent vector with itself.  the tangent innner product yields coefficients in BB(2n-2), third power yields
     * BB(6n-6) => the other term, crossprod_square, is in BB(4n-6) and must therefore be degree-elevated by 2n. */
    dgamma_square       = 
            (dgamma[0].multiply(dgamma[0])) +
            (dgamma[1].multiply(dgamma[1])) +
            (dgamma[2].multiply(dgamma[2]));

    /* third power done via three multiplications, it should be possible in one step, but
     * we need multinomial coefficient then I think...*/
    dgamma_sqcube       = (dgamma_square.multiply(dgamma_square)).multiply(dgamma_square);

    /* elevate crossprod_square by 2n */
    BernsteinPolynomial<6*derivDeg, R, R> crossProdSq_elev;
    crossProdSq_elev = crossprod_square.template elevateDegree<6*derivDeg>();

    /* self-intersection polynomial is now simply the difference. to check if its negative over its
     * entire domain [0,1], check corners values and compute roots */
    p_lsi               = crossProdSq_elev - dgamma_sqcube;

    debugTabDec();
    debugl(3, "BLRCanalSurface::computeLocalSelfIntersectionPolynomial(): done.\n");
}

template <uint32_t degree, typename R>
void
BLRCanalSurface<degree, R>::computeGlobalSelfIntersectionSystem(
    BiBernsteinPolynomial<2*degree-1+derivDeg, 2*degree-1, R, R>    &p,
    BiBernsteinPolynomial<2*degree-1, 2*degree-1+derivDeg, R, R>    &q,
    BernsteinPolynomial<degree+derivDeg, R, R>      &p_edge_t0,
    BernsteinPolynomial<degree+derivDeg, R, R>      &p_edge_t1) const
{
    debugl(2, "BLRCanalSurface::computeIntersectionSystem().\n");
    debugTabInc();

    /* for consistency with the above and the thesis, use const reference Gamma for "this" again */
    BLRCanalSurface<degree, R> const   &Gamma   = (*this);
    BezierCurve<degree, R> const       &gamma   = Gamma.spine_curve;
    BezierCurve<derivDeg, R> const      dgamma  = gamma.getDerivative();

    /* distance vector with trivial solution factored out */
    std::vector<BiBernsteinPolynomial<2*degree-1, 2*degree-1, R, R> >
        dist_nt(3, BiBernsteinPolynomial<2*degree-1, 2*degree-1, R, R>(0));

    std::vector<BiBernsteinPolynomial<degree, degree, R, R> > dist_trivial(3);
    BiBernsteinPolynomial<degree, degree, R, R> tmp;

    dist_trivial[0] = PolyAlg::BernsteinConvertToBiPoly<degree, degree, true>::get(gamma[0])
                    - PolyAlg::BernsteinConvertToBiPoly<degree, degree, false>::get(gamma[0]);

    dist_trivial[1] = PolyAlg::BernsteinConvertToBiPoly<degree, degree, true>::get(gamma[1])
                    - PolyAlg::BernsteinConvertToBiPoly<degree, degree, false>::get(gamma[1]);

    dist_trivial[2] = PolyAlg::BernsteinConvertToBiPoly<degree, degree, true>::get(gamma[2])
                    - PolyAlg::BernsteinConvertToBiPoly<degree, degree, false>::get(gamma[2]);

    /* dist_nt can be computed with the precomputed bivariate polynomials G_{ij}^n */
    uint32_t i, j;
    for (i = 0; i < degree+1; i++)
    {
        for (j = 0; j < degree+1; j++)
        {
            dist_nt[0] += G(i,j) * gamma[0][i];
            dist_nt[1] += G(i,j) * gamma[1][i];
            dist_nt[2] += G(i,j) * gamma[2][i];
        }
    }

    /* p and q are computed as for the intersection of two pipe surfaces, only the specially
     * prepared "non trivial" distance vector dist_nt is used. */

    p = dist_nt[0].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, true>::get(dgamma[0])) +
        dist_nt[1].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, true>::get(dgamma[1])) +
        dist_nt[2].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, true>::get(dgamma[2]));

    q = dist_nt[0].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, false>::get(dgamma[0])) +
        dist_nt[1].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, false>::get(dgamma[1])) +
        dist_nt[2].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, false>::get(dgamma[2]));


    /* there are only two edge systems here, which are called p_edge_t0 and p_edge_t1. since Gamma
     * == Delta in this case, the edges E_x0 and E_y0 logically describe the same search problem
     * and hence only two edge systems need to be considered. since dist(0,0) = 0 and dist(1,1) = 0
     * is trivial, only the two corners (0,1) and (1,0) need to be checked, which again coincide
     * logically, since || gamma(1) - gamma(0) || = || gamma(0) - gamma(1) ||. so only one 
     * candidate non-trvial candidate point from the four corners.. either one */
    Vec3<R>                     vec_gamma_t0, vec_gamma_t1;
    BernsteinPolynomial<degree, R, R>   gamma_t0[3], gamma_t1[3];

    vec_gamma_t0 = gamma.eval(0.0);
    vec_gamma_t1 = gamma.eval(1.0);

    /* convert vectors to constant polynomials in BB(n) / BB(m) */
    for (j = 0; j < 3; j++) {
        gamma_t0[j] = BernsteinPolynomial<degree, R, R>(vec_gamma_t0[j]);
        gamma_t1[j] = BernsteinPolynomial<degree, R, R>(vec_gamma_t1[j]);
    }

    /* compute two edge polynomials */
    p_edge_t0 = 
        dgamma[0].multiply(gamma[0] - gamma_t0[0]) + 
        dgamma[1].multiply(gamma[1] - gamma_t0[1]) + 
        dgamma[2].multiply(gamma[2] - gamma_t0[2]);

    p_edge_t1 = 
        dgamma[0].multiply(gamma[0] - gamma_t1[0]) + 
        dgamma[1].multiply(gamma[1] - gamma_t1[1]) + 
        dgamma[2].multiply(gamma[2] - gamma_t1[2]);

    debugl(3, "BLRCanalSurface::computeGlobalSelfIntersectionSystem: p_edge_t0: BB(%d), p_edge_t1: BB(%d). coeffs vectors follow..\n", p_edge_t0.getDegree(), p_edge_t1.getDegree());

    debugTabDec();
    debugl(2, "BLRCanalSurface::computeIntersectionSystem(): done.\n");
}

template <uint32_t degree, typename R>
void
BLRCanalSurface<degree, R>::computeIntersectionSystem(
    const this_type& Delta,
    BiBernsteinPolynomial<degree+derivDeg, degree, R, R>    &p,
    BiBernsteinPolynomial<degree, degree+derivDeg, R, R>    &q,
    BernsteinPolynomial<degree+derivDeg, R, R>      &p_edge_x0,
    BernsteinPolynomial<degree+derivDeg, R, R>      &p_edge_x1,
    BernsteinPolynomial<degree+derivDeg, R, R>      &p_edge_y0,
    BernsteinPolynomial<degree+derivDeg, R, R>      &p_edge_y1) const
{
    debugl(2, "BLRCanalSurface::computeIntersectionSystem().\n");
    debugTabInc();

   /* NOTE: (this) is Gamma within this context: a const reference is used for better readability. its spine curve
    * is gamma (+ derivative dgamma). same for Delta, delta, ddelta. */
    BLRCanalSurface const  &Gamma   = (*this);
    BezierCurve<degree, R> const   &gamma   = Gamma.spine_curve;
    BezierCurve<derivDeg, R> const   dgamma   = gamma.getDerivative();

    BezierCurve<degree, R> const   &delta   = Delta.spine_curve;
    BezierCurve<derivDeg, R> const   ddelta   = delta.getDerivative();

    uint32_t i, j;

    /* gamma[k] - delta[k] elevated to bidegree (m, n) */
    std::vector<BiBernsteinPolynomial<degree, degree, R, R> >
        dist_gamma_delta(3, BiBernsteinPolynomial<degree, degree, R, R>(0));

    /* compute common factor, the "distance vector" in BB(m, n) */
    for (i = 0; i < degree + 1; i++) {
        for (j = 0; j < degree + 1; j++) {
            dist_gamma_delta[0](i, j) = Gamma.spine_curve[0][i] - Delta.spine_curve[0][j];
            dist_gamma_delta[1](i, j) = Gamma.spine_curve[1][i] - Delta.spine_curve[1][j];
            dist_gamma_delta[2](i, j) = Gamma.spine_curve[2][i] - Delta.spine_curve[2][j];
        }
    } 

    /* p is the inner product of dgamma and the distance vector dist_gamma_delta. this is done
     * component-wise. for this purpose, the components of the derivatives dgamma and ddelta are
     * converted to bivariate polynomials in BB(m-1, 0) and BB(0, n-1), respectively */
    p = dist_gamma_delta[0].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, true>::get(dgamma[0])) +
        dist_gamma_delta[1].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, true>::get(dgamma[1])) +
        dist_gamma_delta[2].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, true>::get(dgamma[2]));

    q = dist_gamma_delta[0].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, false>::get(ddelta[0])) +
        dist_gamma_delta[1].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, false>::get(ddelta[1])) +
        dist_gamma_delta[2].multiply(PolyAlg::BernsteinConvertToBiPoly<derivDeg, 0, false>::get(ddelta[2]));

    /* compute edge polynomials */
    /* the edge polynomial p_edge_x0 are the stationary points of the distance function restricted to the edge E_x0 =
     * {x0} x [y0, y1]. these are the roots of the polynomial
     *
     * p_edge_x0(y) = ddelta(y) \cdot (delta(y) - gamma(x0))
     *
     * the other three edge polynomials are defined accordingly.
     *
     * NOTE: we're working with the magnified curves -> x_0, y_0 = 0.0, x_1, y_1 = 1.0. */
    Vec3<R>                                 vec_gamma_x0, vec_gamma_x1, vec_delta_y0, vec_delta_y1; 
    std::vector<BernsteinPolynomial<degree, R, R>>  gamma_x0(3), gamma_x1(3), delta_y0(3), delta_y1(3);

    vec_gamma_x0    = gamma.eval(0.0);
    vec_gamma_x1    = gamma.eval(1.0);
    vec_delta_y0    = delta.eval(0.0);
    vec_delta_y1    = delta.eval(1.0);
    
    /* convert vectors to constant polynomials in BB(n) / BB(m) */
    for (j = 0; j < 3; j++) {
        gamma_x0[j] = BernsteinPolynomial<degree, R, R>(vec_gamma_x0[j]);
        gamma_x1[j] = BernsteinPolynomial<degree, R, R>(vec_gamma_x1[j]);

        delta_y0[j] = BernsteinPolynomial<degree, R, R>(vec_delta_y0[j]);
        delta_y1[j] = BernsteinPolynomial<degree, R, R>(vec_delta_y1[j]);
    }

    /* compute four edge polynomials by component-wise evaluation of dot product */
    p_edge_x0 = 
        ddelta[0].multiply(delta[0] - gamma_x0[0]) + 
        ddelta[1].multiply(delta[1] - gamma_x0[1]) + 
        ddelta[2].multiply(delta[2] - gamma_x0[2]);

    p_edge_x1 = 
        ddelta[0].multiply(delta[0] - gamma_x1[0]) + 
        ddelta[1].multiply(delta[1] - gamma_x1[1]) + 
        ddelta[2].multiply(delta[2] - gamma_x1[2]);

    p_edge_y0 = 
        dgamma[0].multiply(gamma[0] - delta_y0[0]) + 
        dgamma[1].multiply(gamma[1] - delta_y0[1]) + 
        dgamma[2].multiply(gamma[2] - delta_y0[2]);

    p_edge_y1 = 
        dgamma[0].multiply(gamma[0] - delta_y1[0]) + 
        dgamma[1].multiply(gamma[1] - delta_y1[1]) + 
        dgamma[2].multiply(gamma[2] - delta_y1[2]);

    debugTabDec();
    debugl(2, "CanalSurface::computeIntersectionSystem(): done.\n");
}
