#include "Precompiled/pch.hpp"
#include "Camera.hpp"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Visual/Worldspace.hpp"

namespace
{
	glm::vec3 SphericalToCartesian( float theta, float phi, float radius )
	{
		const float x = radius * glm::sin( phi ) * glm::sin( theta );
		const float y = radius * glm::cos( phi );
		const float z = radius * glm::sin( phi ) * glm::cos( theta );

		return glm::vec3( x, y, z );
	}
}

namespace Visual
{
	///
	/// SphericalCamera
	/// 
	SphericalCamera::SphericalCamera()
	{
	}

	SphericalCamera::~SphericalCamera()
	{
	}

	void SphericalCamera::SetPosition( const glm::vec3& new_target_position )
	{
		target_position = new_target_position;
		dirty = true;
	}

	void SphericalCamera::Move( const glm::vec3& delta )
	{
		target_position += delta;
		dirty = true;
	}

	void SphericalCamera::Pan( const glm::vec3& delta )
	{
		Recalculate();

		target_position += glm::vec3( glm::row( rotation, 0 ) * delta.x );
		target_position += glm::vec3( glm::row( rotation, 1 ) * delta.y );
		target_position += glm::vec3( glm::row( rotation, 2 ) * delta.z );
		dirty = true;
	}

	void SphericalCamera::PanXY( const glm::vec2& delta )
	{
		Recalculate();

		target_position += glm::vec3( glm::row( rotation, 0 ) * delta.x );
		target_position += glm::vec3( glm::row( rotation, 1 ) * delta.y );
		dirty = true;
	}

	void SphericalCamera::SetRadius( float radius_ )
	{
		radius = radius_;
		dirty = true;
	}

	void SphericalCamera::SetRotation( float theta_, float phi_ )
	{
		theta = theta_;
		phi = phi_;
		dirty = true;
	}

	void SphericalCamera::Rotate( float delta_theta, float delta_phi )
	{
		theta += delta_theta;
		phi += delta_phi;
		dirty = true;
	}

	std::pair<float, float> SphericalCamera::GetSphericalRotation() const
	{
		return std::make_pair( theta, phi );
	}

	glm::vec3 SphericalCamera::GetCartesianRotation() const
	{
		return glm::normalize( SphericalToCartesian( theta, phi, radius ) );
	}

	void SphericalCamera::Recalculate() const
	{
		if (!dirty)
			return;

		const auto identity_mat = glm::identity<glm::mat4>();

		auto* non_const_this = const_cast<SphericalCamera*>(this);
		non_const_this->rotation = glm::rotate( identity_mat, theta, Worldspace::Right3 ) * glm::rotate( identity_mat, phi, Worldspace::Up3 );
		non_const_this->mat_view = glm::translate( identity_mat, Worldspace::Forward3 * -radius ) * rotation * glm::translate( identity_mat, target_position );
		non_const_this->mat_view_projection = GetProjectionMatrix() * non_const_this->mat_view;
		non_const_this->dirty = false;
	}


	///
	/// PerspectiveCamera
	/// 
	PerspectiveCamera::PerspectiveCamera()
		: fov( 60 )
		, aspect( 1 )
		, clip_near( 0.1f )
		, clip_far( 100.f )
		, projection_dirty( true )
	{
	}

	PerspectiveCamera::PerspectiveCamera( float fov_, float aspect_, float clip_near_, float clip_far_ )
		: fov( fov_ )
		, aspect( aspect_ )
		, clip_near( clip_near_ )
		, clip_far( clip_far_ )
		, projection_dirty( true )
	{
	}

	void PerspectiveCamera::Reset( float fov_, float aspect_, float clip_near_, float clip_far_ )
	{
		fov = fov_;
		aspect = aspect_;
		clip_near = clip_near_;
		clip_far = clip_far_;
		projection_dirty = true;
	}

	void PerspectiveCamera::SetFOV( float fov_ )
	{
		fov = fov_;
		projection_dirty = true;
	}

	void PerspectiveCamera::SetAspectRatio( float aspect_ )
	{
		aspect = aspect_;
		projection_dirty = true;
	}

	void PerspectiveCamera::SetClipSpace( float clip_near_, float clip_far_ )
	{
		clip_near = clip_near_;
		clip_far = clip_far_;
		projection_dirty = true;
	}

	void PerspectiveCamera::RecalculatePerspective() const
	{
		if (!projection_dirty)
			return;

		auto* non_const_this = const_cast<PerspectiveCamera*>(this);
		non_const_this->mat_projection = glm::perspective( fov, aspect, clip_near, clip_far );
		non_const_this->projection_dirty = false;
	}
}
