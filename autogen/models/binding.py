from dataclasses import dataclass


@dataclass
class Binding:
    template: str
    output: str
