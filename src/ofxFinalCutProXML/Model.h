#pragma once

#include "Constants.h"

OFX_FCPXML_BEGIN_NAMESPACE

typedef float Rate;
typedef int Frame;
typedef float Second;

typedef float Time;
typedef float Duration;

struct Marker
{
	string name;
	string comment;
	Time in, out;
};

struct ClipItem
{
	string name;
	Duration duration;
	Time start, end;
	vector<Marker> markers;
};

struct Project
{
	string name;
	Rate rate;
	Duration duration;
	vector<ClipItem> clipitems;
	vector<Marker> markers;
};

OFX_FCPXML_END_NAMESPACE
