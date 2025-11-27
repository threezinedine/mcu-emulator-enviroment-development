from dataclasses import dataclass


@dataclass
class Template:
    filePath: str
    output: str | None = None
