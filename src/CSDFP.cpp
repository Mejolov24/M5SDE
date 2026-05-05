#include "CSDFP.h"

String CSDFP::_buildPath() {
    String path = "/";
    for (size_t i = 0; i < _pathStack.size(); i++) {
        path += _pathStack[i];
        //path += "/";
    }
    return path;
}

void CSDFP::_goToDir(String dir_name){
    _pathStack.push_back(dir_name);
    _updateDirectoryList();
    return;
}

void CSDFP::_goToAbsoluteDir(const char* path) {
    _pathStack.clear();

    if (!path || path[0] == '\0') return;

    const char* start = path;

    if (*start == '/') start++;

    const char* ptr = start;

    while (*ptr) {
        if (*ptr == '/') {
            if (ptr > start) {
                _pathStack.push_back(String(start).substring(0, ptr - start));
            }
            start = ptr + 1;
        }
        ptr++;
    }

    if (ptr > start) {
        _pathStack.push_back(String(start).substring(0, ptr - start));
    }

    _updateDirectoryList();
}

void CSDFP::_goBack(){
    _pathStack.pop_back();
    _updateDirectoryList();
    return;
}

void CSDFP::_updateDirectoryList() {
    String path = _buildPath();
    _dirList.clear();
    _isDirectory.clear();

    File root = SD.open(path);
    if (!root) {
        _has_dirs = false;
        return;
    }
    else _has_dirs = true;

    File file = root.openNextFile();
    while(file){
        if (file.isDirectory()){
            _isDirectory.push_back(true);
        }
        else{
            _isDirectory.push_back(false);
        }
        _dirList.push_back(String(file.name()));
        file = root.openNextFile();
    }
    root.close();
}


void CSDFP::_render(){
    if (_canvas == nullptr) return;
    if (!_active) return;
    _canvas->fillScreen(BG_COLOR);
    _canvas->drawRect(0, 0, _width, ITEM_HEIGHT, DIR_COLOR);
    _canvas->setTextColor(TEXT_COLOR);
    _canvas->setTextDatum(TC_DATUM);
    _canvas->drawString(_buildPath(), _half_width , 0, &fonts::Font2);
    _canvas->setTextDatum(TL_DATUM);

    uint16_t files_amount = _dirList.size();
    uint16_t draw_offset = ITEM_HEIGHT;
    uint16_t selection_cursor = _cursor_offset + _cursor_index;
    uint8_t window_offset = _cursor_offset + ITEM_WINDOW; // until where we are seeing in the vector
    //if (_cursor_offset > files_amount){window_offset = files_amount;}
    
    
    for (uint16_t i = _cursor_offset; i <= window_offset; i++){
        String current_item_name = _dirList[i];
        bool is_dir = _isDirectory[i];

        if(is_dir) current_item_name = "/" + current_item_name;
        if(i == selection_cursor){
            _canvas->fillRect(0, draw_offset, _width, ITEM_HEIGHT, SELECTION_COLOR);
        }
    
        _canvas->drawRect(0, draw_offset, _width, ITEM_HEIGHT, BORDER_COLOR);
        _canvas->drawString(current_item_name, 0, draw_offset, &fonts::Font2);
        draw_offset += ITEM_HEIGHT;

    }


    _canvas->pushSprite(0,0);

    return;
}

void CSDFP::begin(M5Canvas* targetCanvas, SelectionCallback callback){
    _canvas = targetCanvas;
    _callback = callback;
    _width = _canvas->width();
    _height = _canvas->height();
    _half_width = int(_width / 2);
    return;
}

void CSDFP::open(const char* path){
    if (_active) return;
    _goToAbsoluteDir(path);
    if(!_has_dirs) _updateDirectoryList();
    if (!_has_dirs) return;

    _active = true;
    _render();

    return;
}

void CSDFP::close(){
    if (!_active) return;
    _active = false;

    return;
}

void CSDFP::process_input(Input input){
    if (!_active) return;
    uint16_t files_amount = (int16_t)_dirList.size();

    switch (input)
    {
    case Input::up :
        _cursor_index--;
        // If we move off the top of the current window
        if (_cursor_index < 0) {
            if (_cursor_offset > 0) {
                // Scroll the window up
                _cursor_index = 0;
                _cursor_offset--;
            } else {
                // Wrap around to the very end of the list
                _cursor_offset = (files_amount > ITEM_WINDOW) ? (files_amount - (ITEM_WINDOW + 1)) : 0;
                _cursor_index = (files_amount > ITEM_WINDOW) ? ITEM_WINDOW : (files_amount - 1);
            }
        }
        break;

    case Input::down :
        _cursor_index++;
        // If we move off the bottom of the current window
        if (_cursor_index > ITEM_WINDOW or (_cursor_offset + _cursor_index) >= files_amount) {
            if ((_cursor_offset + ITEM_WINDOW + 1) < files_amount) {
                // Scroll the window down
                _cursor_index = ITEM_WINDOW;
                _cursor_offset++;
            } else {
                // Wrap around to the very beginning
                _cursor_offset = 0;
                _cursor_index = 0;
            }
        }
        break;

    case (Input::select):
        {String current_selection = _dirList[_selection];
        if(_isDirectory[_selection]){
            _goToDir(current_selection);
        }
        else{
            String string_path = _buildPath();
            char char_path[string_path.length() + 1];
            strcpy(char_path, string_path.c_str());
            _callback(char_path);}
        break;}
    
    case Input::back:
        _goBack();
        break;

    default:
    break;
    }
    _selection = _cursor_offset + _cursor_index;
    _render();
}
