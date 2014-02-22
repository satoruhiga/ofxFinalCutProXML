#pragma once

#include "pugixml.hpp"

#include "Constants.h"
#include "Model.h"

OFX_FCPXML_BEGIN_NAMESPACE

class Parser
{
public:
	
	Parser(Project& project) : project(project) {}
	
	bool open(const string& path)
	{
		pugi::xml_document doc;
		if (!doc.load_file(ofToDataPath(path).c_str())) return false;
		
		const pugi::xml_node& node = doc.child("xmeml").child("sequence");
		
		parseProject(project, node);

		return true;
	}
	
private:
	
	Project& project;
	float rate;
	
	void parseProject(Project& project, const pugi::xml_node& node)
	{
		project.name = node.child("name").child_value();
		
		project.rate = node.child("rate").child("timebase").text().as_float(0);
		if (string("TRUE") == node.child("rate").child("ntsc").text().as_string())
			project.rate -= project.rate * 0.001;
		project.duration = node.child("duration").text().as_float() / project.rate;

		rate = project.rate;
		
		parseClipItems(project, node);
		parseMarkers(project, node);
	}
	
	void parseMarkers(Project& project, const pugi::xml_node& node)
	{
		pugi::xpath_query q("marker");
		pugi::xpath_node_set nodes = q.evaluate_node_set(node);
		
		project.markers.clear();
		
		for (int i = 0; i < nodes.size(); i++)
		{
			const pugi::xml_node& n = nodes[i].node();
			
			Marker M;
			M.name = n.child("name").text().as_string();
			M.comment = n.child("comment").text().as_string();
			M.in = n.child("in").text().as_float() / rate;
			M.out = n.child("out").text().as_float() / rate;
			
			project.markers.push_back(M);
		}
		
		sort(project.markers.begin(), project.markers.end(), sort_by_time<Marker>);
	}
	
	void parseClipItems(Project& project, const pugi::xml_node& node)
	{
		pugi::xpath_query q("media/video/track/clipitem");
		pugi::xpath_node_set nodes = q.evaluate_node_set(node);

		project.clipitems.clear();
		
		for (int i = 0; i < nodes.size(); i++)
		{
			const pugi::xml_node& n = nodes[i].node();
			ClipItem CI;
			
			CI.name = n.child("name").text().as_string();
			
			CI.start = n.child("start").text().as_float() / rate;
			CI.end = n.child("end").text().as_float() / rate;
			CI.duration = CI.end - CI.start;
			
			float in = n.child("in").text().as_float() / rate;
			float clip_offset = CI.start - in;
			
			parseClipMarkers(CI, n);
			
			for (int n = 0; n < CI.markers.size(); n++)
			{
				Marker &m = CI.markers[n];
				m.in += clip_offset;
			}
			
			project.clipitems.push_back(CI);
		}
		
		sort(project.clipitems.begin(), project.clipitems.end(), sort_by_cliptime);
	}

	void parseClipMarkers(ClipItem& clipitem, const pugi::xml_node& node)
	{
		pugi::xpath_query q("marker");
		pugi::xpath_node_set nodes = q.evaluate_node_set(node);
		
		clipitem.markers.clear();
		
		for (int i = 0; i < nodes.size(); i++)
		{
			const pugi::xml_node& n = nodes[i].node();
			
			Marker M;
			M.name = n.child("name").text().as_string();
			M.comment = n.child("comment").text().as_string();
			
			M.in = n.child("in").text().as_float() / rate;
			
			if (n.child("out").text().as_float() < -1)
			{
				M.out = n.child("out").text().as_float() / rate;
			}
			else
			{
				M.out = -1;
			}
			
			clipitem.markers.push_back(M);
		}
		
		sort(clipitem.markers.begin(), clipitem.markers.end(), sort_by_time<Marker>);
	}
	
	template <typename T>
	static bool sort_by_time(const T& v0, const T& v1)
	{
		return v0.in < v1.in;
	}
	
	static bool sort_by_cliptime(const ClipItem& v0, const ClipItem& v1)
	{
		return v0.start < v1.start;
	}

};

OFX_FCPXML_END_NAMESPACE
