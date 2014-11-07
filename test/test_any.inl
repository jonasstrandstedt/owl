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


TEST(Any, Null) {
    owl::Any a1;
    owl::Any a2 = "string";
    owl::Any a3 = a1;
    owl::Any a4(a1);

    EXPECT_EQ(a1.is_null(), true);
    EXPECT_EQ(a2.is_null(), false);
    EXPECT_EQ(a3.is_null(), true);
    EXPECT_EQ(a4.is_null(), true);

    EXPECT_EQ(a1.not_null(), false);
    EXPECT_EQ(a2.not_null(), true);
    EXPECT_EQ(a3.not_null(), false);
    EXPECT_EQ(a4.not_null(), false);
    
    a1 = 0;
    EXPECT_EQ(a1.is_null(), false);
    EXPECT_EQ(a2.is_null(), false);
    EXPECT_EQ(a3.is_null(), true);
    EXPECT_EQ(a4.is_null(), true);
}

TEST(Any, String) {
    owl::Any a1 = "string";
    owl::Any a2 = std::string("string");

    EXPECT_EQ(a1.is<const char*>(), false);
    EXPECT_EQ(a1.is<std::string>(), true);
    EXPECT_EQ(a2.is<const char*>(), false);
    EXPECT_EQ(a2.is<std::string>(), true);

    EXPECT_EQ(a1.as<std::string>(), "string");
    EXPECT_EQ(a2.as<std::string>(), "string");
}

TEST(Any, TypeName) {
    owl::Any a1;
    owl::Any a2 = "string";
    EXPECT_EQ(owl::TypeInfo::name<owl::Any>(), "owl::Any");
    EXPECT_EQ(owl::TypeInfo::name(a1), "owl::Any<nullptr>");
    EXPECT_EQ(owl::TypeInfo::name(a2), "owl::Any<std::string>");
}

TEST(Any, Type) {
    owl::Any a0;
    owl::Any a1 = 0;
    owl::Any a2 = 0L;
    owl::Any a3 = 0.0f;
    owl::Any a4 = 0.0;
    owl::Any a5 = 0UL;
    owl::Any a6 = 0ULL;
    owl::Any a7 = "0";
    owl::Any a8 = std::string("0");

    EXPECT_EQ(a0.is_null(), 				true);
    EXPECT_EQ(a0.is<int>(), 				false);
    EXPECT_EQ(a0.is<long>(), 				false);
    EXPECT_EQ(a0.is<float>(), 				false);
    EXPECT_EQ(a0.is<double>(), 				false);
    EXPECT_EQ(a0.is<unsigned long>(), 		false);
    EXPECT_EQ(a0.is<unsigned long long>(), 	false);
    EXPECT_EQ(a0.is<const char*>(), 		false);
    EXPECT_EQ(a0.is<std::string>(), 		false);
    
    EXPECT_EQ(a1.is_null(), 				false);
    EXPECT_EQ(a1.is<int>(), 				true);
    EXPECT_EQ(a1.is<long>(), 				false);
    EXPECT_EQ(a1.is<float>(), 				false);
    EXPECT_EQ(a1.is<double>(), 				false);
    EXPECT_EQ(a1.is<unsigned long>(), 		false);
    EXPECT_EQ(a1.is<unsigned long long>(), 	false);
    EXPECT_EQ(a1.is<const char*>(), 		false);
    EXPECT_EQ(a1.is<std::string>(), 		false);
    
    EXPECT_EQ(a2.is_null(), 				false);
    EXPECT_EQ(a2.is<int>(), 				false);
    EXPECT_EQ(a2.is<long>(), 				true);
    EXPECT_EQ(a2.is<float>(), 				false);
    EXPECT_EQ(a2.is<double>(), 				false);
    EXPECT_EQ(a2.is<unsigned long>(), 		false);
    EXPECT_EQ(a2.is<unsigned long long>(), 	false);
    EXPECT_EQ(a2.is<const char*>(), 		false);
    EXPECT_EQ(a2.is<std::string>(), 		false);
    
    EXPECT_EQ(a3.is_null(), 				false);
    EXPECT_EQ(a3.is<int>(), 				false);
    EXPECT_EQ(a3.is<long>(), 				false);
    EXPECT_EQ(a3.is<float>(), 				true);
    EXPECT_EQ(a3.is<double>(), 				false);
    EXPECT_EQ(a3.is<unsigned long>(), 		false);
    EXPECT_EQ(a3.is<unsigned long long>(), 	false);
    EXPECT_EQ(a3.is<const char*>(), 		false);
    EXPECT_EQ(a3.is<std::string>(), 		false);
    
    EXPECT_EQ(a4.is_null(), 				false);
    EXPECT_EQ(a4.is<int>(), 				false);
    EXPECT_EQ(a4.is<long>(), 				false);
    EXPECT_EQ(a4.is<float>(), 				false);
    EXPECT_EQ(a4.is<double>(), 				true);
    EXPECT_EQ(a4.is<unsigned long>(), 		false);
    EXPECT_EQ(a4.is<unsigned long long>(), 	false);
    EXPECT_EQ(a4.is<const char*>(), 		false);
    EXPECT_EQ(a4.is<std::string>(), 		false);
    
    EXPECT_EQ(a5.is_null(), 				false);
    EXPECT_EQ(a5.is<int>(), 				false);
    EXPECT_EQ(a5.is<long>(), 				false);
    EXPECT_EQ(a5.is<float>(), 				false);
    EXPECT_EQ(a5.is<double>(), 				false);
    EXPECT_EQ(a5.is<unsigned long>(), 		true);
    EXPECT_EQ(a5.is<unsigned long long>(), 	false);
    EXPECT_EQ(a5.is<const char*>(), 		false);
    EXPECT_EQ(a5.is<std::string>(), 		false);
    
    EXPECT_EQ(a6.is_null(), 				false);
    EXPECT_EQ(a6.is<int>(), 				false);
    EXPECT_EQ(a6.is<long>(), 				false);
    EXPECT_EQ(a6.is<float>(), 				false);
    EXPECT_EQ(a6.is<double>(), 				false);
    EXPECT_EQ(a6.is<unsigned long>(), 		false);
    EXPECT_EQ(a6.is<unsigned long long>(), 	true);
    EXPECT_EQ(a6.is<const char*>(), 		false);
    EXPECT_EQ(a6.is<std::string>(), 		false);
    
    EXPECT_EQ(a7.is_null(), 				false);
    EXPECT_EQ(a7.is<int>(), 				false);
    EXPECT_EQ(a7.is<long>(), 				false);
    EXPECT_EQ(a7.is<float>(), 				false);
    EXPECT_EQ(a7.is<double>(), 				false);
    EXPECT_EQ(a7.is<unsigned long>(), 		false);
    EXPECT_EQ(a7.is<unsigned long long>(), 	false);
    EXPECT_EQ(a7.is<const char*>(), 		false);
    EXPECT_EQ(a7.is<std::string>(), 		true);
    
    EXPECT_EQ(a8.is_null(), 				false);
    EXPECT_EQ(a8.is<int>(), 				false);
    EXPECT_EQ(a8.is<long>(), 				false);
    EXPECT_EQ(a8.is<float>(), 				false);
    EXPECT_EQ(a8.is<double>(), 				false);
    EXPECT_EQ(a8.is<unsigned long>(), 		false);
    EXPECT_EQ(a8.is<unsigned long long>(), 	false);
    EXPECT_EQ(a8.is<const char*>(), 		false);
    EXPECT_EQ(a8.is<std::string>(), 		true);
}