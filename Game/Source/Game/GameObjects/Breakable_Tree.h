#pragma once

class Breakable_Tree : public Breakable
{
public:
    Breakable_Tree(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    Breakable_Tree(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname);
    virtual ~Breakable_Tree();

private:

};