#version 440
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D depthStencilTexture;
layout(binding = 2) uniform usampler2D stencilView;

uniform float blendWeight;

// Trims the algorithm from processing darks.
// 0.0833 - upper limit (default, the start of visible unfiltered edges)
// 0.0625 - high quality (faster)
// 0.0312 - visible limit (slower)
uniform float contrastThreshold;

// The minimum amount of local contrast required to apply algorithm.
// 0.333 - too little (faster)
// 0.250 - low quality
// 0.166 - default
// 0.125 - high quality 
// 0.063 - overkill (slower)
uniform float relativeThreshold;

// Might be a good idea to precalculate luminance and save it in a texture for quick look up instead of calculating it every time
float RgbToLuminance(vec3 rgb)
{
    return dot(rgb, vec3(0.2126729f,  0.7151522f, 0.0721750f));
}

struct LuminanceData
{
    // m - middle, n - north, s - south, w - west, e - east
    float m, n, s, w, e;
    // nw - north west, ne - north east, sw - south west, se - south east
    float nw, ne, sw, se;
    // luminance extreme values and luminance contrast
    float highest, lowest, contrast;
};

LuminanceData SampleLuminanceNeighbourhood(vec2 uv, vec2 texelSize)
{
    LuminanceData ld;

    ld.m = RgbToLuminance(texture(screenTexture, uv).rgb);

    ld.n = RgbToLuminance(texture(screenTexture, uv + vec2(0.0, 1.0) * texelSize).rgb);
    ld.s = RgbToLuminance(texture(screenTexture, uv + vec2(0.0, -1.0) * texelSize).rgb);
    ld.w = RgbToLuminance(texture(screenTexture, uv + vec2(-1.0, 0.0) * texelSize).rgb);
    ld.e = RgbToLuminance(texture(screenTexture, uv + vec2(1.0, 0.0) * texelSize).rgb);

    ld.nw = RgbToLuminance(texture(screenTexture, uv + vec2(1.0, -1.0) * texelSize).rgb);
    ld.ne = RgbToLuminance(texture(screenTexture, uv + vec2(1.0, 1.0) * texelSize).rgb);
    ld.sw = RgbToLuminance(texture(screenTexture, uv + vec2(-1.0, -1.0) * texelSize).rgb);
    ld.se = RgbToLuminance(texture(screenTexture, uv + vec2(-1.0, 1.0) * texelSize).rgb);

    ld.highest = max(ld.m, max(ld.n, max(ld.s, max(ld.w, ld.e))));
    ld.lowest = min(ld.m, min(ld.n, min(ld.s, min(ld.w, ld.e))));
    ld.contrast = ld.highest - ld.lowest;

    return ld;
}

float DeterminePixelBlendFactor(LuminanceData ld)
{
    // 2.0 for straight directions and 1.0 for diagonal since diagonals are farther from middle pixel
    float fxaaFilter = 2.0 * (ld.n + ld.s + ld.w + ld.e);
    fxaaFilter += ld.nw + ld.ne + ld.sw + ld.se;
    // divide by the sum of the weights
    fxaaFilter /= 12.0;
    //finally calculate blend factor and saturate
    fxaaFilter = abs(fxaaFilter - ld.m);
    fxaaFilter = clamp(fxaaFilter / ld.contrast, 0.0, 1.0);
    // now smoothen the blend
    float blendFactor = smoothstep(0.0, 1.0, fxaaFilter);
    return blendFactor * blendFactor;
}

struct EdgeData
{
	bool isHorizontal;
    // direction of the offset used for blending
    vec2 pixelStep;
};

EdgeData DetermineEdge (LuminanceData ld)
{
	EdgeData ed;
	float horizontal =
		abs(ld.n + ld.s - 2 * ld.m) * 2 +
		abs(ld.ne + ld.se - 2 * ld.e) +
		abs(ld.nw + ld.sw - 2 * ld.w);
	float vertical =
		abs(ld.e + ld.w - 2 * ld.m) * 2 +
		abs(ld.ne + ld.nw - 2 * ld.n) +
		abs(ld.se + ld.sw - 2 * ld.s);
	ed.isHorizontal = horizontal >= vertical;

    // if edge is horizontal, we blend vertical pixels, otherwise we blend horizontal neighbours
    ed.pixelStep.x = ed.isHorizontal ? 0.0 : 1.0;
    ed.pixelStep.y = ed.isHorizontal ? 1.0 : 0.0;

    // now determine blend direction; pLuminance - positive luminance; nLuminance - negative luminance
    float pLuminance = ed.isHorizontal ? ld.n : ld.e;
	float nLuminance = ed.isHorizontal ? ld.s : ld.w;
	float pGradient = abs(pLuminance - ld.m);
	float nGradient = abs(nLuminance - ld.m);

    if (pGradient < nGradient)
    {
		ed.pixelStep = -ed.pixelStep;
	}

	return ed;
}

vec3 ApplyFXAA(vec3 screenColor, vec2 uv, vec2 texelSize) 
{
	LuminanceData ld = SampleLuminanceNeighbourhood(uv, texelSize);
    float threshold = max(contrastThreshold, relativeThreshold * ld.highest);
    if (ld.contrast < threshold)
    {
	    return screenColor;
	}

    // use the blend factor to linearly interpolate between the middle pixel and its neighbor in the appropriate direction.
    float pixelBlend = DeterminePixelBlendFactor(ld);
    EdgeData ed = DetermineEdge(ld);
    vec2 uvOffset = uv + texelSize * ed.pixelStep * pixelBlend;

	return vec3(texture(screenTexture, uvOffset).rgb);
}

void main()
{
    vec3 screenColor = texture(screenTexture, TexCoords).rgb;
    vec2 texelSize = 1.0 / vec2(textureSize(screenTexture, 0));
    vec3 antiAliasedColor = ApplyFXAA(screenColor, TexCoords, texelSize);

    FragColor = vec4(antiAliasedColor, 1.0);
} 