#include "GamePCH.h"

TextRenderer::TextRenderer(ResourceManager* resourceManager, unsigned int width, unsigned int height, unsigned int windowW, unsigned int windowH)
{
	Texture = 0;
	VBO = 0;
	Shader = 0;

	m_pResources = resourceManager;

    // The texture must be in the exact same format (order of characters must be exactly the same)
    CharacterWidth = width / 16.0f;
    CharacterHeight = height / 6.0f;

    m_WindowWidth = windowW;
    m_WindowHeight = windowH;

    initText2D();
}

TextRenderer::~TextRenderer()
{
	// Delete buffers
	glDeleteBuffers(1, &VBO);
}

void TextRenderer::initText2D() 
{
	// Initialize texture
	Texture = m_pResources->GetTexture("Text");

	// Initialize VBO
	glGenBuffers(1, &VBO);

	// Initialize Shader
	Shader = m_pResources->GetShader("Text")->GetProgram();
}

void TextRenderer::printText2D(const char * text, float x, float y, float previous_line_start_x, float size, NewLineInfo* out_newlineinfo, float spacing) 
{
	unsigned int length = strlen(text);

	// Make sure the characters are on our font map
	for (unsigned int i = 0; i < length; i++)
	{
		char character = text[i] - 32;
		assert(character >= 0 && character <= 94);
	}

	// Fill the buffer
    std::vector<TextVertexFormat> vertices;

    float aspectRatio = CharacterHeight / CharacterWidth;

    int WrapCount = 0;
	int max_chars_in_a_row = m_WindowWidth / (size + spacing) - 1;
	int chars_in_a_row = 0;

	for (unsigned int i = 0; i < length; i++) 
	{
        // Vertex positions
        vec2 vertex_up_left;
        vec2 vertex_up_right;
        vec2 vertex_down_right;
        vec2 vertex_down_left;

        if (WrapCount < 1)
        {
            vertex_up_left = vec2(x + i * (size + spacing), y + size * aspectRatio);
            vertex_up_right = vec2(x + i * (size + spacing) + size, y + size * aspectRatio);
            vertex_down_right = vec2(x + i * (size + spacing) + size, y);
            vertex_down_left = vec2(x + i * (size + spacing), y);
			chars_in_a_row++;
        }
        else
        {
            // Wrap text if it exceeds screen width
			int	x_index = i - (WrapCount * max_chars_in_a_row);
            vertex_up_left = vec2(previous_line_start_x + x_index * (size + spacing), y + size * aspectRatio - WrapCount * CharacterHeight);
            vertex_up_right = vec2(previous_line_start_x + x_index * (size + spacing) + size, y + size * aspectRatio - WrapCount * CharacterHeight);
            vertex_down_right = vec2(previous_line_start_x + x_index * (size + spacing) + size, y - WrapCount * CharacterHeight);
            vertex_down_left = vec2(previous_line_start_x + x_index * (size + spacing), y - WrapCount * CharacterHeight);
        }

        if (vertex_down_right.x >= m_WindowWidth - 2*CharacterWidth)
        {
            WrapCount++;
        }

        // Update the end of this line
        if (out_newlineinfo)
        {
            out_newlineinfo->line_start_x = previous_line_start_x;
            out_newlineinfo->line_end = vertex_down_right;
            // If the last character is already at the right edge of screen, reset line_end to a new line
            if (out_newlineinfo->line_end.x >= m_WindowWidth - 2*CharacterWidth)
                out_newlineinfo->line_end = vec2(previous_line_start_x, vertex_down_right.y - CharacterHeight);
        }

        // Get the ACSII code of each character
        // -32 to get rid of the two empty rows
        char character = text[i] - 32;
        // Mod/Divide by total columns (16), get the column and row of the character
        // Divided by 16(total col) or 6(total row), get the actual uv within 0-1 range
        int uv_x = (character % 16);
        int uv_y = (character / 16);
        float uv_xf = uv_x / 16.0f;
        float uv_yf = uv_y / 6.0f;

        // UVs
        vec2 uv_up_left = vec2(uv_xf, 1.0f - uv_yf);
        vec2 uv_up_right = vec2(uv_xf + 1.0f / 16.0f, 1.0f - uv_yf);
        vec2 uv_down_right = vec2(uv_xf + 1.0f / 16.0f, 1.0f - uv_yf - 1.0f / 6.0f);
        vec2 uv_down_left = vec2(uv_xf, 1.0f - uv_yf - 1.0f / 6.0f);
        
        // Not using IBO
        TextVertexFormat TextVertex[6];
        TextVertex[0] = TextVertexFormat(vertex_up_left, uv_up_left);
        TextVertex[1] = TextVertexFormat(vertex_up_right, uv_up_right);
        TextVertex[2] = TextVertexFormat(vertex_down_right, uv_down_right);
        TextVertex[3] = TextVertexFormat(vertex_down_right, uv_down_right);
        TextVertex[4] = TextVertexFormat(vertex_down_left, uv_down_left);
        TextVertex[5] = TextVertexFormat(vertex_up_left, uv_up_left);

        for (int i = 0; i < 6; i++)
            vertices.push_back(TextVertex[i]);
	}

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TextVertexFormat), &vertices[0], GL_STATIC_DRAW);

    glUseProgram(Shader);

    // Pass in texture
    GLuint uTexture = glGetUniformLocation(Shader, "u_Texture");
    if (uTexture != -1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(uTexture, 0);
    }

    // Pass in position and uv
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GLint aPos = glGetAttribLocation(Shader, "a_Position");
    assert(aPos != -1);
    glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexFormat), (void*)offsetof(TextVertexFormat, Position));
    glEnableVertexAttribArray(aPos);

    GLint aUV = glGetAttribLocation(Shader, "a_UV");
    assert(aUV != -1);
    glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertexFormat), (void*)offsetof(TextVertexFormat, UV));
    glEnableVertexAttribArray(aUV);

    // Pass in screen height and width
    GLint uScreenSize = glGetUniformLocation(Shader, "u_ScreenSize");
    assert(uScreenSize != -1);
    glUniform2f(uScreenSize, (float)m_WindowWidth, (float)m_WindowHeight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw call
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    //glDisable(GL_BLEND);
	
}