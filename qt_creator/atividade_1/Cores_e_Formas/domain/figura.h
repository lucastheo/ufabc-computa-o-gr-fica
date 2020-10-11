#ifndef FIGURA_H
#define FIGURA_H

enum _tipo_de_figura {NONE , CIRCULO, QUADRADO, TRIANGULO , PERSPECTIVA , LINHA};

class Figura
{
    public:
        Figura();
        ~Figura();

        _tipo_de_figura get_tipo();
        int get_x();
        int get_y();
        int get_size_1();
        int get_size_2();
        int get_color_red();
        int get_color_blue();
        int get_color_green();
        int get_color_saturation();

        void set_x(int);
        void set_y(int);
        void set_size_1(int);
        void set_size_2(int);
        void set_color_red(int);
        void set_color_blue(int);
        void set_color_green(int);
        void set_color_saturation(int);
        _tipo_de_figura set_get_tipo(int);

    private:
        int x{10};
        int y{10};
        int size_1{10};
        int size_2{10};
        int color_red{0};
        int color_blue{0};
        int color_green{0};
        int color_saturation{100};
        _tipo_de_figura tipo{NONE};

};

#endif // FIGURA_H
