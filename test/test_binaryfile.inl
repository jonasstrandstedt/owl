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

TEST(BinaryFile, InOut) {

    owl::BinaryFile fileout("binary.bin", owl::BinaryFile::OpenMode::Out);

    ASSERT_TRUE(fileout);

    std::string         s1, s2;
    int                 i1, i2;
    double              d1, d2;
    unsigned long long  u1, u2;
    
    s1 = "string";
    i1 = 42;
    d1 = 123.456;
    u1 = 123456789;
    
    fileout.write(s1);
    fileout.write(i1);
    fileout.write(d1);
    fileout.write(u1);
    
    fileout.close();

    owl::BinaryFile filein("binary.bin", owl::BinaryFile::OpenMode::In);

    ASSERT_TRUE(filein);
    
    filein.read(s2);
    filein.read(i2);
    filein.read(d2);
    filein.read(u2);

    filein.close();
    
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(i1, i2);
    EXPECT_EQ(d1, d2);
    EXPECT_EQ(u1, u2);
}
