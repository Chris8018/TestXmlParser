# Test XML Parser Library for C++

![C/C++ CI](https://github.com/Chris8018/TestXmlParser/workflows/C/C++%20CI/badge.svg)

The purpose of this is to test some C++ XML Libs to see what they are cable of and how they will fit with emotive project.

## Element Text Escaping SYMBOL
XML Text Escape Case

~!@#$%^&*    ()_+{}|:\"  <   > ?`1234567890-=[]\\;',./ - In

~!@#$%^&amp;*()_+{}|: "&lt;&gt;?`1234567890-=[]\\;',./ - Out - Java

~!@#$%^&amp;*()_+{}|: "&lt;&gt;?`1234567890-=[]\\;',./ - Out - C#

~!@#$%^&amp;*()_+{}|: "&lt;&gt;?`1234567890-=[] \;',./ - Out - C++ (Xerces)
                                               

I will let the node name handle by Xerces itself
As for Text, Attribute Key and Value, I might need to handle them myself

-> Can do that either by take Exception or Try to use name checking within the chosen library.
