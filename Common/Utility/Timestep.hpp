#pragma once

struct Timestep
{
	float time;
	float delta;

	Timestep( float time_ = 0.f, float delta_time_ = 0.f )
		: time( time_ ), delta( delta_time_ ) {}
};

struct PreciseTimestep
{
	double time;
	double delta;

	PreciseTimestep( double time_ = 0.f, double delta_time_ = 0.f )
		: time( time_ ), delta( delta_time_ ) {}

	operator Timestep() const { return Timestep( (float)time, (float)delta ); }
};

