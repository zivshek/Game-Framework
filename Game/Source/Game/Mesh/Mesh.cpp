#include "GamePCH.h"

Mesh::Mesh()
{
    m_VBO = 0;
    m_IBO = 0;
	glPointSize(5);
    m_PrimitiveType = GL_TRIANGLES;
}

Mesh::~Mesh()
{
    glDeleteBuffers( 1, &m_VBO );
    glDeleteBuffers( 1, &m_IBO );
}

void Mesh::Init(VertexFormat* verts, int numverts, unsigned int* indices, int numindices, GLenum usage)
{
    m_NumVerts = numverts;
    m_NumIndices = numindices;

    // Generate and fill buffer with our vertex attributes.
    if( m_VBO == 0 )
        glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*numverts, verts, usage );

    // Generate and fill buffer with our indices.
    if( m_IBO == 0 )
        glGenBuffers( 1, &m_IBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numindices, indices, usage );

    CheckForGLErrors();
}

void Mesh::Init(const void* verts, int numverts, int vertexbytesize, GLenum usage)
{
    m_NumVerts = numverts;
    m_NumIndices = 0;

    // Generate and fill buffer with our vertex attributes.
    if( m_VBO == 0 )
        glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, numverts * vertexbytesize, verts, usage );

    CheckForGLErrors();
}

void Mesh::SetupAttributes(ShaderProgram* pShaderProgram)
{
    assert( m_VBO != 0 );

    // Bind our vertex and index buffers.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    if( m_IBO != 0 )
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );

    GLuint programhandle = pShaderProgram->GetProgram();

    // Get the attribute locations.
    GLint aPos = glGetAttribLocation( programhandle, "a_Position" );
    GLint aColor = glGetAttribLocation( programhandle, "a_Color" );
    GLint aUV = glGetAttribLocation( programhandle, "a_UV" );
    GLint aNormal = glGetAttribLocation( programhandle, "a_Normal" );

    // Setup our vbo and attributes.
    {
        // Setup the position attribute.
        assert( aPos != -1 );
        glVertexAttribPointer( aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, pos));
        glEnableVertexAttribArray( aPos );

        // Setup the color attribute.
        if( aColor != -1 )
        {
            glVertexAttribPointer( aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, color) );
            glEnableVertexAttribArray( aColor );
        }

        // Setup the uv attribute.
        if( aUV != -1 )
        {
            glVertexAttribPointer( aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, uv) );
            glEnableVertexAttribArray( aUV );
        }

		// Setup the normal attribute
		if (aNormal != -1)
		{
			glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, normal));
			glEnableVertexAttribArray(aNormal);
		}
    }
}

void Mesh::SetupUniforms(Material* pMaterial, mat4* pMatWorld, mat4* pMatView, mat4* pMatProj, vec3 campos)
{
    ShaderProgram* pShaderProgram = pMaterial->GetShader();
    GLuint programhandle = pShaderProgram->GetProgram();

    // Enable our shader program.
    glUseProgram( programhandle );

    // Get the uniform locations.
    GLint uWorldMatrix = glGetUniformLocation( programhandle, "u_WorldMatrix" );
    GLint uViewMatrix  = glGetUniformLocation( programhandle, "u_ViewMatrix" );
    GLint uProjMatrix  = glGetUniformLocation( programhandle, "u_ProjMatrix" );
    GLint uCameraPosition = glGetUniformLocation( programhandle, "u_CameraPosition" );
    GLint uTexture = glGetUniformLocation( programhandle, "u_Material.texture" );
	GLint uShininess = glGetUniformLocation(programhandle, "u_Material.shininess");
    GLint uSpecular = glGetUniformLocation(programhandle, "u_Material.specular");
    GLint uTime = glGetUniformLocation( programhandle, "u_Time" );
    GLint uCubemap = glGetUniformLocation(programhandle, "u_Skybox");

    // Setup the texture.
    GLuint texturehandle = pMaterial->GetTexture();
    if( texturehandle != 0 && uTexture != -1 )
    {
        glActiveTexture( GL_TEXTURE0 + 0 );
        glBindTexture( GL_TEXTURE_2D, texturehandle );
        glUniform1i( uTexture, 0 );
    }

    if (uCubemap != -1 && texturehandle != -1)
    {
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texturehandle); 
    }

    if( uWorldMatrix != -1 )
    {
        glUniformMatrix4fv( uWorldMatrix, 1, false, &pMatWorld->m11 );
    }
    
    if( uViewMatrix != -1 )
    {
        glUniformMatrix4fv( uViewMatrix, 1, false, &pMatView->m11 );
    }
    
    if( uProjMatrix != -1 )
    {
        glUniformMatrix4fv( uProjMatrix, 1, false, &pMatProj->m11 );
    }

    if( uCameraPosition != -1 )
    {
        glUniform3f( uCameraPosition, campos.x, campos.y, campos.z );
    }

	if (uShininess != -1)
	{
		float shininess = pMaterial->GetShininess();
		glUniform1f(uShininess, shininess);
	}

    if (uSpecular != -1)
    {
        vec3 specular = pMaterial->GetSpecular();
        glUniform3f(uSpecular, specular.x, specular.y, specular.z);
    }
    // Setup time.
    if( uTime != -1 )
    {
        glUniform1f( uTime, (float)GetSystemTime() );
    }

    CheckForGLErrors();
}

void Mesh::SetupLighting(Material* pMaterial, std::map<int, Light*>& lights)
{
    ShaderProgram* pShaderProgram = pMaterial->GetShader();
    GLuint programhandle = pShaderProgram->GetProgram();

    // Enable our shader program.
    glUseProgram(programhandle);

    GLint uNumOfLights = glGetUniformLocation(programhandle, "u_NumOfLights");
    if (uNumOfLights != -1) glUniform1i(uNumOfLights, lights.size());

    // Get the uniform locations.
	int i = 0;
    for (auto const& light : lights)
    {
        std::string name = "u_Lights[" + std::to_string(i) + "].position";
        GLint uPosition = glGetUniformLocation(programhandle, name.c_str());
        name = "u_Lights[" + std::to_string(i) + "].color";
        GLint uLightColor = glGetUniformLocation(programhandle, name.c_str());
        name = "u_Lights[" + std::to_string(i) + "].ambient";
        GLint uAmbient = glGetUniformLocation(programhandle, name.c_str());
        name = "u_Lights[" + std::to_string(i) + "].coneAngle";
        GLint uConeAngle = glGetUniformLocation(programhandle, name.c_str());
        name = "u_Lights[" + std::to_string(i) + "].coneDirection";
        GLint uConeDir = glGetUniformLocation(programhandle, name.c_str());
        name = "u_Lights[" + std::to_string(i) + "].linear";
        GLint uLinear = glGetUniformLocation(programhandle, name.c_str());
        name = "u_Lights[" + std::to_string(i) + "].quadratic";
        GLint uQuadratic = glGetUniformLocation(programhandle, name.c_str());
		name = "u_Lights[" + std::to_string(i) + "].on";
		GLint uOn = glGetUniformLocation(programhandle, name.c_str());
		name = "u_Lights[" + std::to_string(i) + "].flicker";
		GLint uFlicker = glGetUniformLocation(programhandle, name.c_str());

		i++;

        if (uPosition != -1)
        {
            glUniform3f(uPosition, light.second->position.x, light.second->position.y, light.second->position.z);
        }

        if (uLightColor != -1)
        {
            glUniform3f(uLightColor, light.second->color.x, light.second->color.y, light.second->color.z);
        }

        if (uAmbient != -1)
        {
            glUniform1f(uAmbient, light.second->ambient);
        }

        if (uConeAngle != -1)
        {
            glUniform1f(uConeAngle, light.second->coneAngle);
        }

        if (uConeDir != -1)
        {
            glUniform3f(uConeDir, light.second->coneDirection.x, light.second->coneDirection.y, light.second->coneDirection.z);
        }

        if (uLinear != -1)
        {
            glUniform1f(uLinear, light.second->attenuation.x);
        }

        if (uQuadratic != -1)
        {
            glUniform1f(uQuadratic, light.second->attenuation.y);
        }

		if (uOn != -1)
		{
			glUniform1i(uOn, light.second->on);
		}

		if (uFlicker != -1)
		{
			glUniform1i(uFlicker, light.second->flicker);
		}

        CheckForGLErrors();
    }
}

void Mesh::Draw(ShaderProgram* pShaderProgram)
{
    GLuint programhandle = pShaderProgram->GetProgram();

    // Draw the shape.
    if( m_NumIndices > 0 )
    {
        glDrawElements( m_PrimitiveType, m_NumIndices, GL_UNSIGNED_INT, 0 );
    }
    else
    {
        glDrawArrays( m_PrimitiveType, 0, m_NumVerts );
    }

    GLint aPos = glGetAttribLocation( programhandle, "a_Position" );
    GLint aColor = glGetAttribLocation( programhandle, "a_Color" );
    GLint aUV = glGetAttribLocation( programhandle, "a_UV" );
    GLint aNormal = glGetAttribLocation( programhandle, "a_Normal" );

    // Disable the attribute arrays used
    glDisableVertexAttribArray( aPos );

    if( aColor != -1 )
        glDisableVertexAttribArray( aColor );

    if( aUV != -1 )
        glDisableVertexAttribArray( aUV );

    if( aNormal != -1 )
        glDisableVertexAttribArray( aNormal );

    CheckForGLErrors();
}

void Mesh::Setup2DUniforms(ShaderProgram* shader, GLint texture, vec2 pos, vec2 proj)
{
	ShaderProgram* pShaderProgram = shader;
	GLuint programhandle = pShaderProgram->GetProgram();

	// Enable our shader program.
	glUseProgram(programhandle);

	// Get the uniform locations.
	GLint uPosition = glGetUniformLocation(programhandle, "u_Position");
	GLint uProjection = glGetUniformLocation(programhandle, "u_Projection");
	GLint uTexture = glGetUniformLocation(programhandle, "u_Texture");
    GLint uOffset = glGetUniformLocation(programhandle, "u_Offset");
	
	CheckForGLErrors();

	if (uTexture != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(uTexture, 0);
	}

	CheckForGLErrors();

	if (uPosition != -1)
	{
		glUniform2f(uPosition, pos.x, pos.y);
	}
	CheckForGLErrors();

	if (uProjection != -1)
	{
		glUniform2f(uProjection, proj.x, proj.y);
	}
	CheckForGLErrors();

    // This is for postprocessing
    if (uOffset != -1)
    {
        float time = (float)GetSystemTime();
        glUniform1f(uOffset, time / 2 * 3.14159f * 0.75f);
    }
    CheckForGLErrors();
}

void Mesh::SetupIngameUniforms(ShaderProgram * shader, bool poisoned)
{
    ShaderProgram* pShaderProgram = shader;
    GLuint programhandle = pShaderProgram->GetProgram();

    // Enable our shader program.
    glUseProgram(programhandle);

    // Get the uniform locations.
    GLint uPoisoned = glGetUniformLocation(programhandle, "u_Poisoned");

    CheckForGLErrors();

    if (uPoisoned != -1)
    {
        glUniform1i(uPoisoned, poisoned);
    }
    CheckForGLErrors();
}
