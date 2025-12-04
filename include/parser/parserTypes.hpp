#pragma once
#include <vector>
#include <array>
#include <string>
#include <variant>
#include <iostream>

#include "exceptions.hpp"


typedef struct coor2D {
	double _x;
	double _y;

	coor2D( void ) noexcept : _x(.0), _y(.0) {};
	coor2D( double x, double y) noexcept : _x(x), _y(y) {};
	coor2D( coor2D const& coor) noexcept;
	coor2D( std::vector<double> const& );
	coor2D& operator=(coor2D const&) noexcept;
	~coor2D( void ) noexcept {};
} t_coor2D;

typedef struct coor3D {
	double _x;
	double _y;
	double _z;

	coor3D( void ) noexcept : _x(.0), _y(.0), _z(.0) {};
	coor3D( double x, double y, double z) noexcept : _x(x), _y(y), _z(z) {};
	coor3D( coor3D const& coor) noexcept;
	coor3D( std::vector<double> const& );
	coor3D& operator=(coor3D const&) noexcept;
	~coor3D( void ) noexcept {};
} t_coor3D;

typedef struct coor4D {
	double _x;
	double _y;
	double _z;
	double _w;

	coor4D( void ) noexcept : _x(.0), _y(.0), _z(.0), _w(.0) {};
	coor4D( double x, double y, double z, double w) noexcept : _x(x), _y(y), _z(z), _w(w) {};
	coor4D( coor4D const& coor) noexcept;
	coor4D( std::vector<double> const& );
	coor4D& operator=(coor4D const&) noexcept;
	~coor4D( void ) noexcept {};
} t_coor4D;

typedef struct index2D {
	unsigned int _i1;
	unsigned int _i2;

	index2D( void ) noexcept : _i1(0), _i2(0) {};
	index2D( unsigned int i1, unsigned int i2) noexcept : _i1(i1), _i2(i2) {};
	index2D( index2D const& coor) noexcept;
	index2D& operator=(index2D const&) noexcept;
	~index2D( void ) noexcept {};
} t_index2D;

typedef struct index3D {
	unsigned int _i1;
	unsigned int _i2;
	unsigned int _i3;

	index3D( void ) noexcept : _i1(0), _i2(0), _i3(0) {};
	index3D( unsigned int i1, unsigned int i2, unsigned int i3) noexcept : _i1(i1), _i2(i2), _i3(i3) {};
	index3D( index3D const& coor) noexcept;
	index3D& operator=(index3D const&) noexcept;
	~index3D( void ) noexcept {};
} t_index3D;

class VertexCoor {
	public:
		VertexCoor( t_coor3D const& coor) noexcept : _vertex(coor) {};
		VertexCoor( t_coor4D const& coor) noexcept : _vertex(coor) {};
		VertexCoor( std::vector<double> const& );
		~VertexCoor( void ) noexcept {};

		std::variant<t_coor3D, t_coor4D> const& getVertex( void ) const noexcept;

	private:
		std::variant<t_coor3D, t_coor4D> _vertex;
};

class TextureCoor {
	public:
		TextureCoor( double coor ) noexcept : _vertex(coor) {};
		TextureCoor( t_coor2D const& coor ) noexcept : _vertex(coor) {};
		TextureCoor( t_coor3D const& coor ) noexcept : _vertex(coor) {};
		TextureCoor( std::vector<double> const& );
		~TextureCoor( void ) noexcept {};

		std::variant<double, t_coor2D, t_coor3D> const& getTexture( void ) const noexcept;

	private:
		std::variant<double, t_coor2D, t_coor3D> _vertex;
};

class VertexNormCoor {
	public:
		VertexNormCoor( t_coor3D const& coor) noexcept : _vertex(coor) {};
		VertexNormCoor( std::vector<double> const& );
		~VertexNormCoor( void ) noexcept {};

		t_coor3D const& getVertexNorm( void ) const noexcept;

	private:
		t_coor3D _vertex;
};

class VertexSpaceParamCoor {
	public:
		VertexSpaceParamCoor( double coor ) noexcept : _vertex(coor) {};
		VertexSpaceParamCoor( t_coor2D const& coor ) noexcept : _vertex(coor) {};
		VertexSpaceParamCoor( t_coor3D const& coor ) noexcept : _vertex(coor) {};
		VertexSpaceParamCoor( std::vector<double> const& );
		~VertexSpaceParamCoor( void ) noexcept {};

		std::variant<double, t_coor2D, t_coor3D> const& getVertexSpaceParam( void ) const noexcept;

		private:
		std::variant<double, t_coor2D, t_coor3D> _vertex;
};

enum FaceType {
	VERTEX,
	VERTEX_TEXT,
	VERTEX_VNORM,
	VERTEX_TEXT_VNORM
};

class FaceCoor {
	public:
		FaceCoor( void ) noexcept : _index() {};
		FaceCoor( unsigned int coor ) noexcept : _index(coor) {};
		FaceCoor( t_index2D coor ) noexcept : _index(coor) {};
		FaceCoor( t_index3D coor ) noexcept : _index(coor) {};
		FaceCoor( std::vector<unsigned int> const& );
		~FaceCoor( void ) noexcept {};

		std::variant<unsigned int, t_index2D, t_index3D> const& getIndex( void ) const noexcept;

	private:
		// convert the index with the actual address of the vertex?
		std::variant<unsigned int, t_index2D, t_index3D> _index;
};

class Face {
	public:
		Face( FaceType type ) noexcept : _type(type), _smoothing(-1) {};
		Face( FaceType type, std::vector<FaceCoor> const& coors ) noexcept : _type(type), _coors(coors), _smoothing(-1) {};
		~Face( void ) noexcept {};

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( int ) noexcept;

		FaceType 				getFaceType( void ) const noexcept;
		std::vector<FaceCoor>	getCoors( void ) const noexcept;
		std::string 			getObject( void ) const noexcept;
		std::string 			getGroup( void ) const noexcept;
		std::string 			getMaterial( void ) const noexcept;
		int 					getSmoothing( void ) const noexcept;

	private:
		FaceType 				_type;
		std::vector<FaceCoor>	_coors;
		std::string 			_object;
		std::string 			_group;
		std::string 			_material;
		int 					_smoothing;
};

class Line {
	public:
		Line(std::vector<unsigned int> const& coors) noexcept : _coors(coors), _smoothing(-1) {};
		~Line( void ) noexcept {};

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( int ) noexcept;

		std::vector<unsigned int>	getCoors( void ) const noexcept;
		std::string 				getObject( void ) const noexcept;
		std::string 				getGroup( void ) const noexcept;
		std::string 				getMaterial( void ) const noexcept;
		int 						getSmoothing( void ) const noexcept;

	private:
		std::vector<unsigned int>	_coors;
		std::string 				_object;
		std::string 				_group;
		std::string 				_material;
		int 						_smoothing;
};

class ObjData {
	public:
		ObjData( void ) noexcept {};
		~ObjData( void ) noexcept {};
		
		std::vector<std::string> const& 			getTmlFiles( void ) const noexcept;
		std::vector<VertexCoor> const& 				getVertices( void ) const noexcept;
		std::vector<TextureCoor> const& 			getTextureCoors( void ) const noexcept;
		std::vector<VertexNormCoor> const& 			getVerticesNorm( void ) const noexcept;
		std::vector<VertexSpaceParamCoor> const& 	getParamSpaceVertices( void ) const noexcept;
		std::vector<Face> const& 					getFaces( void ) const noexcept;
		std::vector<Line> const& 					getLines( void ) const noexcept;

		void addTmlFile( std::string const& ) noexcept;
		void addVertex( VertexCoor const& ) noexcept;
		void addTextureCoor( TextureCoor const& ) noexcept;
		void addVertexNorm( VertexNormCoor const& ) noexcept;
		void addParamSpaceVertex( VertexSpaceParamCoor const& ) noexcept;
		void addFace( Face const& ) noexcept;
		void addLine( Line const& ) noexcept;

		VertexCoor const& 			getindexVertex( unsigned int ) const;
		TextureCoor const& 			getindexTexture( unsigned int ) const;
		VertexNormCoor const& 		getindexVertexNorm( unsigned int ) const;
		VertexSpaceParamCoor const& getindexVertexSpaceParam( unsigned int ) const;

	private:
		std::vector<std::string> 			_tmlFiles;
		std::vector<VertexCoor> 			_vertices;
		std::vector<TextureCoor> 			_textureCoors;
		std::vector<VertexNormCoor> 		_verticesNorm;
		std::vector<VertexSpaceParamCoor> 	_paramSpaceVertices;
		std::vector<Face> 					_faces;
		std::vector<Line> 					_lines;
};

std::ostream& operator<<(std::ostream& os, const VertexCoor& obj);
std::ostream& operator<<(std::ostream& os, const TextureCoor& obj);
std::ostream& operator<<(std::ostream& os, const VertexNormCoor& obj);
std::ostream& operator<<(std::ostream& os, const VertexSpaceParamCoor& obj);
std::ostream& operator<<(std::ostream& os, const Face& obj);
std::ostream& operator<<(std::ostream& os, const Line& obj);
std::ostream& operator<<(std::ostream& os, const ObjData& obj);
