#version 330 core
out vec4 FragColor;

in vec2 UV;

uniform sampler1D soundTexture;
uniform sampler1D frequencyTexture;
uniform sampler1D frequencyColorCurve;
uniform sampler1D lightColorCurve;

uniform float time;
uniform vec2 texturePixelSize;
uniform vec2 mousePos;
uniform float lightHeight;
/*
HSLUV-GLSL v4.2
HSLUV is a human-friendly alternative to HSL. ( http://www.hsluv.org )
GLSL port by William Malo ( https://github.com/williammalo )
Put this code in your fragment shader.
*/

vec3 hsluv_intersectLineLine(vec3 line1x, vec3 line1y, vec3 line2x, vec3 line2y) {
    return (line1y - line2y) / (line2x - line1x);
}

vec3 hsluv_distanceFromPole(vec3 pointx,vec3 pointy) {
    return sqrt(pointx*pointx + pointy*pointy);
}

vec3 hsluv_lengthOfRayUntilIntersect(float theta, vec3 x, vec3 y) {
    vec3 len = y / (sin(theta) - x * cos(theta));
    if (len.r < 0.0) {len.r=1000.0;}
    if (len.g < 0.0) {len.g=1000.0;}
    if (len.b < 0.0) {len.b=1000.0;}
    return len;
}

float hsluv_maxSafeChromaForL(float L){
    mat3 m2 = mat3(
         3.2409699419045214  ,-0.96924363628087983 , 0.055630079696993609,
        -1.5373831775700935  , 1.8759675015077207  ,-0.20397695888897657 ,
        -0.49861076029300328 , 0.041555057407175613, 1.0569715142428786  
    );
    float sub0 = L + 16.0;
    float sub1 = sub0 * sub0 * sub0 * .000000641;
    float sub2 = sub1 > 0.0088564516790356308 ? sub1 : L / 903.2962962962963;

    vec3 top1   = (284517.0 * m2[0] - 94839.0  * m2[2]) * sub2;
    vec3 bottom = (632260.0 * m2[2] - 126452.0 * m2[1]) * sub2;
    vec3 top2   = (838422.0 * m2[2] + 769860.0 * m2[1] + 731718.0 * m2[0]) * L * sub2;

    vec3 bounds0x = top1 / bottom;
    vec3 bounds0y = top2 / bottom;

    vec3 bounds1x =              top1 / (bottom+126452.0);
    vec3 bounds1y = (top2-769860.0*L) / (bottom+126452.0);

    vec3 xs0 = hsluv_intersectLineLine(bounds0x, bounds0y, -1.0/bounds0x, vec3(0.0) );
    vec3 xs1 = hsluv_intersectLineLine(bounds1x, bounds1y, -1.0/bounds1x, vec3(0.0) );

    vec3 lengths0 = hsluv_distanceFromPole( xs0, bounds0y + xs0 * bounds0x );
    vec3 lengths1 = hsluv_distanceFromPole( xs1, bounds1y + xs1 * bounds1x );

    return  min(lengths0.r,
            min(lengths1.r,
            min(lengths0.g,
            min(lengths1.g,
            min(lengths0.b,
                lengths1.b)))));
}

float hsluv_maxChromaForLH(float L, float H) {

    float hrad = radians(H);

    mat3 m2 = mat3(
         3.2409699419045214  ,-0.96924363628087983 , 0.055630079696993609,
        -1.5373831775700935  , 1.8759675015077207  ,-0.20397695888897657 ,
        -0.49861076029300328 , 0.041555057407175613, 1.0569715142428786  
    );
    float sub1 = pow(L + 16.0, 3.0) / 1560896.0;
    float sub2 = sub1 > 0.0088564516790356308 ? sub1 : L / 903.2962962962963;

    vec3 top1   = (284517.0 * m2[0] - 94839.0  * m2[2]) * sub2;
    vec3 bottom = (632260.0 * m2[2] - 126452.0 * m2[1]) * sub2;
    vec3 top2   = (838422.0 * m2[2] + 769860.0 * m2[1] + 731718.0 * m2[0]) * L * sub2;

    vec3 bound0x = top1 / bottom;
    vec3 bound0y = top2 / bottom;

    vec3 bound1x =              top1 / (bottom+126452.0);
    vec3 bound1y = (top2-769860.0*L) / (bottom+126452.0);

    vec3 lengths0 = hsluv_lengthOfRayUntilIntersect(hrad, bound0x, bound0y );
    vec3 lengths1 = hsluv_lengthOfRayUntilIntersect(hrad, bound1x, bound1y );

    return  min(lengths0.r,
            min(lengths1.r,
            min(lengths0.g,
            min(lengths1.g,
            min(lengths0.b,
                lengths1.b)))));
}

float hsluv_fromLinear(float c) {
    return c <= 0.0031308 ? 12.92 * c : 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}
vec3 hsluv_fromLinear(vec3 c) {
    return vec3( hsluv_fromLinear(c.r), hsluv_fromLinear(c.g), hsluv_fromLinear(c.b) );
}

float hsluv_toLinear(float c) {
    return c > 0.04045 ? pow((c + 0.055) / (1.0 + 0.055), 2.4) : c / 12.92;
}

vec3 hsluv_toLinear(vec3 c) {
    return vec3( hsluv_toLinear(c.r), hsluv_toLinear(c.g), hsluv_toLinear(c.b) );
}

float hsluv_yToL(float Y){
    return Y <= 0.0088564516790356308 ? Y * 903.2962962962963 : 116.0 * pow(Y, 1.0 / 3.0) - 16.0;
}

float hsluv_lToY(float L) {
    return L <= 8.0 ? L / 903.2962962962963 : pow((L + 16.0) / 116.0, 3.0);
}

vec3 xyzToRgb(vec3 tuple) {
    const mat3 m = mat3( 
        3.2409699419045214  ,-1.5373831775700935 ,-0.49861076029300328 ,
       -0.96924363628087983 , 1.8759675015077207 , 0.041555057407175613,
        0.055630079696993609,-0.20397695888897657, 1.0569715142428786  );
    
    return hsluv_fromLinear(tuple*m);
}

vec3 rgbToXyz(vec3 tuple) {
    const mat3 m = mat3(
        0.41239079926595948 , 0.35758433938387796, 0.18048078840183429 ,
        0.21263900587151036 , 0.71516867876775593, 0.072192315360733715,
        0.019330818715591851, 0.11919477979462599, 0.95053215224966058 
    );
    return hsluv_toLinear(tuple) * m;
}

vec3 xyzToLuv(vec3 tuple){
    float X = tuple.x;
    float Y = tuple.y;
    float Z = tuple.z;

    float L = hsluv_yToL(Y);
    
    float div = 1./dot(tuple,vec3(1,15,3)); 

    return vec3(
        1.,
        (52. * (X*div) - 2.57179),
        (117.* (Y*div) - 6.08816)
    ) * L;
}


vec3 luvToXyz(vec3 tuple) {
    float L = tuple.x;

    float U = tuple.y / (13.0 * L) + 0.19783000664283681;
    float V = tuple.z / (13.0 * L) + 0.468319994938791;

    float Y = hsluv_lToY(L);
    float X = 2.25 * U * Y / V;
    float Z = (3./V - 5.)*Y - (X/3.);

    return vec3(X, Y, Z);
}

vec3 luvToLch(vec3 tuple) {
    float L = tuple.x;
    float U = tuple.y;
    float V = tuple.z;

    float C = length(tuple.yz);
    float H = degrees(atan(V,U));
    if (H < 0.0) {
        H = 360.0 + H;
    }
    
    return vec3(L, C, H);
}

vec3 lchToLuv(vec3 tuple) {
    float hrad = radians(tuple.b);
    return vec3(
        tuple.r,
        cos(hrad) * tuple.g,
        sin(hrad) * tuple.g
    );
}

vec3 hsluvToLch(vec3 tuple) {
    tuple.g *= hsluv_maxChromaForLH(tuple.b, tuple.r) * .01;
    return tuple.bgr;
}

vec3 lchToHsluv(vec3 tuple) {
    tuple.g /= hsluv_maxChromaForLH(tuple.r, tuple.b) * .01;
    return tuple.bgr;
}

vec3 hpluvToLch(vec3 tuple) {
    tuple.g *= hsluv_maxSafeChromaForL(tuple.b) * .01;
    return tuple.bgr;
}

vec3 lchToHpluv(vec3 tuple) {
    tuple.g /= hsluv_maxSafeChromaForL(tuple.r) * .01;
    return tuple.bgr;
}

vec3 lchToRgb(vec3 tuple) {
    return xyzToRgb(luvToXyz(lchToLuv(tuple)));
}

vec3 rgbToLch(vec3 tuple) {
    return luvToLch(xyzToLuv(rgbToXyz(tuple)));
}

vec3 hsluvToRgb(vec3 tuple) {
    return lchToRgb(hsluvToLch(tuple * vec3(360.0, 100.0, 100.0)));
}

vec3 rgbToHsluv(vec3 tuple) {
    return lchToHsluv(rgbToLch(tuple)) / vec3(360.0, 100.0, 100.0);
}

vec3 hpluvToRgb(vec3 tuple) {
    return lchToRgb(hpluvToLch(tuple * vec3(360.0, 100.0, 100.0)));
}

vec3 rgbToHpluv(vec3 tuple) {
    return lchToHpluv(rgbToLch(tuple)) / vec3(360.0, 100.0, 100.0);
}

vec3 luvToRgb(vec3 tuple){
    return xyzToRgb(luvToXyz(tuple));
}

// allow vec4's
vec4   xyzToRgb(vec4 c) {return vec4(   xyzToRgb( vec3(c.x,c.y,c.z) ), c.a);}
vec4   rgbToXyz(vec4 c) {return vec4(   rgbToXyz( vec3(c.x,c.y,c.z) ), c.a);}
vec4   xyzToLuv(vec4 c) {return vec4(   xyzToLuv( vec3(c.x,c.y,c.z) ), c.a);}
vec4   luvToXyz(vec4 c) {return vec4(   luvToXyz( vec3(c.x,c.y,c.z) ), c.a);}
vec4   luvToLch(vec4 c) {return vec4(   luvToLch( vec3(c.x,c.y,c.z) ), c.a);}
vec4   lchToLuv(vec4 c) {return vec4(   lchToLuv( vec3(c.x,c.y,c.z) ), c.a);}
vec4 hsluvToLch(vec4 c) {return vec4( hsluvToLch( vec3(c.x,c.y,c.z) ), c.a);}
vec4 lchToHsluv(vec4 c) {return vec4( lchToHsluv( vec3(c.x,c.y,c.z) ), c.a);}
vec4 hpluvToLch(vec4 c) {return vec4( hpluvToLch( vec3(c.x,c.y,c.z) ), c.a);}
vec4 lchToHpluv(vec4 c) {return vec4( lchToHpluv( vec3(c.x,c.y,c.z) ), c.a);}
vec4   lchToRgb(vec4 c) {return vec4(   lchToRgb( vec3(c.x,c.y,c.z) ), c.a);}
vec4   rgbToLch(vec4 c) {return vec4(   rgbToLch( vec3(c.x,c.y,c.z) ), c.a);}
vec4 hsluvToRgb(vec4 c) {return vec4( hsluvToRgb( vec3(c.x,c.y,c.z) ), c.a);}
vec4 rgbToHsluv(vec4 c) {return vec4( rgbToHsluv( vec3(c.x,c.y,c.z) ), c.a);}
vec4 hpluvToRgb(vec4 c) {return vec4( hpluvToRgb( vec3(c.x,c.y,c.z) ), c.a);}
vec4 rgbToHpluv(vec4 c) {return vec4( rgbToHpluv( vec3(c.x,c.y,c.z) ), c.a);}
vec4   luvToRgb(vec4 c) {return vec4(   luvToRgb( vec3(c.x,c.y,c.z) ), c.a);}
// allow 3 floats
vec3   xyzToRgb(float x, float y, float z) {return   xyzToRgb( vec3(x,y,z) );}
vec3   rgbToXyz(float x, float y, float z) {return   rgbToXyz( vec3(x,y,z) );}
vec3   xyzToLuv(float x, float y, float z) {return   xyzToLuv( vec3(x,y,z) );}
vec3   luvToXyz(float x, float y, float z) {return   luvToXyz( vec3(x,y,z) );}
vec3   luvToLch(float x, float y, float z) {return   luvToLch( vec3(x,y,z) );}
vec3   lchToLuv(float x, float y, float z) {return   lchToLuv( vec3(x,y,z) );}
vec3 hsluvToLch(float x, float y, float z) {return hsluvToLch( vec3(x,y,z) );}
vec3 lchToHsluv(float x, float y, float z) {return lchToHsluv( vec3(x,y,z) );}
vec3 hpluvToLch(float x, float y, float z) {return hpluvToLch( vec3(x,y,z) );}
vec3 lchToHpluv(float x, float y, float z) {return lchToHpluv( vec3(x,y,z) );}
vec3   lchToRgb(float x, float y, float z) {return   lchToRgb( vec3(x,y,z) );}
vec3   rgbToLch(float x, float y, float z) {return   rgbToLch( vec3(x,y,z) );}
vec3 hsluvToRgb(float x, float y, float z) {return hsluvToRgb( vec3(x,y,z) );}
vec3 rgbToHsluv(float x, float y, float z) {return rgbToHsluv( vec3(x,y,z) );}
vec3 hpluvToRgb(float x, float y, float z) {return hpluvToRgb( vec3(x,y,z) );}
vec3 rgbToHpluv(float x, float y, float z) {return rgbToHpluv( vec3(x,y,z) );}
vec3   luvToRgb(float x, float y, float z) {return   luvToRgb( vec3(x,y,z) );}
// allow 4 floats
vec4   xyzToRgb(float x, float y, float z, float a) {return   xyzToRgb( vec4(x,y,z,a) );}
vec4   rgbToXyz(float x, float y, float z, float a) {return   rgbToXyz( vec4(x,y,z,a) );}
vec4   xyzToLuv(float x, float y, float z, float a) {return   xyzToLuv( vec4(x,y,z,a) );}
vec4   luvToXyz(float x, float y, float z, float a) {return   luvToXyz( vec4(x,y,z,a) );}
vec4   luvToLch(float x, float y, float z, float a) {return   luvToLch( vec4(x,y,z,a) );}
vec4   lchToLuv(float x, float y, float z, float a) {return   lchToLuv( vec4(x,y,z,a) );}
vec4 hsluvToLch(float x, float y, float z, float a) {return hsluvToLch( vec4(x,y,z,a) );}
vec4 lchToHsluv(float x, float y, float z, float a) {return lchToHsluv( vec4(x,y,z,a) );}
vec4 hpluvToLch(float x, float y, float z, float a) {return hpluvToLch( vec4(x,y,z,a) );}
vec4 lchToHpluv(float x, float y, float z, float a) {return lchToHpluv( vec4(x,y,z,a) );}
vec4   lchToRgb(float x, float y, float z, float a) {return   lchToRgb( vec4(x,y,z,a) );}
vec4   rgbToLch(float x, float y, float z, float a) {return   rgbToLch( vec4(x,y,z,a) );}
vec4 hsluvToRgb(float x, float y, float z, float a) {return hsluvToRgb( vec4(x,y,z,a) );}
vec4 rgbToHslul(float x, float y, float z, float a) {return rgbToHsluv( vec4(x,y,z,a) );}
vec4 hpluvToRgb(float x, float y, float z, float a) {return hpluvToRgb( vec4(x,y,z,a) );}
vec4 rgbToHpluv(float x, float y, float z, float a) {return rgbToHpluv( vec4(x,y,z,a) );}
vec4   luvToRgb(float x, float y, float z, float a) {return   luvToRgb( vec4(x,y,z,a) );}

/*
END HSLUV-GLSL
*/

//https://gist.github.com/sugi-cho/6a01cae436acddd72bdf
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
  
  // Sample the sound texture using the x coordinate of the left, center, and right fragments.
  // Audio samples are in the range [-1:1]
  // To render them in the top half of the window they are transformed into the range [0.5:1.0]
  float soundLeft = texture(soundTexture, UV.x - texturePixelSize.x).r * 0.25 + 0.75;
  float soundCenter = texture(soundTexture, UV.x).r * 0.25 + 0.75;
  float soundRight = texture(soundTexture, UV.x + texturePixelSize.x).r * 0.25 + 0.75;
  
  // Get the half way points between the center audio sample and the left/right audio samples
  float soundLeftMid = (soundLeft + soundCenter) * 0.5;
  float soundRightMid = (soundCenter + soundRight) * 0.5;

  // Get the highest value of leftMid, soundCenter, and rightMid
  // Get the lowest value of leftMid, soundCenter, and (high - 1 vertical pixel)
  // With these two values, the audio samples can be rendered as a continous 1 pixel thick line
  float soundHigh = max(max(soundLeftMid, soundCenter), soundRightMid);
  float soundLow = min(min(min(soundLeftMid, soundCenter), soundRightMid), soundHigh - texturePixelSize.y);
  
  // Get the sound map. soundMap is 1.0 for fragments that are part of the wave, 0.0 for fragments that are not
  // Any fragments that have a y value in the range [soundLow:soundHigh] are set tp 1.0
  float soundMap = step(soundLow, UV.y) * step(UV.y, soundHigh);
  
  // The frequency spectrum is rendered as a "solid of revolution",
  // where the entire spectrum is rotated around the x axis to form a 3d object
  // the position on the solid, as well as a surface normal vector at the current fragment is needed for lighting effects
  
  // Get the frequency magnitudes at the current fragment, as well as the fragments to the left and right.
  float freqLeft = texture(frequencyTexture, UV.x - texturePixelSize.x).r;
  float freqMid = texture(frequencyTexture, UV.x).r;
  float freqRight = texture(frequencyTexture, UV.x + texturePixelSize.x).r;
  
  // Caclulates the freqeuncy map. 
  // freqMap has a value of 1.0 for fragments that are part of the frequency specturm, 
  // and a value of 0.0 for fragments that are not
  // Any fragments that have a y value in the range [freqLower:freqUpper] are set to 1.0
  float linePos = 0.333;
  float lineWidth = texturePixelSize.y*0.5;
  float freqLower = linePos - lineWidth - freqMid;
  float freqUpper = linePos + lineWidth + freqMid;
  float freqMap = step(freqLower, UV.y) * step(UV.y, freqUpper);

  // Get the normal vector on the 2d surface of the frequency spectrum
  // This is calculated by averaging the upwards facing norms of the vectors that point to the left and right fragments
  vec2 freqLeftNorm = normalize(vec2(freqLeft - freqMid, texturePixelSize.x));
  vec2 freqRightNorm = normalize(vec2(freqMid - freqRight, texturePixelSize.x));
  vec2 freqSurfaceNorm2D = normalize(freqLeftNorm + freqRightNorm);
  
  // Gets the normal vector on the 3d surface of the frequency spectrum
  // Position on the current radial "disc" is caclulated with pythagorean theorem
  // This position is normalized and used to construct a rotation matrix
  // that is used to rotate the 2d surface norm to the 3d surface norm
  float radialY = UV.y - linePos;
  float radialZ = sqrt(freqMid*freqMid - radialY*radialY);
  vec3 radialNorm = normalize(vec3(0.0, radialY, radialZ));
  mat3 rotation = mat3(
    1.0, 0.0, 0.0,
    0.0, radialNorm.y, radialNorm.z,
    0.0, -radialNorm.z, radialNorm.y
    );
  vec3 freqSurfaceNorm3D = rotation * vec3(freqSurfaceNorm2D, 0.0);
  
  // Diffuse lighting is the dot product of the frequency surface norm and the light direction
  vec3 freqSurfacePos = vec3(UV, radialZ);
  vec3 lightPos = vec3(mousePos, lightHeight);
  vec3 lightDir = normalize(lightPos - freqSurfacePos);
  float diffuse = dot(freqSurfaceNorm3D, lightDir);
  
  // Specular highlights using blinn-phong model
  // calculated by taking the dot product between the freq surface norm and the
  // halfway vector between the light direction and view direction norms
  float specularStrength = 0.0;
  vec3 viewDir = vec3(0.0, 0.0, 1.0);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float specular = pow(max(dot(freqSurfaceNorm3D, halfwayDir), 0.0), 2) * specularStrength;
  
  float gamma = 2.2;
  vec4 diffuseColor = vec4(pow(texture(lightColorCurve, (diffuse + 1.0) * 0.5).rgb, vec3(gamma)), 1.0);
  vec4 ambientColor = vec4(pow(texture(frequencyColorCurve, freqMid).rgb, vec3(gamma)), 1.0);
  
  vec4 finalColor = (diffuseColor + ambientColor + specular) * freqMap;
  
  finalColor.rgb = rgb2hsv(finalColor.rgb);
  //freqColor.r += UV.x * 5.1;
  //freqColor.r += lightColor.r;
  //freqColor.g *= lightColor.g;
  //freqColor.b *= lightColor.b;
  finalColor.rgb = hsv2rgb(finalColor.rgb);
  
  finalColor.rgb = pow(finalColor.rgb, vec3(1.0/gamma));
  
  FragColor = finalColor;// + sound * soundColor;
}
