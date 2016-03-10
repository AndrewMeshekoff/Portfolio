#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include <string>
#include "ProgramData.h"
#include "Asset.h"

extern ProgramData ProgramContext;

HillPlane::HillPlane(float w, float h, float l, unsigned int seed)
	:HeightMap(256, 250, 5, 20, 10, 25, 150, seed),
	position(0.0, 0.0, 0.0),
	rotationAxis(0.0, 1.0, 0.0),
	rotationDegrees(0.0),
	scaling(w, h, l)
{
	HeightMap.GenerateHeightmap();
	model = HeightMap.exportMesh(1.0, 1.0);

	std::string grassFilePath = std::string(DATA_PATH) + "forest.jpg";
	std::string sandFilePath = std::string(DATA_PATH) + "sand.jpg";

	grassTexture = new Texture(grassFilePath.c_str());
	sandTexture = new Texture(sandFilePath.c_str());
}

void HillPlane::Draw(glutil::MatrixStack modelMatrix)
{
	glutil::MatrixStack Push(modelMatrix);
	modelMatrix.Translate(position);
	modelMatrix.Rotate(rotationAxis, rotationDegrees);
	modelMatrix.Scale(scaling);

	ProgramContext.SetShader(0);
	ProgramContext.UpdateModToCamBlock(modelMatrix.Top());

	glm::mat3 normalMatrix(glm::transpose(glm::inverse(glm::mat3(modelMatrix.Top()))));
	glUniformMatrix3fv(ProgramContext.programList[0]->uniforms[UniformEnums::NORMAL_MOD_TO_CAM], 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	glUniform1i(ProgramContext.programList[0]->uniforms[UniformEnums::GRASS_TEX_TARGET], 0);
	grassTexture->BindTexture(0);

	glUniform1i(ProgramContext.programList[0]->uniforms[UniformEnums::SAND_TEX_TARGET], 1);
	sandTexture->BindTexture(1);

	model->Render();
}

HillPlane::~HillPlane()
{
	delete model;
	delete sandTexture;
	delete grassTexture;
}

Skybox::Skybox(float rotateOffset)
{
	rotationDegrees = rotateOffset;

	std::vector<VertexComponents> format;
	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));

	unsigned int vertexCount = 8;
	unsigned int elementCount = 36;
	model = new Mesh(format, (void*)vertices, vertexCount, elements, elementCount, GL_TRIANGLES);

	std::string frontPath = std::string(DATA_PATH) + "skyrender1.bmp";
	std::string rightPath = std::string(DATA_PATH) + "skyrender2.bmp";
	std::string backPath = std::string(DATA_PATH) + "skyrender4.bmp";
	std::string leftPath = std::string(DATA_PATH) + "skyrender5.bmp";
	std::string topPath = std::string(DATA_PATH) + "skyrender3.bmp";
	std::string bottomPath = std::string(DATA_PATH) + "skyrender6.bmp";
	cubeMap = new Texture(frontPath.c_str(), backPath.c_str(), topPath.c_str(), bottomPath.c_str(), leftPath.c_str(), rightPath.c_str());
}

void Skybox::Draw(glutil::MatrixStack modelMatrix)
{
	glutil::MatrixStack Push(modelMatrix);
	modelMatrix.RotateY(rotationDegrees);
	glm::mat4 zeroTranslation(1.0);
	zeroTranslation[3][3] = 0.0;
	modelMatrix.ApplyMatrix(zeroTranslation);

	ProgramContext.SetShader(1);
	ProgramContext.UpdateModToCamBlock(modelMatrix.Top());

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	cubeMap->BindTexture(0);
	glUniform1i(ProgramContext.programList[1]->uniforms[UniformEnums::SKY_TEX_TARGET], 0);

	model->Render();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

}

Skybox::~Skybox()
{
	delete model;
	delete cubeMap;
}