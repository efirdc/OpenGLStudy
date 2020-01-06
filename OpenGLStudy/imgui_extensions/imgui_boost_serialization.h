#ifndef IMGUI_BOOST_SERIALIZATION_H
#define IMGUI_BOOST_SERIALIZATION_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive & ar, ImVec4 & g, const unsigned int version)
		{
			ar & g.x & g.y & g.z & g.w;
		}

		template<class Archive>
		void serialize(Archive & ar, ImVec2 & g, const unsigned int version)
		{
			ar & g.x & g.y;
		}
	}
}

#endif