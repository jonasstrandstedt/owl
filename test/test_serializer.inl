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


TEST(Serializer, String) {
    
    const std::string s1 = "first";
    const std::string s2 = "second";
    std::string s3, s4;

    std::stringstream ss;
    
    owl::Serializer::serialize(s1, ss);
    owl::Serializer::serialize(s2, ss);
    owl::Serializer::deserialize(s3, ss);
    owl::Serializer::deserialize(s4, ss);
    
    EXPECT_EQ(s3, s1);
    EXPECT_EQ(s4, s2);
    
    EXPECT_EQ(owl::Serializer::bytes_left(ss), 0);
}

TEST(Serializer, MixedTypes) {
    
    std::string         s1, s2;
    int                 i1, i2;
    double              d1, d2;
    unsigned long long  u1, u2;
    
    s1 = "string";
    i1 = 42;
    d1 = 123.456;
    u1 = 123456789;
    
    std::stringstream ss;
    
    owl::Serializer::serialize(s1, ss);
    owl::Serializer::serialize(i1, ss);
    owl::Serializer::serialize(d1, ss);
    owl::Serializer::serialize(u1, ss);
    owl::Serializer::deserialize(s2, ss);
    owl::Serializer::deserialize(i2, ss);
    owl::Serializer::deserialize(d2, ss);
    owl::Serializer::deserialize(u2, ss);
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
    
    EXPECT_EQ(owl::Serializer::bytes_left(ss), 0);
}

TEST(Serializer, FloatVector) {
    
    std::vector<float> fv, fv2;
    fv.push_back(1.5);
    fv.push_back(2.5);
    fv.push_back(3.5);
    fv.push_back(4.5);
    fv.push_back(5.5);
    
    std::stringstream ss;
    
    owl::Serializer::serialize(fv, ss);
    owl::Serializer::deserialize(fv2, ss);
    
    EXPECT_EQ(fv.size(), fv2.size());
    for(size_t i = 0; i < fv.size(); ++i) {
        EXPECT_EQ(fv.at(i), fv2.at(i));
    }
    
    EXPECT_EQ(owl::Serializer::bytes_left(ss), 0);
}

TEST(Serializer, StringVector) {
    
    std::vector<std::string> sv, sv2;
    sv.push_back(std::string("string1"));
    sv.push_back(std::string("string2"));
    sv.push_back(std::string("string3"));
    sv.push_back(std::string("string4"));
    sv.push_back(std::string("string5"));
    
    std::stringstream ss;
    
    owl::Serializer::serialize(sv, ss);
    owl::Serializer::deserialize(sv2, ss);
    
    EXPECT_EQ(sv.size(), sv2.size());
    for(size_t i = 0; i < sv.size(); ++i) {
        EXPECT_EQ(sv.at(i), sv2.at(i));
    }
    
    EXPECT_EQ(owl::Serializer::bytes_left(ss), 0);
}

#if !defined(NDEBUG)
TEST(Serializer, Exceptions) {
    
    std::stringstream ss;
    
    int i;
    int i1 = 1, i2;
    
    owl::Serializer::serialize(i1, ss);
    owl::Serializer::deserialize(i2, ss);
    
    EXPECT_EQ(i2, i1);
    
    try {
        owl::Serializer::deserialize(i, ss);
        
        // Above should throw for debug builds and enter the catch clause
        EXPECT_TRUE(false);
    }
    catch(owl::Serializer::SerializerException& e) {
        EXPECT_TRUE(true);
        
        std::string cause = e.what();
        EXPECT_EQ(cause, "EmptyStream");
    }
    
    try {
    
        // In order for this test to work, float needs to be smaller than double
        ASSERT_TRUE(sizeof(float) < sizeof(double));
        
        float f = 1.0f;
        double d;
        
        owl::Serializer::serialize(f, ss);
        owl::Serializer::deserialize(d, ss);
        
        // Above should throw for debug builds and enter the catch clause
        EXPECT_TRUE(false);
    }
    catch(owl::Serializer::SerializerException& e) {
        EXPECT_TRUE(true);
        
        std::string cause = e.what();
        EXPECT_EQ(cause, "InsufficientData");
    }
    
}
#endif

