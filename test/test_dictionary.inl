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
    EXPECT_EQ(d.empty(), true);

    d.insert("key", "value");
    EXPECT_EQ(d.empty(), false);
}

TEST(Dictionary, RecursiveInsert) {
	std::string key = "key.key2.key3";
    std::string value;
    owl::Dictionary d;
    owl::Dictionary d2;
    
    EXPECT_EQ(d.empty(), true);

    d.insert(key, "value");
    EXPECT_EQ(d.empty(), false);

    EXPECT_EQ(d.get(key, value), true);
    EXPECT_EQ(value, "value");

    EXPECT_EQ(d.get("key", d2), true);
    value = "";
    EXPECT_EQ(d2.get("key2.key3", value), true);
    EXPECT_EQ(value, "value");

}