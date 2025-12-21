#include <fstream>

typedef unsigned char byte;

struct IInputStream {
	virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
	virtual void Write(byte value) = 0;
};

class FileInputStream : public IInputStream {
public:
    FileInputStream(const std::string& filename) {
        file.open(filename, std::ios::in | std::ios::binary);
    }

    bool Read(byte& value) override {
        char buffer;
        if (file.get(buffer)) {
            value = static_cast<byte>(buffer);
            return true;
        }
        return false;
    }

private:
    std::ifstream file;
};


class FileOutputStream : public IOutputStream {
public:
    FileOutputStream(const std::string& filename) {
        file.open(filename, std::ios::out | std::ios::binary);
    }

    void Write(byte value) override {
        file.put(static_cast<char>(value));
    }

private:
    std::ofstream file;
};


void Encode(IInputStream& original, IOutputStream& compressed);

void Decode(IInputStream& compressed, IOutputStream& original);
