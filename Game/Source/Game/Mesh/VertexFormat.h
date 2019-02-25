#ifndef __VertexFormat_H__
#define __VertexFormat_H__

struct VertexFormat
{
    vec3 pos;
    MyColor color;
    vec2 uv;
	vec3 normal;

    VertexFormat() {}
    VertexFormat(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v, vec3 n)
    {
        pos.Set( x, y, z);
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
        uv.Set( u, v );
		normal = n;
    }    

    VertexFormat(float x, float y, float z, MyColor c, float u, float v, vec3 n)
    {
        pos.Set(x, y, z);
        color.r = c.r;
        color.g = c.g;
        color.b = c.b;
        color.a = c.a;
        uv.Set(u, v);
		normal = n;
    }
};

#endif //__VertexFormat_H__
