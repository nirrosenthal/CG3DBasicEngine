//
// Created by ditto on 06/07/2022.
//

#ifndef ASSIGNMENT1_CPP_SCENESHADER_H
#define ASSIGNMENT1_CPP_SCENESHADER_H


#include <string>
#include <igl/opengl/glfw/shader.h>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

enum ParamType {INT, VEC4_INT, FLOAT, VEC4_FLOAT, MAT4_FLOAT};
class ShaderParam {
public:
    ShaderParam(std::string name, ParamType tag, Shader *shader, bool isValueInitialized);
    std::string getName() {return name;};
    ParamType getTag() {return tag;};
    bool getIsInitialized() {return isValueInitialized;};
    virtual void uploadUniform()=0;
    virtual std::shared_ptr<ShaderParam> clone()=0;
protected:
    bool isValueInitialized;
    Shader* shader;
    std::string name;
private:
    ParamType tag;
};

class ShaderIntParam: public ShaderParam {
public:
    ShaderIntParam(std::string name, int value, Shader *shader);
    ShaderIntParam(std::string name, Shader *shader);
    void updateUniformValue(int newVal);
    int getValue() {return value;};
    void uploadUniform();
    std::shared_ptr<ShaderParam> clone();
    int value;
};

class ShaderIntVec4Param: public ShaderParam {
public:
    ShaderIntVec4Param(std::string name, Eigen::Vector4i value, Shader *shader);
    ShaderIntVec4Param(std::string name, Shader *shader);
    void updateUniformValue(Eigen::Vector4i newVal);
    Eigen::Vector4i getValue() {return value;};
    void uploadUniform();
    std::shared_ptr<ShaderParam> clone();
    Eigen::Vector4i value;
};

class ShaderFloatParam: public ShaderParam {
public:
    ShaderFloatParam(std::string name, float value, Shader *shader);
    ShaderFloatParam(std::string name, Shader *shader);
    void updateUniformValue(float newVal);
    float getValue() {return value;};
    void uploadUniform();
    std::shared_ptr<ShaderParam> clone();
    float value;
};

class ShaderFloatVec4Param: public ShaderParam {
public:
    ShaderFloatVec4Param(std::string name, Eigen::Vector4f value, Shader *shader);
    ShaderFloatVec4Param(std::string name, Shader *shader);
    void updateUniformValue(Eigen::Vector4f newVal);
    Eigen::Vector4f getValue() {return value;};
    void uploadUniform();
    std::shared_ptr<ShaderParam> clone();
    Eigen::Vector4f value;
};
class ShaderFloatMat4Param: public ShaderParam {
public:
    ShaderFloatMat4Param(std::string name, Eigen::Matrix4f value, Shader *shader);
    ShaderFloatMat4Param(std::string name, Shader *shader);
    void updateUniformValue(Eigen::Matrix4f newVal);
    Eigen::Matrix4f getValue() {return value;};
    void uploadUniform();
    std::shared_ptr<ShaderParam> clone();
    Eigen::Matrix4f value;
};


class SceneShader {
public:
    SceneShader(std::string name, int id, Shader *engineShader, std::string shaderFolder);
    std::string getName() {return name;};
    int getId() {return id;};
    void uploadAllUniforms();
    std::vector<std::shared_ptr<ShaderParam>> getParams() {return params;};
    void setParams(std::vector<std::shared_ptr<ShaderParam>> newParams);
private:
    std::string name;
    int id;
    Shader *engineShader;
    std::vector<std::shared_ptr<ShaderParam>> params;

};


#endif //ASSIGNMENT1_CPP_SCENESHADER_H
