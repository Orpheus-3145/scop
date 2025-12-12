#pragma once
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm> 

#include "exceptions.hpp"


typedef struct coor2D {
	double _x;
	double _y;

	coor2D( void ) noexcept : _x(.0), _y(.0) {};
	coor2D( double x, double y ) noexcept : _x(x), _y(y) {};
	coor2D( coor2D const& coor ) noexcept;
	explicit coor2D( std::vector<double> const& );
	coor2D& operator=(coor2D const& ) noexcept;
	~coor2D( void ) noexcept {};
} t_coor2D;

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

typedef struct coor4D {
	double _x;
	double _y;
	double _z;
	double _w;

	coor4D( void ) noexcept : _x(.0), _y(.0), _z(.0), _w(.0) {};
	coor4D( double x, double y, double z, double w ) noexcept : _x(x), _y(y), _z(z), _w(w) {};
	coor4D( coor4D const& ) noexcept;
	explicit coor4D( std::vector<double> const& );
	coor4D& operator=( coor4D const&)  noexcept;
	~coor4D( void ) noexcept {};
} t_coor4D;

typedef struct index2D {
	unsigned int _i1;
	unsigned int _i2;

	index2D( void ) noexcept : _i1(0), _i2(0) {};
	index2D( unsigned int i1, unsigned int i2 ) noexcept : _i1(i1), _i2(i2) {};
	index2D( index2D const& ) noexcept;
	explicit index2D( std::vector<unsigned int> const& );
	index2D& operator=(index2D const& ) noexcept;
	~index2D( void ) noexcept {};
} t_index2D;

typedef struct index3D {
	unsigned int _i1;
	unsigned int _i2;
	unsigned int _i3;

	index3D( void ) noexcept : _i1(0), _i2(0), _i3(0) {};
	index3D( unsigned int i1, unsigned int i2, unsigned int i3 ) noexcept : _i1(i1), _i2(i2), _i3(i3) {};
	index3D( index3D const& ) noexcept;
	explicit index3D( std::vector<unsigned int> const& );
	index3D& operator=(index3D const& ) noexcept;
	~index3D( void ) noexcept {};
} t_index3D;

class VertexCoor {
	public:
		explicit VertexCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit VertexCoor( std::vector<double> const& );
		~VertexCoor( void ) noexcept {};

		t_coor3D const& getVertex( void ) const noexcept;

	private:
		// NB add colors save in RGB (t_coor3D) (optional)
		t_coor3D _coor;
};

class TextureCoor {
	public:
		explicit TextureCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit TextureCoor( std::vector<double> const& );
		~TextureCoor( void ) noexcept {};

		t_coor3D const& getTexture( void ) const noexcept;

	private:
		t_coor3D _coor;
};

class VertexNormCoor {
	public:
		explicit VertexNormCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit VertexNormCoor( std::vector<double> const& );
		~VertexNormCoor( void ) noexcept {};

		t_coor3D const& getVertexNorm( void ) const noexcept;

	private:
		t_coor3D _coor;
};

class VertexSpaceParamCoor {
	public:
		explicit VertexSpaceParamCoor( t_coor3D const& coor ) noexcept : _coor(coor) {};
		explicit VertexSpaceParamCoor( std::vector<double> const& );
		~VertexSpaceParamCoor( void ) noexcept {};

		t_coor3D const& getVertexSpaceParam( void ) const noexcept;

	private:
		t_coor3D _coor;
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
		explicit FaceCoor( t_index3D coor ) noexcept : _index(coor) {};
		explicit FaceCoor( std::vector<unsigned int> const& );
		~FaceCoor( void ) noexcept {};

		t_index3D const& getIndex( void ) const noexcept;

	private:
		t_index3D _index;
};

class Face {
	public:
		explicit Face( FaceType type ) noexcept : _type(type), _smoothing(-1) {};
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
		explicit Line(std::vector<unsigned int> const& coors ) noexcept : _coors(coors), _smoothing(-1) {};
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

template <typename T> class RawData {
	public:
		RawData( void ) noexcept : _size(0) ,_dimension(0) {};
		RawData( std::unique_ptr<T[]> const& data, unsigned int size, unsigned int dimension ) noexcept {
			this->_size = size;
			this->_dimension = dimension;
			unsigned int totSize = this->_size * this->_dimension;

			this->_data = std::make_unique<T[]>(totSize);
			std::copy(data.get(), data.get() + totSize, this->_data.get());
		}

		RawData( std::unique_ptr<T[]>&& data, unsigned int size, unsigned int dimension ) noexcept : _data(std::move(data)), _size(size), _dimension(dimension) {};
		RawData( RawData<T> const& other ) noexcept {
			if (this != &other) {
				this->_size = other._size;
				this->_dimension = other._dimension;
				unsigned int totSize = this->_size * this->_dimension;

				this->_data = std::make_unique<T[]>(totSize);
				std::copy(other._data.get(), other._data.get() + totSize, this->_data.get());
			}
		}

		RawData( RawData<T>&& other ) noexcept {
			if (this != &other) {
				this->_size = other._size;	
				this->_dimension = other._dimension;	
				this->_data = std::move(other._data);
			}
		}
		~RawData( void ) noexcept {};

		RawData<T>& operator=(RawData<T> const& other ) {
			if (this != &other) {
				this->_size = other._size;
				this->_dimension = other._dimension;
				unsigned int totSize = this->_size * this->_dimension;

				this->_data.reset(new T[totSize]);
				std::copy(other._data.get(), other._data.get() + totSize, this->_data.get());
			}
			return *this;
		}

		RawData<T>& operator=(RawData<T>&& other ) {
			if (this != &other) {
				this->_size = other._size;
				this->_dimension = other._dimension;
				this->_data = std::move(other._data);
			}
			return *this;
		}

		T* release( void ) noexcept {
			this->_size = 0;
			this->_dimension = 0;
			return this->_data.release();
		}
		T*				getData( void ) const noexcept {return _data.get();};
		unsigned int	getDimension( void ) const noexcept {return _dimension;};
		unsigned int	getSize( void ) const noexcept {return _size;};

	private:
		std::unique_ptr<T[]>	_data;
		unsigned int			_size;
		unsigned int			_dimension;
};

class ObjData {
	public:
		ObjData( void ) noexcept {};
		~ObjData( void ) noexcept {};

		std::vector<std::string> const& 			getTmlFiles( void ) const noexcept;
		std::vector<VertexCoor> const& 				getVertices( void ) const noexcept;
		std::vector<TextureCoor> const& 			getTextureCoors( void ) const noexcept;
		std::vector<VertexNormCoor> const& 			getVerticesNorm( void ) const noexcept;
		std::vector<VertexSpaceParamCoor> const&	getParamSpaceVertices( void ) const noexcept;
		std::vector<Face> const& 					getFaces( void ) const noexcept;
		std::vector<Line> const& 					getLines( void ) const noexcept;

		void addTmlFile( std::string const& ) noexcept;
		void addVertex( VertexCoor const& );
		void addTextureCoor( TextureCoor const& );
		void addVertexNorm( VertexNormCoor const& );
		void addParamSpaceVertex( VertexSpaceParamCoor const& );
		void addFace( Face const& ) noexcept;
		void addLine( Line const& ) noexcept;

		RawData<double>	getVertexData( void ) const noexcept;

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

std::ostream& operator<<( std::ostream&, VertexCoor const& );
std::ostream& operator<<( std::ostream&, TextureCoor const& );
std::ostream& operator<<( std::ostream&, VertexNormCoor const& );
std::ostream& operator<<( std::ostream&, VertexSpaceParamCoor const& );
std::ostream& operator<<( std::ostream&, Face const& );
std::ostream& operator<<( std::ostream&, Line const& );
std::ostream& operator<<( std::ostream&, ObjData const& );
