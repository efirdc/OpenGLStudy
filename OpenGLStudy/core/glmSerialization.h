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

		template<class Archive>
		void serialize(Archive& ar, glm::mat3& m, const unsigned int version)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					ar& m[i][j];
		}
		template<class Archive>
		void serialize(Archive& ar, glm::mat2& m, const unsigned int version)
		{
			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					ar& m[i][j];
		}
		template<class Archive>
		void serialize(Archive& ar, glm::mat4& m, const unsigned int version)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					ar& m[i][j];
		}
		template<class Archive>
		void serialize(Archive& ar, glm::quat& q, const unsigned int version)
		{
			ar& q.x & q.y & q.z & q.w;
		}
	}
}