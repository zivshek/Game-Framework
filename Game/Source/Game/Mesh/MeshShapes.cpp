#include "GamePCH.h"

Mesh* Mesh::CreateBox(vec2 size)
{
    VertexFormat verts[4];

    verts[0] = VertexFormat( -size.x/2, -size.y/2, 0, 255,255,255,255, 0,0, vec3(0, 0, -1));
    verts[1] = VertexFormat( -size.x/2,  size.y/2, 0, 255,255,255,255, 0,1, vec3(0, 0, -1));
    verts[2] = VertexFormat(  size.x/2,  size.y/2, 0, 255,255,255,255, 1,1, vec3(0, 0, -1));
    verts[3] = VertexFormat(  size.x/2, -size.y/2, 0, 255,255,255,255, 1,0, vec3(0, 0, -1));

    unsigned int indices[6] = { 0,1,2, 0,2,3 };

    Mesh* pMesh = new Mesh();
    pMesh->Init( verts, 4, indices, 6, GL_STATIC_DRAW );

    return pMesh;
};

Mesh * Mesh::CreateCube(vec3 size)
{
    VertexFormat v[24];
    float x = size.x;
    float y = size.y;
    float z = size.z;

    MyColor c1 = MyColor(255, 255, 255, 255);
    MyColor c2 = MyColor(100, 255, 255, 255);
    MyColor c3 = MyColor(255, 100, 0, 255);
    MyColor c4 = MyColor(255, 255, 100, 255);
    MyColor c5 = MyColor(100, 0, 100, 255);
    MyColor c6 = MyColor(0, 255, 0, 255);

    /* faces
      _______
     /|   4 /|
    /_|_3__/ |
    |5/  6 |2/
    |/__1__|/   */

    /* vertices going clockwise from top left corner of each face */

    // face 1
    v[0] = VertexFormat(-x / 2, y / 2, -z / 2, c1, 0, 1, vec3(0, 0, -1));
    v[1] = VertexFormat(x / 2, y / 2, -z / 2, c1, 1 / 6.0f, 1, vec3(0, 0, -1));
    v[2] = VertexFormat(x / 2, -y / 2, -z / 2, c1, 1 / 6.0f, 0, vec3(0, 0, -1));
    v[3] = VertexFormat(-x / 2, -y / 2, -z / 2, c1, 0, 0, vec3(0, 0, -1));
    // face 2
    v[4] = VertexFormat(x / 2, y / 2, -z / 2, c2, 1 / 6.0f, 1, vec3(1, 0, 0));
    v[5] = VertexFormat(x / 2, y / 2, z / 2, c2, 2 / 6.0f, 1, vec3(1, 0, 0));
    v[6] = VertexFormat(x / 2, -y / 2, z / 2, c2, 2 / 6.0f, 0, vec3(1, 0, 0));
    v[7] = VertexFormat(x / 2, -y / 2, -z / 2, c2, 1 / 6.0f, 0, vec3(1, 0, 0));
    // face 3
    v[8] = VertexFormat(-x / 2, y / 2, z / 2, c3, 2 / 6.0f, 1, vec3(0, 1, 0));
    v[9] = VertexFormat(x / 2, y / 2, z / 2, c3, 3 / 6.0f, 1, vec3(0, 1, 0));
    v[10] = VertexFormat(x / 2, y / 2, -z / 2, c3, 3 / 6.0f, 0, vec3(0, 1, 0));
    v[11] = VertexFormat(-x / 2, y / 2, -z / 2, c3, 2 / 6.0f, 0, vec3(0, 1, 0));
    // face 4
    v[12] = VertexFormat(x / 2, y / 2, z / 2, c4, 3 / 6.0f, 1, vec3(0, 0, 1));
    v[13] = VertexFormat(-x / 2, y / 2, z / 2, c4, 4 / 6.0f, 1, vec3(0, 0, 1));
    v[14] = VertexFormat(-x / 2, -y / 2, z / 2, c4, 4 / 6.0f, 0, vec3(0, 0, 1));
    v[15] = VertexFormat(x / 2, -y / 2, z / 2, c4, 3 / 6.0f, 0, vec3(0, 0, 1));
    // face 5
    v[16] = VertexFormat(-x / 2, y / 2, z / 2, c5, 4 / 6.0f, 1, vec3(-1, 0, 0));
    v[17] = VertexFormat(-x / 2, y / 2, -z / 2, c5, 5 / 6.0f, 1, vec3(-1, 0, 0));
    v[18] = VertexFormat(-x / 2, -y / 2, -z / 2, c5, 5 / 6.0f, 0, vec3(-1, 0, 0));
    v[19] = VertexFormat(-x / 2, -y / 2, z / 2, c5, 4 / 6.0f, 0, vec3(-1, 0, 0));
    // face 6
    v[20] = VertexFormat(x / 2, -y / 2, -z / 2, c6, 5 / 6.0f, 1, vec3(0, -1, 0));
    v[21] = VertexFormat(-x / 2, -y / 2, -z / 2, c6, 1, 1, vec3(0, -1, 0));
    v[22] = VertexFormat(-x / 2, -y / 2, z / 2, c6, 1, 0, vec3(0, -1, 0));
    v[23] = VertexFormat(x / 2, -y / 2, z / 2, c6, 5 / 6.0f, 0, vec3(0, -1, 0));

    Mesh* pMesh = new Mesh();

    unsigned int indices[36] = { 0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4,
        8, 9,10, 10,11, 8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,22,21, 23,22,20
    };
    pMesh->Init(v, 24, indices, 36, GL_STATIC_DRAW);

    return pMesh;
};

Mesh * Mesh::CreatePlane(vec2 bottomleft, ivec2 vertices, vec2 worldsize, vec2 repeat)
{
	std::vector<VertexFormat> v;
	float x_step = worldsize.x / (vertices.x - 1);
	float z_step = worldsize.y / (vertices.y - 1);

	float uv_x_step = repeat.x / (vertices.x - 1);
	float uv_y_step = repeat.y / (vertices.y - 1);

	MyColor c1 = MyColor(255, 255, 255, 50);

	for (int z = 0; z < vertices.y; z++)
	{
		for (int x = 0; x < vertices.x; x++)
		{
			v.push_back(VertexFormat(x * x_step + bottomleft.x, 0, z * z_step + bottomleft.y, c1, x * uv_x_step, z * uv_y_step, vec3(0,1,0)));
		}
	}

	std::vector<unsigned int> indices;
	int num_of_quads = (vertices.x - 1) * (vertices.y - 1);
	int num_of_ind = num_of_quads * 6;
	for (int i = 0; i < num_of_quads; i++)
	{
		int x = i % (vertices.x - 1);
		int z = i / (vertices.x - 1);
		int bottomleft = x + z * (vertices.x);
		int bottomright = bottomleft + 1;
		int topleft = x + (z + 1)*(vertices.x);;
		int topright = topleft + 1;
		indices.push_back(topleft);
		indices.push_back(topright);
		indices.push_back(bottomright);
		indices.push_back(bottomright);
		indices.push_back(bottomleft);
		indices.push_back(topleft);
	}

	Mesh* pMesh = new Mesh();
	pMesh->Init(&v.at(0), v.size(), &indices.at(0), num_of_ind, GL_STATIC_DRAW);

	return pMesh;
}

Mesh * Mesh::CreateMeshForModel(const std::vector<vec3>& v, const std::vector<vec2>& uv, const std::vector<vec3>& n)
{
	std::vector<VertexFormat> vertices;

	for (unsigned int i = 0; i < v.size(); i++)
	{
		VertexFormat vf;
		vf.pos = v[i];
		vf.uv = uv[i];
		vf.color = MyColor(1, 1, 1, 1);
		vf.normal = n[i];
		vertices.push_back(vf);
	}
	Mesh* pMesh = new Mesh();
	pMesh->Init(&vertices.at(0), vertices.size(), sizeof(VertexFormat), GL_STATIC_DRAW);

	return pMesh;
}