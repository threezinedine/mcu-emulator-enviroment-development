import pytest  # type: ignore
from analyze import Parser


def test_parse_simple_typedef():
    code = """
typedef int MyInt;
"""

    parser = Parser(code)
    parser.Parse()

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "MyInt"
    assert typedef.annotations == []
    assert typedef.underlyingType == "int"


def test_parse_typedef_with_annotations():
    code = """
typedef __attribute__((annotate("custom_type"))) float MyFloat;
"""

    parser = Parser(code)
    parser.Parse()
    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "MyFloat"
    assert typedef.annotations == ["custom_type"]
    assert typedef.underlyingType == "float"


def test_parse_function_pointer_typedef():
    code = """
typedef void (*Callback)(int);
"""

    parser = Parser(code)
    parser.Parse()
    assert len(parser.Typedefs) == 1

    typedef = parser.Typedefs[0]
    assert typedef.name == "Callback"
    assert typedef.annotations == []
    assert typedef.underlyingType == "void (*)(int)"
