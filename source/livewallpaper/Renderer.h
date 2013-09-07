#pragma once
class MeshObject;
class Shader;

namespace Renderer
{
	void RenderMesh(const MeshObject* mesh, const Shader* shader);
}