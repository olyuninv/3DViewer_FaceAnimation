#include "Blendshape.h"

namespace Viewer3D_Blendshapes
{
	blendshape::blendshape()
	{
	}

	blendshape::~blendshape()
	{
	}

	void blendshape::calculateFace(Face neutralFace, int numBlendshapes, Face * blendshapes, float * weights, float* &customPositions, float* &customNormals)
	{
		float newWeightsLength = blendshape::calculateWeightsLength(numBlendshapes, weights);

		float * adjustedWeights = new float[numBlendshapes];
		for (int j = 0; j < numBlendshapes; j++)
		{
			adjustedWeights[j] = weights[j] / newWeightsLength;
		}

		//float neutralWeight = 1.0f / newWeightsLength;

		// set to 0
		for (int i = 0; i < neutralFace.numVertices * 3; i++)
		{
			customPositions[i] = 0.0f;
			customNormals[i] = 0.0f;
		}

		for (int i = 0; i < neutralFace.numVertices * 3; i++)
		{
			for (int j = 0; j < numBlendshapes; j++)
			{
				customPositions[i] += adjustedWeights[j] * blendshapes[j].deltaBlendshape[i]; 
				customNormals[i] += adjustedWeights[j] * blendshapes[j].deltaNormal[i];
			}

			customPositions[i] += neutralFace.vpositions[i];// *neutralWeight;
			customNormals[i] += neutralFace.vnormals[i];// *neutralWeight;
		}

		delete[] adjustedWeights;
		adjustedWeights = nullptr;
	}

	void blendshape::recalculateNormals(std::vector<unsigned int> indices, int numVertices, float * customPositions, float* &customNormals)
	{
		// set to 0
		for (int i = 0; i < numVertices * 3; i++)
		{
			customNormals[i] = 0.0f;
		}

		for (int i = 0; i < indices.size(); i += 3)
		{
			glm::vec3 v1 = glm::vec3(customPositions[indices[i + 1] * 3] - customPositions[indices[i] * 3],
				customPositions[indices[i + 1] * 3 + 1] - customPositions[indices[i] * 3 + 1],
				customPositions[indices[i + 1] * 3 + 2] - customPositions[indices[i] * 3 + 2]);
			glm::vec3 v2 = glm::vec3(customPositions[indices[i + 2] * 3] - customPositions[indices[i] * 3],
				customPositions[indices[i + 2] * 3 + 1] - customPositions[indices[i] * 3 + 1],
				customPositions[indices[i + 2] * 3 + 2] - customPositions[indices[i] * 3 + 2]);

			glm::vec3 normal = glm::cross(v1, v2);

			normal = glm::normalize(normal);
			//glm::vec3 curr_normal = glm::vec3(newNormals[indices[i * 3]], newNormals[indices[i * 3] + 1], newNormals[indices[i * 3] + 2]);

			//glm::vec3 newNormal = glm::normalize(curr_normal + normal);

			for (int j = 0; j < 3; j++)
			{
				customNormals[indices[i + j] * 3 + 0] += normal.x;
				customNormals[indices[i + j] * 3 + 1] += normal.y;
				customNormals[indices[i + j] * 3 + 2] += normal.z;
			}
		}
	}

	float blendshape::calculateWeightsLength(int numWeights, float * weights)
	{
		float squaredWeights = 1.0f;  // this is the weight of the neutral face that we are starting from

		for (int i = 0; i < numWeights; i++)
		{
			squaredWeights += pow(weights[i], 2);
		}

		return sqrt(squaredWeights);
	}
}