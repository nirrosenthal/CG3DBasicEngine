#include "LayerManager.h"
#include <iostream>
using namespace std;

// create new Layer goes by LayerName. also initiate shapes as list of Shapes and set hidden to false 
LayerManager::Layer(string LayerName): layerName(LayerName), shapes(new list<Shape>()), hidden(false){}



// create a new LayerManager by initating layers as a list of Layers and inserting the default layer at first
LayerManager::LayerManager(): layers(new list<Layer>()) {
	layers.push_back(new Layer("default layer"));
}

// side function to find layer layerName on layers using std find function
// if layerName isn't in layerName, returns layers.end() indicator
Layer findLayer(string layerName) {
	for (it = layers.begin(); it != layers.end(); ++it) {
		Layer l = *it;
		if (l.layerName == layerName)
			return l;
	}
	return layers.end();
}

// side function to check if layerName doenst exsist in layers
// return true if layerName isn't in layers
//		  or false if layerName is in layers
boolean isntInLayers(string layerName) {
	Layer l = findLayer(layerName);
	return l == end(layers)
}

// get layerName from layers.
// 
// @inv: layerName exsits in layers
boolean LayerManager::isHidden(string layerName) {
	Layer l = findLayer(layerName);
	return l.hidden;
}

// hide layerName in layers.
// 
// @inv: layerName exsits in layers
void LayerManager::hideLayer(string layerName) {
	if (isntInLayers(layerName))
		return;
	Layer l = findLayer(layerName);
	l.changeHidden(true);
}

// show layerName in layers.
// 
// @inv: layerName exsits in layers
void LayerManager::showLayer(string layerName) {
	if (isntInLayers(layerName))
		return;
	Layer l = findLayer(layerName);
	l.changeHidden(false);
}

// add shape to layerName in layers 
// 
// @inv: layerName exsists in layers and shape isnt in layerName.shapes

void LayerManager::addShape(string layerName, Shape shape) {
	if (isntInLayers(layerName))
		return;
	Layer l = findLayer(layerName);
	l.shapes.push_back(shape);
}

// remove shape from layerName's object list
//
// @inv: layerName exists in layers and shape is in layerName.shapes
void LayerManager::removeShape(string layerName, Shape shape) {
	if (isntInLayers(layerName))
		return;
	Layer l = findLayer(layerName);
	l.shapes.remove(shape);
}

// try to add new layer that goes by layerName to layers.
//
// @return: return true if layerName isn't in layers (meaning the name is usable)
//			or false if layerName is in layers.
boolean LayerManager::addLayer(string layerName) {
	if (!isntInLayers(layerName)) // checks if layerName is not isntInLayers (meaning it is in layers)
		return false;
	Layer l = findLayer(layerName);
	layers.insert(new Layer(layerName));
	return true;
}

void LayerManager::removeLayer(string layerName) {
	if (layerName == "default layer" || isntInLayers(layerName))
		return;
	Layer l = findLayer(layerName);
	layers.remove(l);
}