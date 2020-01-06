#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "glm/glm.hpp"

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive& ar, glm::vec4& g, const unsigned int version)
		{
			ar & g.x & g.y & g.z & g.w;
		}

		template<class Archive>
		void serialize(Archive& ar, glm::vec3& g, const unsigned int version)
		{
			ar & g.x & g.y & g.z;
		}

		template<class Archive>
		void serialize(Archive& ar, glm::vec2& g, const unsigned int version)
		{
			ar & g.x & g.y;
		}
		template<class Archive>
		void serialize(Archive& ar, glm::ivec4& g, const unsigned int version)
		{
			ar& g.x& g.y& g.z& g.w;
		}

		template<class Archive>
		void serialize(Archive& ar, glm::ivec3& g, const unsigned int version)
		{
			ar& g.x& g.y& g.z;
		}

		template<class Archive>
		void serialize(Archive& ar, glm::ivec2& g, const unsigned int version)
		{
			ar& g.x& g.y;
		}
	}
}