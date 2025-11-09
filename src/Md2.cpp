#include "Md2.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <iostream>

using namespace md2model;

Md2::Md2(const char *md2FileName, const char *textureFileName) : _texture(std::make_unique<Texture2D>()),
                                                                 _shaderProgram(std::make_unique<ShaderProgram>()),
                                                                 _pause(false),
                                                                 _position(glm::vec3(0.0f, 0.0f, -25.0f)),
                                                                 _modelLoaded(false),
                                                                 _textureLoaded(false),
                                                                 _bufferInitialized(false)
{
    LoadModel(md2FileName);
    LoadTexture(textureFileName);
    InitBuffer();
    _shaderProgram->loadShaders("shaders/basic.vert", "shaders/basic.frag");
}

Md2::~Md2()
{
    // Clean up
    for(size_t i = 0 ; i < _vaoIndices.size(); i++)
    {
        glDeleteVertexArrays(1, &_vaoIndices[i]);
        glDeleteBuffers(1, &_vboIndices[i]);
    }
    
    // Free malloc'd memory from LoadModel
    if(_model)
    {
        if(_model->pointList)
        {
            free(_model->pointList);
        }
        if(_model->st)
        {
            free(_model->st);
        }
        if(_model->triIndx)
        {
            free(_model->triIndx);
        }
    }
}

void Md2::Draw(int frame, float angle, float interpolation, glm::mat4 &view, glm::mat4 &projection)
{
    assert(_modelLoaded && _textureLoaded && _bufferInitialized);
    _texture->bind(0);
    glm::mat4 model;

    // Rotates around the cube center
    model = glm::translate(model, _position) * glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(model, glm::vec3(0.3, 0.3, 0.3));

    _shaderProgram->use();
    _shaderProgram->setUniform("model", model);
    _shaderProgram->setUniform("view", view);
    _shaderProgram->setUniform("projection", projection);
    _shaderProgram->setUniform("modelView", view * model);

    glBindVertexArray(_vaoIndices[frame]);

    GLint count = _frameIndices[frame].second - _frameIndices[frame].first + 1;
    _shaderProgram->setUniform("interpolation", interpolation);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}

void Md2::LoadTexture(const char *textureFileName)
{
    _texture->loadTexture(textureFileName, true);
    _textureLoaded = true;
}

void Md2::InitBuffer()
{
    std::vector<float> md2Vertices;
    int startFrame = 0;
    int endFrame = _model->numFrames - 1;
    md2model::vector *currentFrame;
    md2model::vector *nextFrame;
    _model->currentFrame = startFrame;
    _model->interpol = 0.0f;

    int vertexIndex = 0;
    int startVertex = 0;

    // fill buffer
    while (_model->currentFrame <= endFrame)
    {
        currentFrame = &_model->pointList[_model->numPoints * _model->currentFrame];
        nextFrame = _model->currentFrame == endFrame ? &_model->pointList[_model->numPoints * startFrame] : &_model->pointList[_model->numPoints * (_model->currentFrame + 1)];
        startVertex = vertexIndex;
        for (int index = 0; index < _model->numTriangles; index++)
        {

            // Start of the vertex data
            for (int p = 0; p < 3; p++)
            {
                // current frame
                for (int j = 0; j < 3; j++)
                {
                    // vertices
                    md2Vertices.emplace_back(currentFrame[_model->triIndx[index].meshIndex[p]].point[j]);
                }

                // next frame
                for (int j = 0; j < 3; j++)
                {
                    // vertices
                    md2Vertices.emplace_back(nextFrame[_model->triIndx[index].meshIndex[p]].point[j]);
                }

                // tex coords
                md2Vertices.emplace_back(_model->st[_model->triIndx[index].stIndex[p]].s);
                md2Vertices.emplace_back(_model->st[_model->triIndx[index].stIndex[p]].t);
                vertexIndex++;
            }
            // End of the vertex data
        }
        _frameIndices[_model->currentFrame] = {startVertex, vertexIndex - 1};
        _model->currentFrame++;
    }

    int frameIndex = startFrame;
    unsigned int vbo, vao;

    for (int i = 0; i < _model->numFrames; i++)
    {
        glGenBuffers(1, &vbo);      // Generate an empty vertex buffer on the GPU
        glGenVertexArrays(1, &vao); // Tell OpenGL to create new Vertex Array Object

        auto count = _frameIndices[frameIndex].second - _frameIndices[frameIndex].first + 1;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);                                                                                           // "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float) * 8, &md2Vertices[_frameIndices[frameIndex].first * 8], GL_STATIC_DRAW); // copy the data from CPU to GPU

        glBindVertexArray(vao); // Make it the current one

        // Current Frame Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
        glEnableVertexAttribArray(0);

        // Next  Frame Position attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Texture Coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        frameIndex++;
        _vaoIndices.emplace_back(vao);
        _vboIndices.emplace_back(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(0); // unbind to make sure other code doesn't change it
    _bufferInitialized = true;
}

void Md2::LoadModel(const char *md2FileName)
{
    FILE *fp;
    int length;

    char *buffer;

    header *head;
    textindx *stPtr;

    frame *fra;
    md2model::vector *pntlst;
    mesh *triIndex, *bufIndexPtr;

    fp = fopen(md2FileName, "rb");
    if (!fp)
    {
        std::cerr << "Error: Could not open MD2 file: " << md2FileName << std::endl;
        return;
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = (char *)malloc(length + 1);
    fread(buffer, sizeof(char), length, fp);

    head = (header *)buffer;
    _model.reset((modData *)malloc(sizeof(modData)));

    _model->pointList = (md2model::vector *)malloc(sizeof(md2model::vector) * head->vNum * head->Number_Of_Frames);
    _model->numPoints = head->vNum;
    _model->numFrames = head->Number_Of_Frames;
    _model->frameSize = head->framesize;

    for (int count = 0; count < head->Number_Of_Frames; count++)
    {
        fra = (frame *)&buffer[head->offsetFrames + head->framesize * count];
        pntlst = (md2model::vector *)&_model->pointList[head->vNum * count];
        for (int count2 = 0; count2 < head->vNum; count2++)
        {
            pntlst[count2].point[0] = fra->scale[0] * fra->fp[count2].v[0] + fra->translate[0];
            pntlst[count2].point[1] = fra->scale[1] * fra->fp[count2].v[1] + fra->translate[1];
            pntlst[count2].point[2] = fra->scale[2] * fra->fp[count2].v[2] + fra->translate[2];
        }
    }

    _model->st = (textcoord *)malloc(sizeof(textcoord) * head->tNum);
    _model->numST = head->tNum;
    stPtr = (textindx *)&buffer[head->offsetTCoord];

    for (int count = 0; count < head->tNum; count++)
    {
        _model->st[count].s = (float)stPtr[count].s / (float)head->twidth;
        _model->st[count].t = (float)stPtr[count].t / (float)head->theight;
    }

    triIndex = (mesh *)malloc(sizeof(mesh) * head->fNum);
    _model->triIndx = triIndex;
    _model->numTriangles = head->fNum;
    bufIndexPtr = (mesh *)&buffer[head->offsetIndx];

    for (int count = 0; count < head->Number_Of_Frames; count++)
    {
        for (int count2 = 0; count2 < head->fNum; count2++)
        {
            triIndex[count2].meshIndex[0] = bufIndexPtr[count2].meshIndex[0];
            triIndex[count2].meshIndex[1] = bufIndexPtr[count2].meshIndex[1];
            triIndex[count2].meshIndex[2] = bufIndexPtr[count2].meshIndex[2];

            triIndex[count2].stIndex[0] = bufIndexPtr[count2].stIndex[0];
            triIndex[count2].stIndex[1] = bufIndexPtr[count2].stIndex[1];
            triIndex[count2].stIndex[2] = bufIndexPtr[count2].stIndex[2];
        }
    }

    _model->currentFrame = 0;
    _model->nextFrame = 1;
    _model->interpol = 0.0;

    free(buffer);
    fclose(fp);
    _modelLoaded = true;
}
