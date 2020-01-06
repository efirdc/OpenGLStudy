#ifndef _SDF
#define _SDF

#include "material.glsl"

struct sdResult
{
    float d;
    BlinnPhongMaterial mat;
};

sdResult sdUnion(sdResult a, sdResult b)
{
    return a.d < b.d ? a : b;
}

sdResult sdSmoothUnion(sdResult a, sdResult b, float k)
{
	float h = max(k - abs(a.d - b.d), 0.0) / k;

	if (a.d > b.d)
	{
		sdResult temp = a;
		a = b;
		b = temp;
	}
		
	a.d -=  h * h * k * (1.0 / 4.0);
	a.mat = materialMix(a.mat, b.mat, h * 0.5);
    return a;
}

sdResult sdInersect(sdResult a, sdResult b)
{
    return a.d > b.d ? a : b;
}

sdResult sdDifference(sdResult a, sdResult b)
{
    return a.d > -b.d ? a : b;
}

sdResult sdSphere(vec3 p, float r, BlinnPhongMaterial mat)
{
  return sdResult(length(p) - r, mat);
}

sdResult sdBox(vec3 p, vec3 b, BlinnPhongMaterial mat)
{
  vec3 q = abs(p) - b;
  return sdResult(length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0), mat);
}

#define SDSCENENORMAL1(sceneFunc)							\
vec3 sdSceneNormal(vec3 p)									\
{															\
    const float eps = 0.0001;								\
    const vec2 h = vec2(eps,0);								\
    return normalize(										\
		vec3(sceneFunc(p+h.xyy).d - sceneFunc(p-h.xyy).d,	\
			 sceneFunc(p+h.yxy).d - sceneFunc(p-h.yxy).d,	\
             sceneFunc(p+h.yyx).d - sceneFunc(p-h.yyx).d));	\
}

#define SDSCENENORMAL2(sceneFunc)			\
vec3 sdSceneNormal(vec3 p)					\
{											\
    const float h = 0.0001;					\
    const vec2 k = vec2(1,-1);				\
    return normalize(						\
		k.xyy*sceneFunc( p + k.xyy*h ).d +	\
        k.yyx*sceneFunc( p + k.yyx*h ).d +	\
        k.yxy*sceneFunc( p + k.yxy*h ).d +	\
        k.xxx*sceneFunc( p + k.xxx*h ).d );	\
}

#endif
