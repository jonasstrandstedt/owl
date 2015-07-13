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


TEST(Dictionary, Empty) {
    owl::Dictionary d;
    EXPECT_TRUE(d.empty());

    d.insert("key", "value");
    EXPECT_FALSE(d.empty());
}

TEST(Dictionary, GetSet) {
    owl::Dictionary d;
    std::string s1, s2;

    s1 = "value";
    d.insert("key", s1);

    EXPECT_TRUE(d.get("key", s2));
    EXPECT_EQ(s2, s1);
}

TEST(Dictionary, RecursiveInsert) {
	std::string key = "key.key2.key3";
    std::string value;
    owl::Dictionary d;
    owl::Dictionary d2;
    
    EXPECT_TRUE(d.empty());

    d.insert(key, "value");
    EXPECT_FALSE(d.empty());

    EXPECT_TRUE(d.get(key, value));
    EXPECT_EQ(value, "value");

    EXPECT_TRUE(d.get("key", d2));
    value = "";
    EXPECT_TRUE(d2.get("key2.key3", value));
    EXPECT_EQ(value, "value");

}

TEST(Dictionary, Serialize) {
    owl::Dictionary dictionary1, dictionary2;
    std::string s1, s2;
    int i1, i2;
    long l1, l2;
    float f1, f2;
    double d1, d2;
    long double ld1, ld2;
    bool b1, b2;
    char c1, c2;

    std::stringstream ss;

    s1 = "string";
    i1 = 1;
    l1 = 1;
    f1 = 1.0f;
    d1 = 1.0;
    ld1 = 1.0;
    b1 = true;
    c1 = 'a';

    dictionary1.insert("s1", s1);
    dictionary1.insert("i1", i1);
    dictionary1.insert("l1", l1);
    dictionary1.insert("f1", f1);
    dictionary1.insert("d1", d1);
    dictionary1.insert("ld1", ld1);
    dictionary1.insert("b1", b1);
    dictionary1.insert("c1", c1);

    owl::Serializer::serialize(dictionary1, ss);
    owl::Serializer::deserialize(dictionary2, ss);

    EXPECT_FALSE(dictionary2.empty());
    EXPECT_TRUE(dictionary2.get("s1", s2));
    EXPECT_TRUE(dictionary2.get("i1", i2));
    EXPECT_TRUE(dictionary2.get("l1", l2));
    EXPECT_TRUE(dictionary2.get("f1", f2));
    EXPECT_TRUE(dictionary2.get("d1", d2));
    EXPECT_TRUE(dictionary2.get("ld1", ld2));
    EXPECT_TRUE(dictionary2.get("b1", b2));
    EXPECT_TRUE(dictionary2.get("c1", c2));

    EXPECT_EQ(s2, s1);
    EXPECT_EQ(i2, i1);
    EXPECT_EQ(l2, l1);
    EXPECT_EQ(l2, l1);
    EXPECT_EQ(f2, f1);
    EXPECT_EQ(d2, d1);
    EXPECT_EQ(ld2, ld1);
    EXPECT_EQ(b2, b1);
    EXPECT_EQ(c2, c1);

}

TEST(Dictionary, RecursiveSerialize) {
    std::string key = "key.key2.key3";
    std::string value;
    owl::Dictionary d1, d2, d3;
    std::stringstream ss;
    
    EXPECT_TRUE(d1.empty());

    d1.insert(key, "value");
    EXPECT_FALSE(d1.empty());

    owl::Serializer::serialize(d1, ss);
    owl::Serializer::deserialize(d2, ss);

    EXPECT_TRUE(d2.get(key, value));

    EXPECT_EQ(value, "value");

    EXPECT_TRUE(d2.get("key", d3));
    value = "";
    EXPECT_TRUE(d3.get("key2.key3", value));
    EXPECT_EQ(value, "value");

}