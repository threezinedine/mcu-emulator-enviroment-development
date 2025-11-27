from dataclasses import dataclass


@dataclass
class Binding:
    file: str
    template: str
    output: str
    dependencies: list[str] | None = None
    extensions: list[str] | None = None
