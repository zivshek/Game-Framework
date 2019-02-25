#ifndef __Mesh_H__
#define __Mesh_H__

class Mesh
{
protected:
    GLuint m_VBO;
    GLuint m_IBO;

    unsigned int m_NumVerts;
    unsigned int m_NumIndices;

    GLenum m_PrimitiveType;

public:
    Mesh();
    virtual ~Mesh();

    virtual void Init(VertexFormat* verts, int numverts, unsigned int* indices, int numindices, GLenum usage);
    virtual void Init(const void* verts, int numverts, int vertexbytesize, GLenum usage);

    virtual void SetupAttributes(ShaderProgram* pShaderProgram);
    virtual void SetupUniforms(Material* pMaterial, mat4* pMatWorld, mat4* pMatView, mat4* pMatProj, vec3 campos);
    virtual void SetupLighting(Material* pMaterial, std::map<int, Light*>& lights);
    virtual void Draw(ShaderProgram* pShaderProgram);

	virtual void Setup2DUniforms(ShaderProgram* shader, GLint texture, vec2 pos, vec2 proj);
    virtual void SetupIngameUniforms(ShaderProgram* shader, bool poisoned);

    // The following functions are defined in MeshShapes.cpp
    static Mesh* CreateBox(vec2 size);
    static Mesh* CreateCube(vec3 size);
	static Mesh* CreatePlane(vec2 bottomleft, ivec2 vertices, vec2 worldsize, vec2 repeat);

	static Mesh* CreateMeshForModel(const std::vector<vec3>& v, const std::vector<vec2>& uv, const std::vector<vec3>& n);
};

#endif //__Game_H__
