//
// Created by ditto on 06/07/2022.
//

#include "SceneShader.h"

#include <utility>
#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fstream>

std::map<std::string, ParamType> TYPE_NAMES = {
        {"int", INT},
        {"vec4_int", VEC4_INT},
        {"float", FLOAT},
        {"vec4_float", VEC4_FLOAT},
        {"mat4_float", MAT4_FLOAT}
};


ShaderParam::ShaderParam(std::string name, ParamType tag, Shader *shader, bool isValueInitialized):
    name(std::move(name)),
    tag(tag),
    shader(shader),
    isValueInitialized(isValueInitialized)
  {
  }

ShaderIntParam::ShaderIntParam(std::string name, int value, Shader *shader):
    ShaderParam(std::move(name), INT, shader, true),
    value(value)
   {
   }

ShaderIntParam::ShaderIntParam(std::string name, Shader *shader):
    ShaderParam(std::move(name), INT, shader, false)
 {
 }

void ShaderIntParam::updateUniformValue(int newVal) {
    value = newVal;
    isValueInitialized = true;
}

void ShaderIntParam::uploadUniform() {
    if(isValueInitialized)
        shader->SetUniform1i(getName(), value);
}

std::shared_ptr<ShaderParam> ShaderIntParam::clone() {
    if(isValueInitialized)
        return std::make_shared<ShaderIntParam>(name, value, shader);
    return std::make_shared<ShaderIntParam>(name, shader);
}


ShaderIntVec4Param::ShaderIntVec4Param(std::string name, Eigen::Vector4i value, Shader *shader):
    ShaderParam(std::move(name), VEC4_INT, shader, true),
    value(value)
{
}

ShaderIntVec4Param::ShaderIntVec4Param(std::string name, Shader *shader):
    ShaderParam(std::move(name), VEC4_INT, shader, false)
 {
 }

void ShaderIntVec4Param::updateUniformValue(Eigen::Vector4i newVal) {
    value = newVal;
    isValueInitialized = true;
}

void ShaderIntVec4Param::uploadUniform() {
    if(isValueInitialized)
        shader->SetUniform4i(getName(), value[0], value[1], value[2], value[3]);
}

std::shared_ptr<ShaderParam> ShaderIntVec4Param::clone() {
    if(isValueInitialized)
        return std::make_shared<ShaderIntVec4Param>(name, value, shader);
    return std::make_shared<ShaderIntVec4Param>(name, shader);
}

ShaderFloatParam::ShaderFloatParam(std::string name, float value, Shader *shader):
    ShaderParam(std::move(name), FLOAT, shader, true),
    value(value)
{
}

ShaderFloatParam::ShaderFloatParam(std::string name, Shader *shader):
    ShaderParam(std::move(name), FLOAT, shader, false)
{
}

void ShaderFloatParam::updateUniformValue(float newVal) {
    value = newVal;
    isValueInitialized = true;
}

void ShaderFloatParam::uploadUniform() {
    if(isValueInitialized)
        shader->SetUniform1f(getName(), value);
}

std::shared_ptr<ShaderParam> ShaderFloatParam::clone() {
    if(isValueInitialized)
        return std::make_shared<ShaderFloatParam>(name, value, shader);
    return std::make_shared<ShaderFloatParam>(name, shader);
}

ShaderFloatVec4Param::ShaderFloatVec4Param(std::string name, Eigen::Vector4f value, Shader *shader):
        ShaderParam(std::move(name), VEC4_FLOAT, shader, true),
        value(value)
{

}

ShaderFloatVec4Param::ShaderFloatVec4Param(std::string name, Shader *shader):
        ShaderParam(std::move(name), VEC4_FLOAT, shader, false)
{

}

void ShaderFloatVec4Param::updateUniformValue(Eigen::Vector4f newVal) {
    value = newVal;
    isValueInitialized = true;
}

void ShaderFloatVec4Param::uploadUniform() {
    if(isValueInitialized)
        shader->SetUniform4f(getName(), value[0], value[1], value[2], value[3]);
}

std::shared_ptr<ShaderParam> ShaderFloatVec4Param::clone() {
    if(isValueInitialized)
        return std::make_shared<ShaderFloatVec4Param>(name, value, shader);
    return std::make_shared<ShaderFloatVec4Param>(name, shader);
}

ShaderFloatMat4Param::ShaderFloatMat4Param(std::string name, Eigen::Matrix4f value, Shader *shader):
        ShaderParam(std::move(name), MAT4_FLOAT, shader, true),
        value(value)
{
}

ShaderFloatMat4Param::ShaderFloatMat4Param(std::string name, Shader *shader):
        ShaderParam(std::move(name), MAT4_FLOAT, shader, false)
{
}

void ShaderFloatMat4Param::updateUniformValue(Eigen::Matrix4f newVal) {
    value = newVal;
    isValueInitialized = true;
}

void ShaderFloatMat4Param::uploadUniform() {
    if(isValueInitialized)
        shader->SetUniformMat4f(getName(), value);
}

std::shared_ptr<ShaderParam> ShaderFloatMat4Param::clone() {
    if(isValueInitialized)
        return std::make_shared<ShaderFloatMat4Param>(name, value, shader);
    return std::make_shared<ShaderFloatMat4Param>(name, shader);
}

SceneShader::SceneShader(std::string name, int id, Shader *engineShader, std::string shaderFolder):
    name(name),
    id(id),
    engineShader(engineShader)
{
    params = {};
    std::ifstream ifs(shaderFolder + name + ".json");
    if(ifs.fail()) {
        return;
    }
    nlohmann::json jsonObj = nlohmann::json::parse(ifs);
    for(auto &param : jsonObj) {
        std::string paramName = param["name"];
        ParamType type = TYPE_NAMES[param["type"]];
        if(type == INT) {
            if(param.find("default") != param.end()) {
                int val = param["default"];
                params.push_back(std::make_shared<ShaderIntParam>(paramName, val, engineShader));
            } else{
                params.push_back(std::make_shared<ShaderIntParam>(paramName, engineShader));
            }
        } else if (type == VEC4_INT) {
            if(param.find("default") != param.end()) {
                auto val = param["default"];
                params.push_back(std::make_shared<ShaderIntVec4Param>(paramName,
                                                                      Eigen::Vector4i(val[0], val[1], val[2], val[3]),
                                                                      engineShader));
            } else {
                params.push_back(std::make_shared<ShaderIntVec4Param>(paramName, engineShader));
            }
        } else if (type == FLOAT) {
            if(param.find("default") != param.end()) {
                float val = param["default"];
                params.push_back(std::make_shared<ShaderFloatParam>(paramName, val, engineShader));
            }else {
                params.push_back(std::make_shared<ShaderFloatParam>(paramName, engineShader));
            }
        } else if (type == VEC4_FLOAT) {
            if(param.find("default") != param.end()) {
                auto val = param["default"];
                params.push_back(std::make_shared<ShaderFloatVec4Param>(paramName,
                                                                        Eigen::Vector4f(val[0], val[1], val[2], val[3]),
                                                                        engineShader));
            } else {
                params.push_back(std::make_shared<ShaderFloatVec4Param>(paramName, engineShader));
            }
        } else { // type == MAT4_Float
            if(param.find("default") != param.end()) {
                auto val = param["default"];
                Eigen::Matrix4f mat;
                mat << val[0][0], val[0][1], val[0][2], val[0][3],
                        val[1][0], val[1][1], val[1][2], val[1][3],
                        val[2][0], val[2][1], val[2][2], val[2][3],
                        val[3][0], val[3][1], val[3][2], val[3][3];
                params.push_back(std::make_shared<ShaderFloatMat4Param>(paramName, mat, engineShader));
            } else {
                params.push_back(std::make_shared<ShaderFloatMat4Param>(paramName, engineShader));
            }
        }

    }

}

void SceneShader::uploadAllUniforms() {
    for(const auto& param : params)
        param->uploadUniform();
}

void SceneShader::setParams(std::vector<std::shared_ptr<ShaderParam>> newParams) {
    params = newParams;
}
