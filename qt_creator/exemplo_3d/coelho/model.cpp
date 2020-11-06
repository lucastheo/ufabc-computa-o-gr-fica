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
    for(size_t i{0};numFaces;i++){
        stream >> notUsed >> a >> b >> c;
        indices[i*3 + 0] = a;
        indices[i*3 + 1] = b;
        indices[i*3 + 2] = c;
    }
    //rescaleModel();
    data.close();


}
