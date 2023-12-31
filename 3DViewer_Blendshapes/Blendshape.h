#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Face.h"

namespace Viewer3D_Blendshapes
{
	class blendshape
	{

	public:
		blendshape();

		~blendshape();

		static void calculateFace(Face neutralFace, int numBlendshapes, Face * blendshapes, float * weights, float* &customPositions, float* &customNormals);

		static void recalculateNormals(std::vector<unsigned int> indices, int numVertices, float * customPositions, float* &customNormals);

		static float calculateWeightsLength(int numWeights, float * weights);
	};
}
