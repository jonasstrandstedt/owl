/*****************************************************************************************
 *                                                                                       *
 * owl                                                                                   *
 *                                                                                       *
 * Copyright (c) 2014 Jonas Strandstedt                                                  *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/


#include <owl/data/typeinfo.h>

TEST(TypeInfo, Standard) {
    EXPECT_EQ(owl::TypeInfo::name<int>(), "int");
    EXPECT_EQ(owl::TypeInfo::name<float>(), "float");
    EXPECT_EQ(owl::TypeInfo::name<double>(), "double");
    EXPECT_EQ(owl::TypeInfo::name<char>(), "char");
    EXPECT_EQ(owl::TypeInfo::name<long>(), "long");
    EXPECT_EQ(owl::TypeInfo::name<unsigned int>(), "unsigned int");
    EXPECT_EQ(owl::TypeInfo::name<unsigned long>(), "unsigned long");
}

TEST(TypeInfo, PointerTypes) {
    EXPECT_EQ(owl::TypeInfo::name<int*>(), "int*");
    EXPECT_EQ(owl::TypeInfo::name<float*>(), "float*");
    EXPECT_EQ(owl::TypeInfo::name<double*>(), "double*");
    EXPECT_EQ(owl::TypeInfo::name<char*>(), "char*");
    EXPECT_EQ(owl::TypeInfo::name<long*>(), "long*");
    EXPECT_EQ(owl::TypeInfo::name<unsigned int*>(), "unsigned int*");
    EXPECT_EQ(owl::TypeInfo::name<unsigned long*>(), "unsigned long*");
}

TEST(TypeInfo, ReferenceTypes) {
    int i = 0;
    int& ir = i;
    const int& cir = i;
    
    EXPECT_EQ(owl::TypeInfo::name(ir), "int&");
    EXPECT_EQ(owl::TypeInfo::name(cir), "const int&");
}