#pragma once

#include "Common/Utility/Math/Vector.hpp"
#include "Common/Utility/Math/Matrix.hpp"

#include "Visual/WorldSpace.hpp"

namespace Visual
{
	class Camera
	{
	public:
		enum class Type
		{
			Free,
			Orthographic,
			Spherical,

			NumCameraTypes,
		};

	public:
		virtual Type GetType() const = 0;

		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

	protected:
		Camera() {}
		virtual ~Camera() = default;
	};

	class PerspectiveCamera
		: public Camera
	{
	public:
		virtual const glm::mat4& GetProjectionMatrix() const override { RecalculatePerspective(); return mat_projection; }

		float GetFOV() const { RecalculatePerspective(); return fov; }
		float GetAspect() const { RecalculatePerspective(); return aspect; }
		float GetClipNear() const { RecalculatePerspective(); return clip_near; }
		float GetClipFar() const { RecalculatePerspective(); return clip_far; }

		virtual void Reset( float fov_deg, float aspect, float clip_near, float clip_far );
		virtual void SetFOV( float fov_deg );
		virtual void SetAspectRatio( float aspect );
		virtual void SetAspectRatio( uint32_t width, uint32_t height ) { SetAspectRatio( (float)height / float( width ) ); }
		virtual void SetClipSpace( float clip_near, float clip_far );

	protected:
		PerspectiveCamera();
		PerspectiveCamera( float fov_deg, float aspect, float clip_near, float clip_far );
		virtual ~PerspectiveCamera() override;

	private:
		void RecalculatePerspective() const; // not really const

		glm::mat4 mat_projection = glm::mat4( 1 );
		bool projection_dirty = true;

		float fov; // degrees
		float aspect;
		float clip_near;
		float clip_far;
	};


	// TODO: Freelook camera
	// TODO: Orthographic camera
	
	class SphericalCamera
		: public PerspectiveCamera
	{
	public:
		SphericalCamera();
		virtual ~SphericalCamera() override;
			
		virtual Type GetType() const override { return Type::Spherical; }
		static Type GetStaticType() { return Type::Spherical; }

		virtual const glm::mat4& GetViewMatrix() const override { Recalculate(); return mat_view; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { Recalculate(); return mat_view_projection; }

		const glm::vec3& GetPosition() const { return target_position; }
		virtual void SetPosition( const glm::vec3& new_target_position );
		// Move relative to world
		virtual void Move( const glm::vec3& delta );
		// Move relative to camera look direction
		virtual void Pan( const glm::vec3& delta );
		// Move relative to camera look direction without changing Z coordinate
		virtual void PanXY( const glm::vec2& delta );

		float GetRadius() const { return radius; }
		virtual void SetRadius( float radius );
		// +: in, -: out
		virtual void Zoom( float distance ) { SetRadius( GetRadius() - distance ); }

		virtual void SetRotation( float pitch_rad, float yaw_rad );
		virtual void SetRotationD( float pitch_deg, float yaw_deg );
		virtual void Rotate( float delta_pitch_rad, float delta_yaw_rad );

		// [pitch, yaw], radians
		_NODISCARD virtual std::pair<float, float> GetSphericalRotation() const;
		_NODISCARD virtual glm::vec3 GetCartesianRotation() const;

	private:
		void Recalculate() const;

		float pitch = 0; // radians
		float yaw = 0; // radians
		float radius = 1; // distance from target, worldspace

		glm::vec3 target_position = { 0, 0, 0 };

		// cached matrices
		glm::mat4 rotation = glm::mat4( 1.f );
		glm::mat4 mat_view = glm::mat4( 1.f );
		glm::mat4 mat_view_projection = glm::mat4( 1.f );

		bool dirty = true;
	};
}
