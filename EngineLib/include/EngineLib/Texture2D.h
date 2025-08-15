#pragma once
#include <string>
#include "EngineTypes.h"

typedef unsigned int GLenum;

class Vector2;

namespace EngineCore {

    // Constant to improve code readability when passing 'FALLBACK' mode.
    const bool TEXTURE2D_FALLBACK = true;

    /**
    * @brief Represents a 2D texture that can be loaded from file and used in OpenGL.
    */
    class Texture2D {
    public:
        /**
        * @brief Default constructor. Does not create/load a texture.
        */
        Texture2D(bool useFallBack = false);

        /**
        * @brief Creats/loads a texture with default settings
        */
        Texture2D(const char* path);

        /**
        * @brief Creats an image with the raw data.
        * @param data is the raw image data
        * @param width of the image
        * @param height of the image
        * @param nrChannels of the image (GL_RED, GL_RGB, GL_RGBA)
        */
        Texture2D(const unsigned char* data, int width, int height, int nrChannels);

        /**
        * @brief Destructor. Deletes the OpenGL texture if valid.
        */
        ~Texture2D();

        /**
        * @brief Loads an image from file and creates an OpenGL texture.
        * @param path Path to the image file.
        */
        void Create(const char* path);

        /**
        * @brief Creats an image with the raw data.
        * @param data is the raw image data
        * @param width of the image
        * @param height of the image
        * @param nrChannels of the image (GL_RED, GL_RGB, GL_RGBA)
        */
        void Create(unsigned char* data, int width, int height, int nrChannels);

        /**
        * @brief Loads/creates an OpenGL texture. if the texture already has a path assigned
        */
        void CreateGL();

        /**
        * @brief Deletes the OpenGL object
        */
        void DeleteGL();

        /**
        * @brief Binds the texture to the given texture unit.
        * @param unit The texture unit index (default: 0).
        */
        void Bind(unsigned int unit = 0);

        /**
        * @brief Unbinds the texture from the given texture unit if currently bound.
        * @param unit The texture unit index (default: 0).
        */
        void Unbind(unsigned int unit = 0);

        /**
        * @brief Sets the same texture coordinate wrapping mode for both X and Y directions. 
        * Must be set before the texture is created
        *
        * Possible values:
        * - GL_REPEAT: Repeats the texture image (tiles it). Coordinates outside [0,1] wrap around.
        * - GL_MIRRORED_REPEAT: Like GL_REPEAT, but mirrors the image every integer repeat.
        * - GL_CLAMP_TO_EDGE: Clamps the coordinates to the edge, so the edge pixel is stretched.
        * - GL_CLAMP_TO_BORDER: Uses a specified border color for coordinates outside [0,1].
        *
        * @param wrappingMode OpenGL wrap mode enum.
        */
        void SetWrapping(unsigned int wrappingMode);

        /**
        * @brief Sets the wrapping mode for the X (S) axis. 
        * Must be set before the texture is created
        * @param wrappingMode See SetWrapping for valid values.
        */
        void SetWrappingX(unsigned int wrappingMode);

        /**
        * @brief Sets the wrapping mode for the Y (T) axis.
        * Must be set before the texture is created
        * @param wrappingMode See SetWrapping for valid values.
        */
        void SetWrappingY(unsigned int wrappingMode);

        /**
        * @brief Sets the same texture filtering mode for both minification and magnification.
        * Must be set before the texture is created
        * Filtering determines how the texture is sampled when it's scaled.
        *
        * Possible values:
        * - GL_NEAREST: Chooses the nearest pixel (blocky look).
        * - GL_LINEAR: Performs linear interpolation between nearby pixels (smooth look).
        *
        * @param filterMode OpenGL filter mode enum.
        */
        void SetFilter(unsigned int filterMode);

        /**
        * @brief Sets the texture filtering mode used when the texture is minified (scaled down).
        * Must be set before the texture is created
        * 
        * For minification only (SetFilterMin), mipmap-compatible options are also valid:
        * - GL_NEAREST_MIPMAP_NEAREST: Nearest mipmap, nearest texel.
        * - GL_LINEAR_MIPMAP_NEAREST: Nearest mipmap, linear sampling inside.
        * - GL_NEAREST_MIPMAP_LINEAR: Linear between mipmaps, nearest texel inside.
        * - GL_LINEAR_MIPMAP_LINEAR: Fully linear (best quality).
        *
        * @param filterMode See SetFilter for valid values.
        */
        void SetFilterMin(unsigned int filterMode);

        /**
        * @brief Sets the texture filtering mode used when the texture is magnified (scaled up).
        * Must be set before the texture is created
        * 
        * Only GL_NEAREST and GL_LINEAR are valid for magnification.
        *
        * @param filterMode OpenGL filter mode enum.
        */
        void SetFilterMag(unsigned int filterMode);


        /**
         * @brief Enables or disables automatic mipmap generation after loading.
         * Must be set before the texture is created
         */
        void SetGenerateMipmaps(bool enable);

        /**
         * @return The OpenGL texture ID.
         */
        unsigned int GetID() const;

        /**
         * @return The file path from which the texture was loaded.
         */
        std::string GetPath() const;

        /**
         * @return The width of the texture in pixels.
         */
        int GetWidth() const;

        /**
         * @return The height of the texture in pixels.
         */
        int GetHeight() const;

        /**
         * @return The dimensions of the texture as a Vector2.
         */
        Vector2 GetSize() const;

        /**
         * @return The number of channels in the loaded image (e.g. 3 = RGB, 4 = RGBA).
         */
        int GetNrChannels() const;

    private:
        bool m_exists = false;

        unsigned int m_opengGLID = ENGINE_INVALID_ID;
        std::string m_path = "";
        unsigned char* m_imageData = nullptr;// gets only set if the constructor is used where the data is set manully
        int m_width = 0;
        int m_height = 0;
        int m_nrChannels = -1;

        unsigned int m_wrappingX;
        unsigned int m_wrappingY;

        // Filter used when the texture is downscaled
        unsigned int m_filterMin;

        // Filter used when the texture is upscaled
        unsigned int m_filterMag;

        bool m_createMipmaps = true;

        void LoadTextureFallback();
        unsigned char* GenerateFallbackTexture();
    };

}