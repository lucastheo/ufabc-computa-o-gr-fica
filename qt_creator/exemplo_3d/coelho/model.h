#ifndef MODEL_H
#define MODEL_H
#include<vector>
#include<QVector4D>
#include<QVector3D>
#include<QString>
#include<QFile>
#include<QTextStream>

class Model
{
public:
    Model();
    ~Model() = default;
    
    std::vector<QVector4D> vertices;
    std::vector<unsigned> indices; 
    unsigned numVertives{0};
    unsigned numFaces;
    
    unsigned vao{0};
    unsigned vboVertice{0};
    unsigned eboIndices{0};
    
    void readOFFFile( QString &);

    void computeBBox();
    void rescalModel();
    QVector3D centrolidBB, minBB, maxBB;
    float diagonalBB{0.0f};

    int currentShader{0};
    std::vector<QString> vertexShaderFileName = {":shaders/vzdepht.glsl"};
    std::vector<QString> fragmentShaderFileName = {":shaders/fzdepht.glsl"};

    std::vector<unsigned> shaderProgram;

};

#endif // MODEL_H
