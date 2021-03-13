#pragma once

class Timestep
{
public:
	Timestep( float time_seconds = 0.0f )
		: time_s( time_seconds ) {}

	operator float() const { return time_s; }

	float GetSeconds() const { return time_s; }
	float GetMilliseconds() const { return time_s * 0.001f; }

private:
	float time_s;
};

class PresiceTimestep
{
public:
	PresiceTimestep( double time_seconds = 0.0 )
		: time_s( time_seconds ) {}

	operator double() const { return time_s; }
	operator Timestep() const { return Timestep( static_cast<float>(time_s) ); }

	double GetSeconds() const { return time_s; }
	double GetMilliseconds() const { return time_s * 0.001; }

private:
	double time_s;
};

