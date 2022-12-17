/**
 * @file compile.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-30
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <string>

#include "parse/parse.h"

using namespace mocoder;

std::shared_ptr<RootNode> Compile(const std::string& source);
