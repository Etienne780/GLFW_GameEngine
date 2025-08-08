#pragma once
#include <fstream>

namespace EngineCore {

    enum FileState {
        FILE_CLOSE = 0,
        FILE_WRITE,
        FILE_READ
    };

    const bool APPEND_TO_FILE = true;

    class File {
    public:
        File(const std::string& path);
        ~File();
        
        bool Open(FileState fileState, bool append = false);
        void Close();
    
        bool Write(const std::string& data);
        bool Write(const void* data, size_t size);
    
        bool ReadAll(std::string& outContent);
        bool Exists() const;

        bool IsFileOpen() const;

        size_t GetFileSize() const;
        std::string GetFilePath() const;
        File& SetFilePath(const std::string& path);
    
        static bool Exists(const std::string& path);
        static bool DeleteFile(const std::string& path);
    
    private:
        std::string m_path;
        std::ifstream m_ifstream;
        std::ofstream m_ofstream;
        FileState m_fileState = FileState::FILE_CLOSE;
    };

}