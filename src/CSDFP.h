# ifndef SDFP
# define SDFP
#include <stdint.h>
#include <vector>
#include <SD.h>
#include <M5Cardputer.h>
extern M5Canvas canvas;

enum class Input : uint8_t{
    up = 0,
    down = 1,
    left = 2,
    right = 3,
    back = 4,
    select = 5
};

class CSDFP{
    private:
    typedef void (*SelectionCallback)(const char path);
    
    M5Canvas* _canvas;
    SelectionCallback _callback = nullptr;
    uint8_t _width = 0;
    uint8_t _height = 0;
    std::vector<String> _pathStack; // stores the current navigation path
    std::vector<String> _dirList;  // stores what the current dir has
    std::vector<bool> _isDirectory;   // parallel vector to remember if _dirList[i] is a folder
    bool _active = false;
    bool _has_dirs = false;
    void _updateDirectoryList();
    void _goToDir(String dir_name);
    void _goBack();
    void _render();
    String _buildPath();
    


    public:
    void begin(M5Canvas* targetCanvas, SelectionCallback callback);
    void open(const char* path); // opens the file picker
    void close(); // closes the file picker
    void process_input(Input input);
};



#endif