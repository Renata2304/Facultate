#version 330
#define BLUR_RADIUS 12
// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform sampler2D textureBlurred;
uniform sampler2D textureSobel;
uniform sampler2D textureHatching;


uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 0; 

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture

vec4 canny(float threshold) {
    // Applying Sobel
    vec2 texelSize = 1.0f / screenSize;
    mat3 kernelX = mat3(
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    );
    mat3 kernelY = mat3(
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    );

    float Dx = 0.0;
    float Dy = 0.0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            vec4 neighbor_color = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            float gray = 0.21 * neighbor_color.r + 0.71 * neighbor_color.g + 0.07 * neighbor_color.b;
            Dx += gray * kernelX[i + 1][j + 1];
            Dy += gray * kernelY[i + 1][j + 1];
        }
    }

    float magnitude = sqrt(Dx * Dx + Dy * Dy);
    if (magnitude < threshold)
    {
        return vec4(1, 1, 1, 1);
    }
    return vec4(0, 0, 0, 1);
}


vec4 horizontalBlur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0.0);
    float kernelSize = float(2 * blurRadius + 1);

    for (int i = -blurRadius; i <= blurRadius; i++)
    {
        vec2 offset = vec2(float(i), 0.0) * texelSize;
        sum += texture(textureImage, textureCoord + offset);
    }

    return sum / kernelSize;
}

vec4 separableBlur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0.0);
    float kernelSize = float(2 * blurRadius + 1);

    for (int i = -blurRadius; i <= blurRadius; i++)
    {
        vec2 offset = vec2(0.0, float(i)) * texelSize;
        sum += texture(textureBlurred, textureCoord + offset);
    }

    return sum / kernelSize;
}


vec4 hashPattern(float a, float b, float c, float intensityThreshold, bool invert, sampler2D inputTexture)
{
    if (outputMode <= 6)
	    textureCoord.y = 1 - textureCoord.y;

    vec4 color = texture(inputTexture, textureCoord);
    float intensity = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;

    if (intensity < intensityThreshold)
    {
        float value = sin(a * textureCoord.x + b * textureCoord.y);
        if (invert)
        {
            return value > c ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
        }
        else
        {
            return value > c ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(1.0, 1.0, 1.0, 1.0);
        }
    }
    return vec4(1.0, 1.0, 1.0, 1.0);
}

void main()
{
    switch (outputMode)
    {
        case 0:
        {
            out_color = texture(textureImage, textureCoord);
            break;

        }
        case 1:
        {
            out_color = canny(0.2);
            break;
        }

        case 2:
        {   
            out_color = horizontalBlur(BLUR_RADIUS);
            break;
        }

        case 3:
        {
            out_color = separableBlur(BLUR_RADIUS);
            break;
        }

        case 4:
        {
            out_color = hashPattern(-100, 400.0, 0.8, 0.3, false, textureBlurred);
            break;
        }

        case 5: 
        {
            out_color = hashPattern(500, 20.0, 0.2, 0.2, true, textureBlurred);
            break;
        }

        case 6: 		
        {
			out_color = hashPattern(700, 300.0, 0.01, 0.25, false, textureBlurred);
			break;
		}

        case 7:
        {
            textureCoord.y = 1 - textureCoord.y;
            vec4 hash1 = hashPattern(-100, 400.0, 0.8, 0.3, false, textureBlurred);
            vec4 hash2 = hashPattern(500.0, 20.0, 0.2, 0.2, true, textureBlurred);
            vec4 hash3 = hashPattern(700, 300.0, 0.01, 0.25, false, textureBlurred);

            vec4 combined = min(hash1, min(hash2, hash3));
            out_color = combined;
			break;
		}

        case 8:
        {   vec4 hatchingResult = texture(textureHatching, textureCoord);
            textureCoord.y = 1 - textureCoord.y;
            vec4 sobelResult = texture(textureSobel, textureCoord);
            
            out_color = min(sobelResult, hatchingResult);
            break;
        }

        default:
        {
            out_color = texture(textureImage, textureCoord);
            break;
        }
    }
}
