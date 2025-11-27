from dataclasses import dataclass


@dataclass
class Template:
    file: str
    output: str | None = None
