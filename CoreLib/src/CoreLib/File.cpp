#include <fstream>
#include <filesystem>

#include "CoreLib\tinyfiledialogs.h"
#include <CoreLib\Log.h>
#include "CoreLib\File.h" 

namespace EngineCore {

    File::File(const std::string& path) 
        : m_path(path) {
    }

    File::~File() {
        Close();
    }

    bool File::Open(FileState fileState, bool append) {
        if (fileState == FileState::FILE_CLOSE) {
            Log::Warn("File: File was not opend, because FileState was set to FILE_CLOSE");
            return false;
        }
        
        if (m_path.empty()) {
            Log::Error("File: Path was empty!");
            return false;
        }

        Close();

        m_fileState = fileState;

        if (fileState == FileState::FILE_WRITE) {
            std::ios_base::openmode mode = std::ios::out;
            if (append)
                mode |= std::ios::app;
            m_ofstream.open(m_path, mode);
            if (!m_ofstream.is_open()) {
                std::filesystem::create_directories(std::filesystem::path(m_path).parent_path());
                m_ofstream.open(m_path, mode);

                if (!m_ofstream.is_open()) {
                    Log::Error("File: Could not open file '{}' for writing!", m_path);
                    m_fileState = FileState::FILE_CLOSE;
                    return false;
                }
            }
        }
        else {
            m_ifstream.open(m_path, std::ios::in);
            if (!m_ifstream.is_open()) {
                Log::Error("File: Could not open file '{}' for reading!", m_path);
                m_fileState = FileState::FILE_CLOSE;
                return false;
            }
        }

        return true;
    }

    void File::Close() {
        if (m_fileState == FileState::FILE_CLOSE)
            return;

        if (m_fileState == FileState::FILE_WRITE && m_ofstream.is_open()) {
            m_ofstream.close();
        }
        else if (m_fileState == FileState::FILE_READ && m_ifstream.is_open()) {
            m_ifstream.close();
        }

        m_fileState = FileState::FILE_CLOSE;
    }

    bool File::Write(const std::string& data) {
        if (m_fileState != FileState::FILE_WRITE || !m_ofstream.is_open()) {
            Log::Error("File: Cannot write, file '{}' not open for writing!", m_path);
            return false;
        }

        m_ofstream << data;

        if (m_ofstream.fail()) {
            Log::Error("File: Write operation failed on file '{}'!", m_path);
            return false;
        }

        return true;
    }

    bool File::Write(const void* data, size_t size) {
        if (m_fileState != FileState::FILE_WRITE || !m_ofstream.is_open()) {
            Log::Error("File: Cannot write, file '{}' not open for writing!", m_path);
            return false;
        }

        m_ofstream.write(reinterpret_cast<const char*>(data), size);

        if (m_ofstream.fail()) {
            Log::Error("File: Write operation failed on file '{}'!", m_path);
            return false;
        }

        return true;
    }


    bool File::ReadAll(std::string& outContent) {
        if (m_fileState != FileState::FILE_READ || !m_ifstream.is_open()) {
            Log::Error("File: Cannot read, file '{}' not open for reading!", m_path);
            return false;
        }

        m_ifstream.seekg(0, std::ios::beg);

        std::ostringstream ss;
        ss << m_ifstream.rdbuf();
        outContent = ss.str();

        return true;
    }

    bool File::Exists() const {
        std::ifstream file(m_path);
        return file.good();
    }

    bool File::IsFileOpen() const {
        if (m_fileState == FileState::FILE_WRITE) {
            return m_ofstream.is_open();
        }
        else if (m_fileState == FileState::FILE_READ) {
            return m_ifstream.is_open();
        }
    }

    size_t File::GetFileSize() const {
        std::ifstream file(m_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            Log::Error("File: Could not open file '{}' to get size!", m_path);
            return 0;
        }
        return static_cast<size_t>(file.tellg());
    }

    std::string File::GetFilePath() const {
        return m_path;
    }

    File& File::SetFilePath(const std::string& path) {
        m_path = path;
        return *this;
    }

    #pragma region static
    
    bool File::Exists(const std::string& path) {
        std::ifstream file(path);
        return file.good();
    }

    bool File::DeleteFile(const std::string& path) {
        if (std::remove(path.c_str()) != 0) {
            Log::Error("File: Could not delete file '{}'!", path);
            return false;
        }
        return true;
    }

    bool  File::CreateDir(const std::string& dir) {
        return std::filesystem::create_directories(dir);
    }

    std::string File::OpenFileDialog(const std::string& title, const std::string& defaultPath) {
        static const char filter[] = "All Files\0*.*\0";
        return OpenFileDialog(title, defaultPath, filter);
    }

    std::string File::OpenFileDialog(const std::string& title, const std::string& defaultPath, const char* filter) {
        const char* filterPatterns[1] = { filter ? filter + strlen("All Files") + 1 : "*.*" };

        const char* result = tinyfd_openFileDialog(
            title.c_str(),
            defaultPath.empty() ? nullptr : defaultPath.c_str(),
            1,
            filterPatterns,
            nullptr,
            0
        );

        return result ? std::string(result) : std::string();
    }

    std::string File::ConvertFilterString(const std::string& extensions) {
        std::string result = "Custom Files";
        result.push_back('\0');
        std::string pattern;

        size_t start = 0;
        while (start < extensions.size()) {
            size_t end = extensions.find(',', start);
            if (end == std::string::npos) end = extensions.size();

            std::string ext = extensions.substr(start, end - start);
            // Trim whitespace
            ext.erase(0, ext.find_first_not_of(" \t"));
            ext.erase(ext.find_last_not_of(" \t") + 1);

            if (!ext.empty()) {
                if (!pattern.empty()) pattern += ';';
                // Ensure extension starts with '*'
                if (ext[0] != '*')
                    pattern += '*';
                pattern += ext;
            }
            start = end + 1;
        }
        result += pattern;
        result.push_back('\0');
        return result;
    }

    #pragma endregion

}