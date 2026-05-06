#ifndef SDFP
#define SDFP
#include <stdint.h>
#include <vector>
#include <SD.h>
#include <M5Cardputer.h>
extern M5Canvas canvas;

class CSDFP{
    public:
    struct ExplorerSettings{
        const uint16_t directory_color = YELLOW;
        const uint16_t background_color = BLACK;
        const uint16_t border_color = WHITE;
        const uint16_t selection_color = BLUE;
        const uint16_t text_color = WHITE;
        const uint16_t item_height = 16;
        const uint16_t item_window = 6;
    };

    private:

    typedef void (*SelectionCallback)(const char* path);
    SelectionCallback _callback = nullptr;
    ExplorerSettings* _settings;
    
    M5Canvas* _canvas;
    uint8_t _width = 0;
    uint8_t _half_width = 0;
    uint8_t _height = 0;
    int8_t _cursor_index = 0; // selection position
    uint16_t _cursor_offset = 0; // real position
    uint16_t _selection = 0; // global pos of selection
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

    enum class Input : uint8_t{
        up = 0,
        down = 1,
        left = 2,
        right = 3,
        back = 4,
        select = 5
    };

    void begin(M5Canvas* targetCanvas, SelectionCallback callback);
    void open(ExplorerSettings* settings = nullptr); // opens the file picker
    void close(); // closes the file picker
    void goToAbsoluteDir(const char* path);
    void process_input(Input input);
};



#endif