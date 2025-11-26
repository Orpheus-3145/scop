#pragma once
#include <vector>
#include <string>


typedef struct coor3D {
    double x;
    double y;
    double z;
} t_coor3D;

typedef struct coor4D {
    double x;
    double y;
    double z;
    double w;
} t_coor4D;

enum FaceType {
    VERTEX,
    VERTEX_TEXT,
    VERTEX_VNORM,
    VERTEX_TEXT_VNORM
};

typedef struct Face {
    FaceType type;
    
} t_Face;


class ObjData {
    private:
        std::vector<std::string> tmlFiles;
        std::vector<t_coor4D> vertices;
        std::vector<t_coor3D> textureCoors;
        std::vector<t_coor3D> verticesNorm;
        std::vector<t_coor3D> paramSpaceVertices;
    public:
}
