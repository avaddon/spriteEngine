#include <iomanip>
#include <png.h>
#include "Texture.h"

Texture::Texture(GLsizei items) :
    Logger(),
    m_textureID(0),
    m_items(items)
{
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glGenSamplers(1, &m_Sampler);
}


Texture::~Texture()
{
    glDeleteSamplers(1, &m_Sampler);
    glDeleteTextures(1, &m_textureID);
}

void Texture::LoadTexture(GLuint width, GLuint height, const GLvoid *imageData)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

void Texture::LoadPNGTexture(const string &filename)
{
    png_structp ptrPNG;
    png_infop ptrInfo;
    unsigned int sig_read = 0;
    int color_type, interlace_type;

    FILE *fp;
    if ((fp = fopen(filename.c_str(), "rb")) == NULL)
        throw 1;

    ptrPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (ptrPNG == NULL)
    {
        fclose(fp);
        throw 2;
    }

    ptrInfo = png_create_info_struct(ptrPNG);
    if (ptrInfo == NULL)
    {
        fclose(fp);
        png_destroy_read_struct(&ptrPNG, NULL, NULL);
        throw 2;
    }

    if (setjmp(png_jmpbuf(ptrPNG)))
    {
        png_destroy_read_struct(&ptrPNG, NULL, NULL);
        fclose(fp);
        throw 2;
    }

    png_init_io(ptrPNG, fp);
    png_set_sig_bytes(ptrPNG, sig_read);

    png_read_png(ptrPNG, ptrInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(ptrPNG, ptrInfo, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

    unsigned int row_bytes = (unsigned int)png_get_rowbytes(ptrPNG, ptrInfo);
    row_bytes += 3 - ((row_bytes - 1) % 4);

    png_byte *outData = (png_byte *)malloc(row_bytes * height * sizeof(png_byte) + 15);
    if (outData == NULL)
    {
        Error("Could not allocate memory for PNG image data. Texture: ", quoteStr(filename));

        png_destroy_read_struct(&ptrPNG, &ptrInfo, NULL);
        fclose(fp);
        throw 2;
    }
    png_bytepp row_pointers = png_get_rows(ptrPNG, ptrInfo);

    for (size_t i = 0; i < height; i++)
    {
        memcpy(outData + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);
    }

    png_destroy_read_struct(&ptrPNG, &ptrInfo, NULL);

    fclose(fp);

    GLint format;
    switch (color_type)
    {
    case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
        break;
    default:
        Error("Unknown libpng color type ", color_type, " in file: ", quoteStr(filename));
        throw 2;
    }

    Debug("Loaded ", quoteStr(filename));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, outData);

    free(outData);

    setFiltering();
}

void Texture::BindTexture(GLuint textureUnit)
{
    DEBUG_OK();

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glBindSampler(textureUnit, m_Sampler);
}

void Texture::setFiltering()
{
    glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLuint Texture::GetID() const
{
    return GLuint(m_textureID);
}

GLsizei Texture::GetItems() const
{
    return GLsizei(m_items);
}

bool Texture::DEBUG_OK() const
{
    HW_ASSERT(m_textureID > 0);
    HW_ASSERT(m_Sampler > 0);
    HW_ASSERT(m_items >= 0);

    return true;
}

string Texture::DEBUG_DUMP() const
{
    std::stringstream result;
    result << endl;
    result << DUMP_VAR(m_textureID) << DUMP_VAR(m_Sampler) << DUMP_VAR(m_items) << endl;
    return result.str();
}
