#pragma once


struct Rangei
{
	Rangei(int mi, int ma)
		: min(mi),
		max(ma)
	{}

	int min;
	int max;
};
struct Rangef
{
	Rangef(float mi, float ma)
		: min(mi),
		max(ma)
	{}
	float min;
	float max;
};


inline float randomRange(float min, float max) {
	return min + (((float)(rand()%RAND_MAX)/(float)RAND_MAX) * (max-min));
}
inline float randomRange(Rangef range) {
	return randomRange(range.min, range.max);
}

inline float randomAngle() {
	return randomRange(0.0f,osg::PI*2.0f);
}

inline std::vector<float> randomRangeVector(const unsigned int& size, Rangef range){
	std::vector<float> vec;
	for(unsigned int i=0; i<size; i++)
		vec.push_back(randomRange(range));
	return vec;
}

