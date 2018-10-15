/*
 * Copyright (C) 2018- kaijiang
 *
 * Author:
 *	kaijiang
 *
 * Source:
 *	https://github.com/kaijiang/dimsum
 *
 * This file is a part of dimsum tool
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#include "dimsumv2.hpp"
#include "dimsum_config.h"
#include "boost/program_options.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    namespace po = boost::program_options;
    std::string desc_text = "compute row vector pairs' similarity.";
    po::options_description desc(desc_text);
    desc.add_options()
        ("help,h", "show this message")
        ("version,v", "show version")
        ("data,d", po::value<std::string>(), "matrix data file")
        ("output,o", po::value<std::string>(), "outout similarity to")
        ("mirror",  "output similarity <i, j> and <j, i> same time")
        ("threshold,t", po::value<float>()->default_value(0.2), "similarity threshold");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")
            || vm.count("data")==0
            || vm.count("output") == 0) {
        cout << desc << endl;
        return 0;
    }
    if (vm.count("version")) {
        std::cout <<"the current version = "
            << DIMSUM_VERSION_MAJOR
            << "." << DIMSUM_VERSION_MINOR
            << std::endl;
        return 0;
    }
    bool mirror = false;
    if (vm.count("mirror"))
        mirror = true;
    std::string matrix_data = vm["data"].as<std::string>();
    std::string output = vm["output"].as<std::string>();
    float threshold = vm["threshold"].as<float>();
    cout << "[0] start loading matrix." << endl;
    kaijiang::PairSimilarityCaculator caculator(matrix_data.c_str(), threshold);
    cout << "[1] loading matrix finished." << endl;
    caculator.Caculate(output.c_str(), mirror);
    cout << "[2] caculating similarity finished." << endl;
    return 0;
}
