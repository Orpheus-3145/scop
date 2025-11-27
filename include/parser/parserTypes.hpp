#pragma once
#include <vector>
#include <string>
#include <variant>


typedef struct coor2D {
	double x;
	double y;

	coor2D( double x, double y) noexcept : x(x) , y(y) {};
	coor2D( coor2D const& coor) noexcept;
	~coor2D( void ) noexcept {};
} t_coor2D;

typedef struct coor3D {
	double x;
	double y;
	double z;

	coor3D( double x, double y, double z) noexcept : x(x) , y(y) , z(z) {};
	coor3D( coor3D const& coor) noexcept;
	~coor3D( void ) noexcept {};
} t_coor3D;

typedef struct coor4D {
	double x;
	double y;
	double z;
	double w;

	coor4D( double x, double y, double z, double w) noexcept : x(x) , y(y) , z(z) , w(w) {};
	coor4D( coor4D const& coor) noexcept;
	~coor4D( void ) noexcept {};
} t_coor4D;

typedef struct index2D {
	unsigned int i1;
	unsigned int i2;

	index2D( unsigned int i1, unsigned int i2) noexcept : i1(i1) , i2(i2) {};
	index2D( index2D const& coor) noexcept;
	~index2D( void ) noexcept {};
} t_index2D;

typedef struct index3D {
	unsigned int i1;
	unsigned int i2;
	unsigned int i3;

	index3D( unsigned int i1, unsigned int i2, unsigned int i3) noexcept : i1(i1) , i2(i2), i3(i3) {};
	index3D( index3D const& coor) noexcept;
	~index3D( void ) noexcept {};
} t_index3D;

class VertexCoor {
	private:
		std::variant<t_coor3D, t_coor4D> vertex;

	public:
		VertexCoor( t_coor3D const& coor) noexcept : vertex(coor) {};
		VertexCoor( t_coor4D const& coor) noexcept : vertex(coor) {};
		~VertexCoor( void ) noexcept {};
};

class TextureCoor {
	private:
		std::variant<double, t_coor2D, t_coor3D> vertex;

	public:
		TextureCoor( double coor ) noexcept : vertex(coor) {};
		TextureCoor( t_coor2D const& coor ) noexcept : vertex(coor) {};
		TextureCoor( t_coor3D const& coor ) noexcept : vertex(coor) {};
		~TextureCoor( void ) noexcept {};
};

using VertexNormCoor = coor3D;

class VertexSpaceParamCoor {
	private:
		std::variant<double, t_coor2D, t_coor3D> vertex;

	public:
		VertexSpaceParamCoor( double coor ) noexcept : vertex(coor) {};
		VertexSpaceParamCoor( t_coor2D const& coor ) noexcept : vertex(coor) {};
		VertexSpaceParamCoor( t_coor3D const& coor ) noexcept : vertex(coor) {};
		~VertexSpaceParamCoor( void ) noexcept {};
};

enum FaceType {
	VERTEX,
	VERTEX_TEXT,
	VERTEX_VNORM,
	VERTEX_TEXT_VNORM
};

class FaceCoor {
	private:
		// convert the index with the actual address of the vertex?
		std::variant<unsigned int, t_index2D, t_index3D> coor;

	public:
		FaceCoor( unsigned int coor ) noexcept : coor(coor) {};
		FaceCoor( t_index2D coor ) noexcept : coor(coor) {};
		FaceCoor( t_index3D coor ) noexcept : coor(coor) {};
		~FaceCoor( void ) noexcept {};
};

class Face {
	private:
		FaceType 				type;
		std::vector<FaceCoor> 	coors;
		std::string 			object;
		std::string 			group;
		std::string 			material;
		int 					smoothing;

	public:
		Face( FaceType type, std::vector<FaceCoor> const& coors) noexcept : type(type) , coors(coors) , smoothing(-1) {};
		~Face( void ) noexcept {};

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( int ) noexcept;

		FaceType 				getFaceType( void ) const noexcept;
		std::vector<FaceCoor> 	getCoors( void ) const noexcept;
		std::string 			getObject( void ) const noexcept;
		std::string 			getGroup( void ) const noexcept;
		std::string 			getMaterial( void ) const noexcept;
		int 					getSmoothing( void ) const noexcept;
};


class ObjData {
	private:
		std::vector<std::string> 			tmlFiles;
		std::vector<VertexCoor> 			vertices;
		std::vector<TextureCoor> 			textureCoors;
		std::vector<VertexNormCoor> 		verticesNorm;
		std::vector<VertexSpaceParamCoor> 	paramSpaceVertices;
		std::vector<Face> 					faces;
		std::vector<std::vector<int>> 		lines;

	public:
		ObjData( void ) noexcept {};
		~ObjData( void ) noexcept {};
		
		void addTmlFile( std::string const& ) noexcept;
		void addVertex( VertexCoor const& ) noexcept;
		void addTextureCoor( TextureCoor const& ) noexcept;
		void addVertexNorm( VertexNormCoor const& ) noexcept;
		void addParamSpaceVertex( VertexSpaceParamCoor const& ) noexcept;
		void addFace( Face const& ) noexcept;
		void addLine( int ) noexcept;

		VertexCoor 				getindexVertex( unsigned int ) const noexcept;
		TextureCoor 			getindexTexture( unsigned int ) const noexcept;
		VertexNormCoor 			getindexVertexNorm( unsigned int ) const noexcept;
		VertexSpaceParamCoor 	getindexVertexSpaceParam( unsigned int ) const noexcept;
};
