Terrain::Terrain() {

}

Terrain::Terrain(int levels) {
	size = ( 2 << (levels + 1) ) - 1;
	e_t = new float[size];
}

Terrain::~Terrain() {
	
}