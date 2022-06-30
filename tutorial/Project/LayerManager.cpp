#include "LayerManager.h"
#include <iostream>
#include <utility>

// create new Layer goes by LayerName. also initiate shapes as list of Shapes and set hidden to false 
Layer::Layer(std::string layerName): layerName(std::move(layerName)), hidden(false)
{
}

void Layer::addShape(std::shared_ptr<SceneShape> shape) {
    shapes.push_back(shape);
}

void Layer::deleteShape(std::shared_ptr<SceneShape> shape) {
    std::vector<std::shared_ptr<SceneShape>> newShapes;
    for(std::shared_ptr<SceneShape> s : shapes) {
        if(s != shape) {
            newShapes.push_back(s);
        }
    }
    shapes = newShapes;
}


// create a new LayerManager by initating layers as a list of Layers and inserting the default layer at first
LayerManager::LayerManager() = default;

// returns the layer with the given name, or nullptr if it doesn't exist.
std::shared_ptr<Layer> LayerManager::getLayer(const std::string& name) {
    if(layers.find(name) == layers.end())
        return nullptr;
    return layers[name];
}

// Creates a layer with the given name, returns nullptr iff it fails.
std::shared_ptr<Layer> LayerManager::addLayer(const std::string& layerName) {
    if(layers.find(layerName) != layers.end())
        return nullptr;
    auto newLayer = std::make_shared<Layer>(layerName);
    layers[layerName] = newLayer;
    return newLayer;
}

// Deletes the given layer, returns false iff it fails.
bool LayerManager::removeLayer(std::shared_ptr<Layer> layer) {
    auto it = layers.find(layer->getName());
    if(it == layers.end() || !layer->getShapes().empty())
        return false;
    layers.erase(it);
    return true;
}