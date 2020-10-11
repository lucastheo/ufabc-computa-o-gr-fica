#include <domain/figura.h>
Figura::Figura()
{
    this->x = {30};
    this->y = {30};
    this->size_1 = {30};
    this->size_2 = {30};
    this->tipo = {NONE};
}


_tipo_de_figura Figura::get_tipo(){
    return this->tipo;
}

int Figura::get_x(){
    return this->x;
}
int Figura::get_y(){
    return this->y;
}
int Figura::get_size_1(){
    return this->size_1;
}
int Figura::get_size_2(){
    return this->size_2;
}
int Figura::get_color_red(){
    return this->color_red;
}
int Figura::get_color_blue(){
    return this->color_blue;
}
int Figura::get_color_green(){
    return this->color_green;
}
int Figura::get_color_saturation(){
    return this->color_saturation;
}

void Figura::set_x(int _x ){
    this->x = _x;
}
void Figura::set_y(int _y ){
    this->y = _y;
}
void Figura::set_size_1(int _size_1 ){
    this->size_1 = _size_1;
}
void Figura::set_size_2(int _size_2){
    this->size_2 = _size_2;
}
void Figura::set_color_red( int color){
    this->color_red = color;
}
void Figura::set_color_blue(int color ){
    this->color_blue = color;
}
void Figura::set_color_green( int color ){
    this->color_green = color;
}
void Figura::set_color_saturation(int color ){
    this->color_saturation = color;
}



_tipo_de_figura Figura::set_get_tipo(int _tipo ){
    if( _tipo == 0){
        this->tipo = NONE;
    }
    else if( _tipo == 1 ){
        this->tipo = CIRCULO;
    }
    else if( _tipo == 2 ){
        this->tipo = QUADRADO;
    }else if( _tipo == 3 ){
        this->tipo = TRIANGULO;
    }else if( _tipo == 4 ){
        this->tipo = PERSPECTIVA;
    }else if ( _tipo == 5 ){
        this->tipo = LINHA;
    }
    return this->tipo;
}
