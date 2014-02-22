#pragma once

#include "ofMain.h"
#include "pugixml.hpp"

#define OFX_FCPXML_BEGIN_NAMESPACE namespace ofx { namespace FinalCutProXML {
#define OFX_FCPXML_END_NAMESPACE } }

OFX_FCPXML_BEGIN_NAMESPACE
OFX_FCPXML_END_NAMESPACE

namespace ofxFinalCutProXML = ofx::FinalCutProXML;
namespace ofxFCPXML = ofxFinalCutProXML;