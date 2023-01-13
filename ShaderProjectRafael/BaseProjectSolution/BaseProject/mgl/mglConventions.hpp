////////////////////////////////////////////////////////////////////////////////
//
// Shader Naming Conventions 
//
// (c)2022 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CONVENTIONS_HPP
#define MGL_CONVENTIONS_HPP

#include <string>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

const std::string MODEL_MATRIX = "ModelMatrix";
const std::string NORMAL_MATRIX = "NormalMatrix";
const std::string VIEW_MATRIX = "ViewMatrix";
const std::string PROJECTION_MATRIX = "ProjectionMatrix";
const std::string TEXTURE_MATRIX = "TextureMatrix";
const std::string CAMERA_BLOCK = "Camera";

const std::string POSITION_ATTRIBUTE = "inPosition";
const std::string NORMAL_ATTRIBUTE = "inNormal";
const std::string TEXCOORD_ATTRIBUTE = "inTexcoord";
const std::string TANGENT_ATTRIBUTE = "inTangent";
const std::string BITANGENT_ATTRIBUTE = "inBitangent";
const std::string COLOR_ATTRIBUTE = "inColor";
const std::string ACTUAL_COLOR_ATTRIBUTE = "actualColor";
const std::string MAIN_COLOR_ATTRIBUTE = "mainColor";
const std::string VEIN_COLOR_ATTRIBUTE = "veinColor";
const std::string SAMPLER_ATTRIBUTE = "sampler";
const std::string IS_STAR_ATTRIBUTE = "isStar";
const std::string LIGHT_POSITION = "lightPosition";

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CONVENTIONS_HPP */
