import clang.cindex as cindex  # type: ignore


class PyObject:
    def __init__(self, cursor: cindex.Cursor):
        self.name = cursor.spelling
        self.annotations: list[str] = []

        for cursor in cursor.get_children():
            if cursor.kind == cindex.CursorKind.ANNOTATE_ATTR:
                self.annotations.append(cursor.displayname)
