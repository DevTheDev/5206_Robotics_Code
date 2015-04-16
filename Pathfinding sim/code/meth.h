#ifndef METH
#define METH

#include <math.h>
#include <smmintrin.h>

#undef min
#undef max
#define min(a, b) (((a) < (b))?(a):(b))
#define max(a, b) (((a) > (b))?(a):(b))

#define pi 3.14159265358979323846

#define sq(a) ((a)*(a))

#define abs(a) (((a) > 0)?(a):(-(a)))

union float2
{
    struct
    {
        float x;
        float y;
    };
    float data[2];
    
    inline float & operator[](uint a)
    {
        return data[a];
    }
    
    inline bool operator==(float2 a)
    {
        return x == a.x && y == a.y; //TODO: vectorize (xor then test all zero is one option)
    }
};


inline float2 scale(float2 a, float s)
{
    __m128 a128 = _mm_set_ps(0.0, 0.0, a[1], a[0]);
    __m128 b128 = _mm_set1_ps(s);
    union
    {
        __m128 product128;
        float2 product;
    };
    product128 = _mm_mul_ps(a128, b128);
    return product;
}

inline float2 add(float2 a, float2 b)
{
    __m128 a128 = _mm_set_ps(0.0, 0.0, a[1], a[0]);
    __m128 b128 = _mm_set_ps(0.0, 0.0, b[1], b[0]);
    union
    {
        float2 sum;
        __m128 sum128;
    };
    sum128 = _mm_add_ps(a128, b128);
    return sum;
}

inline float2 sub(float2 a, float2 b)
{
    __m128 a128 = _mm_set_ps(0.0, 0.0, a[1], a[0]);
    __m128 b128 = _mm_set_ps(0.0, 0.0, b[1], b[0]);
    union
    {
        float2 sum;
        __m128 sum128;
    };
    sum128 = _mm_sub_ps(a128, b128);    
    return sum;
}

inline float dot(float2 a, float2 b)
{
    __m128 a128 = _mm_set_ps(0.0, 0.0, a[1], a[0]);
    __m128 b128 = _mm_set_ps(0.0, 0.0, b[1], b[0]);
    __m128 products128 = _mm_mul_ps(a128, b128);
    float products[4];
    _mm_store_ps(products, products128);
    float out = products[0]+products[1];
    return out;
}

inline float2 complexx(float2 a, float2 b)
{
    __m128 a128 = _mm_set_ps(a[1], a[0], a[1], a[0]);
    __m128 b128 = _mm_set_ps(b[0], b[1], b[1], b[0]);
    union
    {
        __m128 products128;
        float products[4];
    };
    products128 = _mm_mul_ps(a128, b128);
    float2 out = {products[0]-products[1], products[2]+products[3]};//TODO: SIMDize this
    return out;
}

/* inline float32 sqrt(float32 a) */
/* { */
/*     uint32 ai = *(uint32 *) &a; */
/*     union */
/*     { */
/*         uint32 isqrti; */
/*         float32 isqrt; */
/*     }; */
/*     isqrti = (532496103) + (ai >> 1); */
/*     isqrt = 0.5*(isqrt + a/isqrt); //TODO: remove this division */
/*     return isqrt; */
/* } */

inline float32 invsqrt(float32 a)
{
    uint32 ai = *(uint32 *) &a;
    union
    {
        uint32 isqrti;
        float32 isqrt;
    };
    isqrti = 0x5f375a86 - (ai >> 1);
    isqrt *= (1.5f - (a*0.5f*isqrt*isqrt));
    return isqrt;
}

inline float dotme(float2 a)
{
    __m128 a128 = _mm_set_ps(0.0, 0, a[1], a[0]);
    union
    {
        __m128 products128;
        float products[4];
    };
    products128 = _mm_mul_ps(a128, a128);    
    float out = products[0]+products[1];
    return out;
}

inline float norm(float2 a)
{
    return sqrt(dotme(a));
}

inline float2 normalize(float2 a)
{
    float square = dotme(a);
    return scale(a, invsqrt(square));
}

inline float2 perp(float2 a)
{
    float2 out = {a.y, -a.x};
    return out;
}

/* inline float32 floor(float32 x) */
/* { */
/*     __m128 floor_x = _mm_set_ss(x); */

/*     //floor_x = _mm_round_ps(floor_x, _MM_FROUND_TO_NEG_INF);//TODO: use roundps when available */
/*     floor_x = _mm_cvtepi32_ps(_mm_cvttps_epi32(floor_x)); */
    
/*     float32 out; */
/*     _mm_store_ss(&out, floor_x); */
/*     return out; */
/* } */
#endif
