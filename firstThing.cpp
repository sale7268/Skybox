#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>
#include<algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <chrono>



using namespace std;

int si = 0;


// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.7f, 7.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//bool firstMouse = true;
//float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//float pitch = 0.0f;
//float lastX = 800.0f / 2.0;
//float lastY = 600.0 / 2.0;
//float fov = 45.0f;
//
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//    lastX = xpos;
//    lastY = ypos;
//
//    float sensitivity = 0.1f; // change this value to your liking
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    yaw += xoffset;
//    pitch += yoffset;
//
//    // make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (pitch > 89.0f)
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;
//
//    glm::vec3 front;
//    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front.y = sin(glm::radians(pitch));
//    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFront = glm::normalize(front);
//}

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    if (fov >= 1.0f && fov <= 45.0f)
//        fov -= yoffset;
//    if (fov <= 1.0f)
//        fov = 1.0f;
//    if (fov >= 45.0f)
//        fov = 45.0f;
//}

GLFWwindow* initialization()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        cerr << "GLFW Init failed.";
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Final Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        cerr << "Window failed to open.";
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glfwSetCursorPosCallback(window, mouse_callback);
   // glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Init glew
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);


    return window;
}

struct Model {
    GLuint vao;
    GLuint vbo;
    GLsizei vertex_count;

    void cleanup() {
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(1, &this->vbo);
    }

    void draw() {
        glBindVertexArray(this->vao);
        glDrawArrays(GL_TRIANGLES, 0, this->vertex_count);
    }

    static Model load(string file_name) {
        Model model;

        //load geometry from file:
        vector<GLfloat> vertices;
        {
            fstream file(file_name);
            if (!file) {
                cout << "could not find model file\n";
                abort();
            }

            vector<glm::vec3> positions;
            vector<glm::vec2> texcoords;
            vector<glm::vec3> normals;
            string line;
            while (getline(file, line)) {
                istringstream line_stream(line);
                string type;
                line_stream >> type;

                if (type == "v") {
                    GLfloat x, y, z;
                    line_stream >> x >> y >> z;
                    positions.push_back(glm::vec3(x, y, z));
                }
                else if (type == "vn") {
                    GLfloat x, y, z;
                    line_stream >> x >> y >> z;
                    normals.push_back(glm::vec3(x, y, z));
                }
                else if (type == "vt") {
                    GLfloat u, v;
                    line_stream >> u >> v;
                    texcoords.push_back(glm::vec2(u, v));
                }
                else if (type == "f") {
                    string face;
                    
                    //Vertex0
                    //Converting line into something we can work with
                    line_stream >> face;
                    replace(face.begin(), face.end(), '/', ' ');
                    istringstream face_stream0(face);

                    //Parsing out the indicies
                    size_t position_index0;
                    size_t texcoord_index0;
                    size_t normals_index0;
                    face_stream0 >> position_index0;
                    face_stream0 >> texcoord_index0;
                    face_stream0 >> normals_index0;

                    //Get the values at indicies
                    glm::vec3 position0 = positions.at(position_index0 - 1);
                    glm::vec2 texcoord0 = texcoords.at(texcoord_index0 - 1);
                    glm::vec3 normal0 = normals.at(normals_index0 - 1);

                    //Vertex1
                    //Converting line into something we can work with
                    line_stream >> face;
                    replace(face.begin(), face.end(), '/', ' ');
                    istringstream face_stream1(face);

                    //Parsing out the indicies
                    size_t position_index1;
                    size_t texcoord_index1;
                    size_t normals_index1;
                    face_stream1 >> position_index1;
                    face_stream1 >> texcoord_index1;
                    face_stream1 >> normals_index1;

                    //Get the values at indicies
                    glm::vec3 position1 = positions.at(position_index1 - 1);
                    glm::vec2 texcoord1 = texcoords.at(texcoord_index1 - 1);
                    glm::vec3 normal1 = normals.at(normals_index1 - 1);                    

                    //Vertex2
                    //Converting line into something we can work with
                    line_stream >> face;
                    replace(face.begin(), face.end(), '/', ' ');
                    istringstream face_stream2(face);

                    //Parsing out the indicies
                    size_t position_index2;
                    size_t texcoord_index2;
                    size_t normals_index2;
                    face_stream2 >> position_index2;
                    face_stream2 >> texcoord_index2;
                    face_stream2 >> normals_index2;

                    //Get the values at indicies
                    glm::vec3 position2 = positions.at(position_index2 - 1);
                    glm::vec2 texcoord2 = texcoords.at(texcoord_index2 - 1);
                    glm::vec3 normal2 = normals.at(normals_index2 - 1);        

                    glm::vec3 edge0 = position1 - position0;
                    glm::vec3 edge1 = position2 - position0;
                    glm::vec2 delta_texcoords0 = texcoord1 - texcoord0;
                    glm::vec2 delta_texcoords1 = texcoord2 - texcoord0;

                    float f = 1.0f / (delta_texcoords0.x * delta_texcoords1.y - delta_texcoords1.x * delta_texcoords0.y);

                    glm::vec3 tangent;
                    tangent.x = f * (delta_texcoords1.y * edge0.x - delta_texcoords0.y * edge1.x);
                    tangent.y = f * (delta_texcoords1.y * edge0.y - delta_texcoords0.y * edge1.y);
                    tangent.z = f * (delta_texcoords1.y * edge0.z - delta_texcoords0.y * edge1.z);
                    tangent = glm::normalize(tangent);

                    glm::vec3 bitangent;
                    bitangent.x = f * (-delta_texcoords1.x * edge0.x - delta_texcoords0.x * edge1.x);
                    bitangent.y = f * (-delta_texcoords1.x * edge0.y - delta_texcoords0.x * edge1.y);
                    bitangent.z = f * (-delta_texcoords1.x * edge0.z - delta_texcoords0.x * edge1.z);
                    bitangent = glm::normalize(bitangent);

                    //Add the results to the vertex vector
                    vertices.push_back(position0.x);
                    vertices.push_back(position0.y);
                    vertices.push_back(position0.z);
                    vertices.push_back(texcoord0.x);
                    vertices.push_back(texcoord0.y);
                    vertices.push_back(normal0.x);
                    vertices.push_back(normal0.y);
                    vertices.push_back(normal0.z);
                    vertices.push_back(tangent.x);
                    vertices.push_back(tangent.y);
                    vertices.push_back(tangent.z);
                    vertices.push_back(bitangent.x);
                    vertices.push_back(bitangent.y);
                    vertices.push_back(bitangent.z);

                    vertices.push_back(position1.x);
                    vertices.push_back(position1.y);
                    vertices.push_back(position1.z);
                    vertices.push_back(texcoord1.x);
                    vertices.push_back(texcoord1.y);
                    vertices.push_back(normal1.x);
                    vertices.push_back(normal1.y);
                    vertices.push_back(normal1.z);
                    vertices.push_back(tangent.x);
                    vertices.push_back(tangent.y);
                    vertices.push_back(tangent.z);
                    vertices.push_back(bitangent.x);
                    vertices.push_back(bitangent.y);
                    vertices.push_back(bitangent.z);

                    vertices.push_back(position2.x);
                    vertices.push_back(position2.y);
                    vertices.push_back(position2.z);
                    vertices.push_back(texcoord2.x);
                    vertices.push_back(texcoord2.y);
                    vertices.push_back(normal2.x);
                    vertices.push_back(normal2.y);
                    vertices.push_back(normal2.z);
                    vertices.push_back(tangent.x);
                    vertices.push_back(tangent.y);
                    vertices.push_back(tangent.z);
                    vertices.push_back(bitangent.x);
                    vertices.push_back(bitangent.y);
                    vertices.push_back(bitangent.z);
                
                }
            }
        }

        // Send the vertex data to the GPU
        {
            model.vertex_count = vertices.size();

            // Use OpenGL to store it on the GPU
            {
                // Create a Vertex Buffer Object on the GPU
                glGenBuffers(1, &model.vbo);
                // Tell OpenGL that we want to set this as the current buffer
                glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
                // Copy all our data to the current buffer!
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
            }
        }

        // Tell the GPU how to interpret our existing vertex data
        {
            // Create a Vertex Array Object to hold the settings
            glGenVertexArrays(1, &model.vao);

            // Tell OpenGL that we want to set this as the current vertex array
            glBindVertexArray(model.vao);

            // Tell OpenGL the settings for the current 0th vertex array!
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
            GLsizei stride = 14 * sizeof(GLfloat);

            // Tell OpenGL about the position component
            glVertexAttribPointer(
                0, // index
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized
                stride, // stride (how far to the next repetition)
                (void*)0 // first component
                );
            glEnableVertexAttribArray(0);
            //Tell OpenGL about the UV component
            glVertexAttribPointer(
                1, // index
                2, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized
                stride, // stride (how far to the next repetition)
                (void*)(3 * sizeof(GLfloat)) // first component
                );
            glEnableVertexAttribArray(1);
            //Normal components:
            glVertexAttribPointer(
                2, // index
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized
                stride, // stride (how far to the next repetition)
                (void*)(5 * sizeof(GLfloat)) // first component
                );
            glEnableVertexAttribArray(2);
            //Tangent components
            glVertexAttribPointer(
                3, // index
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized
                stride, // stride (how far to the next repetition)
                (void*)(8 * sizeof(GLfloat)) // first component
                );
            glEnableVertexAttribArray(3);

            //BITangent components
            glVertexAttribPointer(
                4, // index
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized
                stride, // stride (how far to the next repetition)
                (void*)(11 * sizeof(GLfloat)) // first component
                );
            glEnableVertexAttribArray(4);
        }
        return model;
    }

};

struct Camera {
    glm::mat4 camera_from_world = glm::mat4(1);

    float fov_degrees = 60.0f;
    float near = 0.1f;
    float far = 1000.0f;

    glm::mat4 view_from_camera(GLFWwindow* window) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0.0f, 0.0f, width, height);
        return glm::perspective(
            glm::radians(this->fov_degrees),
            (float)width / (float)height,
            this->near,
            this->far
            );
    }
};


struct Particle {


    /*float posx = 0.0;
    float posy = 0.0;*/
    glm::mat4 world_from_model;
    glm::vec3 velocity;

    float velocityx = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.0001f));
    float velocityy = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.0001f));
    int timer = 90000;
    int life = rand() % 4 + 2;
    Particle(glm::mat4 world_from_model, glm::vec3 velocity) {
        this->world_from_model = world_from_model;
        this->velocity = velocity;
    }
    //public:
    //    void update() {
    //        this->posx += velocityx;
    //        this->posy += velocityy;
    //        this->timer -= life;
    //    }

};


GLuint load_texture(GLenum active_texture, const char *filename)
{
    glActiveTexture(active_texture);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Tell OpenGL how to scale the texture. We’re going to use “nearest” for now which pixelates it:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // We’re also going to tell OpenGL to repeat the texture infinitely both horizontally and
    // vertically
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLsizei width = 0;
    GLsizei height = 0;
    GLubyte* pixels = stbi_load(filename, &width, &height, NULL, 3);
    if (pixels == NULL || width == 0 || height == 0) {
        abort();
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(pixels);

    return tex;
    //glDeleteTextures(1, &tex);
}

GLuint load_cubemap(
    GLenum active_texture, 
    const char* left,
    const char* front,
    const char* right,
    const char* back,
    const char* top,
    const char* bottom)
{
    glActiveTexture(active_texture);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    // Tell OpenGL how to scale the texture. We’re going to use “nearest” for now which pixelates it:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // We’re also going to tell OpenGL to repeat the texture infinitely both horizontally and
    // vertically
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    {
        GLsizei width = 0;
        GLsizei height = 0;
        GLubyte* pixels = stbi_load(right, &width, &height, NULL, 3);
        if (pixels == NULL || width == 0 || height == 0) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        //stbi_image_free(pixels);
    }
    {
        GLsizei width = 0;
        GLsizei height = 0;
        GLubyte* pixels = stbi_load(left, &width, &height, NULL, 3);
        if (pixels == NULL || width == 0 || height == 0) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        //stbi_image_free(pixels);
    }
    {
        GLsizei width = 0;
        GLsizei height = 0;
        GLubyte* pixels = stbi_load(top, &width, &height, NULL, 3);
        if (pixels == NULL || width == 0 || height == 0) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        //stbi_image_free(pixels);
    }
    {
        GLsizei width = 0;
        GLsizei height = 0;
        GLubyte* pixels = stbi_load(bottom, &width, &height, NULL, 3);
        if (pixels == NULL || width == 0 || height == 0) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        //stbi_image_free(pixels);
    }
    {
        GLsizei width = 0;
        GLsizei height = 0;
        GLubyte* pixels = stbi_load(front, &width, &height, NULL, 3);
        if (pixels == NULL || width == 0 || height == 0) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        //stbi_image_free(pixels);
    }
    {
        GLsizei width = 0;
        GLsizei height = 0;
        GLubyte* pixels = stbi_load(back, &width, &height, NULL, 3);
        if (pixels == NULL || width == 0 || height == 0) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        //stbi_image_free(pixels);
    }
    return tex;
    //glDeleteTextures(1, &tex);
}

GLuint compile_shader(const char *vertex_shaderFile, const char *fragment_shaderFile) {
    // Define shader sourcecode
    std::ifstream vertex_shader_file(vertex_shaderFile);  // usually .vert for vertex shaders, .frag for fragment shaders 
    std::stringstream vertex_shader_stream;
    vertex_shader_stream << vertex_shader_file.rdbuf();
    std::string vertex_shader_string = vertex_shader_stream.str();
    const char* vertex_shader_c_string = vertex_shader_string.c_str();

    std::ifstream fragment_shader_file(fragment_shaderFile);  // usually .vert for vertex shaders, .frag for fragment shaders 
    std::stringstream fragment_shader_stream;
    fragment_shader_stream << fragment_shader_file.rdbuf();
    std::string fragment_shader_string = fragment_shader_stream.str();
    const char* fragment_shader_c_string = fragment_shader_string.c_str();

    // Define some vars
    const int MAX_ERROR_LEN = 512;
    GLint success;
    GLchar infoLog[MAX_ERROR_LEN];

    // Compile the vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_c_string, NULL);
    glCompileShader(vertex_shader);

    // Check for errors
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, MAX_ERROR_LEN, NULL, infoLog);
        std::cout << "vertex shader compilation failed:\n" << infoLog << std::endl;
        abort();
    }

    // Compile the fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_c_string, NULL);
    glCompileShader(fragment_shader);

    // Check for errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, MAX_ERROR_LEN, NULL, infoLog);
        std::cout << "fragment shader compilation failed:\n" << infoLog << std::endl;
        abort();
    }

    // Link the shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // Check for errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, MAX_ERROR_LEN, NULL, infoLog);
        std::cout << "shader linker failed:\n" << infoLog << std::endl;
        abort();
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}


void render_scene(
    GLFWwindow* window,
    Model model,
    GLuint shader_program,
    GLuint sky_shader_program,
    Particle* particles,
    Camera camera) {

    // Set the clear color
    glClearColor(0.5f, 0.5f, 0.1f, 1.0f);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    


    static float red = 0.0f;
    static float dir = 1.0f;
    if (dir > 0.0) {
        red += 0.001f;
        if (red > 1.0) {
            dir = -1.0;
        }
    }
    else {
        red -= 0.001f;
        if (red < 0.0) {
            dir = 1.0;
        }
    }

    glDepthMask(GL_FALSE);
    //Skybox
    {
        // Enable the shader
        glUseProgram(sky_shader_program);

        //GLint color = glGetUniformLocation(sky_shader_program, "triangleColor");
        GLint color_location = glGetUniformLocation(sky_shader_program, "offset");
        GLuint camera_from_world_location = glGetUniformLocation(sky_shader_program, "camera_from_world");

        GLint skybox_location = glGetUniformLocation(sky_shader_program, "skybox");
        glUniform1i(skybox_location, 3); //GL_TEXTURE3

        glm::mat4 camera_from_world_no_translation = glm::mat4(glm::mat3(camera.camera_from_world));
        //Sending in camera lcoation:
        glUniformMatrix4fv(
            camera_from_world_location,
            1,
            GL_FALSE,
            glm::value_ptr(camera_from_world_no_translation)
            );

        GLuint view_from_camera_location = glGetUniformLocation(sky_shader_program, "view_from_camera");
        //Sending in camera lcoation:
        glUniformMatrix4fv(
            view_from_camera_location,
            1,
            GL_FALSE,
            glm::value_ptr(camera.view_from_camera(window))
            );

        for (int i = 0; i < si; i++) {
            Particle* particle = &(particles[i]);

            particle->world_from_model = glm::translate(
                particle->world_from_model,
                particle->velocity
                );
            //Pass matrix
            GLuint world_from_model_location = glGetUniformLocation(sky_shader_program, "world_from_model");
            glUniformMatrix4fv(
                world_from_model_location,
                1,
                GL_FALSE,
                glm::value_ptr(particle->world_from_model)
                );
            /*particle->world_from_model = glm::rotate(
                particle->world_from_model,
                0.0005f,
                glm::vec3(0.0f, 1.0f, 0.0f)
                );*/

            glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            position = particle->world_from_model * position;
            if (position.x > 2.0) {
                particle->velocity.x = (-abs(particle->velocity.x));
            }
            else if (position.x < -1.0) {
                particle->velocity.x = (abs(particle->velocity.x));
            }

            if (position.y > 2.0) {
                particle->velocity.y = (-abs(particle->velocity.y));
            }
            else if (position.y < -1.0) {
                particle->velocity.y = (abs(particle->velocity.y));
            }
            //glUniform4f(color, 0.0, red, red, 1.0);
            model.draw();
        }
    }
    glDepthMask(GL_TRUE);
    //{
    //    // Enable the shader
    //    glUseProgram(shader_program);

    //    GLint color = glGetUniformLocation(shader_program, "triangleColor");
    //    GLint color_location = glGetUniformLocation(shader_program, "offset");
    //    GLuint camera_from_world_location = glGetUniformLocation(shader_program, "camera_from_world");

    //    GLint skybox_location = glGetUniformLocation(shader_program, "skybox");
    //    glUniform1i(skybox_location, 3); //GL_TEXTURE3

    //    //Diffuse Map
    //    GLint diffuse_map_location = glGetUniformLocation(shader_program, "diffuse_map");
    //    glUniform1i(diffuse_map_location, 0); //GL_Texture0

    //    //specular_map
    //    GLint specular_map_location = glGetUniformLocation(shader_program, "specular_map");
    //    glUniform1i(specular_map_location, 1); //GL_Texture1

    //    //normal_map
    //    GLint normal_map_location = glGetUniformLocation(shader_program, "normal_map");
    //    glUniform1i(normal_map_location, 2); //GL_Texture2

    //    glUniform1i(color_location, 0);

    //    //Sending in camera lcoation:
    //    glUniformMatrix4fv(
    //        camera_from_world_location,
    //        1,
    //        GL_FALSE,
    //        glm::value_ptr(camera.camera_from_world)
    //        );

    //    GLuint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
    //    //Sending in camera lcoation:
    //    glUniformMatrix4fv(
    //        view_from_camera_location,
    //        1,
    //        GL_FALSE,
    //        glm::value_ptr(camera.view_from_camera(window))
    //        );

    //    for (int i = 0; i < si; i++) {
    //        Particle* particle = &(particles[i]);

    //        particle->world_from_model = glm::translate(
    //            particle->world_from_model,
    //            particle->velocity
    //            );
    //        //Pass matrix
    //        GLuint world_from_model_location = glGetUniformLocation(
    //            shader_program,
    //            "world_from_model"
    //            );
    //        glUniformMatrix4fv(
    //            world_from_model_location,
    //            1,
    //            GL_FALSE,
    //            glm::value_ptr(particle->world_from_model)
    //            );
    //        particle->world_from_model = glm::rotate(
    //            particle->world_from_model,
    //            0.0005f,
    //            glm::vec3(0.0f, 1.0f, 0.0f)
    //            );

    //        glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //        position = particle->world_from_model * position;
    //        if (position.x > 2.0) {
    //            particle->velocity.x = (-abs(particle->velocity.x));
    //        }
    //        else if (position.x < -1.0) {
    //            particle->velocity.x = (abs(particle->velocity.x));
    //        }

    //        if (position.y > 2.0) {
    //            particle->velocity.y = (-abs(particle->velocity.y));
    //        }
    //        else if (position.y < -1.0) {
    //            particle->velocity.y = (abs(particle->velocity.y));
    //        }
    //        //glUniform4f(color, 0.0, red, red, 1.0);
    //        model.draw();
    //    }
    //}

    // Display the results on screen
    glfwSwapBuffers(window);
}


void cleanup(GLuint shader_program, GLuint sky_shader_program, Model model, vector<GLuint> *tex) {
    glDeleteTextures(tex->size(), &(*tex)[0]);
    glDeleteProgram(shader_program);
    glDeleteProgram(sky_shader_program);
    model.cleanup();
    glfwTerminate();
}

int main(void)
{

    GLFWwindow* window = initialization();
    vector<GLuint> textures;
    //Camera Matrix:
    Camera camera;

    //Shader programs:
    GLuint shader_program = compile_shader("vertex_shaderFile.txt", "fragment_shaderFile.txt");
    GLuint sky_shader_program = compile_shader("sky.vert.txt", "sky.frag.txt");
    //Creating particle
    vector<Particle> particles;
    particles.push_back(Particle(
        glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.0f)),
        glm::vec3(0.0f, 0.0f, 0.0f)
        ));
    /*particles.push_back(Particle(
        glm::scale(
            glm::translate(
                glm::rotate(
                    glm::mat4(1),
                    0.3f,
                    glm::vec3(0.0f, 1.0f, 0.0f)
                    ),
                glm::vec3(0.0f, 0.0f, 0.0f)
                ),
            glm::vec3(0.0f, 0.0f, 0.0f)
            ),
        glm::vec3(0.0f, 0.0f, 0.0f)
        ));*/
    si = particles.size();

    string cube = "cube-normals.obj";
    string monkey = "monkey-normals.obj";
    //Model loading
    Model model = Model::load(cube);

    /*model = Model::load(monkey);*/
     //Loading textures from file:
    textures.push_back(load_texture(GL_TEXTURE0, "PavingStones036_2K_Color.jpg"));
    textures.push_back(load_texture(GL_TEXTURE1, "PavingStones036_2K_Roughness.jpg"));
    textures.push_back(load_texture(GL_TEXTURE2, "PavingStones036_2K_Normal.jpg"));
    textures.push_back(load_cubemap(
        GL_TEXTURE3,
        "palermo_park/left.png",
        "palermo_park/front.png",
        "palermo_park/right.png",
        "palermo_park/back.png",
        "palermo_park/top.png",
        "palermo_park/bottom.png"
        ));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*bool was_focused = false;
        bool is_focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) == 1;
        glDeleteProgram(shader_program);
        if (!was_focused && is_focused) {
            cout << "gained focus" << endl;
            
            shader_program = compile_shader("vertex_shaderFile.txt", "fragment_shaderFile.txt");
        }
        was_focused = is_focused;*/
        // camera/view transformation
        //camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.02f, 0.0f, 0.0f));
        camera.camera_from_world = glm::rotate(
            camera.camera_from_world,
            0.0001f,
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
        //camera.camera_from_world = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        render_scene(window, model, shader_program, sky_shader_program, &particles[0], camera);

        /* Poll for and process events */
        glfwPollEvents();
    }

    cleanup(shader_program, sky_shader_program, model, &textures);
    return 0;
}
