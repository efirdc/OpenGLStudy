// Hash without Sine
// MIT License...
/* Copyright (c)2014 David Hoskins.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
//? #version 440

#ifndef _HASH
#define _HASH

#define UI0 1597334673U
#define UI1 3812015801U
#define UI2 uvec2(UI0, UI1)
#define UI3 uvec3(UI0, UI1, 2798796415U)
#define UIF (1.0 / float(0xffffffffU))


// The labeling refers to the number of values - hash(out)(in)...
//---------------------------------------------------------------------------------------------------------------
float hash11(uint q)
{
	uvec2 n = q * UI2;
	q = (n.x ^ n.y) * UI0;
	return float(q) * UIF;
}

float hash11(float p)
{
	uvec2 n = uint(int(p)) * UI2;
	uint q = (n.x ^ n.y) * UI0;
	return float(q) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
float hash12(uvec2 q)
{
	q *= UI2;
	uint n = (q.x ^ q.y) * UI0;
	return float(n) * UIF;
}

float hash12(vec2 p)
{
	uvec2 q = uvec2(ivec2(p)) * UI2;
	uint n = (q.x ^ q.y) * UI0;
	return float(n) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
float hash13(uvec3 q)
{
	q *= UI3;
	uint n = (q.x ^ q.y ^ q.z) * UI0;
	return float(n) * UIF;
}

float hash13(vec3 p)
{
	uvec3 q = uvec3(ivec3(p)) * UI3;
	q *= UI3;
	uint n = (q.x ^ q.y ^ q.z) * UI0;
	return float(n) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
vec2 hash21(uint q)
{
	uvec2 n = q * UI2;
	n = (n.x ^ n.y) * UI2;
	return vec2(n) * UIF;
}

vec2 hash21(float p)
{
	uvec2 n = uint(int(p)) * UI2;
	n = (n.x ^ n.y) * UI2;
	return vec2(n) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
vec2 hash22(uvec2 q)
{
	q *= UI2;
	q = (q.x ^ q.y) * UI2;
	return vec2(q) * UIF;
}

vec2 hash22(vec2 p)
{
	uvec2 q = uvec2(ivec2(p))*UI2;
	q = (q.x ^ q.y) * UI2;
	return vec2(q) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
vec2 hash23(uvec3 q)
{
	q *= UI3;
	uvec2 n = (q.x ^ q.y ^ q.z) * UI2;
	return vec2(n) * UIF;
}

vec2 hash23(vec3 p)
{
	uvec3 q = uvec3(ivec3(p)) * UI3;
	uvec2 n = (q.x ^ q.y ^ q.z) * UI2;

	return vec2(n) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
vec3 hash31(uint q)
{
	uvec3 n = q * UI3;
	n = (n.x ^ n.y ^ n.z) * UI3;
	return vec3(n) * UIF;
}
vec3 hash31(float p)
{

	uvec3 n = uint(int(p)) * UI3;
	n = (n.x ^ n.y ^ n.z) * UI3;
	return vec3(n) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
vec3 hash32(uvec2 q)
{
	uvec3 n = q.xyx * UI3;
	n = (n.x ^ n.y ^n.z) * UI3;
	return vec3(n) * UIF;
}

vec3 hash32(vec2 q)
{
	uvec3 n = uvec3(ivec3(q.xyx)) * UI3;
	n = (n.x ^ n.y ^ n.z) * UI3;
	return vec3(n) * UIF;
}

//---------------------------------------------------------------------------------------------------------------
vec3 hash33(uvec3 q)
{
	q *= UI3;
	q = (q.x ^ q.y ^ q.z)*UI3;
	return vec3(q) * UIF;
}

vec3 hash33(vec3 p)
{
	uvec3 q = uvec3(ivec3(p)) * UI3;
	q = (q.x ^ q.y ^ q.z)*UI3;
	return vec3(q) * UIF;
}

//----------------------------------------------------------------------------------------
// 4 out, 1 in...
vec4 hash41(float p)
{
	vec4 p4 = fract(vec4(p) * vec4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, p4.wzxy+33.33);
    return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

//----------------------------------------------------------------------------------------
// 4 out, 2 in...
vec4 hash42(vec2 p)
{
	vec4 p4 = fract(vec4(p.xyxy) * vec4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, p4.wzxy+33.33);
    return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

//----------------------------------------------------------------------------------------
// 4 out, 3 in...
vec4 hash43(vec3 p)
{
	vec4 p4 = fract(vec4(p.xyzx)  * vec4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, p4.wzxy+33.33);
    return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

//----------------------------------------------------------------------------------------
// 4 out, 4 in...
vec4 hash44(vec4 p4)
{
	p4 = fract(p4  * vec4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, p4.wzxy+33.33);
    return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}
#endif