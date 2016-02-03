#include "Program.h"

GLuint td::gpu::Program::createShaderFromSource(GLuint shaderType, string source)
{
    printf("Source: ");
    printf(source.c_str());
    printf("\n");

    const GLchar *shaderSource = (const GLchar *)source.c_str();

    GLuint shaderObject = glCreateShader(shaderType);
    glShaderSource(shaderObject, 1, &shaderSource, NULL);

    return shaderObject;
}

GLuint td::gpu::Program::createShaderFromFile(GLuint shaderType, string filePath)
{
    td::log::info("Acquiring shader source from: " + filePath);
    ifstream file(filePath.c_str());

    std::string source = std::string(
         istreambuf_iterator<char>(file),
         istreambuf_iterator<char>()
    );

    return createShaderFromSource(shaderType, source);
}

bool td::gpu::Program::compileShader(GLuint shader)
{
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success)
    {
        char num[15];
        sprintf(num, "%d", shader);

        td::log::info("Shader compiled: " + string(num));
    }
    else
    {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        GLchar infoLog[logSize];
        glGetShaderInfoLog(shader, logSize, &logSize, infoLog);

        glDeleteShader(shader);

        string infoLogString = string((const char *)infoLog);
        td::log::error("Cannot compile shader: " + infoLogString);
    }

    return success;
}

void td::gpu::Program::deleteShader(GLuint shader)
{
    glDeleteShader(shader);

    char num[15];
    sprintf(num, "%d", shader);

    td::log::info("Shader deleted: " + string(num));
}

/*
GLuint td::gpu::Program::createProgramFromFiles(string vertexPath, string fragmentPath)
{
    td::log::info("Creating program from files: \n\t-" + vertexPath + "\n\t-" + fragmentPath);
    GLuint program = GL_FALSE;

    GLuint vertexShader = createShaderFromFile(GL_VERTEX_SHADER, vertexPath);
    if (compileShader(vertexShader))
    {
        GLuint fragmentShader = createShaderFromFile(GL_FRAGMENT_SHADER, fragmentPath);

        if (compileShader(fragmentShader))
        {
            program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);

            GLint isLinked = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

            if (isLinked)
            {
                char num[15];
                sprintf(num, "%d", program);

                td::log::info("Program linked successfuly: " + string(num));
            }
            else
            {
                GLint logSize = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

                GLchar infoLog[logSize];
                glGetProgramInfoLog(program, logSize, &logSize, infoLog);

                glDeleteProgram(program);
                program = GL_FALSE;

                td::log::error("Failed linking program: " + string(infoLog));

            }

            deleteShader(vertexShader);
            deleteShader(fragmentShader);
        }
    }

    char num[15];
    sprintf(num, "%d", program);
    td::log::info("Done creating program: " + string(num));

    return program;
}
*/

bool td::gpu::Program::compile()
{
    m_programId = GL_FALSE;
    GLuint program = glCreateProgram();

    if (m_shaderPaths.vertex.empty())
    {
        td::log::error("No vertex shader path was supplied");
    }
    else if (m_shaderPaths.fragment.empty())
    {
        td::log::error("No fragment shader path was supplied");
    }
    else
    {
        GLuint vertex = createShaderFromFile(GL_VERTEX_SHADER, m_shaderPaths.vertex);
        if (compileShader(vertex))
        {
            GLuint fragment = createShaderFromFile(GL_FRAGMENT_SHADER, m_shaderPaths.fragment);
            if (compileShader(fragment))
            {
                glAttachShader(program, vertex);
                glAttachShader(program, fragment);

                GLint isLinked = GL_FALSE;
                glLinkProgram(program);
                glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

                if (isLinked)
                {
                    char num[15];
                    sprintf(num, "%d", program);

                    td::log::info("Program linked successfuly: " + string(num));
                    m_programId = program;
                }
                else
                {
                    GLint logSize = 0;
                    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

                    GLchar infoLog[logSize];
                    glGetProgramInfoLog(program, logSize, &logSize, infoLog);

                    td::log::error("Failed linking program: " + string(infoLog));

                }

                deleteShader(fragment);
            }

            deleteShader(vertex);
        }
    }

    if (m_programId == GL_FALSE)
    {
        glDeleteProgram(program);
    }

    return m_programId;
}
