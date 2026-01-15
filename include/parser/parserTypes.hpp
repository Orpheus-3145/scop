#pragma once
#include <vector>
#include <array>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm> 

#include "exceptions.hpp"

// remove typedef e t_*
typedef struct coor3D {
	double _x;
	double _y;
	double _z;

	coor3D( void ) noexcept : _x(.0), _y(.0), _z(.0) {};
	coor3D( double x, double y, double z ) noexcept : _x(x), _y(y), _z(z) {};
	coor3D( coor3D const& coor ) noexcept;
	explicit coor3D( std::vector<double> const& );
	coor3D& operator=(coor3D const& ) noexcept;
	~coor3D( void ) noexcept {};
} t_coor3D;

// remove typedef e t_*
typedef struct index3D {
	unsigned int _i1;
	unsigned int _i2;
	unsigned int _i3;

	index3D( void ) noexcept : _i1(0), _i2(0), _i3(0) {};
	index3D( unsigned int i1, unsigned int i2, unsigned int i3 ) noexcept : _i1(i1), _i2(i2), _i3(i3) {};
	index3D( index3D const& ) noexcept;
	explicit index3D( std::vector<unsigned int> const& ) noexcept;
	index3D& operator=(index3D const& ) noexcept;
	~index3D( void ) noexcept {};
} t_index3D;

// NB remove this 4 classes and use only coor3D and index3D?
class VertexCoor {
	public:
		explicit VertexCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit VertexCoor( std::vector<double> const& );
		~VertexCoor( void ) noexcept {};

		t_coor3D const& getVertex( void ) const noexcept;
		double	x( void ) const noexcept;
		double	y( void ) const noexcept;
		double	z( void ) const noexcept;

	private:
		t_coor3D _coor;
};

class TextureCoor {
	public:
		explicit TextureCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit TextureCoor( std::vector<double> const& );
		~TextureCoor( void ) noexcept {};

		t_coor3D const& getTexture( void ) const noexcept;
		double	x( void ) const noexcept;
		double	y( void ) const noexcept;
		double	z( void ) const noexcept;

	private:
		t_coor3D _coor;
};

class VertexNormCoor {
	public:
		explicit VertexNormCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit VertexNormCoor( std::vector<double> const& );
		~VertexNormCoor( void ) noexcept {};

		t_coor3D const& getVertexNorm( void ) const noexcept;
		double	x( void ) const noexcept;
		double	y( void ) const noexcept;
		double	z( void ) const noexcept;

	private:
		t_coor3D _coor;
};

class VertexSpaceParamCoor {
	public:
		explicit VertexSpaceParamCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit VertexSpaceParamCoor( std::vector<double> const& );
		~VertexSpaceParamCoor( void ) noexcept {};

		t_coor3D const& getVertexSpaceParam( void ) const noexcept;
		double	x( void ) const noexcept;
		double	y( void ) const noexcept;
		double	z( void ) const noexcept;

	private:
		t_coor3D _coor;
};

enum FaceType {
	VERTEX,
	VERTEX_TEXT,
	VERTEX_VNORM,
	VERTEX_TEXT_VNORM
};

std::string	faceToString( FaceType );

class Face {
	public:
		explicit Face( FaceType type ) noexcept : _type(type), _smoothing(0) {};
		Face( FaceType type, std::vector<t_index3D> const& coors ) noexcept : _type(type), _coors(coors), _smoothing(0) {};
		~Face( void ) noexcept {};

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( unsigned int ) noexcept;

		FaceType 						getFaceType( void ) const noexcept;
		std::vector<t_index3D> const&	getCoors( void ) const noexcept;
		std::string 					getObject( void ) const noexcept;
		std::string 					getGroup( void ) const noexcept;
		std::string 					getMaterial( void ) const noexcept;
		unsigned int 					getSmoothing( void ) const noexcept;

	private:
		FaceType 				_type;
		std::vector<t_index3D>	_coors;
		std::string 			_object;
		std::string 			_group;
		std::string 			_material;
		unsigned int 			_smoothing;
};

class Line {
	public:
		explicit Line(std::vector<unsigned int> const& coors ) noexcept : _coors(coors), _smoothing(0) {};
		~Line( void ) noexcept {};

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( unsigned int ) noexcept;

		std::vector<unsigned int> const&	getCoors( void ) const noexcept;
		std::string 						getObject( void ) const noexcept;
		std::string 						getGroup( void ) const noexcept;
		std::string 						getMaterial( void ) const noexcept;
		unsigned int 						getSmoothing( void ) const noexcept;

	private:
		std::vector<unsigned int>	_coors;
		std::string 				_object;
		std::string 				_group;
		std::string 				_material;
		unsigned int				_smoothing;
};

struct VBO {
	unsigned int				size;
	unsigned int				stride;
	FaceType					type;
	std::unique_ptr<float[]>	data;

	float const*	getData( void ) const;
	FaceType		getType( void ) const;
};

struct EBO {
	unsigned int					size;
	unsigned int					stride;
	FaceType						type;
	std::unique_ptr<unsigned int[]>	data;

	unsigned int const*	getData( void ) const;
};

class ParsedData {
	public:
		ParsedData( void ) noexcept {};
		~ParsedData( void ) noexcept {};

		std::vector<std::string> const& 				getTmlFiles( void ) const noexcept;
		std::vector<VertexCoor> const& 					getVertices( void ) const noexcept;
		std::vector<TextureCoor> const& 				getTextureCoors( void ) const noexcept;
		std::vector<VertexNormCoor> const& 				getVerticesNorm( void ) const noexcept;
		std::vector<VertexSpaceParamCoor> const&		getParamSpaceVertices( void ) const noexcept;
		std::map<FaceType, std::vector<Face>> const& 	getFaces( void ) const noexcept;
		std::vector<Line> const& 						getLines( void ) const noexcept;

		void addTmlFile( std::string const& ) noexcept;
		void addVertex( VertexCoor const& );
		void addTextureCoor( TextureCoor const& );
		void addVertexNorm( VertexNormCoor const& );
		void addParamSpaceVertex( VertexSpaceParamCoor const& );
		void addFace( Face const& ) noexcept;
		void addLine( Line const& ) noexcept;

		std::shared_ptr<VBO>	createVBO( FaceType ) const;
		std::shared_ptr<VBO>	createVBO( void ) const noexcept;
		std::shared_ptr<EBO>	createEBO( FaceType ) const;

	private:
		std::vector<std::string> 				_tmlFiles;
		std::vector<VertexCoor> 				_vertices;
		std::vector<TextureCoor> 				_textureCoors;
		std::vector<VertexNormCoor> 			_verticesNorm;
		std::vector<VertexSpaceParamCoor> 		_paramSpaceVertices;
		std::map<FaceType,std::vector<Face>>	_faces;
		std::vector<Line> 						_lines;
};

std::ostream& operator<<( std::ostream&, VertexCoor const& );
std::ostream& operator<<( std::ostream&, TextureCoor const& );
std::ostream& operator<<( std::ostream&, VertexNormCoor const& );
std::ostream& operator<<( std::ostream&, VertexSpaceParamCoor const& );
std::ostream& operator<<( std::ostream&, FaceType );
std::ostream& operator<<( std::ostream&, Face const& );
std::ostream& operator<<( std::ostream&, Line const& );
std::ostream& operator<<( std::ostream&, VBO const& );
std::ostream& operator<<( std::ostream&, EBO const& );
std::ostream& operator<<( std::ostream&, ParsedData const& );
