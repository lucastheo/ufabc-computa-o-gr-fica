#include "model.h"

Model::Model()
{
    shaderProgram.resize(fragmentShaderFileName.size());
}

void Model::readOFFFile(QString &fileName ){
    QFile data(fileName);
    data.open(QFile::ReadOnly);
    if( !data.isOpen() ){
        qWarning("Cannot open file");
        return;
    }

    QTextStream stream(&data);
    QString notUsed;

    stream >> notUsed; //OFF
    stream >>numVertives >> numFaces >> notUsed;

    vertices.resize(numVertives);
    indices.resize(3*numFaces);

    float x,y,z;
    for( size_t i{0}; i < numVertives; i++ ){
        stream >> x >> y >> z;
        vertices[i] = QVector4D( x , y , z , 1);
    }
    int a,b,c;
    for(size_t i{0};i<numFaces;i++){
        stream >> notUsed >> a >> b >> c;
        indices[i*3 + 0] = a;
        indices[i*3 + 1] = b;
        indices[i*3 + 2] = c;
    }
    rescaleModel();
    data.close();
}

void Model::computeBBox(){
    auto minLim = std::numeric_limits<float>::lowest();
    auto maxLim = std::numeric_limits<float>::max();

    maxBB = QVector3D( minLim,minLim,minLim);
    minBB = QVector3D( maxLim,maxLim,maxLim);

    for( size_t i{0}; i < numVertives; i++ ){
        maxBB.setX(std::max(maxBB.x(), vertices[i].x()));
        maxBB.setY(std::max(maxBB.y(), vertices[i].y()));
        maxBB.setZ(std::max(maxBB.z(), vertices[i].z()));
        minBB.setX(std::min(minBB.x(), vertices[i].x()));
        minBB.setY(std::min(minBB.y(), vertices[i].y()));
        minBB.setZ(std::min(minBB.z(), vertices[i].z()));
    }

    centrolidBB = 0.5*maxBB + 0.5*minBB;
    diagonalBB = (maxBB - minBB).length();
}

void Model::rescaleModel(){
    computeBBox();
    for( size_t i{0}; i < numVertives; i++){
        vertices[i] = QVector4D( (2.5f/diagonalBB) * vertices[i].toVector3D() - centrolidBB,1);
    }
}
