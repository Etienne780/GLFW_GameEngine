#pragma once
#include <string>

typedef std::string String;
typedef unsigned int GLenum;

class Vector2;

namespace EngineCore {

    /**
     * @brief Represents a 2D texture that can be loaded from file and used in OpenGL.
     */
    class Texture2D {
        friend class Engine;
    public:
        /**
         * @brief Default constructor. Does not create or load a texture.
         */
        Texture2D();

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
         * @brief Binds the texture to the given texture unit.
         * @param unit The texture unit index (default: 0).
         */
        void Bind(unsigned int unit = 0) const;

        /**
         * @brief Unbinds the texture from the given texture unit if currently bound.
         * @param unit The texture unit index (default: 0).
         */
        void Unbind(unsigned int unit = 0) const;

        /**
        * @brief Sets the same texture coordinate wrapping mode for both X and Y directions.
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
        * @param wrappingMode See SetWrapping for valid values.
        */
        void SetWrappingX(unsigned int wrappingMode);

        /**
        * @brief Sets the wrapping mode for the Y (T) axis.
        * @param wrappingMode See SetWrapping for valid values.
        */
        void SetWrappingY(unsigned int wrappingMode);

        /**
        * @brief Sets the same texture filtering mode for both minification and magnification.
        *
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
        *
        * Only GL_NEAREST and GL_LINEAR are valid for magnification.
        *
        * @param filterMode OpenGL filter mode enum.
        */
        void SetFilterMag(unsigned int filterMode);


        /**
         * @brief Enables or disables automatic mipmap generation after loading.
         */
        void SetGenerateMipmaps(bool enable);

        /**
         * @return The OpenGL texture ID.
         */
        int GetID() const;

        /**
         * @return The file path from which the texture was loaded.
         */
        String GetPath() const;

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
        static unsigned char* missingTexture;

        unsigned int m_ID = -1;
        String m_path = "";
        int m_width = 0;
        int m_height = 0;
        int m_nrChannels = -1;
        GLenum m_format;

        unsigned int m_wrappingX;
        unsigned int m_wrappingY;

        // Filter used when the texture is downscaled
        unsigned int m_filterMin;

        // Filter used when the texture is upscaled
        unsigned int m_filterMag;

        bool m_createMipmaps = true;

        void LoadTextureFallback();
        unsigned char* GenerateFallbackTexture();

        static void Cleanup();
    };

}