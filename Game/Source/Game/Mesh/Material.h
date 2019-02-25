#ifndef __Material_H__
#define __Material_H__

class ShaderProgram;

class Material
{
protected:
    ShaderProgram* m_pShader;
    MyColor m_Color;
    GLuint m_Texture;
	float m_Shininess;
    vec3 m_Specular;

public:
    Material() {}
    Material(ShaderProgram* pShader, GLuint texture, float shininess, vec3 specular)
    {
        m_pShader = pShader;
        m_Texture = texture;
		m_Shininess = shininess;
        m_Specular = specular;
    }

    ShaderProgram* GetShader() { return m_pShader;  }
    MyColor GetColor()         { return  m_Color;   }
    GLuint GetTexture()        { return  m_Texture; }
	float GetShininess()       { return m_Shininess;}
    vec3 GetSpecular()         { return m_Specular; }
};

#endif //__Material_H__
