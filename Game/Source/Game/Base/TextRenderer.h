#pragma once

class ResourceManager;

struct TextVertexFormat
{
    vec2 Position;
    vec2 UV;

    TextVertexFormat() {}
    TextVertexFormat(vec2 pos, vec2 uv)
    {
        Position = pos;
        UV = uv;
    }
};

struct NewLineInfo
{
    float line_start_x;
    vec2 line_end;
};

class TextRenderer
{
private:
	GLuint Texture;
	GLuint VBO;
	GLuint Shader;

    float CharacterWidth;
    float CharacterHeight;

    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;

	ResourceManager* m_pResources;

public:
	TextRenderer(ResourceManager* resourceManager, unsigned int width, unsigned int height, unsigned int windowWidth, unsigned int windowHeight);
	~TextRenderer();
	void initText2D();
	void printText2D(const char * text, float x, float y, float previous_line_start_x, float size, NewLineInfo* out_newlineinfo, float spacing = 0);

    float GetCharacterWidth() { return CharacterWidth; }
    float GetCharacterHeight() { return CharacterHeight; }

    void OnSurfaceChanged(unsigned int width, unsigned int height) {
        m_WindowWidth = width;
        m_WindowHeight = height;
    }
};
