#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <cstdlib>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <map>
#include <memory>

class ShaderProgram;
class Texture2D;

namespace md2model
{
    struct header
    {
        int id;
        int version;
        int twidth;
        int theight;
        int framesize;
        int textures;
        int vNum;
        int tNum;
        int fNum;
        int numGLcmds;
        int Number_Of_Frames;
        int offsetSkins;
        int offsetTCoord;
        int offsetIndx;
        int offsetFrames;
        int offsetGLcmds;
        int offsetEnd;
    };

    struct textcoord
    {
        float s;
        float t;
    };

    struct textindx
    {
        short s;
        short t;
    };

    struct framePoint_t
    {
        unsigned char v[3];
        unsigned char normalIndex;
    };

    struct frame
    {
        float scale[3];
        float translate[3];
        char name[16];
        framePoint_t fp[1];
    };

    struct mesh
    {
        unsigned short meshIndex[3];
        unsigned short stIndex[3];
    };

    struct vector
    {
        float point[3];
    };

    struct modData
    {
        int numFrames;
        int numPoints;
        int numTriangles;
        int numST;
        int frameSize;
        int twidth;
        int theight;
        int currentFrame;
        int nextFrame;
        float interpol;
        mesh *triIndx;
        textcoord *st;
        md2model::vector *pointList;
        float x, y, z;
        float nextX, nextY, nextZ;
        float radius;
        float dist_to_player;
        int state;
        float speed;
    };

    class Md2
    {
    public:
        Md2(const char *md2FileName, const char *textureFileName);
        ~Md2();
        // The frame parameter start at 0
        void Draw(int frame, float angle, float interpolation, glm::mat4 &view, glm::mat4 &projection);
        void SetPause(bool pause) { _pause = pause; }

    private:
        void LoadModel(const char *md2FileName);
        void LoadTexture(const char *textureFileName);
        void InitBuffer();

        std::unique_ptr<modData> _model;
        std::unique_ptr<Texture2D> _texture;
        std::vector<GLuint> _vaoIndices;
        std::vector<GLuint> _vboIndices;
        std::unique_ptr<ShaderProgram> _shaderProgram;
        std::map<int, std::pair<int, int>> _frameIndices;
        bool _pause;
        glm::vec3 _position;
        bool _modelLoaded;
        bool _textureLoaded;
        bool _bufferInitialized;
    };
}
