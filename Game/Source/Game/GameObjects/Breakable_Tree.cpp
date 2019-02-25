#include "GamePCH.h"

Breakable_Tree::Breakable_Tree(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, Material * pMaterial)
    : Breakable(pScene, name, pos, rot, scale, pMesh, pMaterial)
{

}

Breakable_Tree::Breakable_Tree(Scene * pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, std::string meshname, std::string materialname)
    : Breakable(pScene, name, pos, rot, scale, meshname, materialname)
{
}

Breakable_Tree::~Breakable_Tree()
{
}


