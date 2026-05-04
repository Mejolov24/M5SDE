#include "CSDFP.h"

String CSDFP::buildPath() {
    String path = "/";
    for (size_t i = 0; i < _pathStack.size(); i++) {
        path += _pathStack[i];
        if (i < _pathStack.size() - 1) path += "/";
    }
    return path;
}

void CSDFP::_goToDir(String dir_name){
    _pathStack.push_back(dir_name);
    _updateDirectoryList();
    return;
}

void CSDFP::_goBack(){
    _pathStack.pop_back();
    _updateDirectoryList();
    return;
}

void CSDFP::_updateDirectoryList() {
    path = buildPath();
    _dirList.clear();
    _isDirectory.clear();

    File root = SD.open(path);
    if (!root) {
        _has_dirs = false;
        return;
    }

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

    _canvas->pushSprite(0,0);

    return;
}

void CSDFP::begin(M5Canvas* targetCanvas, SelectionCallback callback){
    _canvas = targetCanvas;
    _callback = callback;
    _width = _canvas->width();
    _height = _canvas->height();
    return;
}

void CSDFP::open(const char* path){
    if (_active) return;
    if(!_has_dirs) updateDirectoryList(path);
    if (!_has_dirs) return;

    _active = true;

    return;
}

void CSDFP::close(){
    if (!_active) return;
    _active = false;

    return;
}

void CSDFP::process_input(Input input){
    if (!_active) return;

    return;
} 
