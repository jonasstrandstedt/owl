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


TEST(Buffer, String) {
    
    const std::string s1 = "first";
    const std::string s2 = "second";
    std::string s4, s5, s6;

    owl::Buffer b;
    b.serialize(s1);
    b.serialize(s2);
    b.serialize("third");
    
    b.deserialize(s4);
    b.deserialize(s5);
    b.deserialize(s6);
    
    EXPECT_EQ(s1, s4);
    EXPECT_EQ(s2, s5);
    EXPECT_EQ("third", s6);
}

TEST(Buffer, MixedTypes) {
    
    std::string         s1, s2;
    int                 i1, i2;
    double              d1, d2;
    unsigned long long  u1, u2;
    
    s1 = "string";
    i1 = 42;
    d1 = 123.456;
    u1 = 123456789;
    
    owl::Buffer b;
    
    b.serialize(s1);
    b.serialize(i1);
    b.serialize(d1);
    b.serialize(u1);
    
    b.deserialize(s2);
    b.deserialize(i2);
    b.deserialize(d2);
    b.deserialize(u2);
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
    
}

TEST(Buffer, CopyMove) {
    
    std::string         s1, s2;
    int                 i1, i2;
    double              d1, d2;
    unsigned long long  u1, u2;
    
    s1 = "string";
    i1 = 42;
    d1 = 123.456;
    u1 = 123456789;
    
    owl::Buffer b, b2;
    
    b.serialize(s1);
    b.serialize(i1);
    b.serialize(d1);
    b.serialize(u1);
    
    // copy
    b2 = b;
    b.reset();
    
    b2.deserialize(s2);
    b2.deserialize(i2);
    b2.deserialize(d2);
    b2.deserialize(u2);
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
    
    s2 = "";
    i2 = 0;
    d2 = 0.0;
    u2 = 0;
    
    b.serialize(s1);
    b.serialize(i1);
    b.serialize(d1);
    b.serialize(u1);
    
    // move
    b2 = std::move(b);
    
    b2.deserialize(s2);
    b2.deserialize(i2);
    b2.deserialize(d2);
    b2.deserialize(u2);
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
    
}

TEST(Buffer, Store) {
    
    std::string         s1, s2;
    int                 i1, i2;
    double              d1, d2;
    unsigned long long  u1, u2;
    
    s1 = "string";
    i1 = 42;
    d1 = 123.456;
    u1 = 123456789;
    
    owl::Buffer b, b2;
    
    b.serialize(s1);
    b.serialize(i1);
    b.serialize(d1);
    b.serialize(u1);
    
    b.write("binary.bin");
    b2.read("binary.bin");
    
    b2.deserialize(s2);
    b2.deserialize(i2);
    b2.deserialize(d2);
    b2.deserialize(u2);
    
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
    
}

TEST(Buffer, StoreCompress) {
    
    std::string         s1, s2;
    int                 i1, i2;
    double              d1, d2;
    unsigned long long  u1, u2;
    
    s1 = "string";
    i1 = 42;
    d1 = 123.456;
    u1 = 123456789;
    
    owl::Buffer b, b2;
    
    b.serialize(s1);
    b.serialize(i1);
    b.serialize(d1);
    b.serialize(u1);
    
    b.write("binaryCompressed.bin", true);
    b2.read("binaryCompressed.bin");
    
    b2.deserialize(s2);
    b2.deserialize(i2);
    b2.deserialize(d2);
    b2.deserialize(u2);
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
    
}

TEST(Buffer, FloatVector) {
    
    std::vector<float> fv, fv2;
    fv.push_back(1.5);
    fv.push_back(2.5);
    fv.push_back(3.5);
    fv.push_back(4.5);
    fv.push_back(5.5);
    
    owl::Buffer b;
    
    b.serialize(fv);
    b.deserialize(fv2);
    
    EXPECT_EQ(fv.size(), fv2.size());
    for(size_t i = 0; i < fv.size(); ++i) {
        EXPECT_EQ(fv.at(i), fv2.at(i));
    }
    
}

TEST(Buffer, StringVector) {
    
    std::vector<std::string> sv, sv2;
    sv.push_back(std::string("string1"));
    sv.push_back(std::string("string2"));
    sv.push_back(std::string("string3"));
    sv.push_back(std::string("string4"));
    sv.push_back(std::string("string5"));
    
    owl::Buffer b;
    
    b.serialize(sv);
    b.deserialize(sv2);
    
    EXPECT_EQ(sv.size(), sv2.size());
    for(size_t i = 0; i < sv.size(); ++i) {
        EXPECT_EQ(sv.at(i), sv2.at(i));
    }
    
}