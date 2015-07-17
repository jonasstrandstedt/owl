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
    owl::Any a1 = 1;
    const owl::Any& a2 = a1;
    EXPECT_EQ(owl::TypeInfo::name<owl::Any>(), "owl::Any");
    EXPECT_EQ(owl::TypeInfo::name(a2), "const owl::Any&");
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

TEST(Any, Serializing) {

    typedef struct {
        int i;
    } SpecialType;

    SpecialType st;
    st.i = 1;
    int i = 1;

    owl::Any a0;
    owl::Any a1 = 1;
    owl::Any a2 = 1L;
    owl::Any a3 = 1.0f;
    owl::Any a4 = 1.0;
    owl::Any a5 = 1UL;
    owl::Any a6 = 1ULL;
    owl::Any a7 = "1";
    owl::Any a8 = std::string("1");
    owl::Any a9 = st;
    owl::Any a10 = &i;
    
    owl::Any a02;
    owl::Any a12;
    owl::Any a22;
    owl::Any a32;
    owl::Any a42;
    owl::Any a52;
    owl::Any a62;
    owl::Any a72;
    owl::Any a82;
    owl::Any a92;
    owl::Any a102;
    
    std::stringstream ss;
    
    owl::Serializer::serialize(a0, ss);
    owl::Serializer::serialize(a1, ss);
    owl::Serializer::serialize(a2, ss);
    owl::Serializer::serialize(a3, ss);
    owl::Serializer::serialize(a4, ss);
    owl::Serializer::serialize(a5, ss);
    owl::Serializer::serialize(a6, ss);
    owl::Serializer::serialize(a7, ss);
    owl::Serializer::serialize(a8, ss);
    owl::Serializer::serialize(a9, ss);
    owl::Serializer::serialize(a10, ss);
    
    owl::Serializer::deserialize(a02, ss);
    owl::Serializer::deserialize(a12, ss);
    owl::Serializer::deserialize(a22, ss);
    owl::Serializer::deserialize(a32, ss);
    owl::Serializer::deserialize(a42, ss);
    owl::Serializer::deserialize(a52, ss);
    owl::Serializer::deserialize(a62, ss);
    owl::Serializer::deserialize(a72, ss);
    owl::Serializer::deserialize(a82, ss);
    owl::Serializer::deserialize(a92, ss);
    owl::Serializer::deserialize(a102, ss);
    
    EXPECT_TRUE(a02.is_null());
    EXPECT_TRUE(a12.is<int>());
    EXPECT_TRUE(a22.is<long>());
    EXPECT_TRUE(a32.is<float>());
    EXPECT_TRUE(a42.is<double>());
    EXPECT_TRUE(a52.is<unsigned long>());
    EXPECT_TRUE(a62.is<unsigned long long>());
    EXPECT_TRUE(a72.is<std::string>());
    EXPECT_TRUE(a82.is<std::string>());
    EXPECT_TRUE(a92.is_null());
    EXPECT_TRUE(a102.is_null());
    
    EXPECT_EQ(a12.as<int>(), 1);
    EXPECT_EQ(a22.as<long>(), 1);
    EXPECT_EQ(a32.as<float>(), 1.0f);
    EXPECT_EQ(a42.as<double>(), 1.0);
    EXPECT_EQ(a52.as<unsigned long>(), 1);
    EXPECT_EQ(a62.as<unsigned long long>(), 1);
    EXPECT_EQ(a72.as<std::string>(), "1");
    EXPECT_EQ(a82.as<std::string>(), "1");
}

TEST(Any, SerializingVector) {
    std::vector<float> fv, fv2;
    fv.push_back(1.5);
    fv.push_back(2.5);
    fv.push_back(3.5);
    fv.push_back(4.5);
    fv.push_back(5.5);
    
    owl::Any a1 = fv;
    owl::Any a2;
    
    std::stringstream ss;
    
    EXPECT_TRUE(a2.is_null());
    
    owl::Serializer::serialize(a1, ss);
    owl::Serializer::deserialize(a2, ss);
    
    EXPECT_FALSE(a2.is_null());
    EXPECT_TRUE(a2.is<std::vector<float>>());
    
    fv2 = a2.as<std::vector<float>>();
    
    EXPECT_EQ(fv.size(), fv2.size());
    for(size_t i = 0; i < fv.size(); ++i) {
        EXPECT_EQ(fv.at(i), fv2.at(i));
    }
    
    EXPECT_EQ(owl::Serializer::bytes_left(ss), 0);
}
